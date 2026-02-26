#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "unity.h"

#include "esp_gmf_element.h"
#include "esp_gmf_pipeline.h"
#include "esp_gmf_pool.h"
#include "esp_gmf_alc.h"

#include "esp_audio_simple_player.h"
#include "esp_audio_simple_player_advance.h"
#include "esp_codec_dev.h"
#include "esp_gmf_io.h"
#include "esp_gmf_io_embed_flash.h"
#include "esp_gmf_audio_dec.h"

#include "esp_board_init.h"
#include "driver/gpio.h" 


//#include "LVGL_Music.h"

#define Volume_MAX  100

extern bool Music_Next_Flag;
extern uint8_t Volume;
void Audio_Init(void);

void Play_Music(const char* directory, const char* fileName);
void Music_resume(void);
void Music_pause(void);

void Volume_adjustment(uint8_t Vol);