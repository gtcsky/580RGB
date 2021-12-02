/**************************************************************************************************
Filename:       light.c
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


/*********************************************************************
* INCLUDES
*/
#include "types.h"
#include "comdef.h"
#include "OSAL.h"
#include "OSAL_Tasks.h"

/* Application */
#include "error.h"
#include "log.h"
#include "protocol.h"
#include "light.h"
#include "user_color.h"
#include "command_center.h"
#include "gpio.h"
#include "pwm.h"
#include "pwrmgr.h"
#include "display.h"
#include "user_lightEffect.h"

/*********************************************************************
* MACROS
*/

/*********************************************************************
* CONSTANTS
*/


//非0的值要>=LED_REAL_START_DATA
const uint16 table_temperature[][2] =
{
		{0,680},			//3200
		{50,650},			//3300
		{80,620},			//3400
		{110,590},		//3500
		{145,560},		//3600
		{180,534},		//3700
		{215,508},		//3800
		{240,482},		//3900
		{265,456},		//4000
		{290,430},		//4100
		{315,404},		//4200
		{340,378},		//4300
		{365,352},		//4400
		{390,326},		//4500
		{415,300},		//4600
		{440,274},		//4700
		{465,248},		//4800
		{490,222},		//4900
		{515,196},		//5000
		{540,144},		//5100
		{565,118},		//5200
		{590,92},			//5300
		{615,66},			//5400
		{640,40},			//5500
		{680,0},			//5600
};
//
//const float saturationArray[101]=
//{
//	0,0.015,0.030,0.045,0.060,		0.075,0.090,0.105,0.120,0.135,
//	0.150,0.165,0.180,0.195,0.210,	0.225,0.240,0.255,0.270,0.285,
//	0.300,0.315,0.330,0.345,0.360,	0.375,0.390,0.405,0.420,0.435,
//	0.450,0.465,0.480,0.495,0.510,	0.525,0.540,0.555,0.570,0.585,
//	0.600,0.610,0.620,0.630,0.640,	0.650,0.650,0.670,0.680,0.690,
//	0.700,0.710,0.720,0.730,0.740,	0.750,0.760,0.770,0.780,0.790,
//	0.800,0.805,0.810,0.815,0.820,	0.825,0.830,0.835,0.840,0.845,
//	0.850,0.855,0.860,0.865,0.870,	0.875,0.880,0.885,0.890,0.895,
//	0.900,0.905,0.910,0.915,0.920,	0.925,0.930,0.935,0.940,0.945,
//	0.950,0.955,0.960,0.965,0.970,	0.975,0.980,0.985,0.990,0.995,1.000
//};

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* VARIABLES
*/
uint8 light_TaskID;

static light_ctrl_t light;
colorStructType  color;
uint8 pec = 0;
extern	float	vTemperatureCoe;
extern	displayParamsStruct displayParams;
extern	PcaDataStruct		pcaDataStruct;
/*********************************************************************
* FUNCTIONS
*/
static void light_ProcessOSALMsg(osal_event_hdr_t* pMsg);
//static void hsb2Rgb(colorStructType * colorStruct);


