#include <Arduino.h>

#include "USB.h"
#include "USBHIDKeyboard.h"

USBHID HID;
USBHIDKeyboard Keyboard;

void setup()
{
  Serial.begin(115200);

  HID.begin();
  USB.begin();
  Keyboard.begin();
}

void loop() {
  delay(10000);
  Keyboard.press('a');
  delay(100);
  Keyboard.releaseAll();
}