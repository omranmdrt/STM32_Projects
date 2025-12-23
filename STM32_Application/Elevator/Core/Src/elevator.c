/*
 * elevator.c
 * Bu dosya, asansör kontrol sisteminin ana mantığını ve fonksiyonlarını içerir.
 */

#include "elevator.h"
#include "hal_config.h"

// ASANSÖR NESNESİ
Elevator g_elevator;

// ASANSÖR SİSTEMİNİ BAŞLATIR
void ELEVATOR_Init(void){
	g_elevator.state         = STATE_IDLE;
	g_elevator.current_floor = FLOOR_0;
	g_elevator.target_floor  = FLOOR_0;
	g_elevator.overload_state = 0; // Aşırı yük durumu başlangıçta false (0)

	for (int i = 0; i < 5; i++) {
		g_elevator.call_queue[i] = 0;
	}
	 g_elevator.door_timer     = 0;
	 g_elevator.ventilation_on = 0;

	 // Kat göstergesini başlangıç katına ayarla
//	    HAL_SetFloorDisplay(g_elevator.current_floor);
}

// ASANSÖRÜN ANA DÖNGÜSÜ
void ELEVATOR_MainLoop(void)
{

	// AŞIRI YÜK KONTROLÜ
	if (HAL_IsOverloadDetected() && !g_elevator.overload_state) {
		ELEVATOR_HandleOverload();
		return;
	}

	// AŞIRI YÜK KALKTIĞINDA NORMALE DÖNME
	if (!HAL_IsOverloadDetected() && g_elevator.overload_state) {
		ELEVATOR_ClearOverload();
	}

    // ASANSÖR DURUMU GÜNCELLE
    ELEVATOR_UpdateState();

    // ÇAĞRILARI İŞLE
    ELEVATOR_ProcessCalls();

    // DURUMA GÖRE AKSİYON AL
    switch (g_elevator.state)
    {
        case STATE_IDLE:
            // BEKLEME DURUMUNDA YAPILACAKLAR
            break;

        case STATE_MOVING_UP:
            // YUKARI HAREKET EDERKEN YAPILACAKLARI
            HAL_MoveMotorUp();
            if (HAL_IsAtFloor(g_elevator.target_floor)) {
                HAL_StopMotor();
                g_elevator.current_floor = g_elevator.target_floor;
                g_elevator.state = STATE_DOOR_OPENING;
//                HAL_SetFloorDisplay(g_elevator.current_floor);
            }
            break;

        case STATE_MOVING_DOWN:
            // AŞAĞI HAREKET EDERKEN YAPILACAKLARI
            HAL_MoveMotorDown();
            if (HAL_IsAtFloor(g_elevator.target_floor)) {
                HAL_StopMotor();
                g_elevator.current_floor = g_elevator.target_floor;
                g_elevator.state = STATE_DOOR_OPENING;
//                HAL_SetFloorDisplay(g_elevator.current_floor);
            }
            break;

        case STATE_DOOR_OPENING:
            // KAPI AÇILIRKEN YAPILACAKLAR
            HAL_OpenDoor();
            if (HAL_IsDoorOpen()) {
                g_elevator.state = STATE_DOOR_OPEN;     // KAPI TAM AÇILDI MI KONTROLÜ
                g_elevator.door_timer = HAL_GetTick();  // ZAMANLAYICI BAŞLAT
            }
            break;

        case STATE_DOOR_OPEN:
            // KAPI AÇIKKEN YAPILACAKLAR
            // BELİRLİ BİR SÜRE SONRA VEYA KAPI KAPAT TUŞUNA BASILIRSA KAPANMAYA BAŞLA.
            if ((HAL_GetTick() - g_elevator.door_timer > DOOR_OPEN_TIME_MS) || HAL_IsDoorCloseButtonPressed()) {
                HAL_CloseDoor(); // KAPI KAPATMA SİNYALİNİ VER
                g_elevator.state = STATE_DOOR_CLOSING;
            }
            break;

        case STATE_DOOR_CLOSING:
            // KAPI KAPANIRKEN YAPILACAKLAR
            if (HAL_IsDoorClosed()) {
                g_elevator.state = STATE_IDLE;
            }
            // ENGEL ALGILANIRSA KAPIYI TEKRAR AÇ.
            if (HAL_IsDoorObstacleDetected()) {
                HAL_OpenDoor(); //KAPI AÇMA SİNYALİNİ VER.
                g_elevator.state = STATE_DOOR_OPENING;
            }
            break;

        case STATE_ALARM:
            // ALARIM DURUMUNDA YAPILACAKLAR
            HAL_ActivateAlarm();
            // ALARM BUTONU BIRAKILINCA ALARMI KAPAT
            if (!HAL_IsAlarmButtonPressed()) {
                HAL_DeactivateAlarm();
                g_elevator.state = STATE_IDLE;
            }
            break;

        case STATE_PHONE:
            // TELEFON DURUMUNDA YAPILACAKLR
            HAL_ActivatePhoneRing();
            if (!HAL_IsPhoneButtonPressed()) {
                HAL_DeactivatePhoneRing();
                g_elevator.state = STATE_IDLE;
            }
            break;

        default:

    }
}

