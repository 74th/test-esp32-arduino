#if TEST_NO == 3

// stackchan-arduino を使う

#include <Ticker.h>
#include <M5Unified.h>
#include "Stackchan_servo.h"

// CoreS3 Port A
// static const int SERVO_PIN1 = 1;
// static const int SERVO_PIN2 = 2;

// CoreS3 with m5-pantilt
const int SERVO1_PIN = 6;
const int SERVO2_PIN = 7;

static StackchanSERVO Servo;

#define DELAY_MS 40

namespace test3
{
    void setup()
    {
        auto cfg = M5.config();
        M5.begin(cfg);

        M5.Display.setTextSize(2);

        M5.Display.println("@@1");

        Servo.begin(SERVO1_PIN, 90, 0, SERVO2_PIN, 90, 0, ServoType::PWM);

        M5.Display.println("SG90 Servo test (ServoEasing)");

        delay(5000);
    }

    void loop()
    {
        M5.update();

        // Servo.moveX() などはブロッキングする関数である

        // 引数1: 角度
        // 引数2: 移動までの時間
        // Servo.moveX(110, 30);

        // 左向く
        Servo.moveX(110, 300);

        delay(1000);

        // 右向く
        Servo.moveX(70, 900);

        delay(1000);

        // 正面
        Servo.moveX(90, 300);

        delay(3000);

        // 下向く
        Servo.moveY(100, 300);

        delay(1000);

        // 上向く
        Servo.moveY(80, 900);

        delay(1000);

        // 正面
        Servo.moveY(90, 300);

        delay(3000);

        // 傾聴
        Servo.moveY(80, 200);
        delay(2000);
        // 頷く
        Servo.moveY(100, 200);
        delay(100);
        Servo.moveY(90, 200);
        delay(100);
        Servo.moveY(100, 200);
        delay(100);
        Servo.moveY(90, 200);

        delay(3000);
    }
}

#endif