/*********************************************************************
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
void light_Init(uint8 task_id)
{
	light_TaskID = task_id;

	hal_gpio_pin_init(GPIO_LIGHT_POWER, OEN);
//	hal_gpio_pin_init(GPIO_LIGHT_EN, OEN);
	light_off();
}

/*********************************************************************
 * @fn      light_ProcessOSALMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void light_ProcessOSALMsg(osal_event_hdr_t* pMsg)
{
	switch(pMsg->event)
	{
		default:
			// do nothing
			break;
	}
}

/*********************************************************************
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
uint16 light_ProcessEvent(uint8 task_id, uint16 events)
{
	VOID task_id; // OSAL required parameter that isn't used in this function

	if(events & SYS_EVENT_MSG)
	{
		uint8* pMsg;

		if((pMsg = osal_msg_receive(light_TaskID)) != NULL)
		{
			light_ProcessOSALMsg((osal_event_hdr_t*)pMsg);

			// Release the OSAL message
			VOID osal_msg_deallocate(pMsg);
		}

		// return unprocessed events
		return (events ^ SYS_EVENT_MSG);
	}

	if (events & LIGHT_CHANGE_EVT)
	{
		

		return (events ^ LIGHT_CHANGE_EVT);
	}

	// Discard unknown events
	return 0;
}

///*********************************************************************
// * @fn      hsb2Rgb
// *
// * @brief   cal rgb data.
// *
// * @param   none
// *
// * @return  none
// */
//static void hsb2Rgb(colorStructType * colorStruct)
//{
//	uint16 	h=colorStruct->hues;
////	float	 s=colorStruct->saturation;
//	float s=0;
////	float vtRgbRate=0;
//	float	 v=colorStruct->brightness;
//	float r = 0, g = 0, b = 0;
//	float f,p,q,t,total;
//	int i=0;
//	s=saturationArray[(uint8)(colorStruct->saturation*100)];
//	if(h>=360)
//		h=0;
//	if(s>=1.0)
//		s=1.0;
//	if(v>=1.0)
//		v=1.0;
//	i = ( h / 60) % 6;
//	f = ((float)h / 60) - i;
//	p = v * (1 - s);
//	q = v * (1 - f * s);
//	t = v * (1 - (1 - f) * s);
//	switch (i) {
//	case 0:
//		r = v;
//		g = t;
//		b = p;
//		break;
//	case 1:
//		r = q;
//		g = v;
//		b = p;
//		break;
//	case 2:
//		r = p;
//		g = v;
//		b = t;
//		break;
//	case 3:
//		r = p;
//		g = q;
//		b = v;
//		break;
//	case 4:
//		r = t;
//		g = p;
//		b = v;
//		break;
//	case 5:
//		r = v;
//		g = p;
//		b = q;
//		break;
//	default:
//		break;
//	}
//
//	//minChange(&r,&g,&b,s);
//
//	total=r+g+b;
////	r=r*v/total;
////	g=g*v/total;
////	b=b*v/total;
//	r/=total;
//	g/=total;
//	b/=total;
//	if(v>=BrightnessThreshold){
//		r*=1/RedGreenRate;
//		b*=1/BlueGreenRate;
//
////		total=r+g+b;
////		r=r*v/total;
////		g=g*v/total;
////		b=b*v/total;
//	}else{
//		r*=1.05/RedGreenRate+(RedGreenRateStep*(BrightnessThreshold-v)/BrightnessGap);
//		b*=1.11/BlueGreenRate+(BlueGreenRateStep*(BrightnessThreshold-v)/BrightnessGap);
////		total=r+g+b;
////		r=r*v/total;
////		g=g*v/total;
////		b=b*v/total;
////		if(r){
////			r*=1-RGBLED_REAL_START_DUTY_PERCENT;
////			r+=RGBLED_REAL_START_DUTY_PERCENT;
////		}
////		if(g){
////			g*=1-RGBLED_REAL_START_DUTY_PERCENT;
////			g+=RGBLED_REAL_START_DUTY_PERCENT;
////		}
////		if(b){
////			b*=1-RGBLED_REAL_START_DUTY_PERCENT;
////			b+=RGBLED_REAL_START_DUTY_PERCENT;
////		}
//
//	}
//	total=r+g+b;
//	r=r*v/total;
//	g=g*v/total;
//	b=b*v/total;
//
//	r*=0.95/1.0;
//	g*=0.95/1.0;
//	b*=0.95/1.0;
//
//	colorStruct->red=(uint16) (r*PWM_MAX_COUNT);
//	colorStruct->blue=(uint16) (b*PWM_MAX_COUNT);
//	colorStruct->green=(uint16) (g*PWM_MAX_COUNT);
//}

/*********************************************************************
 * @fn      light_off
 *
 * @brief   update light.
 *
 * @param   none
 *
 * @return  none
 */
