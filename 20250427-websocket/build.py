import pathlib
import subprocess

root_dir = pathlib.Path(__file__).parent.resolve()

def build()->None:
    subprocess.run(["npm", "run", "build"],cwd=root_dir / "html", check=True, text=True)
    pass

if __name__ == "__main__":
    build()
