#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <lvgl.h>
#include "lv_conf.h"
#include <demos/lv_demos.h>
#include <esp_heap_caps.h>
#include "Display_ST77916.h"
#include "Touch_CST816.h"

#define LCD_WIDTH     EXAMPLE_LCD_WIDTH
#define LCD_HEIGHT    EXAMPLE_LCD_HEIGHT
#define LVGL_BUF_LEN  (LCD_WIDTH * LCD_HEIGHT / 10)

#define EXAMPLE_LVGL_TICK_PERIOD_MS  10

void Lvgl_Init(void);
void Lvgl_Loop(void);
bool lvgl_port_lock(uint32_t timeout_ms);
void lvgl_port_unlock(void);