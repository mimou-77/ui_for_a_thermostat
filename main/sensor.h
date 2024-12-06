
#include "driver/gpio.h"

#include "dht.h"



//--- macros --- 

#define SENSOR_GPIO GPIO_NUM_10
#define SENSOR_TYPE DHT_TYPE_DHT11  //DHT11, AM2301, or SI7021
#define SENSOR_PERIOD_MS (1000 * 5) //periodic timer triggered each 5s  


//--- extern variables --- 
//external declaration 
extern float t; //temperature 
extern float h; //humidity


//--- types --- 

/// @brief humidity and tempereture ; passed as 1 arg to get_t_h 
typedef struct 
{
    float t;
    float h;
} sensor_args_t;


//--- prototypes --- 

/// @brief configure temperature-humidity sensor dht11 
//          + create sensor task (periodic timer)
void sensor_init();


/// @brief get temperature and humidity values 
/// @param t 
/// @param h 
void get_t_h(void *th);


