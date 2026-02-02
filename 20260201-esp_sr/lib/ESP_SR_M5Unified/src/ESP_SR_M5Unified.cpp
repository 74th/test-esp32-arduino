/*
 * ESP_SR_M5Unified - Implementation
 */
#include "sdkconfig.h"
#if (CONFIG_IDF_TARGET_ESP32S3 || CONFIG_IDF_TARGET_ESP32P4) && (CONFIG_MODEL_IN_FLASH || CONFIG_MODEL_IN_SDCARD)
#include "ESP_SR_M5Unified.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <string.h>

// グローバルオーディオバッファとセマフォ
static int16_t *g_audio_buffer = NULL;
static size_t g_audio_samples = 0;
static size_t g_audio_buffer_capacity = 0;
static SemaphoreHandle_t g_audio_mutex = NULL;
static bool g_has_new_data = false;

static esp_err_t on_sr_fill_m5(void *arg, void *out, size_t len, size_t *bytes_read, uint32_t timeout_ms) {
  return ((ESP_SR_M5Unified_Class *)arg)->_fill(out, len, bytes_read, timeout_ms);
}

static void on_sr_event_m5(void *arg, sr_event_t event, int command_id, int phrase_id) {
  ((ESP_SR_M5Unified_Class *)arg)->_sr_event(event, command_id, phrase_id);
}

ESP_SR_M5Unified_Class::ESP_SR_M5Unified_Class() : cb(NULL), audio_buffer(NULL), buffer_size(0), buffer_ready(false) {}

ESP_SR_M5Unified_Class::~ESP_SR_M5Unified_Class() {
  end();
}

void ESP_SR_M5Unified_Class::onEvent(sr_cb_m5 event_cb) {
  cb = event_cb;
}

bool ESP_SR_M5Unified_Class::begin(
  const sr_cmd_t *sr_commands,
  size_t sr_commands_len,
  sr_channels_t rx_chan,
  sr_mode_t mode,
  const char *input_format
) {
  // オーディオバッファの準備
  // ESP-SR expects 512 samples chunks typically
  buffer_size = 512 * 3; // 3 channels (MNR format internally)
  audio_buffer = (int16_t *)heap_caps_malloc(buffer_size * sizeof(int16_t) * 4, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if (audio_buffer == NULL) {
    return false;
  }

  // グローバルバッファの初期化
  g_audio_buffer_capacity = buffer_size * 4;
  g_audio_buffer = audio_buffer;
  g_audio_samples = 0;
  g_has_new_data = false;

  if (g_audio_mutex == NULL) {
    g_audio_mutex = xSemaphoreCreateMutex();
    if (g_audio_mutex == NULL) {
      heap_caps_free(audio_buffer);
      audio_buffer = NULL;
      return false;
    }
  }

  esp_err_t err = sr_start_m5(
    on_sr_fill_m5,
    this,
    rx_chan,
    mode,
    input_format,
    sr_commands,
    sr_commands_len,
    on_sr_event_m5,
    this
  );

  return (err == ESP_OK);
}

bool ESP_SR_M5Unified_Class::end(void) {
  bool result = (sr_stop_m5() == ESP_OK);

  if (audio_buffer) {
    heap_caps_free(audio_buffer);
    audio_buffer = NULL;
  }

  g_audio_buffer = NULL;
  g_audio_samples = 0;

  return result;
}

bool ESP_SR_M5Unified_Class::setMode(sr_mode_t mode) {
  return sr_set_mode_m5(mode) == ESP_OK;
}

bool ESP_SR_M5Unified_Class::pause(void) {
  return sr_pause_m5() == ESP_OK;
}

bool ESP_SR_M5Unified_Class::resume(void) {
  return sr_resume_m5() == ESP_OK;
}

void ESP_SR_M5Unified_Class::feedAudio(const int16_t *data, size_t samples) {
  if (!data || samples == 0 || g_audio_mutex == NULL) {
    return;
  }

  // データをグローバルバッファにコピー
  if (xSemaphoreTake(g_audio_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
    size_t copy_samples = samples;
    if (copy_samples > g_audio_buffer_capacity) {
      copy_samples = g_audio_buffer_capacity;
    }

    memcpy(g_audio_buffer, data, copy_samples * sizeof(int16_t));
    g_audio_samples = copy_samples;
    g_has_new_data = true;

    xSemaphoreGive(g_audio_mutex);
  }
}

void ESP_SR_M5Unified_Class::_sr_event(sr_event_t event, int command_id, int phrase_id) {
  if (cb) {
    cb(event, command_id, phrase_id);
  }
}

esp_err_t ESP_SR_M5Unified_Class::_fill(void *out, size_t len, size_t *bytes_read, uint32_t timeout_ms) {
  if (out == NULL || bytes_read == NULL || g_audio_mutex == NULL) {
    return ESP_FAIL;
  }

  *bytes_read = 0;

  // タイムアウト処理
  TickType_t start_tick = xTaskGetTickCount();
  TickType_t timeout_ticks = pdMS_TO_TICKS(timeout_ms);

  while (!g_has_new_data) {
    if (timeout_ms != portMAX_DELAY) {
      TickType_t current_tick = xTaskGetTickCount();
      if ((current_tick - start_tick) >= timeout_ticks) {
        return ESP_ERR_TIMEOUT;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }

  // データをコピー
  if (xSemaphoreTake(g_audio_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
    size_t samples_to_copy = g_audio_samples;
    size_t bytes_to_copy = samples_to_copy * sizeof(int16_t);

    if (bytes_to_copy > len) {
      bytes_to_copy = len;
      samples_to_copy = len / sizeof(int16_t);
    }

    memcpy(out, g_audio_buffer, bytes_to_copy);
    *bytes_read = bytes_to_copy;
    g_has_new_data = false;

    xSemaphoreGive(g_audio_mutex);
    return ESP_OK;
  }

  return ESP_FAIL;
}

ESP_SR_M5Unified_Class ESP_SR_M5;

#endif  // CONFIG_IDF_TARGET_ESP32S3