void light_off(void)
{
	pwm_ch_t ch;
	hal_gpio_write(GPIO_LIGHT_POWER, GPIO_HL_LIGHT_POWER_OFF);
//	hal_gpio_write(GPIO_LIGHT_EN, GPIO_HL_LIGHT_EN_OFF);
	light.power_on = false;
	light.en = false;
//	hal_pwm_close_channel(PWM_CH0);
//	hal_pwm_destroy(PWM_CH0);
//	hal_pwm_close_channel(PWM_CH1);
//	hal_pwm_destroy(PWM_CH1);
	ch.pwmN = Red_LED_CHANNEL;
	hal_pwm_ch_stop(ch);
	ch.pwmN = Green_LED_CHANNEL;
	hal_pwm_ch_stop(ch);
	ch.pwmN = Blue_LED_CHANNEL;
	hal_pwm_ch_stop(ch);
	ch.pwmN = CW_LED_CHANNEL;
	hal_pwm_ch_stop(ch);
	ch.pwmN = MW_LED_CHANNEL;
	hal_pwm_ch_stop(ch);
	hal_pwm_stop();
	hal_gpio_pin_init(GPIO_LIGHT_CW, OEN);
	hal_gpio_write(GPIO_LIGHT_CW, 0);
	hal_gpio_pin_init(GPIO_LIGHT_MW, OEN);
	hal_gpio_write(GPIO_LIGHT_MW, 0);
	hal_gpio_pin_init(GPIO_LIGHT_RED, OEN);
	hal_gpio_write(GPIO_LIGHT_RED, 0);
	hal_gpio_pin_init(GPIO_LIGHT_GREEN, OEN);
	hal_gpio_write(GPIO_LIGHT_GREEN, 0);
	hal_gpio_pin_init(GPIO_LIGHT_BLUE, OEN);
	hal_gpio_write(GPIO_LIGHT_BLUE, 0);
//	LOG("\n__light off__\n");
}


void	 pwmInit(void){
	pwm_ch_t ch;
	 hal_pwm_module_init();

//	ch.pwmN = BACKLIGHT_CHANNEL;
//	ch.pwmPin = GPIO_LCD_BACKLIGHT;
//	ch.pwmDiv = PWM_CLK_NO_DIV;
//	ch.pwmMode = PWM_CNT_UP;
//	ch.pwmPolarity = PWM_POLARITY_RISING;
//	ch.cmpVal = PWM_FREQ_CONST;
//	ch.cntTopVal = PWM_FREQ_CONST;
//	hal_pwm_ch_start(ch);

	ch.pwmN = Red_LED_CHANNEL;
	ch.pwmPin = GPIO_LIGHT_RED;
	ch.pwmDiv = PWM_CLK_NO_DIV;
	ch.pwmMode = PWM_CNT_UP;
	ch.pwmPolarity = PWM_POLARITY_RISING;
	ch.cmpVal = PWM_FREQ_CONST;
	ch.cntTopVal = PWM_FREQ_CONST;
	hal_pwm_ch_start(ch);

	ch.pwmN = Green_LED_CHANNEL;
	ch.pwmPin = GPIO_LIGHT_GREEN;
	ch.pwmDiv = PWM_CLK_NO_DIV;
	ch.pwmMode = PWM_CNT_UP;
	ch.pwmPolarity = PWM_POLARITY_RISING;
	ch.cmpVal = PWM_FREQ_CONST;
	ch.cntTopVal = PWM_FREQ_CONST;
	hal_pwm_ch_start(ch);
	ch.pwmN = Blue_LED_CHANNEL;
	ch.pwmPin = GPIO_LIGHT_BLUE;
	ch.pwmDiv = PWM_CLK_NO_DIV;
	ch.pwmMode = PWM_CNT_UP;
	ch.pwmPolarity = PWM_POLARITY_RISING;
	ch.cmpVal = PWM_FREQ_CONST;
	ch.cntTopVal = PWM_FREQ_CONST;
	hal_pwm_ch_start(ch);
	ch.pwmN = CW_LED_CHANNEL;
	ch.pwmPin = GPIO_LIGHT_CW;
	ch.pwmDiv = PWM_CLK_NO_DIV;
	ch.pwmMode = PWM_CNT_UP;
	ch.pwmPolarity = PWM_POLARITY_RISING;
	ch.cmpVal = PWM_FREQ_CONST;
	ch.cntTopVal = PWM_FREQ_CONST;
	hal_pwm_ch_start(ch);
	ch.pwmN = MW_LED_CHANNEL;
	ch.pwmPin = GPIO_LIGHT_MW;
	ch.pwmDiv = PWM_CLK_NO_DIV;
	ch.pwmMode = PWM_CNT_UP;
	ch.pwmPolarity = PWM_POLARITY_RISING;
	ch.cmpVal = PWM_FREQ_CONST;
	ch.cntTopVal = PWM_FREQ_CONST;


	hal_pwm_ch_start(ch);
	hal_gpio_write(GPIO_LIGHT_POWER, GPIO_HL_LIGHT_POWER_ON);
//	hal_gpio_write(GPIO_LIGHT_EN, GPIO_HL_LIGHT_EN_ON);
//	LOG("\n__PWM initiation__\n");
//        hal_pwrmgr_lock(MOD_PWM);
//        PWM_ENABLE_ALL;
}

