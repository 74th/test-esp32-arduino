#if TEST_NO == 4

// stackchan-arduino を使う

#include <Ticker.h>
#include <M5Unified.h>
#include <ESP32Servo.h>

// CoreS3 Port A
// static const int SERVO_PIN1 = 1;
// static const int SERVO_PIN2 = 2;

// CoreS3 with m5-pantilt
const int SERVOX_PIN = 6;
const int SERVOY_PIN = 7;

#define DELAY_MS 40

namespace test4
{
    class LServo
    {
    protected:
        Servo servoX;
        Servo servoY;

        int8_t current_degree_x;
        int8_t current_degree_y;
        int8_t target_degree_x;
        int8_t target_degree_y;
        uint32_t move_start_time;
        uint32_t move_duration_ms;
        uint32_t last_update_time_ms;
        uint32_t easing_division_ms = 10;

    public:
        LServo() {};
        void begin(int x_pin, int y_pin)
        {
            servoX.setPeriodHertz(50);
            servoX.attach(x_pin, 500, 2400);
            servoY.setPeriodHertz(50);
            servoY.attach(y_pin, 500, 2400);
        }
        void moveX(int16_t degree, uint32_t duration_ms)
        {
        }
        void moveY(int16_t degree, uint32_t duration_ms)
        {
        }
        void loop()
        {
        }
    };

    LServo servo;
    uint32_t loop_start_time = 0;
    int8_t state = 0;

    void setup()
    {
        auto cfg = M5.config();
        M5.begin(cfg);

        M5.Display.setTextSize(2);

        servo.begin(SERVOX_PIN, SERVOY_PIN);

        M5.Display.println("SG90 Servo test (ServoEasing)");

        delay(5000);

        loop_start_time = millis();
    }

    void loop()
    {
        M5.update();

        uint32_t now = millis() - loop_start_time;

        if (state == 0 && now >= 0)
        {
            servo.moveX(110, 300);
            state = 1;
            loop_start_time = now;
        }

        if (state == 1 && now >= 1000)
        {
            servo.moveX(70, 1000);
            state = 2;
            loop_start_time = now;
        }

        if (state == 2 && now >= 2000)
        {
            servo.moveX(90, 1000);
            state = 3;
            loop_start_time = now;
        }

        if (state == 3 && now >= 5000)
        {
            state = 0;
            loop_start_time = now;
        }

        servo.loop();
    }
}

#endif
