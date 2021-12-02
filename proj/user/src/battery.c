/**************************************************************************************************
Filename:       battery.c
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
#include "types.h"
#include "adc.h"
#include "battery.h"
#include "command_center.h"
#include "OSAL.h"
#include "OSAL_Timers.h"
#include "string.h"
#include "gpio.h"
#include "error.h"
#include "log.h"
#include "protocol.h"
#include "pwrmgr.h"
#include "clock.h"
#include	"user_flash.h"
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

static batt_ctx_t s_batt_ctx;
static batt_ref_t s_batt_ref;

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
	.channel = ADC_BIT(ADC_CHANNEL_BATT),
	.is_continue_mode = false,
	.is_differential_mode = 0x00,
	.is_high_resolution = ADC_BIT(ADC_CHANNEL_BATT),//0x7F,
};

static protocol_CBs_t battvalue_CBs;

extern	uint8 vAdcErrorTimes;

extern	displayParamsStruct displayParams;
/*********************************************************************
* FUNCTIONS
*/
static int batt_adc_init(void);
static void batt_adc_evt(adc_Evt_t* pev);
static void batt_cal(batt_ctx_t ctx);

/*********************************************************************
 * @fn      batt_RegisterCBs_Value
 *
 * @brief   Register for keys change callback.
 *
 * @param   task_id, event
 *
 * @return  none
 */
static int batt_adc_init(void) {
	if(getAdcStts()){
		osal_start_timerEx(command_center_TaskID, CCS_BATT_CHECK_EVT, BATT_DET_PERIOD_1S);
		if(vAdcErrorTimes++<=10){
			LOG(" \n ad busy b%d  \n",vAdcErrorTimes);
			return PPlus_ERR_BUSY;
		}else{
			LOG(" \n __________adc module reset_____________  \n");
			HW_RESET_MCU(true);
		}
	}
	int ret = hal_adc_config_channel(adc_cfg, batt_adc_evt);

	if(!hal_adc_start()){
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
static void batt_adc_evt(adc_Evt_t* pev) {
	bool is_high_resolution = false;
	bool is_differential_mode = false;
	batt_ctx_t* p_ctx = &s_batt_ctx;

	if (pev->type == HAL_ADC_EVT_DATA) {
		float battVolt=0;
		is_high_resolution = (adc_cfg.is_high_resolution & ADC_BIT(ADC_CHANNEL_BATT)) ? true : false;
		is_differential_mode = (adc_cfg.is_differential_mode & ADC_BIT(ADC_CHANNEL_BATT)) ? true : false;
		battVolt= hal_adc_value_cal((adc_CH_t) (ADC_CHANNEL_BATT), pev->data, pev->size, is_high_resolution, is_differential_mode);
		if (battVolt < 0) {
			osal_stop_timerEx(battvalue_CBs.task_id, battvalue_CBs.restartEvents);
			osal_start_timerEx(battvalue_CBs.task_id, battvalue_CBs.restartEvents, 5);
		} else {
			p_ctx->vbat_value = battVolt * (10 + 2) / 2; //4.7;//11.0;//4.3/0.9;
//			if(getSystemStts())
				p_ctx->vbat_value+=displayParams.adcGap;				//if system on
			batt_cal(*p_ctx);
			osal_set_event(battvalue_CBs.task_id, battvalue_CBs.events);
		}
	} else {
		LOG("\n__ADC error\n");
	}
}
/***************************************************************
 *
 * 	calculate the battery percent by voltage
 *
 * 	@param      	voltage units mV
 *
 * 	@return 		percent
 *
 ****************************************************************/
uint8 getBattPercentByVoltMv(uint16 voltMv) {

	if (voltMv > BATT_VOLTAGE_MAX) {
		return 99;
	} else if (voltMv >= BATT_LV4_THESHOLD) {
		return (80 + (voltMv - BATT_LV4_THESHOLD) * 20 / (BATT_VOLTAGE_MAX - BATT_LV4_THESHOLD));
	} else if (voltMv >= BATT_LV3_THESHOLD) {
		return (60 + (voltMv - BATT_LV3_THESHOLD) * 20 / (BATT_LV4_THESHOLD - BATT_LV3_THESHOLD));
	} else if (voltMv >= BATT_LV2_THESHOLD) {
		return (40 + (voltMv - BATT_LV2_THESHOLD) * 20 / (BATT_LV3_THESHOLD - BATT_LV2_THESHOLD));
	} else if (voltMv >= BATT_LV1_THESHOLD) {
		return (20 + (voltMv - BATT_LV1_THESHOLD) * 20 / (BATT_LV2_THESHOLD - BATT_LV1_THESHOLD));
	} else if (voltMv > BATT_VOLTAGE_MIX) {
		return (voltMv - BATT_VOLTAGE_MIX) * 20 / (BATT_LV1_THESHOLD - BATT_VOLTAGE_MIX);
	} else
		return 0;

}
/***************************************************************
 *
 * 	calculate the battery percent by voltage
 *
 * 	@param      	voltage units V
 *
 * 	@return 		percent
 *
 ****************************************************************/

uint8 getBattPercentByVolt(float volt) {

	uint16 voltInt = (uint16) (volt * 1000);
	return getBattPercentByVoltMv(voltInt);

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
static void batt_cal(batt_ctx_t ctx)
{
	s_batt_ref.volt = ctx.vbat_value*1000;

	if ( s_batt_ref.volt > BATT_VOLTAGE_MAX )
	{
		s_batt_ref.percent = 100;
	}

	else if ( s_batt_ref.volt < BATT_VOLTAGE_MIX )
	{
		s_batt_ref.percent = 0;
	}

	else
	{
		s_batt_ref.percent = (s_batt_ref.volt - BATT_VOLTAGE_MIX)*100/(BATT_VOLTAGE_MAX-BATT_VOLTAGE_MIX);
	}
}

/*********************************************************************
 * @fn      batt_init
 *
 * @brief   Init batt, set "emset". Only call once at systems init.
 *
 * @param   none
 *
 * @return  none
 */
int batt_init(void)
{
	memset(&s_batt_ctx, 0, sizeof(s_batt_ctx));

	hal_gpio_pull_set(ADC_BATT_DET, FLOATING);

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
void batt_RegisterCBs_Value(protocol_CBs_t cbs)
{
	battvalue_CBs.task_id = cbs.task_id;
	battvalue_CBs.events = cbs.events;
	battvalue_CBs.restartEvents = cbs.restartEvents;
}

/*********************************************************************
 * @fn      batt_measure
 *
 * @brief   Start batt measure once. Call back by register when done.
 *
 * @param   none
 *
 * @return  none
 */
void batt_measure(void)
{
	//	LOG("batt_measure\n");
	//Event handler is called immediately after conversion is finished.
	batt_adc_init();

}

/*********************************************************************
 * @fn      batt_get_voltage
 *
 * @brief   return batt valtage(mv).
 *
 * @param   none
 *
 * @return  float
 */
uint16 batt_get_voltage(void)
{
	return s_batt_ref.volt;
}

/*********************************************************************
 * @fn      batt_get_percent
 *
 * @brief   return batt percent.
 *
 * @param   none
 *
 * @return  float
 */
uint8 batt_get_percent(void)
{
	return s_batt_ref.percent;
}

/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