/*********************************************************************
 * @fn      light_update
 *
 * @brief   update light data.
 *
 * @param   none
 *
 * @return  none
 */
void light_update(void) {
	if (!light.power_on || !light.en) {
		//if ((false == hal_gpio_read(GPIO_LIGHT_POWER)) || (false == hal_gpio_read(GPIO_LIGHT_EN))) {
			pwmInit();
		//}
		light.power_on = true;
		hal_gpio_write(GPIO_LIGHT_POWER, GPIO_HL_LIGHT_POWER_ON);
		light.en = true;
//		hal_gpio_write(GPIO_LIGHT_EN, GPIO_HL_LIGHT_EN_ON);

	}
		setCoolData(color.cw);
		setWarmData(color.mw);
		setRedData(color.red);
		setGreenData(color.green);
		setBlueData(color.blue);
	if (color.cw || color.mw || color.red || color.green || color.blue||light.mode||light.effemode) {

		hal_pwm_start();
	} else {
		hal_pwm_stop();
	}
	return ;
}

bool getLightStts(void){
	return	light.power_on;
}


/*********************************************************************
 * @fn      light_ctrl
 *
 * @brief   ctrl light.
 *
 * @param   *pPgk
 *
 * @return  error code
 */
uint8 light_ctrl(uint8 *pPkg) {
	uint8 index = 0;
	uint16 flag = 0;
//	LOG("\n%02x,%02x,%02x,%02x,%02x,%02x,  %02x,%02x,%02x,%02x,%02x\n",*(pPkg),*(pPkg+1),*(pPkg+2),*(pPkg+3),*(pPkg+4),*(pPkg+5),*(pPkg+6),*(pPkg+7),*(pPkg+8),*(pPkg+9),*(pPkg+10));
//	uint32 temp=500000;
//	while(--temp);

	index++;	//len										//byte[0]
	flag = *(pPkg + index) << 8;
	index++;												//byte[1]
	flag |= *(pPkg + index);
	index++;												//byte[2]

	if (!(flag & CCS_FLAG_COMMAND) && !(flag & CCS_FLAG_MODE)) {
		return PPlus_ERR_INVALID_FLAGS;
	}

	light.command = *(pPkg + index);
	index++;												//byte[3]
	switch (light.command) {
	default:
	case MODE_OFF:
		break;

	case MODE_CCT:
		if (*(pPkg + index) != 0) {
			return PPlus_ERR_INVALID_PARAM;
		}
		light.mode = *(pPkg + index);
		index++;
		break;

	case MODE_HSI:
		if (*(pPkg + index) != 0) {
			return PPlus_ERR_INVALID_PARAM;
		}
		light.mode = *(pPkg + index);
		index++;
		break;

	case MODE_FIXED:
		if (*(pPkg + index) == 0) {
			return PPlus_ERR_INVALID_PARAM;
		}

		light.mode = *(pPkg + index);
		index++;
		break;

	case MODE_EFFE:
		if (*(pPkg + index) != 0) {
			*(pPkg + index)=0;
//			return PPlus_ERR_INVALID_PARAM;
		}

		light.mode = *(pPkg + index);
		index++;
		break;
	}

	if (flag & CCS_FLAG_BRIGHTNESS) {
		light.brightness = *(pPkg + index);
		index++;
	}

	if (flag & CCS_FLAG_TEMPERATURE) {
		light.colorTemperature = *(pPkg + index);
		index++;
	}

	if (flag & CCS_FLAG_HUE) {
		light.hues = (*(pPkg + index)) << 8;
		index++;
		light.hues |= *(pPkg + index);
		index++;
	}

	if (flag & CCS_FLAG_SATURATION) {
		light.saturation = *(pPkg + index);
		index++;
	}

	if (flag & CCS_FLAG_EFFECTSMODE) {
		light.effemode = *(pPkg + index);
		index++;
	}

	if (flag & CCS_FLAG_TIMES) {
		light.times = *(pPkg + index);
		index++;
	}

	if (flag & CCS_FLAG_FREQ) {
		light.freq = *(pPkg + index);
		index++;
	}

	if (light.command == MODE_OFF) {
		light_off();
		return PPlus_SUCCESS;
	}

	switch (light.command) {
	default:
	case MODE_OFF:
		light_off();
		break;

	case MODE_CCT:
		updateColorTemp(&displayParams);

		break;

	case MODE_HSI:
		color.cw = 0;
		color.mw = 0;
		if (light.brightness)
			color.brightness = ((float) light.brightness) * (100 - RGBLED_REAL_START_DUTY) / (100 * 100) + (float) RGBLED_REAL_START_DUTY / 100;			//从9%起步
		else
			color.brightness = 0;
		color.saturation = (float) light.saturation / 100;
		color.hues = light.hues;
		hsb2Rgb(&color);

		break;
	case MODE_FIXED:
		color.cw = 0;
		color.mw = 0;
		color.red = 0;
		color.green = 0;
		color.blue = 0;
		displayParams.preinstallEffectNo = light.mode;
//		LOG("_\n fixed mode =%d\n_",displayParams.preinstallEffectNo);
		startLightEffect(&displayParams);
		break;

	case MODE_EFFE:
//		LOG("_\n MODE_EFFE \n_");
		color.cw = 0;
		color.mw = 0;
		color.red = 0;
		color.green = 0;
		color.blue = 0;
//		displayParams.backupArrowIndex=(displayParams.vModeIndex<PreinstallEffect)?displayParams.vModeIndex:displayParams.backupArrowIndex;
//		displayParams.arrowIndex = CustomizeEffect;
//		if(displayParams.fIsFromRGBMode)
//			LOG("\nRGB\n");
//		else
//			LOG("\nCCT\n");
		startLightEffect(&displayParams);
		break;
	}

	light_update();
	return PPlus_SUCCESS;
}

