#include <M5Unified.h>
#include <ESP_I2S.h>
#include <ESP_SR.h>

// CoreS3 built-in mic (ES7210) is on the shared I2S bus. :contentReference[oaicite:3]{index=3}
static constexpr int I2S_PIN_BCK = 34;
static constexpr int I2S_PIN_WS = 33;
static constexpr int I2S_PIN_DIN = 14;

I2SClass i2s;

// wakewordだけやりたいならコマンドは 0 個で試す
// （もし begin が 0 個を許さない実装なら、下の「ダメだった場合」を参照）
static const sr_cmd_t *sr_commands = nullptr;
// static const sr_cmd_t sr_commands[] = {
//     {0, "Test", "TEsT"},
// };
static constexpr size_t sr_commands_len = 0;

void onSrEvent(sr_event_t event, int command_id, int phrase_id)
{
  switch (event)
  {
  case SR_EVENT_WAKEWORD:
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setCursor(0, 0);
    M5.Display.setTextSize(2);
    M5.Display.println("WakeWord!");
    // 必要ならここで次の状態へ
    // ESP_SR.setMode(SR_MODE_COMMAND);
    break;

  case SR_EVENT_TIMEOUT:
    // コマンドモードにしてる場合、一定時間で戻す
    ESP_SR.setMode(SR_MODE_WAKEWORD);
    break;

  case SR_EVENT_COMMAND:
    // command_id / phrase_id を使って分岐（今回はwakewordだけなら不要）
    break;

  default:
    break;
  }
}

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.setRotation(1);
  M5.Display.setTextSize(2);
  M5.Display.println("Init...");

  // ES7210 初期化（これで内蔵マイク側の準備が整うことが多い）
  M5.Mic.begin();

  // I2Sの読み取り側を用意して ESP_SR に渡す :contentReference[oaicite:4]{index=4}
  i2s.setPins(I2S_PIN_BCK, I2S_PIN_WS, -1, I2S_PIN_DIN);
  i2s.setTimeout(1000);

  // CoreS3はデュアルマイクなのでまずは STEREO 設定が無難
  // i2s.begin(I2S_MODE_STD, 16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO);
  i2s.begin(I2S_MODE_STD, 16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO);

  ESP_SR.onEvent(onSrEvent);

  static const sr_cmd_t dummy_cmds[] = {{0, "dummy", "DUMMY"}};

  // ESP_SR.begin(
  //     i2s,
  //     dummy_cmds,
  //     0, // ★commands数を0にする
  //     SR_CHANNELS_STEREO,
  //     SR_MODE_WAKEWORD
  //     // 最後の引数がある版なら nullptr を渡す or 省略（あなたの環境に合わせて）
  // );
  ESP_SR.begin(i2s, sr_commands, sr_commands_len, SR_CHANNELS_STEREO, SR_MODE_WAKEWORD);

  ESP_SR.setMode(SR_MODE_WAKEWORD);

  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.println("Listening...");
}

void loop()
{
  // ESP_SR は内部タスクで回る想定なので loop は基本空でOK :contentReference[oaicite:5]{index=5}
  delay(50);
}
