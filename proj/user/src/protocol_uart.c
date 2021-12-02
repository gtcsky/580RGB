/**************************************************************************************************
Filename:       protocol_uart.c
Revised:        Date: 2020.8.25
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

/* LL */
#include "ll.h"

/* HCI */
#include "hci_tl.h"

/* Driver */
#include "pwrmgr.h"
#include "protocol.h"
#include "uart.h"
#include "gpio.h"

/* L2CAP */
#include "l2cap.h"

/* gap */
#include "gap.h"
#include "gapgattserver.h"

/* GATT */
#include "gatt.h"

#include "gattservapp.h"

/* Profiles */
#include "peripheral.h"

/* Application */
#include "protocol_uart.h"
#include "systems_parameters.h"
#include "bleSmartPeripheral.h"
#include "command_center.h"

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
uint8 protocol_uart_TaskID;   // Task ID for internal task/event processing

static PROTOCOL_uart_t mUart_Ctx;

/*********************************************************************
* FUNCTIONS
*/
static void protocol_uart_ProcessOSALMsg(osal_event_hdr_t* pMsg);
static void protocol_uart_wakeup_handle(GPIO_Pin_e pin,IO_Wakeup_Pol_e type);
static void protocol_uart_evt_hdl(uart_Evt_t* pev);
	
/*********************************************************************
 * @fn      protocol_uart_Init
 *
 * @brief   Initialization function for the protocol of uart App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void protocol_uart_Init(uint8 task_id)
{
	protocol_uart_TaskID = task_id;
	
	uint32 bandrate = protocol_uart_GetBandrate();
	uart_Cfg_t cfg =
	{
		.tx_pin = GPIO_UART_TX,
		.rx_pin = GPIO_UART_RX,
		.rts_pin = GPIO_DUMMY,
		.cts_pin = GPIO_DUMMY,
		.baudrate = 115200,//bandrate,
		.use_fifo = true,
		.hw_fwctrl = false,
		.use_tx_buf = true,
		.parity     = false,
		.evt_handler = protocol_uart_evt_hdl,
	};
	hal_uart_init(cfg);

	hal_uart_set_tx_buf(mUart_Ctx.tx_buf, UART_TX_BUF_SIZE);
	memset(&mUart_Ctx, 0, sizeof(mUart_Ctx));

//	uint8 data[] = {"uart test"};
//	hal_uart_send_buff(data,sizeof(data)-1);

	//config gpio wakeup
	hal_pwrmgr_register(PMM_MOD_UART, NULL, NULL);
	hal_gpio_pin_init(GPIO_UART_WAKEUP, IE);
	hal_gpio_pull_set(GPIO_UART_WAKEUP, WEAK_PULL_UP);
	hal_gpio_wakeup_set(GPIO_UART_WAKEUP, NEGEDGE);
	hal_gpioin_register(GPIO_UART_WAKEUP, NULL, protocol_uart_wakeup_handle);
}

/*********************************************************************
* @fn      GAP_GetadvInt
*
* @brief   set connection interval time
*
* @param   none.
*
* @return  cntInt:connection interval time
*/
uint32 protocol_uart_GetBandrate(void)
{
	uint8 bandrate[3] = {0};
	systems_param_Get_param(ITEM_INTERVALTIME, &bandrate[0], &bandrate[1]);
	
	uint32 cntInt = 0;
	switch ( bandrate[2] )
	{
		default:
		case 0x00:	cntInt = HAL_UART_TBR_9600;		break;
		case 0x01:	cntInt = HAL_UART_TBR_4800;		break;
		case 0x02:	cntInt = HAL_UART_TBR_19200;	break;
		case 0x04:	cntInt = HAL_UART_TBR_38400;	break;
		case 0x05:	cntInt = HAL_UART_TBR_57600;	break;
		case 0x06:	cntInt = HAL_UART_TBR_115200;	break;
		case 0x07:	cntInt = HAL_UART_TBR_230400;	break;
		case 0x08:	cntInt = HAL_UART_TBR_2400;		break;
	}
	return cntInt;
}

