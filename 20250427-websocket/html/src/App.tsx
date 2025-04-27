"use client"
import { WebSocketConnector } from './connector';
import { useRef, useEffect, useState } from 'react';
import TouchPad from './TouchPad';

function App() {
  const wsRef = useRef<WebSocketConnector | null>(null);
  const [connected, setConnected] = useState(false);

  useEffect(() => {
    if (!wsRef.current) {
      wsRef.current = new WebSocketConnector('ws://192.168.1.109:81');
      wsRef.current.onopen = () => {
        console.log('@@1');
        setConnected(true);
      };
      wsRef.current.onclose = () => {
        setConnected(false);
      };
      wsRef.current.connect();
    }
    return () => {
    };
  }, []);

  return (
    <div style={{ width:"100vw", height:"100vh"}}>
      <div style={{ width:"100vw", height:"50vh", position:"absolute", top:"0px"}}>
        {wsRef.current ? <TouchPad ws={wsRef.current} /> : null}
      </div>
      <div style={{position:"absolute", top:"0px", left:"0px"}}>WebSocket接続状態: {connected ? '接続中' : '未接続'}</div>
    </div>
  );
}

export default App
