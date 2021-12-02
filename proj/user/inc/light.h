/**************************************************************************************************
Filename:       light.h
Revised:        Date: 2020.9.10
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

#ifndef _LIGHT_H
#define _LIGHT_H

/*********************************************************************
* INCLUDES
*/
#include "types.h"

/*********************************************************************
* MACROS
*/

/*********************************************************************
* CONSTANTS
*/
#define LIGHT_CHANGE_EVT						0x0010

#define TEMPERATURE_MAX_RATE						0.95
#define TEMPERATURE_MIN_RATE						0.05


#define LED_REAL_START_DUTY						4  // LED 实际亮度起步4%
#define RGBLED_REAL_START_DUTY					9  // LED 实际亮度起步4%
#define RGBLED_REAL_START_DUTY_PERCENT			(RGBLED_REAL_START_DUTY*0.01)  // LED 实际亮度起步4%

//#define LED_REAL_START_DATA  						0x0F  //冷光和暖光最低有效亮度
#define LED_REAL_START_DATA  						16//0x14  //冷光和暖光最低有效亮度
#define RedGreenRate								1.3
#define BlueGreenRate								1.6
#define RedGreenRateStep							(1-(1/RedGreenRate))
#define BlueGreenRateStep							(1-(1/BlueGreenRate))
#define BrightnessThreshold							0.40
#define BrightnessGap								(BrightnessThreshold-RGBLED_REAL_START_DUTY*1.0/100)
#define BrightnessCompersationGap					(RGBLED_REAL_START_DUTY-LED_REAL_START_DUTY)
#define BrightnessCompersationStep					((RGBLED_REAL_START_DUTY-LED_REAL_START_DUTY)/(3*BrightnessGap*100))

#define TEMPERATURE_COE_NORMAL					1.0
#define TEMPERATURE_COE_LOW						0.8

#define RGB_MAX_RATE								1.0
#define RGB_MIN_RATE								0.90

#define 	PWM_MAX_COUNT							800
#define	PWM_FRQ_CONST							PWM_MAX_COUNT
#define	PWM_FREQ_CONST							PWM_MAX_COUNT
#define	PWM_PERIOD_CONST						PWM_MAX_COUNT

#define 	Red_LED_CHANNEL		PWM_CH2
#define 	Green_LED_CHANNEL		PWM_CH3
#define 	Blue_LED_CHANNEL		PWM_CH4
#define 	CW_LED_CHANNEL		PWM_CH0
#define 	MW_LED_CHANNEL		PWM_CH1
#define	BACKLIGHT_CHANNEL	PWM_CH5
/*********************************************************************
* TYPEDEFS
*/
typedef struct
{
	bool power_on;
	bool en;
	uint8 flag;
	uint8 command;				//off,cct,hsi,fixed,effe
	uint8 mode;					//0=normorl,fiexed mode number.
	uint8 brightness;			//0-100
	uint8 colorTemperature;
	uint16 hues;				//0-360
	uint8 saturation;			//1-100
	uint8 effemode;
	uint8 times;
	uint8 freq;
} light_ctrl_t;

typedef struct
{
	uint16 cw;
	uint16 mw;
	uint16 red;
	uint16 green;
	uint16 blue;
	uint16 hues;
	float saturation;
	float brightness;
} colorStructType;

typedef struct{
	UINT16	valueOfRed;
	UINT16	valueOfGreen;
	UINT16	valueOfBlue;
	UINT16	valueOfCw;
	UINT16	valueOfMw;
}PcaDataStruct;

typedef enum
{
	MODE_OFF = 0,
	MODE_CCT = 1,
	MODE_HSI = 2,
	MODE_FIXED = 3,
	MODE_EFFE = 4
} light_command;

/*********************************************************************
* VARIABLES
*/

/*********************************************************************
* FUNCTIONS
*/

/**
 * @fn      light_Init
 *
 * @brief   Initialization function for the light App Task.
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
extern void light_Init(uint8 task_id);

/**
 * @fn      light_ProcessEvent
 *
 * @brief   light application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
extern uint16 light_ProcessEvent(uint8 task_id, uint16 events);

/**
 * @fn      light_ctrl
 *
 * @brief   ctrl light.
 *
 * @param   *pPgk
 *
 * @return  error code
 */
extern uint8 light_ctrl(uint8 *pPkg);


extern 	uint8 light_ctrl(uint8 *pPkg);
extern	void light_off(void);
extern	void updateColorTemp(displayParamsStruct * disParams);
extern	void	updateRGBLamp(displayParamsStruct * disParams);
extern	void	 loadTargetRGBValue(displayParamsStruct * disParams,PcaDataStruct * pcaData);
extern	void	 loadTargetColorTemperatureValue(displayParamsStruct * disParams,PcaDataStruct * pcaData);
extern	bool getLightStts(void);
void	 	pwmInit(void);
extern void light_update(void) ;
/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
#endif	//_LIGHT_H
