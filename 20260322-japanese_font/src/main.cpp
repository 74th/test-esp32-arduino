#include <M5Unified.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <algorithm>
#include <cstring>
#include <vector>
#include "config.h"

void drawJapanese()
{
  M5.Display.clear();
  M5.Display.setTextSize(1);
  M5.Display.setTextFont(&fonts::lgfxJapanGothic_16);
  M5.Display.printf("ハローワールド！\n");
  M5.Display.printf("こんにちは、世界！\n");
}

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);

  drawJapanese();
}

void loop()
{
}
