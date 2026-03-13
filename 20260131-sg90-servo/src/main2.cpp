#include <Ticker.h>
#include <M5Unified.h>
// #include <ESP32Servo.h>
#include "ServoEasing.hpp"

// CoreS3 Port A
// static const int SERVO_PIN1 = 1;
// static const int SERVO_PIN2 = 2;

// CoreS3 with m5-pantilt
const int SERVO1_PIN = 6;
const int SERVO2_PIN = 7;

static ServoEasing Servo1;
static ServoEasing Servo2;

#define DELAY_MS 40

namespace test2
{
    void setup()
    {
        auto cfg = M5.config();
        M5.begin(cfg);

        M5.Display.setTextSize(2);
        M5.Display.println("SG90 Servo test (ServoEasing)");

        Servo1.attach(SERVO1_PIN, 90);
        Servo2.attach(SERVO2_PIN, 90);
        delay(5000);
    }

    void loop()
    {
        M5.update();
        // 引数1: 角度
        // 引数2: 秒速角度 度/秒
        // Servo1.startEaseTo(110, 30);

        // 左向く
        Servo1.startEaseTo(110, 30);

        delay(1000);

        // 右向く
        Servo1.startEaseTo(70, 60);

        delay(1000);

        // 正面
        Servo1.startEaseTo(90, 30);

        delay(3000);

        // 下向く
        Servo2.startEaseTo(100, 30);

        delay(1000);

        // 上向く
        Servo2.startEaseTo(80, 60);

        delay(1000);

        // 正面
        Servo2.startEaseTo(90, 30);

        delay(3000);

        // 傾聴
        Servo2.startEaseTo(80, 100);
        delay(2000);
        // 頷く
        Servo2.startEaseTo(100, 100);
        delay(300);
        Servo2.startEaseTo(90, 100);
        delay(300);
        Servo2.startEaseTo(100, 100);
        delay(300);
        Servo2.startEaseTo(90, 100);

        delay(3000);
    }
}
