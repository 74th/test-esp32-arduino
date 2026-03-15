#include <M5Unified.h>
#include <SCServo.h>
#include <gob_unifiedButton.hpp>

#define SCS_SRIAL_RX_PIN 17 // Port.C Yellow
#define SCS_SRIAL_TX_PIN 18 // Port.C White

SCSCL sc;
goblib::UnifiedButton unifiedButton;

void setID(uint8_t oldID, uint8_t newID) {
    sc.unLockEprom(oldID);
    sc.writeByte(oldID, SCSCL_ID, newID);
    sc.LockEprom(newID);
}

void setup()
{
    M5.begin();
    unifiedButton.begin(&M5.Display);
    log_i("setup start");
    Serial2.begin(1000000, SERIAL_8N1, SCS_SRIAL_RX_PIN, SCS_SRIAL_TX_PIN); // SCSerial RX, TX
    sc.pSerial = &Serial2;
    // sc.WritePos(1, 511, 0, 1500);
    // sc.WritePos(2, 511, 0, 1500);
    log_i("Setup complete");

    delay(300);

    int current1 = sc.ReadCurrent(1);
    int current2 = sc.ReadCurrent(2);
    log_i("current1: %d", current1);
    log_i("current2: %d", current2);

    // ID 2をセットする
    // 一度だけ実行する
    // log_i("Start setup ID 1->2");
    // setID(1, 2);
}

void loop()
{
    M5.update();

    delay(3000);

    M5.Display.clear();
    M5.Display.setCursor(0, 0);

    sc.PWMMode(1, false);
    sc.PWMMode(2, false);

    // 移動
    // 引数1: ID
    // 引数2: 目標位置 (0-1023、511が正面)
    // 引数3: 目標到達時間 (ms、0は無指定)
    // 引数4: 目標到達速度 (0-1023、0は無指定)
    // 引数3、4のどちらかを指定する
    // sc.WritePos(1, 511+128, 0, 100);

    log_i("1)");
    // 右向く
    sc.WritePos(1, 511+128, 0, 100);
    delay(1000);
    // 左向く
    sc.WritePos(1, 511-128, 0, 200);
    delay(1000);
    // 正面
    sc.WritePos(1, 511, 0, 100);

    delay(1000);

    log_i("2)");
    // 上向く
    sc.WritePos(2, 511+128, 0, 100);
    delay(1000);
    // 下向く
    // sc.WritePos(2, 511-128, 0, 200);
    // delay(1000);
    // 正面
    sc.WritePos(2, 511, 0, 100);
}
