# UART image write speed test

| USB Serial IC       |  Baudrate | Board                   | total(s) | rate(kbit/s) |
| ------------------- | --------: | ----------------------- | -------: | -----------: |
| CH9102F             |   115,200 | 74th-esp32-s3-dev-board |    100.3 |        136.4 |
| CH9102F             |   460,800 | 74th-esp32-s3-dev-board |     27.7 |        494.7 |
| CH9102F             |   921,600 | 74th-esp32-s3-dev-board |     24.5 |        559.2 |
| CH9102F             | 1,500,000 | 74th-esp32-s3-dev-board |     20.5 |        666.4 |
| CH9102F             | 2,000,000 | 74th-esp32-s3-dev-board |     18.8 |        726.9 |
| CH9102F             | 2,500,000 | 74th-esp32-s3-dev-board |    error |        error |
| CH9102F             |   115,200 | M5Stack Stamp C3        |     82.7 |        144.8 |
| CH9102F             |   460,800 | M5Stack Stamp C3        |     26.7 |        447.7 |
| CH9102F             |   921,600 | M5Stack Stamp C3        |     24.7 |        483.7 |
| CH9102F             | 1,500,000 | M5Stack Stamp C3        |     23.3 |        512.9 |
| CH9102F             | 2,000,000 | M5Stack Stamp C3        |     22.2 |        539.8 |
| CH9102F             | 2,500,000 | M5Stack Stamp C3        |    error |        error |
| CH340C              |   115,200 | 74th-esp32-s3-dev-board |    101.0 |        135.5 |
| CH340C              |   460,800 | 74th-esp32-s3-dev-board |     27.8 |        492.3 |
| CH340C              |   921,600 | 74th-esp32-s3-dev-board |     26.1 |        523.3 |
| CH340C              | 1,500,000 | 74th-esp32-s3-dev-board |     22.3 |        612.4 |
| CH340C              | 2,000,000 | 74th-esp32-s3-dev-board |     20.6 |        662.7 |
| CH340C              | 2,500,000 | 74th-esp32-s3-dev-board |    error |        error |
| CH552T              |   500,000 | M5Stack Atom Lite       |     23.1 |        487.0 |
| CH552T              |   750,000 | M5Stack Atom Lite       |     15.8 |        710.0 |
| CH552T              | 1,500,000 | M5Stack Atom Lite       |     12.9 |        869.7 |
| CH552T              | 2,000,000 | M5Stack Atom Lite       |    error |        error |
| Internal USB Serial | 1,500,000 | 74th-esp32-s3-dev-board |     19.3 |        709.5 |
| Internal USB Serial | 2,000,000 | 74th-esp32-s3-dev-board |     19.3 |        710.4 |
| Internal USB Serial | 2,500,000 | 74th-esp32-s3-dev-board |     19.2 |        713.5 |
| Internal USB Serial | 3,000,000 | 74th-esp32-s3-dev-board |     19.4 |        705.7 |
| Internal USB Serial | 4,000,000 | 74th-esp32-s3-dev-board |     19.3 |        708.1 |
| Internal USB Serial | 2,000,000 | M5Stack Stamp C3U       |     12.6 |        952.7 |
| Internal USB Serial | 4,000,000 | M5Stack Stamp C3U       |     12.4 |        965.6 |
| Internal USB Serial |   115,200 | M5Stack Stamp S3        |     11.3 |      1,207.3 |
| Internal USB Serial |   460,800 | M5Stack Stamp S3        |     11.3 |      1,215.6 |
| Internal USB Serial |   921,600 | M5Stack Stamp S3        |     11.3 |      1,214.1 |
| Internal USB Serial | 1,500,000 | M5Stack Stamp S3        |     11.3 |      1,215.0 |
| Internal USB Serial | 1,500,000 | M5Stack Stamp S3        |     11.3 |      1,215.0 |
| Internal USB Serial | 2,000,000 | M5Stack Stamp S3        |     11.2 |      1,222.0 |
| Internal USB Serial | 4,000,000 | M5Stack Stamp S3        |     11.1 |      1,235.3 |
