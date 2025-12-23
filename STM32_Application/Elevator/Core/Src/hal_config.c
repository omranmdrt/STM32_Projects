/*
 * hal_config.c
 * Bu dosya, donanım soyutlama katmanı (HAL) fonksiyonlarının implementasyonlarını içerir.
 * STM32G030C8T6 mikrodenetleyici için tasarlanmıştır.
 */

#include "hal_config.h"

extern TIM_HandleTypeDef htim1;
extern ADC_HandleTypeDef hadc1;

// DEBOUNCE İÇİN GEREKLİ DEĞİŞKENLER
uint32_t last_button_press_time[10] = {0}; // TUŞLAR İÇİN SON BASILMA ZAMANLARI

// MOTOR YUKARIYA HAREKET ETTİR.
void HAL_MoveMotorUp(void)
{
    HAL_GPIO_WritePin(Motor_Up_GPIO_Port, Motor_Up_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Motor_Down_GPIO_Port, Motor_Down_Pin, GPIO_PIN_RESET);
    // Motor PWM'i ETKİNLEŞTİR
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, MOTOR_PWM_DUTY_CYCLE_DEFAULT);
}

// MOTOR AŞAĞAYA HAREKET ETTİR.
void HAL_MoveMotorDown(void)
{
    HAL_GPIO_WritePin(Motor_Up_GPIO_Port, Motor_Up_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Motor_Down_GPIO_Port, Motor_Down_Pin, GPIO_PIN_SET);
    // Motor PWM'i ETKİNLEŞTİR
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, MOTOR_PWM_DUTY_CYCLE_DEFAULT);
}

// MOTORU DURDUR
void HAL_StopMotor(void)
{
    HAL_GPIO_WritePin(Motor_Up_GPIO_Port, Motor_Up_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Motor_Down_GPIO_Port, Motor_Down_Pin, GPIO_PIN_RESET);
    // Motor PWM'i DEVERE DIŞI BIRAK
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0); //GÖREV DÖNGÜSÜ SIFIRA AYARLA
}

// KAPI AÇAR
void HAL_OpenDoor(void)
{
    HAL_GPIO_WritePin(Motor_Door_Open_GPIO_Port, Motor_Door_Open_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Motor_Door_Close_GPIO_Port, Motor_Door_Close_Pin, GPIO_PIN_RESET);
}

// KAPI KAPATIR
void HAL_CloseDoor(void)
{
    HAL_GPIO_WritePin(Motor_Door_Open_GPIO_Port, Motor_Door_Open_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Motor_Door_Close_GPIO_Port, Motor_Door_Close_Pin, GPIO_PIN_SET);
}

// KAPI HAREKETİNİ DURDUR
void HAL_StopDoor(void)
{
    HAL_GPIO_WritePin(Motor_Door_Open_GPIO_Port, Motor_Door_Open_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Motor_Door_Close_GPIO_Port, Motor_Door_Close_Pin, GPIO_PIN_RESET);
}

// ALARM AKTİF ET
void HAL_ActivateAlarm(void)
{
    HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
}

// ALARIM PASİF ET
void HAL_DeactivateAlarm(void)
{
    HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);
}

// TELEFONU ÇALDIR
void HAL_ActivatePhoneRing(void)
{
    HAL_GPIO_WritePin(Phone_Module_GPIO_Port, Phone_Module_Pin, GPIO_PIN_SET);
}

// TELEFONU DEVRE DIŞI BIRAK
void HAL_DeactivatePhoneRing(void)
{
    HAL_GPIO_WritePin(Phone_Module_GPIO_Port, Phone_Module_Pin, GPIO_PIN_RESET);
}

