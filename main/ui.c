#include "ui.h"

#include "lvgl.h"
#include "esp_log.h"

#include "sensor.h"//to use extern variables t and h


static const char *TAG = "ui";

static lv_obj_t * layer_heat;
static lv_obj_t * layer_cool;
static lv_obj_t * heat_label;
static lv_obj_t * cool_label;
static lv_obj_t * switch_mode;
static lv_style_t switch_style_heat;
static lv_style_t switch_style_cool;
lv_obj_t * label_temp_inside; //value of temp inside



/// @brief main ui function
void ui()
{
    layer_heat = lv_obj_create(lv_scr_act());
    layer_cool = lv_obj_create(lv_scr_act());

    set_mode('h'); //default mode is heat mode

    //switch
    //creation + cb + align
    switch_mode = lv_switch_create(lv_scr_act());
    lv_obj_add_event_cb(switch_mode, switch_mode_value_changed_cb, LV_EVENT_ALL, NULL);
    lv_obj_align(switch_mode, LV_ALIGN_CENTER, 0, 93);
    //style heat properties
    lv_style_set_bg_color(&switch_style_heat, DARK_RED);
    lv_style_set_opa(&switch_style_heat, LV_OPA_100);
    lv_style_set_height(&switch_style_heat, 20);
    lv_style_set_width(&switch_style_heat, 60);
    //style cool properties
    lv_style_set_bg_color(&switch_style_cool, DARK_BLUE);
    lv_style_set_color_filter_dsc(&switch_style_cool, LV_PART_MAIN);
    lv_style_set_opa(&switch_style_cool, LV_OPA_100);
    lv_style_set_height(&switch_style_cool, 20);
    lv_style_set_width(&switch_style_cool, 60);
    //apply styles
    lv_obj_add_style(switch_mode, &switch_style_heat, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(switch_mode, &switch_style_cool, LV_PART_INDICATOR | LV_STATE_CHECKED);

    //heat label + cool label next to switch
    //heat label
    heat_label = lv_label_create(lv_scr_act());
    lv_obj_align(heat_label, LV_ALIGN_CENTER, -58, 93);
    lv_label_set_text(heat_label, "Heat");
    // lv_style_t style_label_hc;
    // lv_style_init(&style_label_hc);
    // lv_style_set_text_font(&style_label_hc, &lv_font_montserrat_20);
    // lv_obj_add_style(heat_label, &style_label_hc, LV_PART_MAIN);

    lv_obj_set_style_text_font(heat_label, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_style_text_color(heat_label, DARK_RED, LV_PART_MAIN);
    //cool label
    cool_label = lv_label_create(lv_scr_act());
    lv_obj_align(cool_label, LV_ALIGN_CENTER, 58, 93);
    lv_label_set_text(cool_label, "Cool");
    // lv_obj_add_style(cool_label, &style_label_hc, LV_PART_MAIN);
    lv_obj_set_style_text_font(cool_label, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_style_text_color(cool_label, GREY, LV_PART_MAIN);

    //temp_inside label (val)
    label_temp_inside = lv_label_create(lv_scr_act());
    lv_obj_align(label_temp_inside, LV_ALIGN_CENTER, 10, 44);
    //label_temp_inside properties
    static lv_style_t style_temp_inside;
    lv_style_init(&style_temp_inside);
    lv_style_set_text_font(&style_temp_inside, &lv_font_montserrat_20);
    lv_obj_add_style(label_temp_inside, &style_temp_inside, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_temp_inside, BEIGE, LV_PART_MAIN);
    lv_obj_add_style(label_temp_inside, &style_temp_inside, LV_PART_MAIN);
    lv_label_set_text_fmt(label_temp_inside, "%.1f", t);


    //timer
    //used to update temperature_inside_val
    lv_timer_t * t_h_update_timer = lv_timer_create(update_t_h_cb, T_H_UPDATE_PERIOD_MS, NULL);
    lv_timer_set_repeat_count(t_h_update_timer, -1); //repeates infinitely
    lv_timer_ready(t_h_update_timer); //start the timer on the next call of
    // lv _timer_handler()

}


//--- functions ---

/// @brief ~~ HEAT: arc value changed cb => updates label_temp_setpoint
/// @param e 
static void h_arc_value_changed_event_cb(lv_event_t * e)
{
    
    lv_obj_t * arc = lv_event_get_target(e);
    lv_obj_t * label = lv_event_get_user_data(e);

    lv_label_set_text_fmt(label, "%.1f", (float)(lv_arc_get_value(arc) * 0.5 + T_MIN_H));  
}


/// @brief ~~ HEAT: arc value changed cb => updates label_temp_setpoint
/// @param e 
static void c_arc_value_changed_event_cb(lv_event_t * e)
{
    
    lv_obj_t * arc = lv_event_get_target(e);
    lv_obj_t * label = lv_event_get_user_data(e);

    lv_label_set_text_fmt(label, "%.1f", (float)(lv_arc_get_value(arc) * 0.5 + T_MIN_C));
}


/// @brief set mode to heat or cool
/// @param m mode 
static void set_mode(char m) 
{

    if(m == 'h') //heat mode
    {

        lv_obj_del(layer_cool);

        lv_obj_t * label_temp_setpoint;
        lv_obj_t * label_temp_unit;
        lv_obj_t * label_temp_inside_icon;
        lv_obj_t * label_temp_inside_unit;
        lv_obj_t * arc;
    

        layer_heat = lv_obj_create(lv_scr_act());
        lv_obj_set_size(layer_heat, lv_disp_get_hor_res(NULL), lv_disp_get_hor_res(NULL)-80);
        lv_obj_center(layer_heat);


        label_temp_setpoint = lv_label_create(layer_heat);
        lv_obj_center(label_temp_setpoint);

        //label_temp_setpoint properties
        lv_obj_set_style_text_color(label_temp_setpoint, DARK_RED, LV_PART_MAIN);
        static lv_style_t style_temp_setpoint_h;
        lv_style_init(&style_temp_setpoint_h);
        lv_style_set_text_font(&style_temp_setpoint_h, &lv_font_montserrat_48);
        lv_obj_add_style(label_temp_setpoint, &style_temp_setpoint_h, LV_PART_MAIN);

        label_temp_unit = lv_label_create(layer_heat);
        lv_obj_align(label_temp_unit, LV_ALIGN_CENTER, 44, -29);
        //label_temp_unit properties
        lv_obj_set_style_text_color(label_temp_unit, DARK_RED, LV_PART_MAIN);
        static lv_style_t style_unit;
        lv_style_init(&style_unit);
        lv_style_set_text_font(&style_unit, &lv_font_montserrat_20);
        lv_obj_add_style(label_temp_unit, &style_unit, LV_PART_MAIN);
        lv_label_set_text(label_temp_unit, "°C");
    
        label_temp_inside_icon = lv_label_create(layer_heat);
        lv_obj_align(label_temp_inside_icon, LV_ALIGN_CENTER, -25, 44);
        //label_temp_inside_icon properties
        lv_obj_set_style_text_color(label_temp_inside_icon, BEIGE, LV_PART_MAIN);
        static lv_style_t style_inside_icon;
        lv_style_init(&style_inside_icon);
        lv_style_set_text_font(&style_inside_icon, &lv_font_montserrat_20);
        lv_obj_add_style(label_temp_inside_icon, &style_inside_icon, LV_PART_MAIN);
        lv_label_set_text(label_temp_inside_icon, LV_SYMBOL_HOME " ");

        //label_temp_inside used to be here

        label_temp_inside_unit = lv_label_create(layer_heat);
        lv_obj_align(label_temp_inside_unit, LV_ALIGN_CENTER, 32, 31);
        //label_temp_inside_unit properties
        lv_obj_set_style_text_color(label_temp_inside_unit, BEIGE, LV_PART_MAIN);
        static lv_style_t style_inside_unit;
        lv_style_init(&style_inside_unit);
        lv_style_set_text_font(&style_inside_unit, &lv_font_montserrat_10);
        lv_obj_add_style(label_temp_inside_unit, &style_inside_unit, LV_PART_MAIN);
        lv_label_set_text_fmt(label_temp_inside_unit, "°C");



        //arc
        arc = lv_arc_create(layer_heat);
        lv_obj_align(arc, LV_ALIGN_CENTER, 0, 0);

        //arc properties
        lv_obj_set_style_arc_width(arc, 7, LV_PART_MAIN); //back part of the arc
        lv_obj_set_style_arc_width(arc, 7, LV_PART_INDICATOR); //front part of the arc
        lv_obj_set_style_arc_color(arc, LIGHT_RED, LV_PART_MAIN);
        lv_obj_set_style_arc_color(arc, DARK_RED, LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(arc, DARK_RED, LV_PART_KNOB);
        lv_obj_set_style_arc_rounded(arc,false,LV_PART_MAIN);
        lv_obj_set_style_arc_rounded(arc,false,LV_PART_INDICATOR);
        lv_obj_set_style_arc_rounded(arc,false,LV_PART_KNOB);
        lv_obj_set_size(arc, 200, 200);
        lv_arc_set_rotation(arc, 135);
        lv_arc_set_bg_angles(arc, 0, 270);
        lv_arc_set_range(arc, 0, 70);
        lv_arc_set_value(arc, 49);

        //arc-label cb
        //add cb : arc value changed => update label_temp_setpoint with the arc value
        lv_obj_add_event_cb(arc, h_arc_value_changed_event_cb, LV_EVENT_VALUE_CHANGED, label_temp_setpoint);

        /*Manually update the label_temp_setpoint for the first time*/
        lv_event_send(arc, LV_EVENT_VALUE_CHANGED, NULL);   

    }
    else
    if (m == 'c') //cool mode
    {
        lv_obj_del(layer_heat);

        lv_obj_t * label_temp_setpoint;
        lv_obj_t * label_temp_unit;
        lv_obj_t * label_temp_inside_icon;
        lv_obj_t * label_temp_inside_unit;
        lv_obj_t * arc;
    

        layer_cool = lv_obj_create(lv_scr_act());
        lv_obj_set_size(layer_cool, lv_disp_get_hor_res(NULL), lv_disp_get_hor_res(NULL)-80);
        lv_obj_center(layer_cool);

        label_temp_setpoint = lv_label_create(layer_cool);
        lv_obj_center(label_temp_setpoint);

        //label_temp_setpoint properties
        lv_obj_set_style_text_color(label_temp_setpoint, DARK_BLUE, LV_PART_MAIN);
        static lv_style_t style_temp_setpoint_h;
        lv_style_init(&style_temp_setpoint_h);
        lv_style_set_text_font(&style_temp_setpoint_h, &lv_font_montserrat_48);
        lv_obj_add_style(label_temp_setpoint, &style_temp_setpoint_h, LV_PART_MAIN);

        label_temp_unit = lv_label_create(layer_cool);
        lv_obj_align(label_temp_unit, LV_ALIGN_CENTER, 44, -29);
        //label_temp_unit properties
        lv_obj_set_style_text_color(label_temp_unit, DARK_BLUE, LV_PART_MAIN);
        static lv_style_t style_unit;
        lv_style_init(&style_unit);
        lv_style_set_text_font(&style_unit, &lv_font_montserrat_20);
        lv_obj_add_style(label_temp_unit, &style_unit, LV_PART_MAIN);
        lv_label_set_text(label_temp_unit, "°C");
    
        label_temp_inside_icon = lv_label_create(layer_cool);
        lv_obj_align(label_temp_inside_icon, LV_ALIGN_CENTER, -25, 44);
        //label_temp_inside_icon properties
        lv_obj_set_style_text_color(label_temp_inside_icon, BEIGE, LV_PART_MAIN);
        static lv_style_t style_inside_icon;
        lv_style_init(&style_inside_icon);
        lv_style_set_text_font(&style_inside_icon, &lv_font_montserrat_20);
        lv_obj_add_style(label_temp_inside_icon, &style_inside_icon, LV_PART_MAIN);
        lv_label_set_text(label_temp_inside_icon, LV_SYMBOL_HOME " ");

        label_temp_inside_unit = lv_label_create(layer_cool);
        lv_obj_align(label_temp_inside_unit, LV_ALIGN_CENTER, 32, 31);
        //label_temp_inside_unit properties
        lv_obj_set_style_text_color(label_temp_inside_unit, BEIGE, LV_PART_MAIN);
        static lv_style_t style_inside_unit;
        lv_style_init(&style_inside_unit);
        lv_style_set_text_font(&style_inside_unit, &lv_font_montserrat_10);
        lv_obj_add_style(label_temp_inside_unit, &style_inside_unit, LV_PART_MAIN);
        lv_label_set_text_fmt(label_temp_inside_unit, "°C");

        //arc
        arc = lv_arc_create(layer_cool);
        lv_obj_align(arc, LV_ALIGN_CENTER, 0, 0);

        //arc properties
        lv_obj_set_style_arc_width(arc, 7, LV_PART_MAIN); //back part of the arc
        lv_obj_set_style_arc_width(arc, 7, LV_PART_INDICATOR); //front part of the arc
        lv_obj_set_style_arc_color(arc, LIGHT_BLUE, LV_PART_MAIN);
        lv_obj_set_style_arc_color(arc, DARK_BLUE, LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(arc, DARK_BLUE, LV_PART_KNOB);
        lv_obj_set_style_arc_rounded(arc,false,LV_PART_MAIN);
        lv_obj_set_style_arc_rounded(arc,false,LV_PART_INDICATOR);
        lv_obj_set_style_arc_rounded(arc,false,LV_PART_KNOB);
        lv_obj_set_size(arc, 200, 200);
        lv_arc_set_rotation(arc, 135);
        lv_arc_set_bg_angles(arc, 0, 270);
        lv_arc_set_range(arc, 0, 22);
        lv_arc_set_value(arc, 6);

        //arc-label cb
        //add cb : arc value changed => update label_temp_setpoint with the arc value
        lv_obj_add_event_cb(arc, c_arc_value_changed_event_cb, LV_EVENT_VALUE_CHANGED, label_temp_setpoint);

        /*Manually update the label_temp_setpoint for the first time*/
        lv_event_send(arc, LV_EVENT_VALUE_CHANGED, NULL); 
        
    } 
}


/// @brief switch value changed => update switch color + heat/cool labels colors 
/// @param e
static void switch_mode_value_changed_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        
        if (lv_obj_has_state(obj, LV_STATE_CHECKED))//cool
        {
            lv_obj_set_style_bg_color(obj,DARK_RED, LV_PART_MAIN );
            lv_obj_add_style(obj, &switch_style_cool, LV_PART_INDICATOR);
            lv_obj_set_style_text_color(cool_label, DARK_BLUE, LV_PART_MAIN);
            lv_obj_set_style_text_color(heat_label, GREY, LV_PART_MAIN);
            ESP_LOGI(TAG, "checked");
            set_mode('c');
            ESP_LOGI(TAG, "mode 'c' set");

        }
        else //not checked //heat
        {
            lv_obj_add_style(obj, &switch_style_heat, LV_PART_MAIN);
            lv_obj_set_style_text_color(heat_label, DARK_RED, LV_PART_MAIN);
            lv_obj_set_style_text_color(cool_label, GREY, LV_PART_MAIN);
            ESP_LOGI(TAG, "not checked");
            set_mode('h');
            ESP_LOGI(TAG, "mode 'h' set");
            
        }
        lv_obj_move_foreground(switch_mode);
        lv_obj_move_foreground(heat_label);
        lv_obj_move_foreground(cool_label);
        lv_obj_move_foreground(label_temp_inside);
    }
}

/// @brief cb that executes each time the timer timeouts; 
///         updates values of t and h
/// @param timer 
void update_t_h_cb(lv_timer_t * timer)
{
    lv_label_set_text_fmt(label_temp_inside, "%.1f", t);
}