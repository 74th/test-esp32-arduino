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

constexpr int kScreenWidth = 320;
constexpr int kScreenHeight = 240;
constexpr int kPngWidth = 64;
constexpr int kPngHeight = 64;

float pos_x = (kScreenWidth - kPngWidth) * 0.5f;
float pos_y = (kScreenHeight - kPngHeight) * 0.5f;
float dir_x = 1.0f;
float dir_y = 1.0f;
uint32_t last_update_ms = 0;

constexpr float kSpeedX = 80.0f;  // pixels per second
constexpr float kSpeedY = 60.0f;  // pixels per second

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
  const uint32_t now_ms = millis();
  const float delta = (last_update_ms == 0) ? 0.0f : static_cast<float>(now_ms - last_update_ms) / 1000.0f;
  last_update_ms = now_ms;

  pos_x += dir_x * kSpeedX * delta;
  pos_y += dir_y * kSpeedY * delta;

  if (pos_x <= 0.0f)
  {
    pos_x = 0.0f;
    dir_x = 1.0f;
  }
  else if (pos_x >= kScreenWidth - kPngWidth)
  {
    pos_x = static_cast<float>(kScreenWidth - kPngWidth);
    dir_x = -1.0f;
  }

  if (pos_y <= 0.0f)
  {
    pos_y = 0.0f;
    dir_y = 1.0f;
  }
  else if (pos_y >= kScreenHeight - kPngHeight)
  {
    pos_y = static_cast<float>(kScreenHeight - kPngHeight);
    dir_y = -1.0f;
  }

  hud.fillScreen(TFT_BLACK);
  const uint8_t* png_start = _binary_data_74th_64x64_png_start;
  const uint8_t* png_end = _binary_data_74th_64x64_png_end;
  const uint32_t png_size = static_cast<uint32_t>(png_end - png_start);
  hud.drawPng(png_start, png_size, static_cast<int>(pos_x), static_cast<int>(pos_y));

  hud.pushSprite(0, 0);
}
