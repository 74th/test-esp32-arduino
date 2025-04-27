"use client";
import { WebSocketConnector } from "./connector";
import { useRef } from "react";

function TouchPad({ ws }: { ws: WebSocketConnector }) {
  const isTouching = useRef(false);
  const lastPos = useRef<{ x: number; y: number } | null>(null);

  const getPoint = (e: React.TouchEvent | React.MouseEvent) => {
    if ("touches" in e) {
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
      console.log("移動量", { dx, dy });
      ws.SendMouseMove(dx, dy, 0);
    }
    lastPos.current = point;
  };

  const handleEnd = () => {
    isTouching.current = false;
    lastPos.current = null;
  };

  return (
    <div
      style={{
        height: "100%",
        width: "99%",
      }}
    >
      <div
        style={{
          width: "100%",
          height: "80%",
          backgroundColor: "black",
          border: "1px solid white",
        }}
        onTouchStart={handleStart}
        onTouchMove={handleMove}
        onTouchEnd={handleEnd}
        onMouseDown={handleStart}
        onMouseMove={(e) => {
          if (isTouching.current) handleMove(e);
        }}
        onMouseUp={handleEnd}
        onMouseLeave={handleEnd}
      ></div>
    </div>
  );
}

export default TouchPad;
