#!/bin/bash
cp ~/Library/Arduino15/packages/esp32/tools/esp32s3-libs/3.3.6/esp_sr/srmodels.bin ./
~/.platformio/penv/bin/python ~/.platformio/packages/tool-esptoolpy/esptool.py  --chip esp32s3 --port /dev/tty.usbmodem114401  --baud 1500000 write_flash 0xd10000 "./srmodels.bin"
