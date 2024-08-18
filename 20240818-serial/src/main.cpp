#include <Arduino.h>

#define BTN GPIO_NUM_3
#define UART_TX GPIO_NUM_7
#define UART_RX GPIO_NUM_8

void on_serial1_receive()
{
  unsigned long now = millis();
  char c[6];
  Serial1.readBytes(c, 6);
  Serial.printf("[%d]on_serial1_receive Data:%0X\n", now, c[4]);
}

void setup()
{
  pinMode(BTN, INPUT_PULLUP);

  // ARDUINO_USB_CDC_ON_BOOT=1 の場合は、USB CDC がデフォルトのシリアルポートになる
  Serial.begin(115200);

  // USB CDC 以外のUARTをピン指定する
  Serial1.setPins(GPIO_NUM_8, GPIO_NUM_7, -1, -1);
  Serial1.onReceive(on_serial1_receive);
  Serial1.begin(9600); // for CH9329
}

void send_ch9329(uint8_t modifier, uint8_t keycode[6])
{
  uint8_t buf[14];

  buf[0] = 0x57;
  buf[1] = 0xAB;
  buf[2] = 0x00;
  buf[3] = 0x02;
  buf[4] = 0x08;
  // モディファイヤ
  buf[5] = modifier;
  buf[6] = 0x00;
  // キーコード1
  buf[7] = keycode[0];
  // キーコード2
  buf[8] = keycode[1];
  // キーコード3
  buf[9] = keycode[2];
  // キーコード4
  buf[10] = keycode[3];
  // キーコード5
  buf[11] = keycode[4];
  // キーコード6
  buf[12] = keycode[5];
  // チェックサム
  uint8_t checksum = 0;
  for (int i = 0; i < 13; i++)
  {
    checksum += buf[i];
  }
  buf[13] = checksum;

  for (int i = 0; i < 14; i++)
  {
    Serial1.write(buf[i]);
  }
}

void send_ch9329_f12()
{
  uint8_t keycode[] = {0x45, 0x00, 0x00, 0x00, 0x00, 0x00};
  send_ch9329(0x00, keycode);
  uint8_t keycode2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  send_ch9329(0x00, keycode2);
  sys_delay_ms(10);
}

bool btn_pushed = false;
uint32_t next_output_timeming = 0;

void loop()
{
  unsigned long now = millis();
  if (digitalRead(BTN) == LOW)
  {
    if (btn_pushed == false)
    {
      Serial.printf("[%d]BTN pushed\r\n", now);
      send_ch9329_f12();
      btn_pushed = true;
    }
  }
  else
  {
    btn_pushed = false;
  }

  if (now > next_output_timeming)
  {
    next_output_timeming = now + 500;
    // Serial1.printf("[%d]Serial1: waiting\r\n", now);
    Serial.printf("[%d]Serial: waiting\r\n", now);
  }
}
