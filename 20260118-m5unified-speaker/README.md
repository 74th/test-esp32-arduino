# M5UnifiedのSpeakerのテスト

dataにあるvoiceは、"VOICEVOX:No.7" を使用しています。

## PCM再生がぐしゃぐしゃになる原因と修正

問題:
- `scripts/create_voicevox_voice.py` で生成した `data/voice.pcm` は 16bit 整数（signed 16-bit, little-endian）で保存されています。
- `src/main.cpp` の `playRaw` 呼び出しにバイト配列（`const uint8_t*`）を渡すと、ライブラリの 8-bit オーバーロードが選択され、16-bit データが 8-bit として解釈されてしまい、音が潰れて聞こえます。

対応:
- `src/main.cpp` の PCM 再生呼び出しを `reinterpret_cast<const int16_t*>` に変更し、16-bit 用オーバーロードを選択するようにしました。
- そのため、配列長はバイト長を `sizeof(int16_t)` で割った値を渡す必要があります（既にその形で計算済み）。

確認方法（ターゲットデバイスで実行）:
1. ビルド:

```bash
pio run
```

2. デバイスに書き込み:

```bash
pio run --target upload
```

3. デバイスで `BtnA` を押して PCM 再生（`TEST` マクロが `2`）を選択し、音を確認してください。

備考:
- エンディアンは WAV の標準であるリトルエンディアンなので、ESP32（リトルエンディアン）ではそのまま使えます。
- もし別のサンプル幅・チャネル構成で問題がある場合は、`scripts/create_voicevox_voice.py` 側での出力パラメータ（サンプルレート、チャネル、サンプル幅）と `main.cpp` の設定を合わせてください。
