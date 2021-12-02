/**************************************************************************************************
Filename:       TEMP.c
Revised:        Date: 2020.9.28
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
#include "types.h"
#include "adc.h"
#include "temp.h"
#include "command_center.h"
#include "OSAL.h"
#include "OSAL_Timers.h"
#include "string.h"
#include "gpio.h"
#include "error.h"
#include "log.h"
#include "protocol.h"
#include	"user_flash.h"
//static uint8 vAdcErrorTimes=0;
extern	uint8 vAdcErrorTimes;
/*********************************************************************
* MACROS
*/

/*********************************************************************
* CONSTANTS
*/
const float table_res[] =
{
	1794,	//0
	1351,	//5
	1025,	//10
	785,		//15
	605,		//20
	470,		//25
	364,		//30
	284,		//35
	223,		//40
	176,		//45
	140,		//50
	112,		//55
	90,		//60
	73,		//65
	59.1,		//70
	48.2,		//75
	39.5,		//80
	32.6,		//85
	27,			//90
	22.4,		//95
	18.7,		//100
	15.7,		//105
	14,		//110
	12,		//115
	11,		//120
	9,		//125
};

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* VARIABLES
*/
static float temp_value = 0;
/*
channel:
is_differential_mode:
is_high_resolution:
[bit7~bit2]=[p20,p15~p11],ignore[bit1,bit0]
when measure adc(not battery),we'd better use high_resolution.
when measure battery,we'd better use no high_resolution and keep the gpio alone.

differential_mode is rarely used,
if use please config channel as one of [ADC_CH3DIFF,ADC_CH2DIFF,ADC_CH1DIFF],
and is_high_resolution as one of [0x80,0x20,0x08],
then the pair of [P20~P15,P14~P13,P12~P11] will work.
other adc channel cannot work.
*/
static adc_Cfg_t adc_cfg =
{
	.channel = ADC_BIT(ADC_CHANNEL_TEMP),
	.is_continue_mode = false,
	.is_differential_mode = 0x00,
	.is_high_resolution =ADC_BIT(ADC_CHANNEL_TEMP),// 0x80,//0x7F,
};

static protocol_CBs_t tempvalue_CBs;

/*********************************************************************
* FUNCTIONS
*/
static int temp_adc_init(void);
static void temp_adc_evt(adc_Evt_t* pev);
static void temp_cal(float val);

/*********************************************************************
 * @fn      temp_RegisterCBs_Value
 *
 * @brief   Register for temp change callback.
 *
 * @param   task_id, event
 *
 * @return  none
 */
static int temp_adc_init(void)
{
	if(getAdcStts()){
		osal_start_timerEx(command_center_TaskID, CCS_TEMP_CHECK_EVT, TEMPEARTURE_DET_PERIOD_2S);
		if(vAdcErrorTimes++<=10){
			LOG(" \n ad busy T%d  \n",vAdcErrorTimes);
			return PPlus_ERR_BUSY;
		}else{
			LOG(" \n __________adc module reset_________  \n");
//			storeExceptionStts();
//			hal_gpio_write(SW_RESET_MCU, 0);
//			while(1);
			HW_RESET_MCU(true);
		}
	}
	int ret = hal_adc_config_channel(adc_cfg, temp_adc_evt);

	if(ret)
	{
		return ret;
	}

	if(!hal_adc_start()){
		//LOG("\n _start temp\n");
		vAdcErrorTimes=0;
		return PPlus_SUCCESS;
	}
	return PPlus_ERR_BUSY;
}

/*********************************************************************
 * @fn      batt_adc_evt
 *
 * @brief   Handle adc evt.
 *
 * @param   adc_Evt_t
 *
 * @return  none
 */
static void temp_adc_evt(adc_Evt_t* pev) {
	bool is_high_resolution = false;
	bool is_differential_mode = false;
	float value = 0;

//	hal_gpio_write(ADC_TEMP_POWER, 0);

	if (pev->type == HAL_ADC_EVT_DATA) {
		is_high_resolution = (adc_cfg.is_high_resolution & ADC_BIT(ADC_CHANNEL_TEMP)) ? true : false;
		is_differential_mode = (adc_cfg.is_differential_mode & ADC_BIT(ADC_CHANNEL_TEMP)) ? true : false;
		//LOG("\nstts:%d",is_high_resolution);
		value = hal_adc_value_cal((adc_CH_t) (ADC_CHANNEL_TEMP), pev->data, pev->size, is_high_resolution, is_differential_mode);
		if (value < 0) {
			//osal_set_event(tempvalue_CBs.task_id, tempvalue_CBs.restartEvents);
			osal_stop_timerEx( tempvalue_CBs.task_id, tempvalue_CBs.restartEvents );
			osal_start_timerEx( tempvalue_CBs.task_id, tempvalue_CBs.restartEvents, 5 );
		} else {
//		LOG("ADC_VALUE[%x %x]\n", value, value);
//		value = value*(10+2)/2;//4.7;//11.0;//4.3/0.9;
			temp_cal(value);
			osal_set_event(tempvalue_CBs.task_id, tempvalue_CBs.events);
//		osal_start_timerEx( tempvalue_CBs.task_id, tempvalue_CBs.events, 1000 );
		}
	}
}

/*********************************************************************
 * @fn      batt_cal
 *
 * @brief   Cal batt adc result.
 *
 * @param   batt_ref_t
 *
 * @return  none
 */
//#include "display.h"
static void temp_cal(float val) {
	float res_ntc = val * 1000 * TEMP_RES_PULLUP / TEMP_POWER_VOLTAGE;

	for (uint8 i = 0; i < sizeof(table_res) / sizeof(table_res[0]); i++) {
		if (res_ntc > table_res[i]) {
			temp_value = (i ) * 5 - (res_ntc - table_res[i]) / ((table_res[i] - table_res[i + 1]) / 5);
			return;
		}
	}
	temp_value = 0;
}
/*********************************************************************
 * @fn      temp_init
 *
 * @brief   Init batt, set "emset". Only call once at systems init.
 *
 * @param   none
 *
 * @return  none
 */
int temp_init(void)
{
	memset(&temp_value, 0, sizeof(temp_value));

	hal_gpio_pull_set(ADC_TEMP_DET, FLOATING);
	hal_gpio_pin_init(ADC_TEMP_POWER, OEN);
	hal_gpio_write(ADC_TEMP_POWER, 1);
//	while(1);
	return PPlus_SUCCESS;
}

/*********************************************************************
 * @fn      batt_RegisterCBs_Value
 *
 * @brief   Register for keys change callback.
 *
 * @param   task_id, event
 *
 * @return  none
 */
void temp_RegisterCBs_Value(protocol_CBs_t cbs)
{
	tempvalue_CBs.task_id = cbs.task_id;
	tempvalue_CBs.events = cbs.events;
	tempvalue_CBs.restartEvents = cbs.restartEvents;
}

/*********************************************************************
 * @fn      temp_measure
 *
 * @brief   Start temp measure once. Call back by register when done.
 *
 * @param   none
 *
 * @return  none
 */
void temp_measure(void)
{
//	LOG("temp_measure\n");
	//Event handler is called immediately after conversion is finished.
	temp_adc_init();
}

/*********************************************************************
 * @fn      temp_get_value
 *
 * @brief   return temp value.
 *
 * @param   none
 *
 * @return  float
 */
float temp_get_value(void)
{
	return temp_value;
}

/*********************************************************************
 * @fn      setTemperature
 *
 * @brief   return Non
 *
 * @param   float
 *
 * @return  None
 */
void setTemperature(float temp) {
	temp_value = temp;
}
/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
