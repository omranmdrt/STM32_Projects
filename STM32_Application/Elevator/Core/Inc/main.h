/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Floor_N1_But_Pin GPIO_PIN_0
#define Floor_N1_But_GPIO_Port GPIOA
#define Floor_N1_But_EXTI_IRQn EXTI0_1_IRQn
#define Floor_0_But_Pin GPIO_PIN_1
#define Floor_0_But_GPIO_Port GPIOA
#define Floor_0_But_EXTI_IRQn EXTI0_1_IRQn
#define Floor_1_But_Pin GPIO_PIN_2
#define Floor_1_But_GPIO_Port GPIOA
#define Floor_1_But_EXTI_IRQn EXTI2_3_IRQn
#define Floor_2_But_Pin GPIO_PIN_3
#define Floor_2_But_GPIO_Port GPIOA
#define Floor_2_But_EXTI_IRQn EXTI2_3_IRQn
#define Floor_3_But_Pin GPIO_PIN_4
#define Floor_3_But_GPIO_Port GPIOA
#define Floor_3_But_EXTI_IRQn EXTI4_15_IRQn
#define Alarm_But_Pin GPIO_PIN_5
#define Alarm_But_GPIO_Port GPIOA
#define Alarm_But_EXTI_IRQn EXTI4_15_IRQn
#define Door_Open_But_Pin GPIO_PIN_6
#define Door_Open_But_GPIO_Port GPIOA
#define Door_Open_But_EXTI_IRQn EXTI4_15_IRQn
#define Overload_Pin GPIO_PIN_7
#define Overload_GPIO_Port GPIOA
#define Fan_But_Pin GPIO_PIN_0
#define Fan_But_GPIO_Port GPIOB
#define Phone_But_Pin GPIO_PIN_1
#define Phone_But_GPIO_Port GPIOB
#define Door_Close_But_Pin GPIO_PIN_10
#define Door_Close_But_GPIO_Port GPIOB
#define Door_Close_But_EXTI_IRQn EXTI4_15_IRQn
#define Door_Open_Sensor_Pin GPIO_PIN_11
#define Door_Open_Sensor_GPIO_Port GPIOB
#define Door_Open_Sensor_EXTI_IRQn EXTI4_15_IRQn
#define Door_Close_Sensor_Pin GPIO_PIN_12
#define Door_Close_Sensor_GPIO_Port GPIOB
#define Door_Close_Sensor_EXTI_IRQn EXTI4_15_IRQn
#define Floor_3_Sensor_Pin GPIO_PIN_13
#define Floor_3_Sensor_GPIO_Port GPIOB
#define Floor_3_Sensor_EXTI_IRQn EXTI4_15_IRQn
#define Floor_2_Sensor_Pin GPIO_PIN_14
#define Floor_2_Sensor_GPIO_Port GPIOB
#define Floor_2_Sensor_EXTI_IRQn EXTI4_15_IRQn
#define Floor_1_Sensor_Pin GPIO_PIN_15
#define Floor_1_Sensor_GPIO_Port GPIOB
#define Floor_1_Sensor_EXTI_IRQn EXTI4_15_IRQn
#define Floor_0_Sensor_Pin GPIO_PIN_8
#define Floor_0_Sensor_GPIO_Port GPIOA
#define Floor_0_Sensor_EXTI_IRQn EXTI4_15_IRQn
#define Floor_N1_Sensor_Pin GPIO_PIN_9
#define Floor_N1_Sensor_GPIO_Port GPIOA
#define Floor_N1_Sensor_EXTI_IRQn EXTI4_15_IRQn
#define Overload_LED_Pin GPIO_PIN_7
#define Overload_LED_GPIO_Port GPIOC
#define Motor_Door_Close_Pin GPIO_PIN_15
#define Motor_Door_Close_GPIO_Port GPIOA
#define Motor_Door_Open_Pin GPIO_PIN_0
#define Motor_Door_Open_GPIO_Port GPIOD
#define Floor_LEDs_Pin GPIO_PIN_1
#define Floor_LEDs_GPIO_Port GPIOD
#define Phone_Module_Pin GPIO_PIN_2
#define Phone_Module_GPIO_Port GPIOD
#define Fan_Relay_Pin GPIO_PIN_3
#define Fan_Relay_GPIO_Port GPIOD
#define Moto_Pwm_Pin GPIO_PIN_3
#define Moto_Pwm_GPIO_Port GPIOB
#define Motor_Down_Pin GPIO_PIN_4
#define Motor_Down_GPIO_Port GPIOB
#define Motor_Up_Pin GPIO_PIN_5
#define Motor_Up_GPIO_Port GPIOB
#define Buzzer_Pin GPIO_PIN_6
#define Buzzer_GPIO_Port GPIOB
#define Door_Obstacle_Sensor_Pin GPIO_PIN_7
#define Door_Obstacle_Sensor_GPIO_Port GPIOB
#define Door_Obstacle_Sensor_EXTI_IRQn EXTI4_15_IRQn

/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
