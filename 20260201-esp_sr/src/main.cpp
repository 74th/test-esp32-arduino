#include <M5Unified.h>
#include <ESP_SR.h>

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Mic.begin();
}

static constexpr size_t MIC_READ_SAMPLES = 256;
const int SAMPLE_RATE = 16000;

void loop()
{
  M5.update();
  if (M5.Mic.isEnabled())
  {
    static int16_t mic_buf[MIC_READ_SAMPLES];
    if (M5.Mic.record(mic_buf, MIC_READ_SAMPLES, SAMPLE_RATE))
    {

    }
  }
}
