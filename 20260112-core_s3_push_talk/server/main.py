from __future__ import annotations

import struct
import wave
from datetime import datetime
from pathlib import Path

from fastapi import FastAPI, HTTPException, Request, WebSocket, WebSocketDisconnect

app = FastAPI(title="CoreS3 PCM receiver")

BASE_DIR = Path(__file__).resolve().parent
RECORDINGS_DIR = BASE_DIR / "recordings"
RECORDINGS_DIR.mkdir(parents=True, exist_ok=True)

WS_HEADER_FMT = "<4sIHH"  # kind, sample_rate, channels, reserved
WS_HEADER_SIZE = struct.calcsize(WS_HEADER_FMT)
WS_KIND_PCM1 = b"PCM1"


def _ulaw_byte_to_linear(sample: int) -> int:
    """Convert a single μ-law byte to 16-bit PCM (int).

    Kept for compatibility if we ever need to accept μ-law again.
    """

    u_val = (~sample) & 0xFF
    t = ((u_val & 0x0F) << 3) + 0x84
    t <<= (u_val & 0x70) >> 4
    if u_val & 0x80:
        return 0x84 - t
    return t - 0x84


def mulaw_to_pcm16(payload: bytes) -> bytes:
    out = bytearray(len(payload) * 2)
    for i, b in enumerate(payload):
        sample = _ulaw_byte_to_linear(b)
        struct.pack_into("<h", out, i * 2, sample)
    return bytes(out)


@app.get("/health")
async def health() -> dict[str, str]:
    return {"status": "ok"}


@app.websocket("/ws/audio")
async def websocket_audio(ws: WebSocket):
    await ws.accept()
    try:
        while True:
            message = await ws.receive_bytes()
            if len(message) < WS_HEADER_SIZE:
                await ws.close(code=1003, reason="header too short")
                return

            kind, sample_rate, channels, _ = struct.unpack(WS_HEADER_FMT, message[:WS_HEADER_SIZE])
            if kind != WS_KIND_PCM1:
                await ws.close(code=1003, reason="unsupported kind")
                return
            if sample_rate <= 0 or channels <= 0:
                await ws.close(code=1003, reason="invalid header values")
                return

            pcm = message[WS_HEADER_SIZE:]
            sample_width = 2
            if len(pcm) == 0 or len(pcm) % (sample_width * channels) != 0:
                await ws.close(code=1003, reason="invalid pcm length")
                return

            frames = len(pcm) // (sample_width * channels)
            duration_seconds = frames / float(sample_rate)

            timestamp = datetime.utcnow().strftime("%Y%m%d_%H%M%S_%f")
            filename = f"rec_ws_{timestamp}.wav"
            filepath = RECORDINGS_DIR / filename

            with wave.open(str(filepath), "wb") as wav_fp:
                wav_fp.setnchannels(channels)
                wav_fp.setsampwidth(sample_width)
                wav_fp.setframerate(sample_rate)
                wav_fp.writeframes(pcm)

            await ws.send_json(
                {
                    "text": f"Saved as {filename}",
                    "sample_rate": sample_rate,
                    "frames": frames,
                    "channels": channels,
                    "duration_seconds": round(duration_seconds, 3),
                    "path": f"recordings/{filename}",
                }
            )
    except WebSocketDisconnect:
        return


@app.post("/api/v1/audio")
async def receive_audio(request: Request) -> dict[str, object]:
    codec = request.headers.get("X-Codec", "pcm16le").lower()
    if codec not in {"pcm16", "pcm16le", "mulaw", "ulaw"}:
        raise HTTPException(status_code=400, detail="Unsupported codec. Use pcm16le (preferred) or mulaw.")

    sample_rate_raw = request.headers.get("X-Sample-Rate", "16000")
    try:
        sample_rate = int(sample_rate_raw)
        if sample_rate <= 0:
            raise ValueError
    except ValueError:
        raise HTTPException(status_code=400, detail="X-Sample-Rate must be a positive integer.")

    payload = await request.body()
    if not payload:
        raise HTTPException(status_code=400, detail="Request body is empty.")

    try:
        if codec in {"pcm16", "pcm16le"}:
            if len(payload) % 2 != 0:
                raise HTTPException(status_code=400, detail="PCM16 payload size must be even.")
            pcm = payload
        else:
            pcm = mulaw_to_pcm16(payload)
    except HTTPException:
        raise
    except Exception as exc:  # pragma: no cover - defensive
        raise HTTPException(status_code=400, detail=f"Failed to decode audio payload: {exc}")

    frames = len(pcm) // 2
    duration_seconds = frames / float(sample_rate)

    timestamp = datetime.utcnow().strftime("%Y%m%d_%H%M%S_%f")
    filename = f"rec_{timestamp}.wav"
    filepath = RECORDINGS_DIR / filename

    with wave.open(str(filepath), "wb") as wav_fp:
        wav_fp.setnchannels(1)
        wav_fp.setsampwidth(2)
        wav_fp.setframerate(sample_rate)
        wav_fp.writeframes(pcm)

    # Arduino 側は text/audio_mulaw を読む簡易パーサーなので、最低限 text を返す。
    return {
        "text": f"Saved as {filename}",
        "audio_mulaw": "",  # 今回はサーバーで再変換しない
        "sample_rate": sample_rate,
        "frames": frames,
        "duration_seconds": round(duration_seconds, 3),
        "path": f"recordings/{filename}",
    }


def main() -> None:
    import uvicorn

    uvicorn.run("server.main:app", host="0.0.0.0", port=8000, reload=True)


if __name__ == "__main__":
    main()
