// Arduino IDE: board = ESP32S3系, ライブラリ: M5Unified, ArduinoJSON(任意)
// 事前に: Tools→PSRAM有効（SEはPSRAM無しでも動くよう小さめバッファ）

#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <base64.h>

//////////////////// 設定 ////////////////////
const char *WIFI_SSID = "YOUR_SSID";
const char *WIFI_PASS = "YOUR_PASS";
const char *SERVER_URL = "http://<PCのIP>:8000/api/v1/audio"; // 後述FastAPIのエンドポイント
const int SAMPLE_RATE = 16000;                                // 16kHz モノラル
const int MAX_RECORD_MS = 5000;                               // 最大5秒（テスト用）
/////////////////////////////////////////////

// μ-law 変換（G.711）
static inline uint8_t linear2ulaw(int16_t pcm)
{
  const int cBias = 0x84;
  const int cClip = 32635;
  int mask;
  int seg;
  uint8_t uval;

  // get sign and magnitude of value
  int16_t p = pcm;
  if (p < 0)
  {
    p = -p;
    mask = 0x7F;
  }
  else
  {
    mask = 0xFF;
  }
  if (p > cClip)
    p = cClip;
  p += cBias;

  // find segment
  if (p >= 256)
  {
    seg = 7;
    for (int v = p >> 8; v; v >>= 1)
      seg--;
    uval = (uint8_t)(((seg << 4) | ((p >> (seg + 3)) & 0x0F)));
  }
  else
  {
    uval = (uint8_t)(p >> 4);
  }
  return uval ^ mask;
}

static inline int16_t ulaw2linear(uint8_t u_val)
{
  u_val = ~u_val;
  int t = ((u_val & 0x0F) << 3) + 0x84;
  t <<= ((unsigned)u_val & 0x70) >> 4;
  return (u_val & 0x80) ? (0x84 - t) : (t - 0x84);
}

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
  M5.Display.setTextSize(2);
  M5.Display.println("CoreS3 SE - AI Home Agent (HTTP)");

  connectWiFi();
  M5.Display.printf("WiFi: %s\n", WiFi.localIP().toString().c_str());

  // Mic/Speaker setup
  M5.Speaker.setVolume(200); // 0-255
}

void loop()
{
  M5.update();

  // 長押しで録音、離して送信
  if (M5.BtnA.wasPressed())
  {
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setCursor(0, 0);
    M5.Display.println("Recording... (hold A)");

    // マイク開始
    auto mic_cfg = M5.Mic.config();
    mic_cfg.dma_buf_count = 3;
    mic_cfg.dma_buf_len = 256;
    mic_cfg.sample_rate = SAMPLE_RATE;
    mic_cfg.stereo = false;
    M5.Mic.config(mic_cfg);
    M5.Mic.begin();

    std::vector<int16_t> pcm;
    pcm.reserve(SAMPLE_RATE * (MAX_RECORD_MS / 1000));

    uint32_t start = millis();
    while (M5.BtnA.isPressed())
    {
      // 一定量ずつ読み出し
      while (M5.Mic.isRecording() < 2)
      {
        std::vector<int16_t> tmp(256 / sizeof(int16_t));
        M5.Mic.record(tmp.data(), tmp.size() * sizeof(int16_t));
        // 片ch化（念のため）＆蓄積
        for (size_t i = 0; i < tmp.size(); ++i)
          pcm.push_back(tmp[i]);
      }
      if (millis() - start > MAX_RECORD_MS)
        break;
      M5.update();
      delay(1);
    }
    M5.Mic.end();

    if (pcm.empty())
    {
      M5.Display.println("No audio captured.");
      delay(800);
      return;
    }

    // μ-law に変換
    std::vector<uint8_t> mulaw;
    mulaw.resize(pcm.size());
    for (size_t i = 0; i < pcm.size(); ++i)
      mulaw[i] = linear2ulaw(pcm[i]);

    // HTTP POST
    M5.Display.println("Uploading...");
    if ((WiFi.status() == WL_CONNECTED))
    {
      HTTPClient http;
      http.begin(SERVER_URL);
      http.addHeader("Content-Type", "application/octet-stream");
      http.addHeader("X-Sample-Rate", String(SAMPLE_RATE));
      http.addHeader("X-Codec", "mulaw");
      int code = http.POST(mulaw.data(), mulaw.size());
      if (code == 200)
      {
        String payload = http.getString(); // JSON
        http.end();

        // かなり簡単なJSONパース（ArduinoJSON推奨だが最小なら手書き可）
        // "text":"...","audio_mulaw":"<base64>"
        String text;
        String audio_b64;

        int tpos = payload.indexOf("\"text\"");
        if (tpos >= 0)
        {
          int q1 = payload.indexOf(":", tpos) + 1;
          int s1 = payload.indexOf("\"", q1) + 1;
          int s2 = payload.indexOf("\"", s1);
          text = payload.substring(s1, s2);
          text.replace("\\n", "\n");
        }
        int apos = payload.indexOf("\"audio_mulaw\"");
        if (apos >= 0)
        {
          int q1 = payload.indexOf(":", apos) + 1;
          int s1 = payload.indexOf("\"", q1) + 1;
          int s2 = payload.indexOf("\"", s1);
          audio_b64 = payload.substring(s1, s2);
        }

        // 表示
        M5.Display.fillScreen(TFT_BLACK);
        M5.Display.setCursor(0, 0);
        if (text.length())
        {
          M5.Display.println("Response (text):");
          M5.Display.println(text);
        }
        else
        {
          M5.Display.println("No text in response.");
        }

        // 音声があれば再生（μ-law → PCM → I2S）
        // if (audio_b64.length() > 0)
        // {
        //   std::String base64Str = audio_b64;
        //   std::vector<uint8_t> mulaw_rx;
        //   mulaw_rx.resize(base64Str.length() * 3 / 4 + 8);
        //   int outlen = base64_decode((char *)mulaw_rx.data(), base64Str.c_str(), base64Str.length());
        //   mulaw_rx.resize(outlen);

        //   std::vector<int16_t> pcm_rx(mulaw_rx.size());
        //   for (size_t i = 0; i < mulaw_rx.size(); ++i)
        //     pcm_rx[i] = ulaw2linear(mulaw_rx[i]);

        //   // 再生
        //   auto spk_cfg = M5.Speaker.config();
        //   spk_cfg.sample_rate = SAMPLE_RATE;
        //   spk_cfg.stereo = false;
        //   M5.Speaker.config(spk_cfg);
        //   M5.Speaker.begin();

        //   M5.Display.println("\nPlaying audio...");
        //   M5.Speaker.playRaw((const uint8_t *)pcm_rx.data(),
        //                      pcm_rx.size() * sizeof(int16_t),
        //                      SAMPLE_RATE, m5spk_voice_t::m5spk_voice_0,
        //                      true); // true: wait end
        //   M5.Speaker.end();
        //   M5.Display.println("Done.");
        // }
      }
      else
      {
        http.end();
        M5.Display.printf("HTTP error %d\n", code);
      }
    }
    else
    {
      M5.Display.println("WiFi disconnected.");
    }
    delay(800);
  }
}
