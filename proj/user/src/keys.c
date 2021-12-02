/**************************************************************************************************
Filename:       keys.c
Revised:        Date: 2020.9.12
Revision:       1.0

Description:


Copyright 2012 Boutgh R&D.. All rights reserved.

IMPORTANT: Your use of this Software is limited to those specific rights
granted under the terms of a software license agreement between the user
who downloaded the software, his/her employer (which must be your employer)
and Boughg R&D. (the "License").  You may not use this Software unless you
agree to abide by the terms of the License. The License limits your use,
and you acknowledge, that the Software may not be modified,copied or
distributed unless embedded on a Texas Bough LTD., which is integrated into
your product.  Other than for the foregoing purpose, you may not use,
reproduce, copy, prepare derivative works of, modify, distribute, perform,
display or sell this Software and/or its documentation for any purpose.

YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
PROVIDED THIS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
BOUGH OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE,
STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE
THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED
TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES,
LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS,
TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT
LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

Should you have any questions regarding your right to use this Software,
contact Bouth R&D at www.bough.com.cn
**************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/* Copyright(c) 2012 Bough Technology Corp. All rights reserved.                                 */
/*-----------------------------------------------------------------------------------------------*/

/*********************************************************************
* INCLUDES
*/
#include <string.h>
#include "types.h"
#include "bcomdef.h"
#include "OSAL.h"
#include "OSAL_Tasks.h"

/* Driver */
#include "pwrmgr.h"
#include "gpio.h"
#include "protocol.h"
#include "keys.h"

/*********************************************************************
* MACROS
*/

/*********************************************************************
* CONSTANTS
*/

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* VARIABLES
*/
static protocol_CBs_t debounceCBs;
static protocol_CBs_t keysChange;
static uint16 keys_Save_keys = 0;

/*********************************************************************
* FUNCTIONS
*/

/*********************************************************************
 * gpio_wakeup_Task
 * Task gpio wakeup sample code
 * The followinng code shows P14 wakeup the system when there is a posedge or negedge.
 */
static void posedge_callback_wakeup(GPIO_Pin_e pin,IO_Wakeup_Pol_e type)
{
	if(type == POSEDGE)
	{
		osal_start_timerEx(debounceCBs.task_id,debounceCBs.events,HAL_KEY_DEBOUNCE_TIME);
	}
	else
	{
		
	}
}

static void negedge_callback_wakeup(GPIO_Pin_e pin,IO_Wakeup_Pol_e type)
{
	if(type == NEGEDGE)
	{
		osal_start_timerEx(debounceCBs.task_id,debounceCBs.events,HAL_KEY_DEBOUNCE_TIME);
	}
	else
	{
		
	}
}

/*********************************************************************
 * @fn      keys_Init
 *
 * @brief   Initialization function for the driver. Only call by hal_init().
 *
 * @param   none
 *
 * @return  none
 */
