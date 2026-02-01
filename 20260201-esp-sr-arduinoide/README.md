# ESP-SR を使ってWake Up Wordsを実現（動いてない！！/ Not working yet!!）

> [!Caution]
> これで動かせていません！！ Not working yet!!

ESP-SR のサンプルコードを、CoreS3用で動かそうとしたが、動いていない。。。

ボードマネージャにて、"esp32"の3.3.6、"M5Stack"の3.2.5で動作確認。

partitions.csv は、https://github.com/espressif/arduino-esp32/blob/master/tools/partitions/esp_sr_16.csv を使用する。
Arduino IDEのメニューでPartition Schemaを"ESP-SR"に変更するが、"M5Stack"の方はちゃんと聞いているか分からない。。。

esp32だと、自動で Hi,ESP の音声認識モデルが使われる様子。
M5Stackだと、partitions.csvの準備と、別途 srmodels.bin の書き込みが必要。

[srmodels.bin の描き込み ./upload_models.sh](./upload_models.sh)
