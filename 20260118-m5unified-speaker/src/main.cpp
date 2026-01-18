#include <M5Unified.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <algorithm>
#include <cstring>
#include <vector>
#include "config.h"

extern const uint8_t _binary_data_voice_wav_start[];
extern const uint8_t _binary_data_voice_wav_end[];

extern const uint8_t _binary_data_voice_pcm_start[];
extern const uint8_t _binary_data_voice_pcm_end[];

// #define TEST 1 // WAV再生
// #define TEST 2 // PCM再生
#define TEST 3 // PCMストリーミング再生

// ストリーミング状態（ファイルスコープに置き、loop の繰り返し間で保持する）
static bool streaming = false;
static size_t cur_sample = 0;
static uint32_t next_feed_ms = 0;

// ストリーミングで使用する PCM のメタデータ / ポインタ
static const int16_t *g_pcm16 = nullptr;
static size_t g_total_samples = 0;
static size_t g_chunk_samples = 0;
static uint32_t g_sample_rate = 24000;
static bool g_stereo = false;

// チャンク長（ミリ秒単位、例: 1000 = 1秒チャンク）
static const uint32_t CHUNK_MS = 500;

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Speaker.setVolume(128);
}

void loop()
{
  M5.update();

  if (M5.BtnA.wasPressed())
  {
    M5.Display.clear();
    M5.Display.setCursor(0, 0);
    M5.Display.println("BtnA Pressed");

#if TEST == 1
    M5.Display.println("Playing WAV...");
    size_t wav_size = _binary_data_voice_wav_end - _binary_data_voice_wav_start;
    M5.Speaker.playWav(_binary_data_voice_wav_start, wav_size, 0, true);
#endif

#if TEST == 2
    M5.Display.println("Playing PCM...");

    size_t pcm_size = _binary_data_voice_pcm_end - _binary_data_voice_pcm_start;
    size_t sample_len = pcm_size / sizeof(int16_t);
    uint32_t sample_rate_ = 24000;
    bool stereo_ = false;

    // int16_t*にキャストしないと、8bit PCMとして扱われてしまい、音割れする
    M5.Speaker.playRaw(reinterpret_cast<const int16_t *>(_binary_data_voice_pcm_start), sample_len, sample_rate_, stereo_, 1, 0, true);
#endif

#if TEST == 3
    M5.Display.println("Streaming PCM (TEST 3)...");

    // ストリーミング用のグローバルを初期化し、最初のチャンクを送信する
    size_t pcm_size = _binary_data_voice_pcm_end - _binary_data_voice_pcm_start;
    g_pcm16 = reinterpret_cast<const int16_t *>(_binary_data_voice_pcm_start);
    g_total_samples = pcm_size / sizeof(int16_t);
    g_sample_rate = 24000;
    g_stereo = false;

    const uint32_t chunk_ms = CHUNK_MS;
    const size_t channels = g_stereo ? 2 : 1;
    // チャンクあたりの各チャンネルのサンプル数を計算: sample_rate * (chunk_ms / 1000)
    const size_t per_channel_samples = (size_t)(((uint64_t)g_sample_rate * chunk_ms) / 1000);
    g_chunk_samples = per_channel_samples * channels;

    // ストリーミング開始フラグを立てる
    streaming = true;
    cur_sample = 0;

    // 最初のチャンクを即時送信
    size_t send_len = std::min(g_chunk_samples, g_total_samples - cur_sample);
    M5.Display.printf("first chunk... send_len=%d samples\n", send_len);
    M5.Speaker.playRaw(g_pcm16 + cur_sample, send_len, g_sample_rate, g_stereo, 1, 0, false);
    cur_sample += send_len;

    // 次のチャンク供給は最初は半チャンク後にスケジューリング（CHUNK_MS の半分）
    next_feed_ms = millis() + (CHUNK_MS / 2);
#endif
  }
  if (M5.BtnA.wasReleased())
  {
    M5.Display.println("BtnA Released");

    if (M5.Speaker.isPlaying())
    {
      M5.Speaker.stop();
    }
    // ストリーミングを停止し、状態をリセットして次回の押下で再初期化できるようにする
    streaming = false;
    cur_sample = 0;
    next_feed_ms = 0;
    g_pcm16 = nullptr;
    g_total_samples = 0;
  }
  // Streaming feed logic (runs every loop)
#if TEST == 3
  if (streaming)
  {
    // チャンク供給処理: 初回の半チャンク遅延後、その後は CHUNK_MS 毎にチャンクを供給する
    if (cur_sample < g_total_samples && (int32_t)(millis() - next_feed_ms) >= 0)
    {
      size_t send_len = std::min(g_chunk_samples, g_total_samples - cur_sample);
      M5.Display.printf("next chunk... send_len=%d samples\n", send_len);
      M5.Speaker.playRaw(g_pcm16 + cur_sample, send_len, g_sample_rate, g_stereo, 1, 0, false);
      cur_sample += send_len;

      // 次の供給は CHUNK_MS ミリ秒後
      next_feed_ms += CHUNK_MS;
    }

    // ストリーミング完了時の処理
    if (cur_sample >= g_total_samples)
    {
      streaming = false;
      M5.Display.println("Streaming finished");
    }
  }
#endif
}