/*********************************************************************
 * @fn      bleSmartPeripheral_ProcessOSALMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void protocol_uart_ProcessOSALMsg(osal_event_hdr_t* pMsg)
{
	switch(pMsg->event)
	{
		default:
			// do nothing
			break;
	}
}

/*********************************************************************
 * @fn      protocol_uart_ProcessEvent
 *
 * @brief   protocol uart application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
uint16 protocol_uart_ProcessEvent(uint8 task_id, uint16 events)
{
	VOID task_id; // OSAL required parameter that isn't used in this function

	if(events & SYS_EVENT_MSG)
	{
		uint8* pMsg;

		if((pMsg = osal_msg_receive(protocol_uart_TaskID)) != NULL)
		{
			protocol_uart_ProcessOSALMsg((osal_event_hdr_t*)pMsg);

			// Release the OSAL message
			VOID osal_msg_deallocate(pMsg);
		}

		// return unprocessed events
		return (events ^ SYS_EVENT_MSG);
	}

	if (events & PROTOCOL_UART_TIMEOUT_EVT)
	{
		hal_gpio_pin_init(GPIO_UART_WAKEUP, IE);
		hal_gpio_pull_set(GPIO_UART_WAKEUP, WEAK_PULL_UP);
		hal_gpio_wakeup_set(GPIO_UART_WAKEUP, NEGEDGE);
		hal_gpioin_register(GPIO_UART_WAKEUP, NULL, protocol_uart_wakeup_handle);
		hal_pwrmgr_unlock(PMM_MOD_UART);

		return (events ^ PROTOCOL_UART_TIMEOUT_EVT);
	}

	// Discard unknown events
	return 0;
}

/*********************************************************************
 * @fn      protocol_uart_wakeup_handle
 *
 * @brief   handle uart wakeup pin intterupt callback event
 *
 * @param   GPIO_Pin_e, IO_Wakeup_Pol_e
 *
 * @return  none
 */
static void protocol_uart_wakeup_handle(GPIO_Pin_e pin,IO_Wakeup_Pol_e type)
{
//	uint8 data[] = {"uart test"};
//	hal_uart_send_buff(data,sizeof(data)-1);

	if(pin == GPIO_UART_RX && type == NEGEDGE)
	{
		if ( hal_pwrmgr_is_lock(PMM_MOD_UART) == false )
		{
			hal_pwrmgr_lock(PMM_MOD_UART);
		}

		hal_gpioin_disable(GPIO_UART_RX);

		hal_gpio_fmux_set(GPIO_UART_RX, UART_RX);   //set fmux(uart rx)
		osal_start_timerEx( protocol_uart_TaskID, PROTOCOL_UART_TIMEOUT_EVT, 50 );
	}
}

/*********************************************************************
 * @fn      protocol_uart_evt_hdl
 *
 * @brief   handle uart intterupt callback event
 *
 * @param   uart_Evt_t
 *
 * @return  none
 */
static void protocol_uart_evt_hdl(uart_Evt_t* pev)
{
	uint16 index = 0;
	if (pev->type == UART_EVT_TYPE_RX_DATA)
	{

		memcpy(mUart_Ctx.rx_buf + mUart_Ctx.rx_size, pev->data, pev->len);
		mUart_Ctx.rx_size += pev->len;
	}
	else if (pev->type == UART_EVT_TYPE_RX_DATA_TO)
	{
		if((mUart_Ctx.rx_size + pev->len)>=UART_RX_BUF_SIZE)
		{
			mUart_Ctx.rx_size = 0;
			hal_pwrmgr_unlock(PMM_MOD_UART);
			return;
		}

		memcpy(mUart_Ctx.rx_buf + mUart_Ctx.rx_size, pev->data, pev->len);
		mUart_Ctx.rx_size += pev->len;

		hal_pwrmgr_unlock(PMM_MOD_UART);


		// Check SOP
		for ( index = 0; index < mUart_Ctx.rx_size-1; index++ )
		{
			if ( mUart_Ctx.rx_buf[index] == UART_SOP )
			{
				break;
			}
		}

		if ( index+2 >= mUart_Ctx.rx_size )
		{
			return;
		}

		protocol_uartCB_Handle(mUart_Ctx.rx_size-index, &mUart_Ctx.rx_buf[index]);
		mUart_Ctx.rx_size = 0;
	}
	else if (pev->type == UART_EVT_TYPE_TX_COMPLETED)
	{
		mUart_Ctx.tx_size = 0;
		hal_pwrmgr_unlock(PMM_MOD_UART);
	}
	else
	{
//		uint8 data[] = {"uart test"};
//		hal_uart_send_buff(data,sizeof(data)-1);
	}
}

/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
