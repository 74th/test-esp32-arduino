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

#define DELAY_MS 40

    void loop()
    {
        M5.update();
        delay(1000);
    }
}
