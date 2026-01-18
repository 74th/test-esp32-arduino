import wave
import io
from pathlib import Path
import asyncio

from vvclient import Client as VVClient

WORKSPACE_ROOT = Path(__file__).parent.parent
DATA_DIR = WORKSPACE_ROOT / "data"

def create_voicevox_client() -> VVClient:
    return VVClient(base_uri="http://localhost:50021")


async def main():
    async with create_voicevox_client() as client:
        print("access voicevox server")
        audio_query = await client.create_audio_query(
            "おはようございます、今日の体調はいかがでしょうか？ 今日の東京は晴れて、とても乾燥して寒い日になりそうです。お出かけの時は暖かくしなさいね。今日こそはジムに行きましょう。", speaker=29
        )
        wav_bytes = await audio_query.synthesis(speaker=29)
        print("done")
        with open(DATA_DIR / "voice.wav", "wb") as f:
            f.write(wav_bytes)
        print("wav file saved")

        with wave.open(io.BytesIO(wav_bytes), "rb") as wf:
            pcm_bytes = wf.readframes(wf.getnframes())
            tts_sample_rate = wf.getframerate()
            tts_channels = wf.getnchannels()
            tts_sample_width = wf.getsampwidth()
        print("tts_sample_rate:", tts_sample_rate)
        print("tts_channels:", tts_channels)
        print("tts_sample_width:", tts_sample_width)
        # tts_sample_rate: 24000
        # tts_channels: 1
        # tts_sample_width: 2

        with open(DATA_DIR / "voice.pcm", "wb") as f:
            f.write(pcm_bytes)
        print("pcm file saved")


if __name__ == "__main__":
    asyncio.run(main())
