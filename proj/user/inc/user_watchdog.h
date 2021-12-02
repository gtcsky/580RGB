/*
 * user_watchdog.h
 *
 *  Created on: 2020年10月20日
 *      Author: Sky
 */

#ifndef BG589_INC_USER_WATCHDOG_H_
#define BG589_INC_USER_WATCHDOG_H_
#include "types.h"
#include "watchdog.h"

void	 userWatchDogInit(uint8 task_id);

uint16 userWatchDogProcessEvent(uint8 task_id, uint16 events) ;
#endif /* BG589_INC_USER_WATCHDOG_H_ */
