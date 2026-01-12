server/docker-compose.yml に以下のコンテナを起動するようにして。
50021 ポートをホストに公開すること。

```
これはVOICEVOX 音声合成エンジンです。
https://github.com/VOICEVOX/voicevox_engine

実行
CPU
docker pull voicevox/voicevox_engine:cpu-latest
docker run --rm -it -p '127.0.0.1:50021:50021' voicevox/voicevox_engine:cpu-latest
```
