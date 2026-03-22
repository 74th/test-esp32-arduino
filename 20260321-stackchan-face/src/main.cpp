#include <M5Unified.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <algorithm>
#include <cstring>
#include <vector>
#include "config.h"

void drawForState()
{
  uint16_t bg_color;
  uint16_t font_color;

  bg_color = TFT_DARKGRAY;
  font_color = TFT_WHITE;

  M5.Display.fillRect(0, 220, 320, 240, bg_color);
  M5.Display.setFont(&fonts::Font2);
  M5.Display.setTextSize(1);
  M5.Display.setTextColor(font_color, bg_color);
  M5.Display.setCursor(10, 222);
}

void drawFace()
{
  uint32_t eye_y = 102;
  uint32_t between_eyes = 135;
  uint32_t eye_size = 8;
  uint32_t mouth_y = 157;
  uint32_t mouth_width = 85;
  uint32_t mouth_height = 4;
  M5.Display.fillCircle(160 - between_eyes / 2, eye_y, eye_size, TFT_WHITE);
  M5.Display.fillCircle(160 + between_eyes / 2, eye_y, eye_size, TFT_WHITE);
  M5.Display.fillRect(160 - mouth_width / 2, mouth_y, mouth_width, mouth_height, TFT_WHITE);
  M5.Display.printf("Idle");
}

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.clear();
  M5.Display.setTextSize(2);
  drawForState();
  drawFace();
}

void loop()
{
}
