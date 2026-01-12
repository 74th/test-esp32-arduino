---
agent: agent
---
src/main.cpp にM5Stack CoreS3から、マイクを使って、mulawでデータを読み取って転送するコードはできた。
次はサーバ側のコードを作りたい。

mulawを受け取って、wavファイルにするサーバーのコードを server/main.py に実装して。
ライブラリのインストールが必要であれば、`uv add fastapi` とかで追加して。
