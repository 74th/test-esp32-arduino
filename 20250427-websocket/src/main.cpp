#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <WebSocketsServer.h>
#include <Adafruit_NeoPixel.h>

#include <wifi_settings.hpp>

#define CH9329_TX_PIN GPIO_NUM_7
#define CH9329_RX_PIN GPIO_NUM_8
#define LED_PIN GPIO_NUM_2

#define LED_COLOR_SETUP Adafruit_NeoPixel::Color(0, 32, 0)
#define LED_COLOR_NORMAL Adafruit_NeoPixel::Color(0, 0, 32)
#define LED_COLOR_CONNECT Adafruit_NeoPixel::Color(32, 32, 0)
#define LED_COLOR_OPERATION Adafruit_NeoPixel::Color(0, 32, 32)

WiFiMulti wifiMulti;
WebSocketsServer webSocket = WebSocketsServer(81);
Adafruit_NeoPixel pixels(1, LED_PIN, NEO_GRB + NEO_KHZ800);

void hexdump(const void *mem, uint32_t len, uint8_t cols = 16)
{
  const uint8_t *src = (const uint8_t *)mem;
  Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
  for (uint32_t i = 0; i < len; i++)
  {
    if (i % cols == 0)
    {
      Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }
    Serial.printf("%02X ", *src);
    src++;
  }
  Serial.printf("\n");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{

  switch (type)
  {
  case WStype_DISCONNECTED:
#ifdef ENABLE_DEBUG_PRINT
    Serial.printf("[%u] Disconnected!\n", num);
#endif
    pixels.setPixelColor(0, LED_COLOR_NORMAL);
    pixels.show();
    break;
  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(num);
#ifdef ENABLE_DEBUG_PRINT
    Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
#endif

    webSocket.sendTXT(num, "Connected");
    pixels.setPixelColor(0, LED_COLOR_CONNECT);
    pixels.show();
  }
  break;

  case WStype_BIN:
#ifdef ENABLE_DEBUG_PRINT
    Serial0.printf("[%u] get binary length: %u\r\n", num, length);
#endif

    if (length == 0)
    {
      break;
    }

#ifdef ENABLE_DEBUG_PRINT
    hexdump(payload, length);
#endif

    if (payload[0] == 0x57)
    {
      Serial1.write(payload, length);
    }

    break;
  case WStype_TEXT:
  case WStype_ERROR:
  case WStype_FRAGMENT_TEXT_START:
  case WStype_FRAGMENT_BIN_START:
  case WStype_FRAGMENT:
  case WStype_FRAGMENT_FIN:
    break;
  }
}

void setup()
{
#ifdef ENABLE_DEBUG_PRINT
  Serial.begin(115200);

  Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();
#endif

  pixels.begin();

  pixels.setPixelColor(0, LED_COLOR_SETUP);
  pixels.show();

  Serial1.setPins(CH9329_RX_PIN, CH9329_TX_PIN);
  Serial1.begin(9600);

  delay(1000);

  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(100);
  }

#ifdef ENABLE_DEBUG_PRINT
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.flush();
#endif

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  pixels.setPixelColor(0, LED_COLOR_NORMAL);
  pixels.show();
}

void loop()
{
  webSocket.loop();
}