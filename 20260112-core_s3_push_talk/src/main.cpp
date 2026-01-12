// Arduino IDE: board = ESP32S3系, ライブラリ: M5Unified, Links2004/WebSocketsClient
// 事前に: Tools→PSRAM有効（SEはPSRAM無しでも動くよう小さめバッファ）

#include <M5Unified.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <cstring>
#include <vector>
#include "config.h"

//////////////////// 設定 ////////////////////
const char *WIFI_SSID = WIFI_SSID_H;
const char *WIFI_PASS = WIFI_PASSWORD_H;
const char *SERVER_HOST = SERVER_HOST_H;
const int SERVER_PORT = SERVER_PORT_H;
const char *SERVER_PATH = SERVER_PATH_H; // WebSocket エンドポイント
const int SAMPLE_RATE = 16000;           // 16kHz モノラル
const int MAX_RECORD_MS = 5000;          // 最大5秒（テスト用）
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
static WebSocketsClient wsClient;

struct __attribute__((packed)) WsAudioHeader
{
  char kind[4];        // "PCM1"
  uint32_t sampleRate; // LE
  uint16_t channels;   // 1
  uint16_t reserved;   // 0
};

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
  M5.Display.println("CoreS3 SE - AI Home Agent (WS)");

  connectWiFi();
  M5.Display.printf("WiFi: %s\n", WiFi.localIP().toString().c_str());

  // Mic/Speaker setup
  M5.Speaker.setVolume(200); // 0-255
  M5.Mic.begin();

  wsClient.begin(SERVER_HOST, SERVER_PORT, SERVER_PATH);
  wsClient.onEvent([](WStype_t type, uint8_t *payload, size_t length)
                   {
                     switch (type)
                     {
                     case WStype_DISCONNECTED:
                       M5.Display.println("WS: disconnected");
                       break;
                     case WStype_CONNECTED:
                       M5.Display.printf("WS: connected %s\n", SERVER_PATH);
                       break;
                     case WStype_TEXT:
                       M5.Display.printf("WS msg: %.*s\n", (int)length, payload);
                       break;
                     default:
                       break;
                     } });
  wsClient.setReconnectInterval(2000);
  wsClient.enableHeartbeat(15000, 3000, 2);
}

void loop()
{
  M5.update();
  wsClient.loop();

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
    // WebSocket BIN (ヘッダ + PCM16LE生データ)
    M5.Display.println("Uploading...");
    if ((WiFi.status() == WL_CONNECTED) && wsClient.isConnected())
    {
      size_t byte_size = record_size * sizeof(int16_t);

      WsAudioHeader header{};
      memcpy(header.kind, "PCM1", 4);
      header.sampleRate = (uint32_t)SAMPLE_RATE;
      header.channels = 1;
      header.reserved = 0;

      std::vector<uint8_t> packet;
      packet.resize(sizeof(WsAudioHeader) + byte_size);
      memcpy(packet.data(), &header, sizeof(WsAudioHeader));
      memcpy(packet.data() + sizeof(WsAudioHeader), rec_data, byte_size);

      wsClient.sendBIN(packet.data(), packet.size());
      M5.Display.println("WS send done.");
    }
    else
    {
      M5.Display.println("WS not connected.");
    }
    state = STATE_IDLE;
    M5.Display.println("Press and hold Btn A to record.");
  }
}
