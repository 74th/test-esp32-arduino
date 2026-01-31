#include <M5Unified.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <algorithm>
#include <cstring>
#include <vector>
#include "config.h"

extern const uint8_t _binary_data_74th_64x64_png_start[];
extern const uint8_t _binary_data_74th_64x64_png_end[];

LGFX_Sprite hud(&M5.Display);

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);

  hud.setColorDepth(16);
  hud.createSprite(320, 240);
}

void loop()
{
  M5.update();
  hud.fillScreen(TFT_BLACK);
  const uint8_t* png_start = _binary_data_74th_64x64_png_start;
  const uint8_t* png_end = _binary_data_74th_64x64_png_end;
  const uint32_t png_size = static_cast<uint32_t>(png_end - png_start);
  hud.drawPng(png_start, png_size, 140, 100);

  hud.pushSprite(0, 0);
}
