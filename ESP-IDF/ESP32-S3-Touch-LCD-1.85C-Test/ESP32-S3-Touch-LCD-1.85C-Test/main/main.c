#include "ST77916.h"
#include "PCF85063.h"
#include "SD_MMC.h"
#include "Wireless.h"
#include "TCA9554PWR.h"
#include "LVGL_Example.h"
#include "BAT_Driver.h"
#include "Audio_Driver.h"
#include "MIC_Speech.h"
#include "esp_board_init.h"
#include "LVGL_Music.h"
void Driver_Loop(void *parameter)
{
    Wireless_Init();

    while(1)
    {
        PCF85063_Loop();
        BAT_Get_Volts();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    vTaskDelete(NULL);
}

void Driver_Init(void)
{
    Flash_Searching();
    BAT_Init();
    EXIO_Init();                    // Example Initialize EXIO
    PCF85063_Init();
    xTaskCreatePinnedToCore(
        Driver_Loop, 
        "Other Driver task",
        4096, 
        NULL, 
        3, 
        NULL, 
        0);
}


void app_main(void)
{

    ESP_ERROR_CHECK(esp_board_init(16000, 2, 16));
    Driver_Init();
    SD_Init();
    LCD_Init();
    Audio_Init();

    MIC_Speech_init();
    Volume_adjustment(60);
    //Play_Music("/sdcard","fc1.mp3");
    LVGL_Init();   // returns the screen object

// /********************* Demo *********************/
    Lvgl_Example1();
    
    // lv_demo_widgets();
    // lv_demo_keypad_encoder();
    // lv_demo_benchmark();
    // lv_demo_stress();
    // lv_demo_music();

    while (1) {
        // raise the task priority of LVGL and/or reduce the handler period can improve the performance
        vTaskDelay(pdMS_TO_TICKS(10));
        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
        lv_timer_handler();
    }

}






