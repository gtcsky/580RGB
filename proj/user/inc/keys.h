/**************************************************************************************************
Filename:       keys.h
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

#ifndef _KEYS_H
#define _KEYS_H

/*********************************************************************
* INCLUDES
*/
#include "types.h"
#include "gpio.h"
#include "protocol.h"

/*********************************************************************
* MACROS
*/
#define HAL_KEY_DEBOUNCE_TIME				20	//debounce time ms

#define KEY_MAX								5
#define KEY_POWER								0x01
#define KEY_LIGHT_UP							0x02
#define KEY_LIGHT_DOWN						0x04
#define KEY_FUNC_UP							0x08
#define KEY_FUNC_DOWN							0x10
#define KEY_CHARGE_DET							0x20
#define KEY_CHARGE_FULL						0x40

/*********************************************************************
* CONSTANTS
*/

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* VARIABLES
*/

/*********************************************************************
* FUNCTIONS
*/

/**
 * @fn      keys_Init
 *
 * @brief   Initialization function for the driver. Only call by hal_init().
 *
 * @param   none
 *
 * @return  none
 */
extern void keys_Init(void);

/**
 * @fn      keys_RegisterCBs
 *
 * @brief   Register for keys change callback.
 *
 * @param   task_id, event
 *
 * @return  none
 */
extern void keys_RegisterCBs(protocol_CBs_t cbs);

/**
 * @fn      keys_RegisterDebounceCBs
 *
 * @brief   Register for keys debounce callback.
 *
 * @param   task_id, event
 *
 * @return  none
 */
extern void keys_RegisterDebounceCBs(protocol_CBs_t cbs);

/**
 * @fn      keys_Debounce_Handle
 *
 * @brief   Handle keys debounce event.
 *
 * @param   none
 *
 * @return  none
 */
extern void keys_Debounce_Handle(void);

/**
 * @fn      keys_read
 *
 * @brief   Handle keys debounce event.
 *
 * @param   none
 *
 * @return  uint16
 */
extern uint16 keys_read(void);

/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
#endif // _KEYS_H
