#include "TCA9554PWR.h"
#include "esp_check.h"
#include "esp_log.h"

/*****************************************************  Operation register REG   ****************************************************/
static i2c_master_dev_handle_t i2c_handle;

static char *TAG = "tca9554";

uint8_t Read_REG(uint8_t REG)                                // Read the value of the TCA9554PWR register REG
{
    uint8_t bitsStatus = 0;                                                             
    ESP_RETURN_ON_ERROR(i2c_master_transmit_receive(i2c_handle, (uint8_t[]) {REG}, 1, &bitsStatus, 1, 0xffff), TAG, "Read input reg failed");
    return bitsStatus;                                                                
}
void Write_REG(uint8_t REG,uint8_t Data)                    // Write Data to the REG register of the TCA9554PWR
{
    uint8_t data[] = {REG, Data};
    ESP_RETURN_VOID_ON_ERROR(i2c_master_transmit(i2c_handle, data, sizeof(data), 0xffff), TAG, "Write output reg failed");

}
/********************************************************** Set EXIO mode **********************************************************/       
void Mode_EXIO(uint8_t Pin,uint8_t State)                 // Set the mode of the TCA9554PWR Pin. The default is Output mode (output mode or input mode). State: 0= Output mode 1= input mode    
{
    uint8_t bitsStatus = Read_REG(TCA9554_CONFIG_REG);                                
    uint8_t Data = (0x01 << (Pin-1)) | bitsStatus;  
    Write_REG(TCA9554_CONFIG_REG,Data);            
}
void Mode_EXIOS(uint8_t PinState)                        // Set the mode of the 7 pins from the TCA9554PWR with PinState   
{
    Write_REG(TCA9554_CONFIG_REG,PinState);                             
}

/********************************************************** Read EXIO status **********************************************************/       
uint8_t Read_EXIO(uint8_t Pin)                            // Read the level of the TCA9554PWR Pin
{
    uint8_t inputBits =Read_REG(TCA9554_INPUT_REG);                                  
    uint8_t bitStatus = (inputBits >> (Pin-1)) & 0x01;                             
    return bitStatus;                                                              
}
uint8_t Read_EXIOS(void)                                  // Read the level of all pins of TCA9554PWR
{
  uint8_t inputBits = Read_REG(TCA9554_INPUT_REG);                                     
  return inputBits;                                                                    
}

/********************************************************** Set the EXIO output status **********************************************************/  
void Set_EXIO(uint8_t Pin,bool State)                  // Sets the level state of the Pin without affecting the other pins(PIN：1~8)
{
    uint8_t Data = 0;
    uint8_t bitsStatus = Read_REG(TCA9554_OUTPUT_REG);         
    if(Pin < 9 && Pin > 0){     
        if(State)                                     
            Data = (0x01 << (Pin-1)) | bitsStatus;                 
        else
            Data = (~(0x01 << (Pin-1)) & bitsStatus);  
        Write_REG(TCA9554_OUTPUT_REG,Data);
    }
    else                                                                             
        printf("Parameter error, please enter the correct parameter!\r\n");

}
void Set_EXIOS(uint8_t PinState)                     // Set 7 pins to the PinState state such as :PinState=0x23, 0010 0011 state (the highest bit is not used)
{
    Write_REG(TCA9554_OUTPUT_REG,PinState);                                            
}

/********************************************************** Flip EXIO state **********************************************************/  
void Set_Toggle(uint8_t Pin)                              // Flip the level of the TCA9554PWR Pin
{
    uint8_t bitsStatus = Read_EXIO(Pin);                                              
    Set_EXIO(Pin,(bool)!bitsStatus);
}


/********************************************************* TCA9554PWR Initializes the device ***********************************************************/  
void TCA9554PWR_Init(uint8_t PinState)                  // Set the seven pins to PinState state, for example :PinState=0x23, 0010 0011 State (the highest bit is not used) (Output mode or input mode) 0= Output mode 1= Input mode. The default value is output mode
{
    // i2c_master_init();     
    i2c_master_bus_handle_t i2c_bus = esp_ret_i2c_handle();
        esp_err_t ret = ESP_OK;
    const i2c_device_config_t i2c_dev_cfg = {
        .device_address = TCA9554_ADDRESS,
        .scl_speed_hz = 400000,
    };
    ESP_GOTO_ON_ERROR(i2c_master_bus_add_device(i2c_bus, &i2c_dev_cfg, &i2c_handle), err, TAG, "Add new I2C device failed");

    Mode_EXIOS(PinState);   

    err:

}

esp_err_t EXIO_Init(void)
{
    TCA9554PWR_Init(0x00);
    return ESP_OK;
}
