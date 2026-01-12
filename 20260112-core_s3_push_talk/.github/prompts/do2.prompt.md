---
agent: agent
---
src/main.cpp がM5Stack CoreS3からマイクを使ってmulawでデータを読み取って転送するコードになっている。
server/main.py にmulawを受け取ってwavファイルにするサーバーのコードが実装されている。

なんか凄く音質が悪い。
mulawのエンコードが問題かもしれない。
PCM16bitで、直接M5Stack CoreS3からServerに送るコードに変更して。