void keys_Init(void)
 {
	keys_Save_keys = 0;

	gpio_struct_t gpio_struct[KEY_MAX];
	uint8 index = 0;
	//KEY_POWER
	gpio_struct[index].pin = GPIO_KEY_POWER;
	gpio_struct[index].ioe = IE;
	gpio_struct[index].bit_action = Bit_DISABLE;
	gpio_struct[index].pull_type = WEAK_PULL_UP;
	gpio_struct[index].wakeup_pol = NEGEDGE;
	gpio_struct[index].wakeup_t.posedgeHdl = posedge_callback_wakeup;
	gpio_struct[index++].wakeup_t.negedgeHdl = negedge_callback_wakeup;

//	//KEY_LIGHT_UP
//	gpio_struct[index].pin = P5;
//	gpio_struct[index].ioe = IE;
//	gpio_struct[index].bit_action = Bit_DISABLE;
//	gpio_struct[index].pull_type = WEAK_PULL_UP;
//	gpio_struct[index].wakeup_pol = NEGEDGE;
//	gpio_struct[index].wakeup_t.posedgeHdl = posedge_callback_wakeup;
//	gpio_struct[index++].wakeup_t.negedgeHdl = negedge_callback_wakeup;

//	//KEY_LIGHT_DOWN
//	gpio_struct[index].pin = GPIO_KEY_LIGHT_DOWN;
//	gpio_struct[index].ioe = IE;
//	gpio_struct[index].bit_action = Bit_DISABLE;
//	gpio_struct[index].pull_type = WEAK_PULL_UP;
//	gpio_struct[index].wakeup_pol = NEGEDGE;
//	gpio_struct[index].wakeup_t.posedgeHdl = posedge_callback_wakeup;
//	gpio_struct[index++].wakeup_t.negedgeHdl = negedge_callback_wakeup;

//KEY_FUNC_UP
	gpio_struct[index].pin = GPIO_KEY_FUNC_UP;
	gpio_struct[index].ioe = IE;
	gpio_struct[index].bit_action = Bit_DISABLE;
	gpio_struct[index].pull_type = WEAK_PULL_UP;
	gpio_struct[index].wakeup_pol = NEGEDGE;
	gpio_struct[index].wakeup_t.posedgeHdl = posedge_callback_wakeup;
	gpio_struct[index++].wakeup_t.negedgeHdl = negedge_callback_wakeup;

	//KEY_FUNC_DOWN
	gpio_struct[index].pin = GPIO_KEY_FUNC_DOWN;
	gpio_struct[index].ioe = IE;
	gpio_struct[index].bit_action = Bit_DISABLE;
	gpio_struct[index].pull_type = WEAK_PULL_UP;
	gpio_struct[index].wakeup_pol = NEGEDGE;
	gpio_struct[index].wakeup_t.posedgeHdl = posedge_callback_wakeup;
	gpio_struct[index++].wakeup_t.negedgeHdl = negedge_callback_wakeup;

	//GPIO_CHARGE_DET
	gpio_struct[index].pin = GPIO_CHARGE_DET;
	gpio_struct[index].ioe = IE;
	gpio_struct[index].bit_action = Bit_DISABLE;
	gpio_struct[index].pull_type = WEAK_PULL_UP;
	gpio_struct[index].wakeup_pol = NEGEDGE;
	gpio_struct[index].wakeup_t.posedgeHdl = posedge_callback_wakeup;
	gpio_struct[index++].wakeup_t.negedgeHdl = negedge_callback_wakeup;

	//GPIO_CHARGE_FULL
	gpio_struct[index].pin = GPIO_CHARGE_FULL;
	gpio_struct[index].ioe = IE;
	gpio_struct[index].bit_action = Bit_DISABLE;
	gpio_struct[index].pull_type = PULL_DOWN;
	gpio_struct[index].wakeup_pol = NEGEDGE;
	gpio_struct[index].wakeup_t.posedgeHdl = NULL;
	gpio_struct[index++].wakeup_t.negedgeHdl = NULL;

	for (uint8 i = 0; i < KEY_MAX; i++) {
		hal_gpio_pin_init(gpio_struct[i].pin, gpio_struct[i].ioe);
		hal_gpio_pull_set(gpio_struct[i].pin, gpio_struct[i].pull_type);
		hal_gpio_wakeup_set(gpio_struct[i].pin, gpio_struct[i].wakeup_pol);
		hal_gpioin_register(gpio_struct[i].pin, gpio_struct[i].wakeup_t.posedgeHdl, gpio_struct[i].wakeup_t.negedgeHdl);
	}
}

/*********************************************************************
 * @fn      keys_RegisterCBs
 *
 * @brief   Register for keys change callback.
 *
 * @param   task_id, event
 *
 * @return  none
 */
void keys_RegisterCBs(protocol_CBs_t cbs)
{
	keysChange.task_id = cbs.task_id;
	keysChange.events = cbs.events;
}

/*********************************************************************
 * @fn      keys_RegisterDebounceCBs
 *
 * @brief   Register for keys debounce callback.
 *
 * @param   task_id, event
 *
 * @return  none
 */
void keys_RegisterDebounceCBs(protocol_CBs_t cbs)
{
	debounceCBs.task_id = cbs.task_id;
	debounceCBs.events = cbs.events;
}

/*********************************************************************
 * @fn      keys_Debounce_Handle
 *
 * @brief   Handle keys debounce event.
 *
 * @param   none
 *
 * @return  none
 */
void keys_Debounce_Handle(void)
{
	uint16 keys = 0;
	
	if ( keys == keys_Save_keys )
	{
		if ( keysChange.task_id && keysChange.events )
		{
			osal_set_event(keysChange.task_id, keysChange.events);
		}
	}
	
	keys_Save_keys = 0;
}

/*********************************************************************
 * @fn      keys_read
 *
 * @brief   Handle keys debounce event.
 *
 * @param   none
 *
 * @return  uint16
 */
uint16 keys_read(void) {
	uint16 keys = 0;

	if (!hal_gpio_read(GPIO_KEY_POWER)) {
		keys |= KEY_POWER;
	}

//	if ( !hal_gpio_read(GPIO_KEY_LIGHT_UP) )
//	{
//		keys |= KEY_LIGHT_UP;
//	}

//	if ( !hal_gpio_read(GPIO_KEY_LIGHT_DOWN) )
//	{
//		keys |= KEY_LIGHT_DOWN;
//	}

	if (!hal_gpio_read(GPIO_KEY_FUNC_UP)) {
		keys |= KEY_FUNC_UP;
	}

	if (!hal_gpio_read(GPIO_KEY_FUNC_DOWN)) {
		keys |= KEY_FUNC_DOWN;
	}

	if (!hal_gpio_read(GPIO_CHARGE_DET)) {
		keys |= KEY_CHARGE_DET;
		keys &= ~ KEY_CHARGE_FULL;
	}
	if (hal_gpio_read(GPIO_CHARGE_FULL)) {
		if (!hal_gpio_read(GPIO_CHARGE_DET)) {
			keys |= KEY_CHARGE_FULL;
		}
	}
	return keys;
}

/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
