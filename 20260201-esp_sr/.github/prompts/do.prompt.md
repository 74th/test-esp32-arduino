---
agent: agent
---
ESP_SRという、音声認識ライブラリを使って、ウェイクアップワードを実装したい。
しかし、lib/ESP_SR は ESP-SR の Arduino 用のラッパライブラリだけれど、M5Unified もこのライブラリもI2Sマイクを使っているため、両立させるのが難しい。

src/main.cpp の loop に M5.Mic.record(...) を読んで、マイクの音声データを取得しているよ。
lib/ESP_SR を参考に、lib/ESP_SR_M5Unified というライブラリを作り、loop() 内で取得している音声データを lib/ESP_SR_M5Unified 側に渡して、ESP_SR 側で音声認識を行うようにしたい。
