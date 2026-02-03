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
  Serial.begin(115200);
  delay(100);
  Serial.println("\n\n==== ESP_SR_M5Unified Test ====");

  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.setRotation(1);
  M5.Display.setTextSize(2);
  M5.Display.println("Init...");

  // ES7210 初期化(これで内蔵マイク側の準備が整うことが多い)
  auto mic_cfg = M5.Mic.config();
  mic_cfg.sample_rate = 16000;
  mic_cfg.stereo = true;  // ステレオ設定（ES7210は2チャンネル出力）
  M5.Mic.config(mic_cfg);
  M5.Mic.begin();

  Serial.println("Mic initialized");

  Serial.println("Mic initialized");

  // ESP_SR_M5Unifiedを初期化
  ESP_SR_M5.onEvent(onSrEvent);

  Serial.println("Starting ESP_SR_M5...");

  // コマンド0個でWakeWordのみ
  // 注: AFE内部は常に3チャンネル形式を期待
  // ES7210は2チャンネル出力なので、SR_CHANNELS_STEREOを使用
  // MMフォーマット = 両方ともマイクチャンネル → 内部で[Mic1,Mic2,0]に変換
  bool success = ESP_SR_M5.begin(
    sr_commands,
    sr_commands_len,
    SR_CHANNELS_STEREO,  // 入力は2チャンネル（ステレオ）
    SR_MODE_WAKEWORD,
    "MM"  // M=mic, M=mic → 内部で[Mic,Mic,0]形式に
  );

  Serial.printf("ESP_SR_M5.begin() = %d\n", success);

  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(0, 0);

  if (success) {
    M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Display.println("Listening...");
    M5.Display.println();
    M5.Display.setTextSize(1);
    M5.Display.println("Say: 'Hi Stack Chan'");
    Serial.println("Ready to listen!");
  } else {
    M5.Display.setTextColor(TFT_RED, TFT_BLACK);
    M5.Display.println("Init Failed!");
    Serial.println("ERROR: Init Failed!");
  }
}

void loop()
{
  M5.update();

  // M5.Micから256サンプル(ステレオ=512サンプル分のデータ)を読み取り
  // 2回分を蜂積してESP-SRに渡す
  static int16_t audio_buf[256];
  static int16_t large_buf[512];
  static bool first_half = true;

  bool success = M5.Mic.record(audio_buf, 256);

  static uint32_t loop_count = 0;
  static uint32_t error_count = 0;
  static uint32_t last_log_time = 0;

  if (success) {
    // 256サンプルをバッファに蓄積（2回で512サンプル）
    if (first_half) {
      memcpy(large_buf, audio_buf, 256 * sizeof(int16_t));
      first_half = false;
    } else {
      memcpy(large_buf + 256, audio_buf, 256 * sizeof(int16_t));
      // 512サンプル貫まったのでESP-SRに渡す（ステレオなので512サンプル=256フレーム）
      ESP_SR_M5.feedAudio(large_buf, 512);
      first_half = true;

      // 1秒ごとにログ出力
      uint32_t now = millis();
      if (now - last_log_time >= 1000) {
        int32_t sum = 0;
        for (int i = 0; i < 10; i++) {
          sum += abs(large_buf[i]);
        }
        Serial.printf("loop: count=%d, avg_level=%d, errors=%d, interval=%dms\n",
                      loop_count, sum / 10, error_count, now - last_log_time);
        last_log_time = now;
      }
      loop_count++;
    }
  } else {
    error_count++;
    if (error_count % 100 == 0) {
      Serial.printf("WARNING: M5.Mic.record failed, count=%d\n", error_count);
    }
  }

  // delay()なし - M5.Mic.record()が適切に待機
}
