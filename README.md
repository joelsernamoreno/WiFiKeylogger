Simple hardware keylogger with wireless connectivity (WiFi).

## Hardware requirements:

1. Arduino Pro Micro 3.3V
2. ESP8266-01
3. USB Host

## Pinout:


### Pinout arduino pro micro:





### Pinout ESP8266-01:





### Pinout ESP8266-01 programmer mode:


![Pinout ESP8266-01 programmer mode](https://github.com/joelsernamoreno/WiFiKeylogger/blob/master/images/esp8266programmer.PNG)


### Pinout WiFi Keylogger:


![Pinout WiFi Keylogger](https://github.com/joelsernamoreno/WiFiKeylogger/blob/master/images/keylogger.PNG)


## Instalation:


### Software requirements:

1. Download and Install the Arduino IDE: https://www.arduino.cc/en/main/software
2. Go to File - Preferences. Locate the field "Additional Board Manager URLs:" Add "http://arduino.esp8266.com/stable/package_esp8266com_index.json" without quotes. Click "Ok"

If Arduino IDE gives you the following error: "Error downloading http://arduino.esp8266.com/stable/package_esp8266com_index.json" use "https://github.com/esp8266/Arduino/releases/download/2.3.0/package_esp8266com_index.json" instead.

3. Select Tools - Board - Boards Manager. Search for "esp8266". Install "esp8266 by ESP8266 community version 2.3.0". Click "Close".
4. Download/extract WiFiKeylogger repository.

### Programming mode ESP8266-01:


1. Connect the breadboard to the computer with the circuit shown in the esp8266-01 programmer mode.
2. Open the ESP8266-01.ino sketch from the source folder.
3. Select Tools - Board - "Generic ESP8266 Module".
4. Select Tools – Upload Speed “115200”.
5. Select Tools – Flash Size “512K (64K SPIFFS)”.
6. Select Sketch - "Export Compiled Binary".
7. Now flash the firmware to the ESP chip using one of the following tools:

Linux: https://github.com/AprilBrother/esptool

sudo python esptool.py --port=/dev/ttyUSB0 --baud 115200 write_flash 0x00000 /home/WiFiKeylogger/ESP8266-01/ESP8266-01.ino.generic.bin --flash_size 32m

Windows: https://github.com/nodemcu/nodemcu-flasher

### Upload WifiKeylogger sketch:

1. Connect the breadboard to the computer with the circuit shown in the WiFi Keylogger pinout. 
1. Open the WiFiKeylogger.ino sketch from the source folder.
2. Select Tools - Board – "Arduino Lilypad USB".
3. Select the Port your device is connected to under Tools – Port.
4. Upload the sketch.
