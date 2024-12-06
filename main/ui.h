
#pragma once

#include "lvgl.h"


//--- macros --- 
 
// #define MASK_WIDTH 100
// #define MASK_HEIGHT 45

#define T_H_UPDATE_PERIOD_MS (1000 * 5) //same as SENSOR_PERIOD_MS


#define VIOLET 	 (lv_color_hex(0x6a329f))
#define FUSHIA 	 (lv_color_hex(0xc90076))
#define GREEN 	 (lv_color_hex(0x8fce00))
#define LIGHT_RED 	 (lv_color_hex(0xe58383))
#define DARK_RED 	 (lv_color_hex(0xcc0000))
#define LIGHT_BLUE 	 (lv_color_hex(0x6fa8dc))
#define DARK_BLUE 	 (lv_color_hex(0x3d85c6))
#define BEIGE     (lv_color_hex(0x9e917c))

#define GREY 	 (lv_color_hex(0x999999))
#define LIGHT_GREY 	 (lv_color_hex(0xeeeeee))



#define T_MAX_H 	 (30)   //max value of temperature setpoint for heat mode
#define T_MIN_H 	 (-5)   //min value of temperature setpoint for heat mode
#define T_MAX_C 	 (30)   //max value of temperature setpoint for cool mode
#define T_MIN_C 	 (19)   //min value of temperature setpoint for cool mode


//--- types --- 




/// @brief user data to send to the t_h_update cb
typedef struct t_h_val
{
    float t_val ;
    float h_val;
};






//--- headers ---



/// @brief main ui function
void ui();


/// @brief ~~ HEAT: arc value changed cb => updates label_temp_setpoint
/// @param e 
static void h_arc_value_changed_event_cb(lv_event_t * e);

/// @brief ~~ COOL: arc value changed cb => updates label_temp_setpoint
/// @param e 
static void c_arc_value_changed_event_cb(lv_event_t * e);

/// @brief set mode to heat or cool
/// @param m mode 
static void set_mode(char m);

/// @brief switch value changed => update switch color + heat/cool labels colors 
/// @param e 
static void switch_mode_value_changed_cb(lv_event_t * e);

/// @brief cb that executes each time the timer timeouts; 
///         updates values of t and h
/// @param timer 
void update_t_h_cb(lv_timer_t * timer);

