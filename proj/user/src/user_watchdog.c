/*
 * user_watchdog.c
 *
 *  Created on: 2020年10月20日
 *      Author: Sky
 */
#include "user_watchdog.h"
#include "log.h"
#include "pwrmgr.h"
#include "hal_mcu.h"

void userWatchDogInit(uint8 task_id) {

	//Watchdog_Init(task_id);
}

uint16 userWatchDogProcessEvent(uint8 task_id, uint16 events) {
//	LOG("\n   wdt event \n ");
//	LOG("\n  MOD_LCD_On=%d \n ",hal_pwrmgr_is_lock(MOD_LCD_On));
//	LOG("\n  MOD_Charging=%d \n ",hal_pwrmgr_is_lock(MOD_Charging));
//	LOG("\n  MOD_Exti=%d \n ",hal_pwrmgr_is_lock(MOD_EXTI));
	return Watchdog_ProcessEvent(task_id, events);
}
