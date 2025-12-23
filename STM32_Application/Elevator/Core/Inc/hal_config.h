/*
 * hal_config.h
 *
 *  Created on: Aug 7, 2025
 *      Author: Umran
 */

#ifndef INC_HAL_CONFIG_H_
#define INC_HAL_CONFIG_H_

#include "stm32g0xx_hal.h"
#include "elevator.h"
#include "main.h"

// ZAMANLAYICI TANIMLAMA
#define DOOR_OPEN_TIME_MS   5000 // KAPININ AÇIK KALMA SÜRESİ (MS)

// DEBOUNCE İÇİN GEREKLİ DEĞİŞKENLER
#define DEBOUNCE_TIME_MS 100 // MS

extern uint32_t last_button_press_time[10]; // TUŞLAR İÇİN SON BASILMA ZAMANLARI

// %50 DUTY CYCLE İÇİN
#define MOTOR_PWM_DUTY_CYCLE_DEFAULT 500 // (999 + 1) * 0.5 = 500
// %75 DUTY CYCLE İÇİN
#define MOTOR_PWM_DUTY_CYCLE_HIGH 750     // (999 + 1) * 0.75 = 750

// AŞIRI YÜK KONTROLÜ İÇİN EŞİK DEĞERİ

#define OVERLOAD_THRESHOLD 3000



// FLOOR BUTTONS

#define Floor_N1_But_Pin GPIO_PIN_0
#define Floor_N1_But_GPIO_Port GPIOA

#define Floor_0_But_Pin GPIO_PIN_1
#define Floor_0_But_GPIO_Port GPIOA

#define Floor_1_But_Pin GPIO_PIN_2
#define Floor_1_But_GPIO_Port GPIOA

#define Floor_2_But_Pin GPIO_PIN_3
#define Floor_2_But_GPIO_Port GPIOA

#define Floor_3_But_Pin GPIO_PIN_4
#define Floor_3_But_GPIO_Port GPIOA

//SPECIAL BUTTONS

#define Alarm_But_Pin GPIO_PIN_5
#define Alarm_But_GPIO_Port GPIOA

#define Door_Open_But_Pin GPIO_PIN_6
#define Door_Open_But_GPIO_Port GPIOA

#define Fan_But_Pin GPIO_PIN_0
#define Fan_But_GPIO_Port GPIOB

#define Phone_But_Pin GPIO_PIN_1
#define Phone_But_GPIO_Port GPIOB

#define Door_Close_But_Pin GPIO_PIN_10
#define Door_Close_But_GPIO_Port GPIOB

// FLOORS SENSOR

#define Floor_3_Sensor_Pin GPIO_PIN_13
#define Floor_3_Sensor_GPIO_Port GPIOB

#define Floor_2_Sensor_Pin GPIO_PIN_14
#define Floor_2_Sensor_GPIO_Port GPIOB

#define Floor_1_Sensor_Pin GPIO_PIN_15
#define Floor_1_Sensor_GPIO_Port GPIOB

#define Floor_0_Sensor_Pin GPIO_PIN_8
#define Floor_0_Sensor_GPIO_Port GPIOA

#define Floor_N1_Sensor_Pin GPIO_PIN_9
#define Floor_N1_Sensor_GPIO_Port GPIOA

// DOOR SENSORS

#define Door_Open_Sensor_Pin GPIO_PIN_11
#define Door_Open_Sensor_GPIO_Port GPIOB

#define Door_Close_Sensor_Pin GPIO_PIN_12
#define Door_Close_Sensor_GPIO_Port GPIOB

#define Door_Obstacle_Sensor_Pin GPIO_PIN_7
#define Door_Obstacle_Sensor_GPIO_Port GPIOB

// OUTPUT

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

#define Motor_Door_Close_Pin GPIO_PIN_15
#define Motor_Door_Close_GPIO_Port GPIOA

#define Motor_Door_Open_Pin GPIO_PIN_0
#define Motor_Door_Open_GPIO_Port GPIOD



// FONKİSYON PROTOTİPLERİ

void HAL_MoveMotorUp(void);                 // MOTOR YUKARIYA HAREKET ETTİR.
void HAL_MoveMotorDown(void);               // MOTORU AŞAĞI HAREKET ETTİRİR
void HAL_StopMotor(void);                   // MOTORU DURDUR.
void HAL_OpenDoor(void);                    // KAPIYI AÇAR.
void HAL_CloseDoor(void);                   // KAPIYI KAPATIR.
void HAL_StopDoor(void);                    // KAPI HAREKETİNİ DURDURUR
void HAL_ActivateAlarm(void);               // ALARIM EKTİF ET
void HAL_DeactivateAlarm(void);             // ALARIM PASİF ET
void HAL_ActivatePhoneRing(void);           // TELEFON ÇALMA SİNYALİNİ ETKİNLEŞTİR
void HAL_DeactivatePhoneRing(void);         // TELEFON ÇALMA SİNYALİNİ DEVRE DIŞI BIRAK
void HAL_ControlVentilation(uint8_t state); // FAN KONTROL ET
//void HAL_SetFloorDisplay(Floor floor);      // KAT GÖSTERGESİNİ AYARLAR

// DURUM SORGULAMA FONKSİYONLARI
uint8_t HAL_IsAlarmButtonPressed(void);
uint8_t HAL_IsPhoneButtonPressed(void);
uint8_t HAL_IsDoorOpenButtonPressed(void);
uint8_t HAL_IsDoorCloseButtonPressed(void);
uint8_t HAL_IsVentilationButtonPressed(void);
uint8_t HAL_IsFloorButtonPressed(Floor floor);
uint8_t HAL_IsAtFloor(Floor floor);
uint8_t HAL_IsDoorOpen(void);
uint8_t HAL_IsDoorClosed(void);
uint8_t HAL_IsDoorObstacleDetected(void);

uint16_t HAL_ReadLoadSensor(void);
uint8_t HAL_IsOverloadDetected(void);
void HAL_BlinkOverloadLED(uint8_t state);



#endif /* INC_HAL_CONFIG_H_ */
