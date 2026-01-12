server/main.py に、voicevoxで音声合成を行うコードを追加しようとしている。
ひとまず、音声がちゃんとCoreS3から流れることを確認したい。

main.pyに以下のように`await audio_query.synthesis(speaker=1)`でwavのバイト列が手に入るようになっている。

```
                async with create_voicevox_client() as client:
                    audio_query = await client.create_audio_query(
                        "こんにちは！", speaker=1
                    )
                    with open("voice.wav", "wb") as f:
                        f.write(await audio_query.synthesis(speaker=1))
```

このwavを`voice.wav`として保存するのではなく、CoreS3に送って、CoreS3上のスピーカーで再生させてほしい。
