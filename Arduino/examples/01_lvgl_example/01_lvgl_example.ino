#include "Display_ST77916.h"
#include "LVGL_Driver.h"
#include "I2C_Driver.h"
#include <lvgl.h>
#include "lv_conf.h"
#include <demos/lv_demos.h>

void setup()
{
    I2C_Init();
    TCA9554PWR_Init(0x00);   
    Backlight_Init();
    LCD_Init();
    Lvgl_Init();

    lvgl_port_lock(0);

     lv_demo_widgets();
    // lv_demo_benchmark();
    // lv_demo_keypad_encoder();
    // lv_demo_music();
    // lv_demo_stress();


    lvgl_port_unlock();
  
}

void loop() {


}