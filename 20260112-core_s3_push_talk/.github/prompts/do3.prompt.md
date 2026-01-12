---
agent: agent
---
src/main.cpp がM5Stack CoreS3からマイクを使ってPCM16bitでデータを読み取って転送するコードになっている。
server/main.py にPCM16bitを受け取ってwavファイルにするサーバーのコードが実装されている。

これを、WebSocketで音声を送受信するコードに変更して。
のちのちはリアルタイム化したいけれど、一旦5秒分の音声データをまとめて送信してwavファイルに保存する形で良い。

ESP32側はLinks2004/WebSocketsライブラリを使って実装して。
サーバー側はなにかWebSocketライブラリを使って実装して。
fastapiで良いなら`uv add fastapi[websockets]`で追加して。

後々WebSocketでWav以外のデータもやりとりするかもしれないので、メッセージの冒頭にデータ種別を示すヘッダを付与する形で実装して。