void	updateRGBLamp(displayParamsStruct * disParams){
	color.cw = 0;
	color.mw = 0;
	//color.brightness = (float)light.brightness/100;
	if(disParams->brightness)
		color.brightness=((float)disParams->brightness)*(100-RGBLED_REAL_START_DUTY)/(100*100)+(float)RGBLED_REAL_START_DUTY/100;			//从9%起步
	else
		color.brightness=0;
	color.saturation = (float)disParams->saturation/100;
	color.hues = disParams->hues;
	hsb2Rgb(&color);
//	LOG("\n  R:%d G:%d B:%d \n ",color.red,color.green,color.blue);
//	LOG("\n  power_on=%d    light.en=%d\n ",light.power_on,light.en);
	light_update();
}

void updateColorTemp(displayParamsStruct * disParams){

	u16 vtCoolData=0, vtWarmData=0;
//	LOG("\n__B:%d T:%d",disParams->brightness,disParams->colorTemperature);
	if(disParams->brightness){
	//	vtWarmData = ((uint32)(table_temperature[disParams->colorTemperature-32][0]) * (uint32)(disParams->brightness*80/100*94+600) )/10000;
	//	vtCoolData = ((uint32)(table_temperature[disParams->colorTemperature-32][1]) * (uint32)(disParams->brightness*80/100*94+600) )/10000;
		vtCoolData = table_temperature[(disParams->colorTemperature - MIN_ColorTemp)][0];
		vtWarmData = table_temperature[(disParams->colorTemperature - MIN_ColorTemp)][1];
	}
	if(vtCoolData){
		vtCoolData-=LED_REAL_START_DATA;
		setCoolData((vTemperatureCoe*vtCoolData *(((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)+LED_REAL_START_DATA));				//从4%起步
	}else{
		setCoolData(0);
	}
	if(vtWarmData){
		vtWarmData-=LED_REAL_START_DATA;
		setWarmData((vTemperatureCoe*vtWarmData *(((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)+LED_REAL_START_DATA));			//从4%起步
	}else{
		setWarmData(0);
	}

	color.cw=pcaDataStruct.valueOfCw;
	color.mw=pcaDataStruct.valueOfMw;
	color.red = 0;
	color.green = 0;
	color.blue = 0;
//	LOG("\n__cc:%d  cm:%d",color.cw,color.mw);
//	LOG("\n__pc:%d  pm:%d",pcaDataStruct.valueOfCw,pcaDataStruct.valueOfMw);

	color.brightness =disParams->brightness;
	color.hues = 0;
	color.saturation = 0;
}

void	 loadTargetColorTemperatureValue(displayParamsStruct * disParams,PcaDataStruct * pcaData){
	u16 vtCoolData=0, vtWarmData=0;
	if(disParams->brightness){
		vtCoolData = table_temperature[(disParams->colorTemperature - MIN_ColorTemp)][0];
		vtWarmData = table_temperature[(disParams->colorTemperature - MIN_ColorTemp)][1];
	}
	if(vtCoolData){
		vtCoolData-=LED_REAL_START_DATA;
		pcaData->valueOfCw=(U16)((vTemperatureCoe*vtCoolData *(((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)+LED_REAL_START_DATA));				//从4%起步
	}else{
		pcaData->valueOfCw=0;
	}
	if(vtWarmData){
		vtWarmData-=LED_REAL_START_DATA;
		pcaData->valueOfMw=(U16)((vTemperatureCoe*vtWarmData *(((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)+LED_REAL_START_DATA));			//从4%起步
	}else{
		pcaData->valueOfMw=0;
	}
	pcaData->valueOfRed=0;
	pcaData->valueOfGreen=0;
	pcaData->valueOfBlue=0;
}

void	 loadTargetRGBValue(displayParamsStruct * disParams,PcaDataStruct * pcaData){
	colorStructType csType={0};
	if(disParams->brightness)
		csType.brightness=((float)disParams->brightness)*(100-RGBLED_REAL_START_DUTY)/(100*100)+(float)RGBLED_REAL_START_DUTY/100;			//从9%起步
	else
		csType.brightness=0;
	csType.saturation = (float)disParams->saturation/100;
	csType.hues = disParams->hues;
	hsb2Rgb(&csType);
	pcaData->valueOfRed=csType.red;
	pcaData->valueOfGreen=csType.green;
	pcaData->valueOfBlue=csType.blue;
	pcaData->valueOfMw=0;
	pcaData->valueOfCw=0;
}
/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