// KAT ÇAĞRISI EKLE
void ELEVATOR_AddCall(Floor floor)
{
    if (floor >= FLOOR_N1 && floor <= FLOOR_3) {
        g_elevator.call_queue[floor + 1] = 1; // -1'i 0. indekse, 3'ü 4. indekse eşle
    }
}

// KAT ÇAĞRILARI İŞLER
void ELEVATOR_ProcessCalls(void)
{
    if (g_elevator.state == STATE_IDLE || g_elevator.state == STATE_DOOR_OPEN) {
        // YUKARI YÖNDE ÇAĞIRI ARA
        for (int i = g_elevator.current_floor + 1; i <= FLOOR_3; i++) {
            if (g_elevator.call_queue[i + 1] == 1) {
                g_elevator.target_floor = (Floor)i;
                g_elevator.state = STATE_MOVING_UP;
                g_elevator.call_queue[i + 1] = 0; // ÇAĞRIYI TEMİZLE
                return;
            }
        }

        // AŞAĞI YÖNDE ÇAĞIRI ARA
        for (int i = g_elevator.current_floor - 1; i >= FLOOR_N1; i--) {
            if (g_elevator.call_queue[i + 1] == 1) {
                g_elevator.target_floor = (Floor)i;
                g_elevator.state = STATE_MOVING_DOWN;
                g_elevator.call_queue[i + 1] = 0; // ÇAĞRI TEMZİLE
                return;
            }
        }
    }
}

// ASANSÖR DURUMU TEMİZLE
void ELEVATOR_UpdateState(void)
{
    // Alarm TUŞU BASLI İSE ALARM DURUMUNA GEÇ
    if (HAL_IsAlarmButtonPressed() && g_elevator.state != STATE_ALARM) {
        g_elevator.state = STATE_ALARM;
    }

    // TELEFON TUŞU BASLI İSE ALARM DURUMUNA GEÇ
    if (HAL_IsPhoneButtonPressed() && g_elevator.state != STATE_PHONE) {
        g_elevator.state = STATE_PHONE;
    }

    // KAPI AÇMA
    if (HAL_IsDoorOpenButtonPressed() && (g_elevator.state == STATE_IDLE || g_elevator.state == STATE_DOOR_OPEN || g_elevator.state == STATE_DOOR_CLOSING)) {
        g_elevator.state = STATE_DOOR_OPENING;
    }

}


void ELEVATOR_HandleOverload(void)
{
    // MOTORU DURDUR
    HAL_StopMotor();

    // KAPIYI AÇ
    if (!HAL_IsDoorOpen()) {
        HAL_OpenDoor();
        g_elevator.state = STATE_DOOR_OPENING;
    }

    // ALARM AKTİF ET
    HAL_ActivateAlarm();
    HAL_BlinkOverloadLED(1);  // Aşırı yük LED'ini yak

    // Özel durum bayrağını set et
    g_elevator.overload_state = 1;
}


void ELEVATOR_ClearOverload(void)
{
    // Alarmları pasifleştir
    HAL_DeactivateAlarm();
    HAL_BlinkOverloadLED(0);  // Aşırı yük LED'ini söndür

    // Kullanıcı kapı kapat butonuna basarsa normale dön
    if (HAL_IsDoorCloseButtonPressed()) {
        HAL_CloseDoor();
        g_elevator.state = STATE_DOOR_CLOSING;
        g_elevator.overload_state = 0;
    }
}
