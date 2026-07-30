/*****************************************************************************
* | File      	:   PCF85063.c
* | Author      :   Waveshare team
* | Function    :   PCF85063 driver
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-02-02
* | Info        :   Basic version
*
******************************************************************************/
#include "PCF85063.h"
#include "esp_check.h"
#include "esp_log.h"

static char *TAG = "PCF85063";

datetime_t datetime= {0};
static i2c_master_dev_handle_t i2c_handle;
static uint8_t decToBcd(int val);
static int bcdToDec(uint8_t val);

const unsigned char MonthStr[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov","Dec"};
/******************************************************************************
function:	PCF85063 initialized
parameter:
            
Info:Initiate Normal Mode, RTC Run, NO reset, No correction , 24hr format, Internal load capacitane 12.5pf
******************************************************************************/
void PCF85063_Init()
{

	i2c_master_bus_handle_t i2c_bus = esp_ret_i2c_handle();
    esp_err_t ret = ESP_OK;
    const i2c_device_config_t i2c_dev_cfg = {
        .device_address = PCF85063_ADDRESS,
        .scl_speed_hz = 400000,
    };
    i2c_master_bus_add_device(i2c_bus, &i2c_dev_cfg, &i2c_handle);

	uint8_t Value = RTC_CTRL_1_DEFAULT|RTC_CTRL_1_CAP_SEL;

	uint8_t data[2] = {RTC_CTRL_1_ADDR, Value};
    ESP_RETURN_VOID_ON_ERROR(i2c_master_transmit(i2c_handle, data, 2, 0xffff), TAG, "Write output reg failed");

	// datetime_t Now_datetime= {0};
	// Now_datetime.year = 2024;
	// Now_datetime.month = 9;
	// Now_datetime.day = 20;
	// Now_datetime.dotw = 5;
	// Now_datetime.hour = 9;
	// Now_datetime.minute = 50;
	// Now_datetime.second = 0;
	// PCF85063_Set_All(Now_datetime);
}

void PCF85063_Loop(void)
{
  PCF85063_Read_Time(&datetime);
}
/******************************************************************************
function:	Reset PCF85063
parameter:
Info:		
******************************************************************************/
void PCF85063_Reset()
{
	uint8_t Value = RTC_CTRL_1_DEFAULT|RTC_CTRL_1_CAP_SEL|RTC_CTRL_1_SR;
	uint8_t data[] = {RTC_CTRL_1_ADDR, Value};
    ESP_RETURN_VOID_ON_ERROR(i2c_master_transmit(i2c_handle, data, sizeof(data), 0xffff), TAG, "Write output reg failed");
}

/******************************************************************************
function:	Set Time 
parameter:
Info:		
******************************************************************************/
void PCF85063_Set_Time(datetime_t time)
{
	uint8_t data[] = {RTC_SECOND_ADDR,
					  decToBcd(time.second),
					  decToBcd(time.minute),
					  decToBcd(time.hour)};
    ESP_RETURN_VOID_ON_ERROR(i2c_master_transmit(i2c_handle, data, sizeof(data), 0xffff), TAG, "Write output reg failed");
}

/******************************************************************************
function:	Set Date
parameter:
Info:		
******************************************************************************/
void PCF85063_Set_Date(datetime_t date)
{
	uint8_t data[] = {RTC_DAY_ADDR,
					  decToBcd(date.day),
					  decToBcd(date.dotw),
					  decToBcd(date.month),
					  decToBcd(date.year - YEAR_OFFSET)};
	ESP_RETURN_VOID_ON_ERROR(i2c_master_transmit(i2c_handle, data, sizeof(data), 0xffff), TAG, "Write output reg failed");
}

/******************************************************************************
function:	Set Time And Date
parameter:
Info:		
******************************************************************************/
void PCF85063_Set_All(datetime_t time)
{
	uint8_t data[] = {RTC_SECOND_ADDR,
					  decToBcd(time.second),
					  decToBcd(time.minute),
					  decToBcd(time.hour),
					  decToBcd(time.day),
					  decToBcd(time.dotw),
					  decToBcd(time.month),
					  decToBcd(time.year - YEAR_OFFSET)};
	ESP_RETURN_VOID_ON_ERROR(i2c_master_transmit(i2c_handle, data, sizeof(data), 0xffff), TAG, "Write output reg failed");
}

/******************************************************************************
function:	Read Time And Date
parameter:
Info:		
******************************************************************************/
void PCF85063_Read_Time(datetime_t *time)
{
	uint8_t buf[7] = {0};

	ESP_RETURN_VOID_ON_ERROR(i2c_master_transmit_receive(i2c_handle, (uint8_t[]) {RTC_SECOND_ADDR}, 1, buf, 7, 0xffff), TAG, "Read input reg failed");
	time->second = bcdToDec(buf[0] & 0x7F);
	time->minute = bcdToDec(buf[1] & 0x7F);
	time->hour = bcdToDec(buf[2] & 0x3F);
	time->day = bcdToDec(buf[3] & 0x3F);
	time->dotw = bcdToDec(buf[4] & 0x07);
	time->month = bcdToDec(buf[5] & 0x1F);
	time->year = bcdToDec(buf[6])+YEAR_OFFSET;
}

/******************************************************************************
function:	Enable Alarm and Clear Alarm flag
parameter:			
Info:		
******************************************************************************/
void PCF85063_Enable_Alarm()
{
	uint8_t Value = RTC_CTRL_2_DEFAULT | RTC_CTRL_2_AIE;
	Value &= ~RTC_CTRL_2_AF;
	uint8_t data[] = {RTC_CTRL_2_ADDR, Value};
	ESP_RETURN_VOID_ON_ERROR(i2c_master_transmit(i2c_handle, data, sizeof(data), 0xffff), TAG, "Write output reg failed");
}

/******************************************************************************
function:	Get Alarm flay
parameter:			
Info:		
******************************************************************************/
uint8_t PCF85063_Get_Alarm_Flag()
{
	uint8_t Value = 0;
	ESP_RETURN_ON_ERROR(i2c_master_transmit_receive(i2c_handle, (uint8_t[]) {RTC_CTRL_2_ADDR}, 1, &Value, 1, 0xffff), TAG, "Read input reg failed");
	//printf("Value = 0x%x",Value);
	Value &= RTC_CTRL_2_AF | RTC_CTRL_2_AIE;
	return Value;
}

/******************************************************************************
function:	Set Alarm
parameter:			
Info:		
******************************************************************************/
void PCF85063_Set_Alarm(datetime_t time)
{

	uint8_t data[5] ={RTC_SECOND_ALARM,
		decToBcd(time.second)&(~RTC_ALARM),
		decToBcd(time.minute)&(~RTC_ALARM),
		decToBcd(time.hour)&(~RTC_ALARM),
		//decToBcd(time.day)&(~RTC_ALARM),
		//decToBcd(time.dotw)&(~RTC_ALARM)
		RTC_ALARM, 	//disalbe day
		RTC_ALARM	//disalbe weekday
	};
	ESP_RETURN_VOID_ON_ERROR(i2c_master_transmit(i2c_handle, data, sizeof(data), 0xffff), TAG, "Write output reg failed");
}

/******************************************************************************
function:	
parameter:			
Info:		
******************************************************************************/
void PCF85063_Read_Alarm(datetime_t *time)
{
	uint8_t bufss[6] = {0};
	ESP_RETURN_VOID_ON_ERROR(i2c_master_transmit_receive(i2c_handle, (uint8_t[]) {RTC_SECOND_ALARM}, 1, bufss, 6, 0xffff), TAG, "Read input reg failed");
	time->second = bcdToDec(bufss[0] & 0x7F);
	time->minute = bcdToDec(bufss[1] & 0x7F);
	time->hour = bcdToDec(bufss[2] & 0x3F);
	time->day = bcdToDec(bufss[3] & 0x3F);
	time->dotw = bcdToDec(bufss[4] & 0x07);
}


/******************************************************************************
function:	Convert normal decimal numbers to binary coded decimal
parameter:			
Info:		
******************************************************************************/
static uint8_t decToBcd(int val)
{
	return (uint8_t)((val / 10 * 16) + (val % 10));
}

/******************************************************************************
function:	Convert binary coded decimal to normal decimal numbers
parameter:			
Info:		
******************************************************************************/
static int bcdToDec(uint8_t val)
{
	return (int)((val / 16 * 10) + (val % 16));
}

/******************************************************************************
function:	
parameter:	
Info:		
******************************************************************************/
void datetime_to_str(char *datetime_str,datetime_t time)
{
	sprintf(datetime_str, " %d.%d.%d  %d %d:%d:%d ", time.year, time.month, 
			time.day, time.dotw, time.hour, time.minute, time.second);
} 