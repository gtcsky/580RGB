/**************************************************************************************************
Filename:       protocol_uart.h
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

#ifndef PROTOCOL_UART_H
#define PROTOCOL_UART_H

/*********************************************************************
* INCLUDES
*/
#include "types.h"
#include "protocol.h"

/*********************************************************************
* MACROS
*/

/*********************************************************************
* CONSTANTS
*/

#define PROTOCOL_UART_TIMEOUT_EVT						0x0001

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* VARIABLES
*/
extern uint8 protocol_uart_TaskID;

/*********************************************************************
* FUNCTIONS
*/ 
/**
 * @fn      protocol_uart_Init
 *
 * @brief   Initialization function for the protocol of uart Task.
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
extern void protocol_uart_Init(uint8 task_id);

/**
* @fn      GAP_GetadvInt
*
* @brief   set connection interval time
*
* @param   none.
*
* @return  cntInt:connection interval time
*/
extern uint32 protocol_uart_GetBandrate(void);

/*********************************************************************
 * @fn      protocol_uart_ProcessEvent
 *
 * @brief   protocol of uart application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
extern uint16 protocol_uart_ProcessEvent(uint8 task_id, uint16 events);

/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
#endif // PROTOCOL_UART_H
