/**************************************************************************************************
Filename:       command_center.h
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

#ifndef COMMAND_CENTER_H
#define COMMAND_CENTER_H

/*********************************************************************
* INCLUDES
*/
#include "types.h"

/*********************************************************************
* MACROS
*/
/* mode of system */
#define CCS_MODE_OFF							0x00
#define CCS_MODE_ADVERT							0x01
#define CCS_MODE_CONNECTING						0x02
#define CCS_MODE_LINKLOSS						0x03

#define CCS_SOP_CONTROL							0xaa
#define CCS_SOP_READ								0xa5
#define CCS_SOP_REPLY								0xba

#define CCS_FLAG_COMMAND						0x0001
#define CCS_FLAG_MODE								0x0002
#define CCS_FLAG_BRIGHTNESS						0x0004
#define CCS_FLAG_TEMPERATURE						0x0008
#define CCS_FLAG_HUE								0x0010
#define CCS_FLAG_SATURATION						0x0020
#define CCS_FLAG_EFFECTSMODE						0x0040
#define CCS_FLAG_TIMES								0x0080
#define CCS_FLAG_FREQ								0x0100
#define CCS_FLAG_ONTIME							0x0200

#define CCS_LIGHT_MODE_OFF						0
#define CCS_LIGHT_MODE_CCT						1
#define CCS_LIGHT_MODE_HSI							2
#define CCS_LIGHT_MODE_FIXEDMODE					3
#define CCS_LIGHT_MODE_EFFECTMODE				4

#define CCS_CMD_READ_STATUS						0x00
#define CCS_CMD_READ_RGBVALUE					0x01
#define CCS_CMD_READ_BRIGHTNESS					0x02
#define CCS_CMD_READ_TEMPERATRE					0x03
#define CCS_CMD_READ_FIXEDMODEVALUE			0x04
#define CCS_CMD_READ_EFFECTSMODEVALUE			0x05
#define CCS_CMD_READ_TEMPVALUE					0x06

#define APP_COMMAND_CONTROL					0xfc
#define APP_COMMAND_TRUNOFF					0xa0
#define APP_COMMAND_TRUNON						0xa1
#define APP_COMMAND_FLASH						0xd1
#define APP_COMMAND_TRIGGER						0xf0

#define CCS_EFFECTMODE_NORMAL					0
#define CCS_EFFECTMODE_FLASH						1
#define CCS_EFFECTMODE_TRIGGER					2
#define CCS_EFFECTMODE_LEDOFF						0
#define CCS_EFFECTMODE_LEDON						1

/*********************************************************************
* CONSTANTS
*/
#define CCS_BATT_CHECK_EVT						0x0001  //for battery detect
#define CCS_BATT_VALUE_EVT						0x0002  //event for battery voltage value update
#define CCS_KEY_CHANGE_EVT					0x0004
#define CCS_GETTEMP_EVT						0x0008

#define CCS_BLEIOC_EVT							0x0010
#define TIMER_LIGHT_EFFECT						0x0020
//#define CCS_LEDFIXMODE_EVT						0x0040
//#define CCS_LEDEFFECTMODE_EVT					0x0080

#define CCS_TEMP_CHECK_EVT						0x0100
#define CCS_TEMP_VALUE_EVT						0x0200

#define CCS_GATT_NOTIFY_EVT					0x1000
#define CCS_BLERF_EVT       						0x2000
#define CCS_TESTMODE_EVT						0x4000


//#define CCS_BATT_CHECK_EVT						0x0001  //for battery detect
//#define CCS_BATT_VALUE_EVT						0x0002  //event for battery voltage value update
//#define CCS_KEY_CHANGE_EVT					0x0004
//#define CCS_KEY_CONT_EVT						0x0008
//#define CCS_GETTEMP_EVT						0x0010
//#define CCS_BLEIOC_EVT							0x0020
////#define CCS_LEDEFFECTMODE_EVT				0x0040
//#define CCS_GATT_NOTIFY_EVT					0x1000
//#define CCS_EEPROM_EVT						0x2000
//#define CCS_TEST_MODE_EVT						0x4000


#define	BATTERY_RESISTANCE				0.080		//(3.90-3.68)/1.85	for charging with protected board
#define 	DISCHARGE_RESISTENCE				0.080		//110mR for discharging

