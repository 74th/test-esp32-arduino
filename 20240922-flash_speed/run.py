from datetime import datetime
import shlex
import subprocess


PORT = "/dev/ttyACM0"
# PORT = "/dev/ttyUSB0"

CHIP = "esp32s3"
# CHIP = "esp32c3"
# CHIP = "esp32"

if CHIP == "esp32s3":
    FIRMWARE = "/home/nnyn/Downloads/circuitpython/adafruit-circuitpython-espressif_esp32s3_devkitc_1_n16-en_US-9.2.0-beta.0.bin"
elif CHIP == "esp32c3":
    FIRMWARE = "/home/nnyn/Downloads/circuitpython/adafruit-circuitpython-espressif_esp32c3_devkitm_1_n4-en_US-9.1.4.bin"
elif CHIP == "esp32":
    FIRMWARE = "/home/nnyn/Downloads/circuitpython/adafruit-circuitpython-m5stack_atom_lite-en_US-9.1.4.bin"

BAUDRATE = 115_200
# BAUDRATE = 460_800
# BAUDRATE = 500_000
# BAUDRATE = 750_000
# BAUDRATE = 921_600
# BAUDRATE = 1_500_000
# BAUDRATE = 2_000_000
# BAUDRATE = 2_500_000
# BAUDRATE = 3_000_000
# BAUDRATE = 4_000_000

cmd = [
    "esptool.py",
    f"--chip={CHIP}",
    f"--port={PORT}",
    f"--baud={BAUDRATE}",
    "write_flash",
    "-z",
    "0",
    FIRMWARE,
]

print(shlex.join(cmd))

start = datetime.now()
output = subprocess.run(cmd, check=True)
end = datetime.now()

print("Baudrate: ", BAUDRATE)
print("Time taken: ", int((end - start).total_seconds() * 1000), "ms")
