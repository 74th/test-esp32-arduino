# USB Serial経由のWiFiセットアップ

- USB Serialで、WiFiセットアップするためのAPIを、JSON RPCを実装
- ブラウザのWeb Serial機能を使って、USBのシリアル経由でJSON RPCを呼び出す

## 実装したRPC

```js
// WiFiの設定
{"jsonrpc": "2.0", "method": "set_wifi_creds", "params": {"ssid": "hoge", "pass": "foo"}, "id":1}
// 応答
{"jsonrpc":"2.0","id":1,"result":"ok"}

// WiFiの設定を取得
{"jsonrpc": "2.0", "method": "get_wifi_creds", "id":1}
// 応答
{"jsonrpc":"2.0","id":1,"result":{"ssid":"hoge","pass":"foo"}}

// IPアドレスの取得
{"jsonrpc": "2.0", "method": "get_ip", "id":1}
// 応答
{"jsonrpc":"2.0","id":1,"result":{"ip":"192.168.1.109"}}

// Macアドレスを取得
{"jsonrpc": "2.0", "method": "get_mac_address", "id":1}
// 応答
{"jsonrpc":"2.0","id":1,"result":{"mac_address":"A0:76:4E:B3:67:DC"}}
```

## develop html

Web SerialはHTTPSでしか使えないため、ngrokを利用する。
環境変数 NGROCK_AUTH_TOKEN を設定した上で以下を実行する。

```
cd html
npm run dev
```
