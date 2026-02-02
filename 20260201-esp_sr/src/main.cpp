#include <M5Unified.h>
#include <ESP_SR_M5Unified.h>

// wakewordだけやりたいならコマンドは 0 個で試す
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
    M5.Display.fillScreen(TFT_GREEN);
    M5.Display.setCursor(10, 10);
    M5.Display.setTextSize(3);
    M5.Display.setTextColor(TFT_BLACK, TFT_GREEN);
    M5.Display.println("WakeWord!");
    M5.Display.println("Detected!");
    // 必要ならここで次の状態へ
    // ESP_SR_M5.setMode(SR_MODE_COMMAND);

    // 2秒後に自動的にWakeWordモードに戻す
    delay(2000);
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setCursor(0, 0);
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Display.println("Listening...");
    ESP_SR_M5.setMode(SR_MODE_WAKEWORD);
    break;

  case SR_EVENT_TIMEOUT:
    // コマンドモードにしてる場合、一定時間で戻す
    ESP_SR_M5.setMode(SR_MODE_WAKEWORD);
    break;

  case SR_EVENT_COMMAND:
    // command_id / phrase_id を使って分岐(今回はwakewordだけなら不要)
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

  // ES7210 初期化(これで内蔵マイク側の準備が整うことが多い)
  auto mic_cfg = M5.Mic.config();
  M5.Mic.setSampleRate(16000);
  M5.Mic.begin();

  // ESP_SR_M5Unifiedを初期化
  ESP_SR_M5.onEvent(onSrEvent);

  // コマンド0個でWakeWordのみ
  bool success = ESP_SR_M5.begin(
    sr_commands,
    sr_commands_len,
    SR_CHANNELS_STEREO,  // CoreS3はステレオマイク
    SR_MODE_WAKEWORD,
    "MN"  // M=mic, N=null
  );

  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(0, 0);

  if (success) {
    M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Display.println("Listening...");
    M5.Display.println();
    M5.Display.setTextSize(1);
    M5.Display.println("Say: 'Hi ESP'");
  } else {
    M5.Display.setTextColor(TFT_RED, TFT_BLACK);
    M5.Display.println("Init Failed!");
  }
}

void loop()
{
  M5.update();

  // M5.Micからオーディオデータを取得
  static int16_t mic_buf[256];
  size_t samples_read = M5.Mic.record(mic_buf, 256);

  if (samples_read > 0) {
    // ESP_SR_M5Unifiedにオーディオデータを送る
    ESP_SR_M5.feedAudio(mic_buf, samples_read);
  }

  // 少し待機（CPU負荷軽減）
  delay(10);
}
