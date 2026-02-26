/*****************************************************************************
  | File        :   LVGL_Driver.c
  
  | help        : 
    The provided LVGL library file must be installed first
******************************************************************************/
#include "LVGL_Driver.h"

uint32_t bufSize;
lv_color_t *disp_draw_buf1;
lv_color_t *disp_draw_buf2;
// static lv_color_t* buf1 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN , MALLOC_CAP_SPIRAM);
// static lv_color_t* buf2 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN , MALLOC_CAP_SPIRAM);
    
static StaticSemaphore_t lvgl_mutex_buf;
static SemaphoreHandle_t lvgl_mutex = NULL;


/**
 * 初始化 LVGL 互斥锁
 * 应在 lv_init() 之前调用
 */
static void lvgl_port_lock_init(void) {
    if(lvgl_mutex == NULL) {
        // 修复：传递静态缓冲区给函数
        lvgl_mutex = xSemaphoreCreateRecursiveMutexStatic(&lvgl_mutex_buf);
        if(lvgl_mutex == NULL) {
            Serial.println("Failed to create LVGL recursive mutex!");
        }
    }
}

/**
 * 锁定 LVGL 操作
 * @param timeout_ms 超时时间（毫秒），0 表示无限等待
 * @return 成功返回 true，失败返回 false
 */
bool lvgl_port_lock(uint32_t timeout_ms) {
    if(lvgl_mutex == NULL) return false;

    TickType_t timeout_ticks = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    
    // 递归锁定
    if(xSemaphoreTakeRecursive(lvgl_mutex, timeout_ticks) == pdTRUE) {
        return true;
    } else {
        Serial.println("LVGL lock timeout!");
        return false;
    }
}

/**
 * 解锁 LVGL 操作
 */
void lvgl_port_unlock(void) {
    if(lvgl_mutex != NULL) {
        // 递归解锁
        xSemaphoreGiveRecursive(lvgl_mutex);
    }
}







/*  Display flushing 
    Displays LVGL content on the LCD
    This function implements associating LVGL data to the LCD screen
*/
void my_disp_flush( lv_display_t *disp, const lv_area_t *area, uint8_t * px_map)
{
  LCD_addWindow(area->x1, area->y1, area->x2, area->y2, ( uint16_t *)px_map);
  lv_display_flush_ready(disp);
}

/*Read the touchpad*/

void my_touchpad_read( lv_indev_t * indev_drv, lv_indev_data_t * data )
{
  Touch_Read_Data();
  if (touch_data.points != 0x00) {
    data->point.x = touch_data.x;
    data->point.y = touch_data.y;
    data->state = LV_INDEV_STATE_PR;
    // printf("LVGL : X=%u Y=%u points=%d\r\n",  touch_data.x , touch_data.y,touch_data.points);
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
  if (touch_data.gesture != NONE ) {    
  }
  touch_data.x = 0;
  touch_data.y = 0;
  touch_data.points = 0;
  touch_data.gesture = NONE;
}

/*use Arduinos millis() as tick source*/
static uint32_t my_tick(void)
{
    return millis();
}

void LVGL_Loop(void *parameter)
{
    while(1)
    {
      lvgl_port_lock(0);
      lv_timer_handler(); /* let the GUI do its work */
      lvgl_port_unlock();
      vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelete(NULL);
}


void Lvgl_Init(void)
{
  lvgl_port_lock_init();

  bufSize = EXAMPLE_LCD_WIDTH * EXAMPLE_LCD_HEIGHT;
  disp_draw_buf1 = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  disp_draw_buf2 = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

  lv_init();

  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(my_tick);

  lv_display_t * disp;

    /*Else create a display yourself*/
  disp = lv_display_create(LCD_WIDTH, LCD_HEIGHT);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_buffers(disp, disp_draw_buf1, disp_draw_buf2, bufSize*2, LV_DISPLAY_RENDER_MODE_FULL);

  /*Initialize the (dummy) input device driver*/
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); //Touchpad should have POINTER type
  lv_indev_set_read_cb(indev, my_touchpad_read);

  xTaskCreatePinnedToCore(LVGL_Loop, "LVGL Loop",8196, NULL, 4, NULL,0);
}

