#include <M5Unified.h>
#include <ESP32Servo.h>

// CoreS3 Port A
// static const int SERVO_PIN1 = 1;
// static const int SERVO_PIN2 = 2;

// CoreS3 with m5-pantilt
static const int SERVO_PIN1 = 6;
static const int SERVO_PIN2 = 7;

static Servo servo1;
static Servo servo2;

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.setTextSize(2);
  M5.Display.println("SG90 Servo test (ESP32Servo)");

  servo1.setPeriodHertz(50);
  servo1.attach(SERVO_PIN1, 500, 2400);

  servo2.setPeriodHertz(50);
  servo2.attach(SERVO_PIN2, 500, 2400);

  servo1.write(90); // 中央
  servo2.write(90); // 中央
  delay(500);
}

#define DELAY_MS 40

void loop()
{
  M5.update();

  // 90->0->180->0->90
  for (int a = 90; a >= 0; a -= 2)
  {
    servo1.write(a);
    delay(DELAY_MS);
  }
  for (int a = 0; a <= 180; a += 2)
  {
    servo1.write(a);
    delay(DELAY_MS);
  }
  for (int a = 180; a >= 0; a -= 2)
  {
    servo1.write(a);
    delay(DELAY_MS);
  }
  for (int a = 0; a <= 90; a += 2)
  {
    servo1.write(a);
    delay(DELAY_MS);
  }

  // 90->0->180->0->90
  for (int a = 90; a >= 80; a -= 2)
  {
    servo2.write(a);
    delay(DELAY_MS);
  }
  for (int a = 80; a <= 110; a += 2)
  {
    servo2.write(a);
    delay(DELAY_MS);
  }
  for (int a = 110; a >= 80; a -= 2)
  {
    servo2.write(a);
    delay(DELAY_MS);
  }
  for (int a = 80; a <= 90; a += 2)
  {
    servo2.write(a);
    delay(DELAY_MS);
  }

}
