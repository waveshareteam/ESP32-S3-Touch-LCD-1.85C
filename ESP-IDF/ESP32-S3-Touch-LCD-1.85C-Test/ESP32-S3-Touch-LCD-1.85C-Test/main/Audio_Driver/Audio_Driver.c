#include "Audio_Driver.h"

static const char *TAG = "AUDIO";

#define MAX_MP3 2
static const char *dec_file_path[MAX_MP3] = {
    "wozai.mp3",
    "fssx.mp3",
};

bool Music_Next_Flag = 0;
uint8_t Volume = Volume_MAX - 50;
esp_asp_handle_t handle = NULL;

void Audio_PA_EN(){

    gpio_set_level(GPIO_NUM_15,1);
}
void Audio_PA_DIS(){
    gpio_set_level(GPIO_NUM_15,0);
}

static int out_data_callback(uint8_t *data, int data_size, void *ctx)
{
    esp_audio_play((int16_t*)data,data_size,500 / portTICK_PERIOD_MS);
    return 0;
}

static int in_data_callback(uint8_t *data, int data_size, void *ctx)
{
    int ret = fread(data, 1, data_size, ctx);
    ESP_LOGD(TAG, "%s-%d,rd size:%d", __func__, __LINE__, ret);
    return ret;
}

static int mock_event_callback(esp_asp_event_pkt_t *event, void *ctx)
{
    if (event->type == ESP_ASP_EVENT_TYPE_MUSIC_INFO) 
    {
        esp_asp_music_info_t info = {0};
        memcpy(&info, event->payload, event->payload_size);
        ESP_LOGI(TAG, "Get info, rate:%d, channels:%d, bits:%d ,bitrate = %d", info.sample_rate, info.channels, info.bits,info.bitrate);
    } 
    else if (event->type == ESP_ASP_EVENT_TYPE_STATE) 
    {
        esp_asp_state_t st = 0;
        memcpy(&st, event->payload, event->payload_size);

        ESP_LOGI(TAG, "Get State, %d,%s", st, esp_audio_simple_player_state_to_str(st));
        /*if (ctx && ((st == ESP_ASP_STATE_STOPPED) || (st == ESP_ASP_STATE_FINISHED) || (st == ESP_ASP_STATE_ERROR))) {
            xSemaphoreGive((SemaphoreHandle_t)ctx);
        }*/
        if(st == ESP_ASP_STATE_FINISHED)
        {
            Music_Next_Flag = 1;
        }
    }
    return 0;
}


void pipeline_init(void)
{
    esp_log_level_set("*", ESP_LOG_INFO);

    gpio_reset_pin(GPIO_NUM_15);                        
    gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);   
    gpio_set_level(GPIO_NUM_15,0);

    esp_asp_cfg_t cfg = {
        .in.cb = NULL,
        .in.user_ctx = NULL,
        .out.cb = out_data_callback,
        .out.user_ctx = NULL,
    };

    esp_gmf_err_t err = esp_audio_simple_player_new(&cfg, &handle);
    TEST_ASSERT_EQUAL(ESP_OK, err);
    err = esp_audio_simple_player_set_event(handle, mock_event_callback, NULL);
}



static void audio_play_with_url(char *url)
{
    esp_asp_state_t state;
    esp_audio_simple_player_get_state(handle, &state);
    if(ESP_ASP_STATE_NONE != state)
    {
        Audio_PA_DIS();
        esp_gmf_err_t err = esp_audio_simple_player_stop(handle);
    }
    // esp_gmf_audio_dec_reconfig_by_sound_info(dec_el, &info);
    esp_gmf_err_t err = esp_audio_simple_player_run(handle, url, NULL);
    Audio_PA_EN();
}



void audio_stop_play(void)
{
    esp_asp_state_t state;
    esp_audio_simple_player_get_state(handle, &state);
    if(ESP_ASP_STATE_NONE != state)
    {
        Audio_PA_DIS();
        esp_gmf_err_t err = esp_audio_simple_player_stop(handle);
    }
}

void Play_Music(const char* directory, const char* fileName)
{
    const int maxPathLength = 100; 
    char filePath[maxPathLength];                                   
    snprintf(filePath, maxPathLength, "file:/%s/%s", directory, fileName);   
    ESP_LOGI(TAG,"play music:%s",filePath);
    audio_play_with_url(filePath);

}

void Music_resume(void)
{

    esp_asp_state_t state;
    esp_audio_simple_player_get_state(handle, &state);
    if(ESP_ASP_STATE_PAUSED == state)
    {
        esp_audio_simple_player_resume(handle);
        Audio_PA_EN();
    }
}
void Music_pause(void)
{

    Audio_PA_DIS();
    esp_audio_simple_player_pause(handle);
    
    
}


void Audio_Init(void) 
{
    pipeline_init();
    Volume_adjustment(Volume);

}
void Volume_adjustment(uint8_t Vol)
{
    if(Vol > Volume_MAX )
        printf("Audio : The volume value is incorrect. Please enter 0 to 21\r\n");
    else  
    {
        esp_audio_set_play_vol(Vol);
        Volume = Vol;
    }
        
    
}

