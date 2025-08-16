#include <Arduino.h>

#include "USB.h"
#include "USBHIDKeyboard.h"

USBHID HID;
USBHIDKeyboard Keyboard(HID);

void setup()
{
  // CDC(Serial) を使いたい場合
  Serial.begin(115200);

  // HID開始→USB開始（順番はどちらでも動きますが、この順がわかりやすい）
  HID.begin();
  USB.begin();

  // 例: キー送信
  delay(1500); // ホストが列挙し終わるのを待つ
  Keyboard.print("Hello from AtomS3U!");
}

void loop() {}