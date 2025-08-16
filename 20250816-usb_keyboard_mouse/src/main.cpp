#include <Arduino.h>
#include <M5Unified.h>

#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"

USBHID HID;
USBHIDKeyboard Keyboard;
USBHIDMouse Mouse;

void setup()
{
  Serial.begin(115200);
  M5.begin();

  HID.begin();
  USB.begin();
  Keyboard.begin();
  Mouse.begin();
}

void loop()
{
  M5.update();
  if (M5.BtnA.isHolding())
  {
    Mouse.move(0, 4);
    delay(100);
  }
  if (M5.BtnA.wasClicked())
  {
    Keyboard.press('a');
    delay(100);
    Keyboard.releaseAll();
  }
}