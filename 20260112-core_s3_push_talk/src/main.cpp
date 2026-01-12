// Arduino IDE: board = ESP32S3系, ライブラリ: M5Unified, ArduinoJSON(任意)
// 事前に: Tools→PSRAM有効（SEはPSRAM無しでも動くよう小さめバッファ）

#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"

//////////////////// 設定 ////////////////////
const char *WIFI_SSID = WIFI_SSID_H;
const char *WIFI_PASS = WIFI_PASSWORD_H;
const char *SERVER_URL = SERVER_URL_H; // 後述FastAPIのエンドポイント
const int SAMPLE_RATE = 16000;          // 16kHz モノラル
const int MAX_RECORD_MS = 5000;         // 最大5秒（テスト用）
/////////////////////////////////////////////

#define STATE_IDLE 0
#define STATE_RECORDING 1
#define STATE_SENDING 2

uint8_t state = 0;
static constexpr const size_t record_length = 200;
static constexpr const size_t record_number = SAMPLE_RATE * MAX_RECORD_MS / 1000 / record_length;
static constexpr const size_t record_samplerate = SAMPLE_RATE;
static constexpr const size_t record_size = record_number * record_length;
static size_t rec_record_idx = 2;
static size_t draw_record_idx = 0;
static int16_t *rec_data;

void connectWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(300);
  }
}

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);

  rec_data = (typeof(rec_data))heap_caps_malloc(record_size * sizeof(int16_t), MALLOC_CAP_8BIT);
  memset(rec_data, 0, record_size * sizeof(int16_t));

  M5.Display.setTextSize(2);
  M5.Display.println("CoreS3 SE - AI Home Agent (HTTP)");

  connectWiFi();
  M5.Display.printf("WiFi: %s\n", WiFi.localIP().toString().c_str());

  // Mic/Speaker setup
  M5.Speaker.setVolume(200); // 0-255
  M5.Mic.begin();
}

void loop()
{
  M5.update();

  if (state == STATE_IDLE)
  {
    M5.Display.setCursor(0, 40);
    M5.Display.println("Press and hold Btn A to record.");

    if (M5.BtnA.wasPressed())
    {
      M5.Display.println("Recording... (hold A)");
      state = STATE_RECORDING;
      rec_record_idx = 0;
    }
    return;
  }

  if (state == STATE_RECORDING)
  {
    if (M5.Mic.isEnabled())
    {
      static constexpr int shift = 6;
      auto data = &rec_data[rec_record_idx * record_length];
      if (M5.Mic.record(data, record_length, record_samplerate))
      {
        data = &rec_data[draw_record_idx * record_length];

        if (++rec_record_idx >= record_number)
        {
          rec_record_idx = 0;
          state = STATE_SENDING;
          M5.Display.println("recording full.");
        }
      }
    }
  }
  if (state == STATE_SENDING)
  {
    // HTTP POST (PCM16LE 生データ)
    M5.Display.println("Uploading...");
    if ((WiFi.status() == WL_CONNECTED))
    {
      HTTPClient http;
      http.begin(SERVER_URL);
      http.addHeader("Content-Type", "application/octet-stream");
      http.addHeader("X-Sample-Rate", String(SAMPLE_RATE));
      http.addHeader("X-Codec", "pcm16le");

      size_t byte_size = record_size * sizeof(int16_t);
      int code = http.POST((uint8_t *)rec_data, byte_size);
      if (code == 200)
      {
        M5.Display.println("Upload successful.");
      }
      else
      {
        M5.Display.printf("Upload failed. Code: %d\n", code);
      }
      http.end();
    }
    else
    {
      M5.Display.println("WiFi not connected.");
    }
    state = STATE_IDLE;
    M5.Display.println("Press and hold Btn A to record.");
  }
}
