/**************************************************************************************************
Filename:       battery.h
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

#ifndef _BATTERY_HEAD_FILE
#define _BATTERY_HEAD_FILE

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

/*********************************************************************
* TYPEDEFS
*/
//typedef void (* batt_evt_hdl_t)(batt_evt_t batt_evt);

typedef struct
{
    uint8_t		percent;
    uint16_t	volt;
} batt_ref_t;

typedef struct _batt_ctx_t
{
    bool            is_charging;
    float           vbat_value;
} batt_ctx_t;

typedef enum{
	BATT_CHARGE_PLUG = 1,
	BATT_CHARGE_UNPLUG,
	BATT_VOLTAGE,
}batt_evt_t;

enum
{
    BATT_ST_NORMAL = 0,
    BATT_ST_CHARGING,
    BATT_ST_CHARGING_FULL,
};

/*********************************************************************
* VARIABLES
*/

/*********************************************************************
* FUNCTIONS
*/

/**
 * @fn      batt_init
 *
 * @brief   Init batt, set "emset". Only call once at systems init.
 *
 * @param   none
 *
 * @return  none
 */
extern int batt_init(void);

/**
 * @fn      batt_RegisterCBs_Value
 *
 * @brief   Register for keys change callback.
 *
 * @param   task_id, event
 *
 * @return  none
 */
extern void batt_RegisterCBs_Value(protocol_CBs_t cbs);

/**
 * @fn      batt_measure
 *
 * @brief   Start batt measure once. Call back by register when done.
 *
 * @param   none
 *
 * @return  none
 */
extern void batt_measure(void);

/**
 * @fn      batt_voltage
 *
 * @brief   return batt valtage(mv).
 *
 * @param   none
 *
 * @return  float
 */
extern uint16 batt_get_voltage(void);

/**
 * @fn      batt_get_percent
 *
 * @brief   return batt percent.
 *
 * @param   none
 *
 * @return  float
 */
extern uint8 batt_get_percent(void);


uint8 getBattPercentByVolt(float volt);
/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
#endif
