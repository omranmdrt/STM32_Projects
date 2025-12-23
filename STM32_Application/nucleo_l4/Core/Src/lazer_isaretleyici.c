/*
 * lazer_isaretleyici.c
 *
 *  Created on: Feb 18, 2025
 *      Author: dkarakus
 */

#include <lazer_isaretleyici.h>

uint32_t adcbuffer;
E_DeviceMode currentMode  = MODE_OFF;
E_DeviceMode previousMode = MODE_OFF;

void SetVisibleLazerPWM(uint16_t pwm_vis_lazer) {
    __HAL_TIM_SET_COMPARE(&VIS_LAZER_TIM_TYPEDEF, VIS_LAZER_TIM_CHANNEL, pwm_vis_lazer);  // Visible Lazer
}

void SetVisibleFlashPWM(uint16_t pwm_vis_flash) {
    __HAL_TIM_SET_COMPARE(&VIS_FLASH_TIM_TYPEDEF, VIS_FLASH_TIM_CHANNEL, pwm_vis_flash);  // Visible Flash
}

 void SetIRLazerPWM(uint16_t pwm_ir_lazer) {
    __HAL_TIM_SET_COMPARE(&IR_LAZER_TIM_TYPEDEF, IR_LAZER_TIM_CHANNEL, pwm_ir_lazer);  // IR Lazer
}

void SetIRFlashPWM(uint16_t pwm_ir_flash) {
    __HAL_TIM_SET_COMPARE(&IR_FLASH_TIM_TYPEDEF, IR_FLASH_TIM_CHANNEL, pwm_ir_flash);  // IR Lazer
}

void StartPWM(){
	HAL_TIM_PWM_Start(&VIS_LAZER_TIM_TYPEDEF, VIS_LAZER_TIM_CHANNEL);
	HAL_TIM_PWM_Start(&VIS_FLASH_TIM_TYPEDEF, VIS_FLASH_TIM_CHANNEL);
	HAL_TIM_PWM_Start(&IR_LAZER_TIM_TYPEDEF, IR_LAZER_TIM_CHANNEL);
	HAL_TIM_PWM_Start(&IR_FLASH_TIM_TYPEDEF, IR_FLASH_TIM_CHANNEL);

}

void StartDMA(){

	HAL_ADC_Start_DMA(&ADC_IN_TYPEDEF, &adcbuffer, 1);
}


void ChangeDeviceMode(uint32_t adc_value){

	currentMode = adc_value / (ADC_MAX_VALUE / ADC_RESOLUTION);
}

void DeviceModeHandler(void) {

    if (currentMode != previousMode) {
    	//If the mode is changed, reset all PWM signals
    	SetVisibleLazerPWM(0);
    	SetVisibleFlashPWM(0);
    	SetIRLazerPWM(0);
    	SetIRFlashPWM(0);
        previousMode = currentMode;
    }

    switch (currentMode) {

        case MODE_VISIBLE_LASER_LOW:
        	SetVisibleLazerPWM(VIS_LAZER_LOW_LEVEL);
            break;

        case MODE_VISIBLE_LASER_HIGH:
        	SetVisibleLazerPWM(VIS_LAZER_HIGH_LEVEL);
            break;

        case MODE_VISIBLE_FLASHLIGHT:
        	SetVisibleFlashPWM(VIS_FLASHLIGHT_LEVEL);
            break;

        case MODE_VISIBLE_LASER_FLASHLIGHT:
        	SetVisibleLazerPWM(VIS_LAZER_HIGH_LEVEL); // TODO: LOW LEVEL OR HIGH LEVEL??????
        	SetVisibleFlashPWM(VIS_FLASHLIGHT_LEVEL);
            break;

        case MODE_VISIBLE_FLASHLIGHT_FLASH:
        	SetVisibleLazerPWM(VIS_LAZER_HIGH_LEVEL);// TODO: LOW LEVEL OR HIGH LEVEL??????
			SetVisibleFlashPWM(VIS_FLASHLIGHT_LEVEL);
			HAL_Delay(VIS_FLASH_DELAY);
			SetVisibleFlashPWM(0);
			HAL_Delay(VIS_FLASH_DELAY);

            break;

        case MODE_OFF:
            break;

        case MODE_IR_LASER_LOW:
        	SetIRLazerPWM(IR_LAZER_LOW_LEVEL);
            break;

        case MODE_IR_LASER_HIGH:
        	SetIRLazerPWM(IR_LAZER_HIGH_LEVEL);
            break;

        case MODE_IR_FLASHLIGHT_LOW:
        	SetIRFlashPWM(IR_FLASHLIGHT_LOW_LEVEL);
            break;

        case MODE_IR_FLASHLIGHT_HIGH:
        	SetIRFlashPWM(IR_FLASHLIGHT_HIGH_LEVEL);
            break;

        case MODE_IR_LASER_FLASH:
			SetIRLazerPWM(0);
			HAL_Delay(IR_LAZER_DELAY);
			SetIRLazerPWM(IR_LAZER_HIGH_LEVEL);
			HAL_Delay(IR_LAZER_DELAY);
            break;

        case MODE_IR_LASER_FLASHLIGHT_SEQUENTIAL:
        	SetIRLazerPWM(IR_LAZER_HIGH_LEVEL);// TODO: LOW LEVEL OR HIGH LEVEL??????
        	SetIRFlashPWM(IR_FLASHLIGHT_HIGH_LEVEL);// TODO: LOW LEVEL OR HIGH LEVEL??????

            break;
    }

}
