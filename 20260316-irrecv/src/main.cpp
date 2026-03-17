#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

const uint16_t kRecvPin = GPIO_NUM_32; // 例: 実配線に合わせて変更
const uint32_t kBaudRate = 115200;

// まずはここを大きめに
const uint16_t kCaptureBufferSize = 1024;

// エアコン用に長め
const uint8_t kTimeout = 50;

// UNKNOWNを拾いやすくする
const uint16_t kMinUnknownSize = 6;

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;

void setup()
{
    Serial.begin(kBaudRate);
    irrecv.setUnknownThreshold(kMinUnknownSize);
    irrecv.enableIRIn();
}

void loop()
{
    if (irrecv.decode(&results))
    {
        if (results.overflow)
        {
            Serial.printf("BUFFER FULL at %u\n", kCaptureBufferSize);
        }
        Serial.println(resultToHumanReadableBasic(&results));
        Serial.println(resultToSourceCode(&results));
        irrecv.resume();
    }
}
