#include <stdio.h>

#include "ui.h"

#include "sensor.h"

#include "bsp/esp-bsp.h"
#include "esp_log.h"












static const char *TAG = "main";


void app_main(void)
{
    //---- config board -----
    //init i2c (will be used for touch)
    bsp_i2c_init();
    //init display ili9341 and lvgl
    bsp_display_cfg_t cfg =
    {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(), //lvgl task and timer default
        .buffer_size = BSP_LCD_H_RES * CONFIG_BSP_LCD_DRAW_BUF_HEIGHT,
        .double_buffer = 0,
        .flags = 
        {
            .buff_dma = true,
        }
    };
    bsp_display_start_with_config(&cfg); //init lvgl, display, brightness, indev
    // set brightness to 100
    bsp_display_backlight_on();

    //--- config sensor --- 
    sensor_init();

    //---- ui ----

    ui();

}
