/**
 * 
 * @copyright Copyright 2021 Espressif Systems (Shanghai) Co. Ltd.
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#pragma once

#include <stdbool.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/**
 * @brief Add dev board pin defination and check target.
 * 
 */

#if CONFIG_ESP32_S3_BOX_BOARD
    #include "esp32_s3_box_board.h"
#elif CONFIG_ESP32_S3_AUDIO_Board
    #include "ESP32_S3_AUDIO_Board.h"
#else 
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Power module of dev board. This can be expanded in the future.
 * 
 */
typedef enum {
    POWER_MODULE_LCD = 1,       /*!< LCD power control */
    POWER_MODULE_AUDIO,         /*!< Audio PA power control */
    POWER_MODULE_ALL = 0xff,    /*!< All module power control */
} power_module_t;

/**
 * @brief Deinit SD card
 * 
 * @param mount_point Path where partition was registered (e.g. "/sdcard")
 * @return 
 *    - ESP_OK: Success
 *    - Others: Fail
 */
esp_err_t bsp_sdcard_deinit(char *mount_point);

/**
 * @brief Init SD crad
 * 
 * @param mount_point Path where partition should be registered (e.g. "/sdcard")
 * @param max_files Maximum number of files which can be open at the same time
 * @return
 *    - ESP_OK                  Success
 *    - ESP_ERR_INVALID_STATE   If esp_vfs_fat_register was already called
 *    - ESP_ERR_NOT_SUPPORTED   If dev board not has SDMMC/SDSPI
 *    - ESP_ERR_NO_MEM          If not enough memory or too many VFSes already registered
 *    - Others                  Fail
 */
esp_err_t bsp_sdcard_init(char *mount_point, size_t max_files);

/**
 * @brief Special config for dev board
 * 
 * @return
 *    - ESP_OK: Success
 *    - Others: Fail
 */
esp_err_t bsp_board_init(uint32_t sample_rate, int channel_format, int bits_per_chan);


esp_err_t bsp_audio_play(const int16_t* data, int length, TickType_t ticks_to_wait);

/**
 * @brief Get the record pcm data.
 * 
 * @param is_get_raw_channel Whether to get the recording data of the original number of channels. 
 *                           Otherwise, the corresponding number of channels will be filtered based on the board.
 * @param buffer The buffer where the data is stored.
 * @param buffer_len The buffer length.
 * @return
 *    - ESP_OK                  Success
 *    - Others                  Fail
 */
esp_err_t bsp_get_feed_data(bool is_get_raw_channel, int16_t *buffer, int buffer_len);

/**
 * @brief Get the record channel number.
 * 
 * @return The record channel number.
 */
int bsp_get_feed_channel(void);

/**
 * @brief Get the input format of the board.
 * 
 * @return The input format of the board, like "MMR"
 */
char* bsp_get_input_format(void);

/**
 * @brief Set play volume
 * 
 * @return
 *    - ESP_OK: Success
 *    - Others: Fail
 */
esp_err_t bsp_audio_set_play_vol(int volume);

/**
 * @brief Get play volume
 * 
 * @return
 *    - ESP_OK: Success
 *    - Others: Fail
 */
esp_err_t bsp_audio_get_play_vol(int *volume);


/**
 * @brief return i2c drv
 * 
 * @return
 *    - drv handle
 *    - NULL
 */
i2c_master_bus_handle_t bsp_ret_i2c_handle(void);

/**
 * @brief return i2s drv
 * 
 * @return
 *    - drv handle
 *    - NULL
 */
esp_codec_dev_handle_t bsp_ret_play_dev(void);

#ifdef __cplusplus
}
#endif
