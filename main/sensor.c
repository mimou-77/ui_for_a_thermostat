
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sensor.h"

#include "dht.h"


static esp_err_t err = ESP_OK;
static const char *TAG = "sensor";


//--- global variables --- 
//definition
float t = 0; //temperature
float h = 0; //humidity





//--- functions --- 


/// @brief configure temperature-humidity sensor dht11
void sensor_init()
{
    // gpio config
    gpio_config_t gpio_cfg = {};
    gpio_cfg.intr_type = GPIO_INTR_DISABLE;
    gpio_cfg.mode = GPIO_MODE_INPUT;
    gpio_cfg.pin_bit_mask = (1ULL<<SENSOR_GPIO);
    gpio_cfg.pull_down_en = 0;
    gpio_cfg.pull_up_en = 1;
    gpio_config(&gpio_cfg);
    ESP_LOGI(TAG, "gpio_config done");
    gpio_reset_pin(SENSOR_GPIO);

    ESP_LOGI(TAG, "pin reset done");

    //sensor args to be passed to cb (to get_t_h)
    sensor_args_t th = 
    {
        .t = t,
        .h = h,
    };

    //create a periodic esp timer
    // each SENSOR_PERIOD_MS, the timer executes the cb get_t_h(&th)
    const esp_timer_create_args_t timer_args = 
    {
        .callback = &get_t_h,
        .name = "sensor_timer",
        .arg = &th,        //args to be passed to the callback
    };

    esp_timer_handle_t sensor_timer_h;

    err = esp_timer_create(&timer_args, &sensor_timer_h);

    ESP_LOGI(TAG, "timer created");

    esp_timer_start_periodic(sensor_timer_h, 1000 * SENSOR_PERIOD_MS);

    ESP_LOGI(TAG, "started timer, time since boot = %lld us", esp_timer_get_time());
}


/// @brief get temperature and humidity values and store them in the extern global variables t and h
/// @param t 
/// @param h 
void get_t_h(void * th)
{   
    ESP_LOGI(TAG, "entered get_t_h");

    sensor_args_t * _th = th;

    t = _th->t;
    h = _th->h;

    err = dht_read_float_data(SENSOR_TYPE, SENSOR_GPIO, &h, &t);

    ESP_LOGI(TAG, "t = %f, h = %f", t, h);
}

