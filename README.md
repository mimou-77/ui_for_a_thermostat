# UI_for_a_thermostat
LVGL ui for a thermostat with updated temperature/humidity values retrieved every 5s from DHT11 sensor.
Uses esp-idf.

> [!NOTE]
> Compatible only with esp32-S3-box3. You can modify the code to make it compatible with your MCU.
> <br/> GPIO configuration for the sensor is in **Sensor.h**
> <br/> UI configuration is in **ui.h**

## Getting started:
clone the repository:
```
git clone https://github.com/mimou-77/ui_for_a_thermostat.git
```
delete build directory 
```
rm -rf ./build
```
build the project via esp-idf
```
idf.py build
```
flash the firmware ; with X: the number of the usb port to which your ESP32-S3-BOX-3 board is connected
```
idf.py flash -p /dev/ttyACMX
```
you can monitor the usb port to see console logs with
```
idf.py monitor -p /dev/ttyACMX
```

