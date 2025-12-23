/*
 *  APDS9930.h
 *
 *  Created on: Dec 16, 2025
 *      Author: Umran
 */

#ifndef INC__APDS9930_H_
#define INC__APDS9930_H_

#include "stm32f4xx_hal.h"

// Sensörün I2C adresi (7-bit)
#define APDS9930_I2C_ADDR 0x39

// Register Adresleri
#define APDS9930_ENABLE    0x00
#define APDS9930_ATIME     0x01
#define APDS9930_PTIME     0x02
#define APDS9930_WTIME     0x03
#define APDS9930_CONFIG    0x0D
#define APDS9930_PPULSE    0x0E
#define APDS9930_CONTROL   0x0F
#define APDS9930_ID        0x12
#define APDS9930_STATUS    0x13
#define APDS9930_CDATAL    0x14
#define APDS9930_CDATAH    0x15
#define APDS9930_RDATAL    0x16
#define APDS9930_RDATAH    0x17
#define APDS9930_GDATAL    0x18
#define APDS9930_GDATAH    0x19
#define APDS9930_BDATAL    0x1A
#define APDS9930_BDATAH    0x1B
#define APDS9930_PDATAL    0x1C
#define APDS9930_PDATAH    0x1D

// Fonksiyon Prototipleri (main.c'den çağıracağımız fonksiyonlar)
uint8_t APDS9930_Init(I2C_HandleTypeDef *hi2c);
uint8_t APDS9930_ReadProximity(I2C_HandleTypeDef *hi2c, uint16_t *proximity);
uint8_t APDS9930_ReadAmbientLight(I2C_HandleTypeDef *hi2c, uint16_t *light);


#endif /* INC__APDS9930_H_ */
