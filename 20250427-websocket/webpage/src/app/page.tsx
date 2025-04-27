"use client"
import { WebSocketConnector } from '../../../html/src/connector';
import { useRef, useEffect, useState } from 'react';

function TouchPad({ ws }: { ws: WebSocketConnector}) {
  const isTouching = useRef(false);
  const lastPos = useRef<{ x: number; y: number } | null>(null);

  const getPoint = (e: React.TouchEvent | React.MouseEvent) => {
    if ('touches' in e) {
      const touch = e.touches[0];
      return { x: touch.clientX, y: touch.clientY };
    } else {
      return { x: e.clientX, y: e.clientY };
    }
  };

  const handleStart = (e: React.TouchEvent | React.MouseEvent) => {
    isTouching.current = true;
    lastPos.current = getPoint(e);
  };

  const handleMove = (e: React.TouchEvent | React.MouseEvent) => {
    if (!isTouching.current || !lastPos.current) return;
    const point = getPoint(e);
    const dx = point.x - lastPos.current.x;
    const dy = point.y - lastPos.current.y;
    if (dx !== 0 || dy !== 0) {
      console.log('移動量', { dx, dy });
      ws.SendMouseMove(dx, dy);
    }
    lastPos.current = point;
  };

  const handleEnd = () => {
    isTouching.current = false;
    lastPos.current = null;
  };

  return (
    <div
      style={{ width: '100%', height: '400px', backgroundColor: 'lightgray' }}
      onTouchStart={handleStart}
      onTouchMove={handleMove}
      onTouchEnd={handleEnd}
      onMouseDown={handleStart}
      onMouseMove={e => { if (isTouching.current) handleMove(e); }}
      onMouseUp={handleEnd}
      onMouseLeave={handleEnd}
    ></div>
  );
}

export default function Home() {
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
    // クリーンアップで切断
    return () => {
      wsRef.current?.close();
    };
  }, []);

  const handleSend = () => {
    wsRef.current?.send('Hello from Next.js!');
  };

  return (
    <div>
      <div>WebSocket接続状態: {connected ? '接続中' : '未接続'}</div>
      <button onClick={handleSend}>テキスト送信</button>
      <div>
        {wsRef.current ? <TouchPad ws={wsRef.current} /> : null}
      </div>
    </div>
  );
}
