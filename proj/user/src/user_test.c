/*
 * user_test.c
 *
 *  Created on: 2020年11月4日
 *      Author: Sky
 */

#include "user_test.h"
#include "gpio.h"
#include "log.h"

void TesstPinInfo(void) {
	hal_gpio_pin_init(P15, OEN);
	if (hal_gpio_read(P15)) {
		//LOG("\n set Low\n");
		hal_gpio_write(P15, 0);
	} else {
		//LOG("\n set High\n");
		hal_gpio_write(P15, 1);
	}
}