#define	CHARGE_VOLT_DIVIATION			0.06//		0.06V
//#define 	RED_POWER_RATING					3.15	//3.15=4.14*0.76
//#define 	GREEN_POWER_RATING				2.37	//2.37=4.16*0.57
//#define 	BLUE_POWER_RATING				4.16	//4.16=4.12*1.01
//#define	CW_POWER_RATING					5.21	//5.21=4.43W/MAX_CW_DUTY    		4.43W=4.10v*1.08A
//#define	MW_POWER_RATING					5.30		//5.30=4.51/MAX_CW_DUTY       		4.51W=4.10v*1.10A
#define 	RED_POWER_RATING					2.29	//2.29=4.17*0.55
#define 	GREEN_POWER_RATING				2.67	//2.67=4.17*0.64
#define 	BLUE_POWER_RATING				2.87// 2.87=4.17*0.69
#define	CW_POWER_RATING					5.21	//5.21=4.43W/MAX_CW_DUTY    		4.43W=4.10v*1.08A
#define	MW_POWER_RATING					5.30	//5.30=4.51/MAX_CW_DUTY       		4.51W=4.10v*1.10A
#define	MAX_CW_DUTY						(1.0)
#define	MAX_MW_DUTY						(1.0)

#define	MAX_CHARGE_CURRENT				1.9					//2.0A
#define	KEEP_VOLT_THRESHOLD				4.05					//4.00V
#define	CHG_VOLT_FLOAT_HTHRESHOLD		(BATT_LV4_THESHOLD*0.001)					//3.60V
#define	CHG_VOLT_FLOAT_LTHRESHOLD		(BATT_LV4_THESHOLD*0.001-0.1)				//3.50V
#define	CHG_VOLT_FLOAT_EMPTY				3.40					//3.50V
#define	CHRAGE_FULL_VOLT					4.15//(BATT_VOLTAGE_MAX*0.001)//4.30
#define	BATT_ARRAY_SIZE					10
#define	PARAMS_DATA_RESET				0
/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* VARIABLES
*/
extern uint8 command_center_TaskID;
extern uint8 fIsInvalidMacAddr;

/*********************************************************************
* FUNCTIONS
*/
/**
 * @fn      command_center_Init
 *
 * @brief   Initialization function for the Simple BLE Peripheral App Task.
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
extern void command_center_Init(uint8 task_id);

/*********************************************************************
 * @fn      command_center_ProcessEvent
 *
 * @brief   command center application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
extern uint16 command_center_ProcessEvent(uint8 task_id, uint16 events);
/**
* @fn      CCS_SIMPLEGATT_DATA_PROCESS
*
* @brief   Process an package of recive from apps
*
* @param   pMsg - message to process
*
* @return  bool
*/
extern bool CCS_SIMPLEGATT_DATA_PROCESS( uint8 *pPkg );
/**
* @fn      CCS_LUMECONTROL_DATA_PROCESS
*
* @brief   Process an package of recive from apps
*
* @param   pMsg - message to process
*
* @return  none
*/
extern bool CCS_LUMECONTROL_DATA_PROCESS( uint8 *pPkg );

/**
* @fn      CCS_LUMECUBE_DATA_PROCESS
*
* @brief   Process an package of recive from apps
*
* @param   pMsg - message to process
*
* @return  none
*/
extern bool CCS_LUMECUBE_DATA_PROCESS( uint8 *pPkg );

/**
 * @fn      CCS_Systems_on
 *
 * @brief   Systems online.
 *
 * @param   none
 *
 * @return  none
 */
extern void CCS_Systems_on(void);

/**
 * @fn      CCS_Systems_off
 *
 * @brief   Systems outline.
 *
 * @param   none
 *
 * @return  none
 */
extern void CCS_Systems_off(void);

/**
 * @fn      CCS_GET_HOTDISPLAYSTATUS
 *
 * @brief   
 *
 * @param   
 *
 * @return  events not processed
 */
extern bool CCS_GET_HOTDISPLAYSTATUS(void);

/**
 * @fn      CCS_GET_ChargeStatus
 *
 * @brief   
 *
 * @param   
 *
 * @return  events not processed
 */
extern bool CCS_GET_ChargeStatus(void);

/**
 * @fn      CCS_GET_BLE_Status
 *
 * @brief   
 *
 * @param   
 *
 * @return  bool
 */
extern bool CCS_GET_BLE_Status(void);


extern	float getCompensationVolt(void);
void	 	entryBurnInTest(void);
extern	void	 startLightEffectEvent(void);
extern	void	 stopLightEffectEvent(void);
bool getSystemStts(void);
void updateSystemStts(bool stts);
void HW_RESET_MCU(bool backup);
void versionDisplay(void);
bool protocol_uartCB_Handle(uint16 len, uint8 *pPkg);
void entryFactoryMode(void);
void	 factoryModeRGBOn(uint16 hues,uint8 brightness);
void	 factoryModeColorTempOn(uint8 cTemp,uint8 brightness);
/***************************************************
 *
 *		Brightness -  Key Function
 *
 ****************************************************/
void keyBrightnessDecProcess(u16 *flag);

/***************************************************
 *
 *		Brightness +  Key Function
 *
 ****************************************************/
void keyBrightnessIncProcess(u16 *flag);

void setModeInfo(u16 *flag, uint8 *cmd);
/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
#endif // COMMAND_CENTER_H
