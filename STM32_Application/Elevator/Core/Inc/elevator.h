/*
 * elevator.h
 *
 *  Created on: Aug 7, 2025
 *      Author: Umran
 */

#ifndef INC_ELEVATOR_H_
#define INC_ELEVATOR_H_


#include "stm32g0xx_hal.h"



// ASANSÖRÜN DURUMLARINI TANIMLAYAN ENUM YAPISI

typedef enum {
    STATE_IDLE,         // BEKLEME DURUMU
    STATE_MOVING_UP,    // YUKARI HAREKET
    STATE_MOVING_DOWN,  // AŞAĞI HAREKET
    STATE_DOOR_OPEN,    // KAPI AÇIK
    STATE_DOOR_CLOSING, // KAPI KAPANIYOR
    STATE_DOOR_OPENING, // KAPI AÇILIYOR
    STATE_ALARM,        // ALARM DURUMU
    STATE_PHONE         // TELEFON DURUMU

} ElevatorState;

// KATLARI TANIMLAYAN ENUM YAPISI

typedef enum {
    FLOOR_N1 = -1,  // -1. Kat
    FLOOR_0  =  0,  //  0. Kat (Giriş)
    FLOOR_1  =  1,  //  1. Kat
    FLOOR_2  =  2,  //  2. Kat
    FLOOR_3  =  3   //  3. Kat
} Floor;


// ASANSÖRÜN ANA VERİ YAPISI

typedef struct {
    ElevatorState state;      // MEVCUT DURUMU
    Floor current_floor;      // MEVCUT KAT
    Floor target_floor;       // HEDEF KAT
    uint8_t call_queue[5];    // KAT ÇAĞRI SIRASI  (0: ÇAĞRI YOK, 1: CAĞRI VAR)
    uint32_t door_timer;      // KAPI ZAMANLAYICISI
    uint8_t ventilation_on;   // FAN DURUMU (0: KAPALI, 1: AÇIK)
    uint8_t overload_state;   // AŞIRI YÜK DURUMU
} Elevator;

// FONKİSYON PROTOTİPLERİ

void ELEVATOR_Init(void);           // ASANSÖR SİSTEMİ BAŞLATIR
void ELEVATOR_MainLoop(void);       // ASANSÖR ANA DÖNGÜSÜ
void ELEVATOR_AddCall(Floor floor); // KAT ÇAĞRISI EKLER
void ELEVATOR_ProcessCalls(void);   // KAT ÇAĞRILARINI İŞLER
void ELEVATOR_UpdateState(void);    // ASANSÖR DURUMUNU GÜNCELLER
void ELEVATOR_HandleOverload(void);
void ELEVATOR_ClearOverload(void);




#endif /* INC_ELEVATOR_H_ */
