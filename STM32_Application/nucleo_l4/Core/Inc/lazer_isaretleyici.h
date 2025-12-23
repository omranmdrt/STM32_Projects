#include "main.h"
#include <stdint.h>

#define VIS_LAZER_LOW_LEVEL   		50
#define VIS_LAZER_HIGH_LEVEL  		255

#define VIS_FLASHLIGHT_LEVEL   		255
#define VIS_FLASH_DELAY   			200

#define IR_LAZER_LOW_LEVEL   		50
#define IR_LAZER_HIGH_LEVEL  		255
#define IR_LAZER_DELAY   			200

#define IR_FLASHLIGHT_LOW_LEVEL   	50
#define IR_FLASHLIGHT_HIGH_LEVEL  	255
#define IR_FLASH_DELAY   			200


#define VIS_LAZER_TIM_TYPEDEF  htim1  //the visible lazer timer typedef
#define VIS_LAZER_TIM_CHANNEL  TIM_CHANNEL_1  //the visible lazer timer channel typedef

#define VIS_FLASH_TIM_TYPEDEF  htim1  //the visible flash timer typedef
#define VIS_FLASH_TIM_CHANNEL  TIM_CHANNEL_2  //the visible flash timer channel typedef

#define IR_LAZER_TIM_TYPEDEF   htim1  //the ir lazer timer typedef
#define IR_LAZER_TIM_CHANNEL   TIM_CHANNEL_3  //the ir lazer timer channel typedef


#define IR_FLASH_TIM_TYPEDEF   htim1  //the ir flash timer typedef
#define IR_FLASH_TIM_CHANNEL   TIM_CHANNEL_4  //the ir flash timer channel typedef

#define ADC_IN_TYPEDEF		   hadc1

#define ADC_RESOLUTION         12    // adc is set in 12-bit resolution
#define ADC_MAX_VALUE          (2 ^ (ADC_RESOLUTION) - 1) //ADC max value is 4095

typedef enum {
    MODE_VISIBLE_LASER_LOW,
    MODE_VISIBLE_LASER_HIGH,
    MODE_VISIBLE_FLASHLIGHT,
    MODE_VISIBLE_LASER_FLASHLIGHT,
    MODE_VISIBLE_FLASHLIGHT_FLASH,
	MODE_OFF,
    MODE_IR_LASER_LOW,
    MODE_IR_LASER_HIGH,
    MODE_IR_FLASHLIGHT_LOW,
    MODE_IR_FLASHLIGHT_HIGH,
    MODE_IR_LASER_FLASH,
    MODE_IR_LASER_FLASHLIGHT_SEQUENTIAL
} E_DeviceMode;

extern E_DeviceMode currentMode;
extern E_DeviceMode previousMode;
extern uint32_t adcbuffer;

void SetVisibleLazerPWM(uint16_t pwm_vis_lazer);
void SetVisibleFlashPWM(uint16_t pwm_vis_flash);
void SetIRLazerPWM(uint16_t pwm_ir_lazer);
void SetIRFlashPWM(uint16_t pwm_ir_flash);
void StartPWM();
void StartDMA();
void DeviceModeHandler(void);
void ChangeDeviceMode(uint32_t adc_value);