// FANI KONTROL ET
void HAL_ControlVentilation(uint8_t state)
{
    if (state) {
        HAL_GPIO_WritePin(Fan_Relay_GPIO_Port, Fan_Relay_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(Fan_Relay_GPIO_Port, Fan_Relay_Pin, GPIO_PIN_RESET);
    }
}


// DURUM SORGULAMA FONKSİYONLARI
uint8_t HAL_IsAlarmButtonPressed(void)
{
    return (HAL_GPIO_ReadPin(Alarm_But_GPIO_Port, Alarm_But_Pin) == GPIO_PIN_RESET); // Pull-up olduğu için RESET basılı demektir
}

uint8_t HAL_IsPhoneButtonPressed(void)
{
    return (HAL_GPIO_ReadPin(Phone_But_GPIO_Port, Phone_But_Pin) == GPIO_PIN_RESET);
}

uint8_t HAL_IsDoorOpenButtonPressed(void)
{
    return (HAL_GPIO_ReadPin(Door_Open_But_GPIO_Port, Door_Open_But_Pin) == GPIO_PIN_RESET);
}

uint8_t HAL_IsDoorCloseButtonPressed(void)
{
    return (HAL_GPIO_ReadPin(Door_Close_But_GPIO_Port, Door_Close_But_Pin) == GPIO_PIN_RESET);
}

uint8_t HAL_IsVentilationButtonPressed(void)
{
    return (HAL_GPIO_ReadPin(Fan_But_GPIO_Port, Fan_But_Pin) == GPIO_PIN_RESET);
}

uint8_t HAL_IsFloorButtonPressed(Floor floor)
{
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;

    switch (floor) {
        case FLOOR_N1:
            GPIOx = Floor_N1_But_GPIO_Port;
            GPIO_Pin = Floor_N1_But_Pin;
            break;
        case FLOOR_0:
            GPIOx = Floor_0_But_GPIO_Port;
            GPIO_Pin = Floor_0_But_Pin;
            break;
        case FLOOR_1:
            GPIOx = Floor_1_But_GPIO_Port;
            GPIO_Pin = Floor_1_But_Pin;
            break;
        case FLOOR_2:
            GPIOx = Floor_2_But_GPIO_Port;
            GPIO_Pin = Floor_2_But_Pin;
            break;
        case FLOOR_3:
            GPIOx = Floor_3_But_GPIO_Port;
            GPIO_Pin = Floor_3_But_Pin;
            break;
        default:
            return 0; // Geçersiz kat
    }
    return (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET);
}

uint8_t HAL_IsAtFloor(Floor floor)
{
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;

    switch (floor) {
        case FLOOR_N1:
            GPIOx = Floor_N1_Sensor_GPIO_Port;
            GPIO_Pin = Floor_N1_Sensor_Pin;
            break;
        case FLOOR_0:
            GPIOx = Floor_0_Sensor_GPIO_Port;
            GPIO_Pin = Floor_0_Sensor_Pin;
            break;
        case FLOOR_1:
            GPIOx = Floor_1_Sensor_GPIO_Port;
            GPIO_Pin = Floor_1_Sensor_Pin;
            break;
        case FLOOR_2:
            GPIOx = Floor_2_Sensor_GPIO_Port;
            GPIO_Pin = Floor_2_Sensor_Pin;
            break;
        case FLOOR_3:
            GPIOx = Floor_3_Sensor_GPIO_Port;
            GPIO_Pin = Floor_3_Sensor_Pin;
            break;
        default:
            return 0; // Geçersiz kat
    }
    return (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET); // Sensör aktif olduğunda SET varsayalım
}

uint8_t HAL_IsDoorOpen(void)
{
    return (HAL_GPIO_ReadPin(Door_Open_Sensor_GPIO_Port, Door_Open_Sensor_Pin) == GPIO_PIN_SET);
}

uint8_t HAL_IsDoorClosed(void)
{
    return (HAL_GPIO_ReadPin(Door_Close_Sensor_GPIO_Port, Door_Close_Sensor_Pin) == GPIO_PIN_SET);
}

uint8_t HAL_IsDoorObstacleDetected(void)
{
    return (HAL_GPIO_ReadPin(Door_Obstacle_Sensor_GPIO_Port, Door_Obstacle_Sensor_Pin) == GPIO_PIN_SET);
}

// Eksik fonksiyonları ekleyin
void HAL_BlinkOverloadLED(uint8_t state)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, (state) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// ADC okuma fonksiyonu (gerçek uygulamada ADC kullanılacak)
uint16_t HAL_ReadLoadSensor(void)
{
    // ADC DÖNÜŞÜMÜNÜ BAŞLAT
    HAL_ADC_Start(&hadc1); // hadc1, ADC yapılandırma yapısı
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // DÖNÜŞÜM TAMAMLANANA KADAR BEKLE
    return HAL_ADC_GetValue(&hadc1); // ADC DEĞERİNİ DÖNDÜR
}

// Aşırı yük tespit fonksiyonu
uint8_t HAL_IsOverloadDetected(void)
{
    // Gerçek uygulamada ADC'den okunan değerle karşılaştırma yapılacak
    return (HAL_ReadLoadSensor() > OVERLOAD_THRESHOLD);
}

//// KAT GÖSTERGE AYARLARI
//void HAL_SetFloorDisplay(Floor floor)
//{
//    // Tüm LED'leri kapat
//    HAL_GPIO_WritePin(FLOOR_LED_N1_GPIO_Port, FLOOR_LED_N1_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(FLOOR_LED_0_GPIO_Port , FLOOR_LED_0_Pin , GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(FLOOR_LED_1_GPIO_Port , FLOOR_LED_1_Pin , GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(FLOOR_LED_2_GPIO_Port , FLOOR_LED_2_Pin , GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(FLOOR_LED_3_GPIO_Port , FLOOR_LED_3_Pin , GPIO_PIN_RESET);
//
//    // İlgili katın LED'ini yak
//    switch (floor) {
//        case FLOOR_N1:
//            HAL_GPIO_WritePin(FLOOR_LED_N1_GPIO_Port, FLOOR_LED_N1_Pin, GPIO_PIN_SET);
//            break;
//        case FLOOR_0:
//            HAL_GPIO_WritePin(FLOOR_LED_0_GPIO_Port, FLOOR_LED_0_Pin, GPIO_PIN_SET);
//            break;
//        case FLOOR_1:
//            HAL_GPIO_WritePin(FLOOR_LED_1_GPIO_Port, FLOOR_LED_1_Pin, GPIO_PIN_SET);
//            break;
//        case FLOOR_2:
//            HAL_GPIO_WritePin(FLOOR_LED_2_GPIO_Port, FLOOR_LED_2_Pin, GPIO_PIN_SET);
//            break;
//        case FLOOR_3:
//            HAL_GPIO_WritePin(FLOOR_LED_3_GPIO_Port, FLOOR_LED_3_Pin, GPIO_PIN_SET);
//            break;
//        default:
//            break;
//    }
//}

