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
> **optional:** delete build directory to have an idea on how to set parameters of the firmware (fonts used, ...)
```
rm -rf ./build
```
> set target as esp32-s3
```
idf.py set-target esp32s3
```
> [!NOTE]
> we added this line to the project's CMakeLists.txt to include the skconfig file to the firmware as the LVGL configuration file (sdkconfig file generated from the menuconfig)
> <br/> be sure to add this line ↑ before the line: **project(ui3_therlostat)** in the CMakeLists.txt
```
set(LVGL_CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/build/config/sdkconfig.h")
```
> open the menuconfig to configure LVGL parameters
```
idf.py menuconfig
```
> go to: Component config > LVGL configuration, here we can configure options like the display's color depth, we can enable widgets, fonts, ...
> <br/> by default, only the **Monserrat_14** font is enabled, we enable other fonts by going to: font usage > enable built-in fonts
> <br/> the fonts used in the project are: 10, 14, 16, 20, 18, 20, 24, 48 , enable them by checking them ✓
> <br/> save the configuration by typing **s** then **enter** the configuration will be saved the file: **sdkconfig.h**

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

