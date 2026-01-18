# M5UnifiedのSpeakerのテスト

dataにあるvoiceは、"VOICEVOX:No.7" を使用しています。

定数TESTを変更することで、以下のテストを切り替えられます。

```
#define TEST 1
```

- TEST 1: WAV再生のテスト(playWav)
- TEST 2: PCM再生のテスト(playRaw)
- TEST 3: PCMストリーミング再生のテスト(playRawを複数回呼び出し)
