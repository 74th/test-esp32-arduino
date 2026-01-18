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
#define TEST 2 // PCM再生

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Speaker.setVolume(128);
}

void loop()
{
  M5.update();

  if (M5.BtnA.wasPressed()){
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
    M5.Speaker.playRaw(reinterpret_cast<const int16_t*>(_binary_data_voice_pcm_start), sample_len, sample_rate_, stereo_, 1, 0, true);
#endif
  }
  if (M5.BtnA.wasReleased()){
    M5.Display.setCursor(0, 20);
    M5.Display.println("BtnA Released");

    if(M5.Speaker.isPlaying()){
      M5.Speaker.stop();
    }
  }
}
