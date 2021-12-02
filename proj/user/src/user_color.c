/*
 * user_color.c
 *
 *  Created on: 2020年10月23日
 *      Author: Sky
 */
#include "user_color.h"
#include "log.h"
//#include "io_define.h"
//#include "user_advtime.h"
//#include "user_menu.h"
extern	PcaDataStruct		pcaDataStruct;
extern	colorStructType  *pUserColor;
extern	colorStructType  color;
float	vTemperatureCoe=1.0;

const float saturationArray[101]={
				0,0.015,0.030,0.045,0.060,			0.075,0.090,0.105,0.120,0.135,
				0.150,0.165,0.180,0.195,0.210,		0.225,0.240,0.255,0.270,0.285,
				0.300,0.315,0.330,0.345,0.360,		0.375,0.390,0.405,0.420,0.435,
				0.450,0.465,0.480,0.495,0.510,		0.525,0.540,0.555,0.570,0.585,
				0.600,0.610,0.620,0.630,0.640,		0.650,0.650,0.670,0.680,0.690,
				0.700,0.710,0.720,0.730,0.740,		0.750,0.760,0.770,0.780,0.790,
				0.800,0.805,0.810,0.815,0.820,		0.825,0.830,0.835,0.840,0.845,
				0.850,0.855,0.860,0.865,0.870,		0.875,0.880,0.885,0.890,0.895,
				0.900,0.905,0.910,0.915,0.920,		0.925,0.930,0.935,0.940,0.945,
				0.950,0.955,0.960,0.965,0.970,		0.975,0.980,0.985,0.990,0.995,1.000
};

/************************************************************************
 *
 * 通过设置捕获值设置PWM 占空比
 *
 ************************************************************************/
void	 setCoolDuty(float duty){
	//	LOG("\nset cool:%d\n",(U16)(duty*PWM_PERIOD_CONST));
		updatePWMDuty(CW_LED_CHANNEL,duty);
}

void	 setCoolData(u16 pwmData){
	float duty=0;
	duty=(float)pwmData/PWM_PERIOD_CONST;
	setCoolDuty(duty);
	pcaDataStruct.valueOfCw=pwmData;
}
/************************************************************************
 *
 * 通过设置捕获值设置PWM 占空比
 *
 ************************************************************************/
void setWarmDuty(float duty){
	//	LOG("\nset warm:%d\n",(U16)(duty*PWM_PERIOD_CONST));
	updatePWMDuty(MW_LED_CHANNEL,duty);
}
void setWarmData(u16 pwmData){
	float duty=0;
	duty=(float)pwmData/PWM_PERIOD_CONST;
	updatePWMDuty(MW_LED_CHANNEL,duty);
	pcaDataStruct.valueOfMw=pwmData;
}

void setRedDuty(float duty){
	updatePWMDuty(Red_LED_CHANNEL,duty);
}
void	 setRedData(u16 pwmData){
	float duty=0;
	duty=(float)pwmData/PWM_PERIOD_CONST;
	setRedDuty(duty);
	pcaDataStruct.valueOfRed=pwmData;
}

void setGreenDuty(float duty){
	updatePWMDuty(Green_LED_CHANNEL,duty);
}
void	 setGreenData(u16 pwmData){
	float duty=0;
	duty=(float)pwmData/PWM_PERIOD_CONST;
	setGreenDuty(duty);
	pcaDataStruct.valueOfGreen=pwmData;

}

void	 setBlueDuty(float duty){
		updatePWMDuty(Blue_LED_CHANNEL,duty);
}

void	 setBlueData(u16 pwmData){
	float duty=0;
	duty=(float)pwmData/PWM_PERIOD_CONST;
	setBlueDuty(duty);
	pcaDataStruct.valueOfBlue=pwmData;
}

void updateColor(colorStructType * colorStruct){

	turnOffColorTempLamp();
	setRedData(vTemperatureCoe*(colorStruct->red));			//Red
	setGreenData(vTemperatureCoe*(colorStruct->green));		//Green
	setBlueData(vTemperatureCoe*(colorStruct->blue));		//Blue

}

void updateRgbOnly(colorStructType * colorStruct){
	setRedData(vTemperatureCoe*(colorStruct->red));			//Red
	setGreenData(vTemperatureCoe*(colorStruct->green));		//Green
	setBlueData(vTemperatureCoe*(colorStruct->blue));		//Blue
}

/***********************************************************************************************************
  *  @brief
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void originalHsv2Rgb(uint16 hues, float brightness, uint8 saturation, float * rData, float * gData, float * bData) {
	UINT16 h = hues;
	float s = 0;
	float v = brightness;
	float r = 0, g = 0, b = 0;
	float f, p, q, t;
	int i = 0;
	s = saturationArray[saturation];
	if (h >= 360)
		h = 0;
	if (s >= 1.0)
		s = 1.0;
	if (v >= 1.0)
		v = 1.0;
	i = (h / 60) % 6;
	f = ((float) h / 60) - i;
	p = v * (1 - s);
	q = v * (1 - f * s);
	t = v * (1 - (1 - f) * s);
	switch (i) {
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	case 5:
		r = v;
		g = p;
		b = q;
		break;
	default:
		break;
	}
	*rData=r;
	*gData=g;
	*bData=b;
//	LOG("R:\n%d\tG:%d\tB:%d",(uint8)(r*255),(uint8)(g*255),(uint8)(b*255));
}
/************************************************************************
 *
 *	HSV转RGB
 *
 ************************************************************************/
void hsb2Rgb(colorStructType * colorStruct) {
	float r = 0, g = 0, b = 0, total = 0;
	float v = colorStruct->brightness;
	UINT16 h = colorStruct->hues;
//	originalHsv2Rgb(h, 100*colorStruct->brightness, 100*colorStruct->saturation, &r, &g, &b);
	float s = 0;
	float f, p, q, t;
	int i = 0;
	s = saturationArray[(u8) (colorStruct->saturation * 100)];
	if (h >= 360)
		h = 0;
	if (s >= 1.0)
		s = 1.0;
	if (v >= 1.0)
		v = 1.0;
	i = (h / 60) % 6;
	f = ((float) h / 60) - i;
	p = v * (1 - s);
	q = v * (1 - f * s);
	t = v * (1 - (1 - f) * s);
	switch (i) {
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	case 5:
		r = v;
		g = p;
		b = q;
		break;
	default:
		break;
	}

	total = r + g + b;
	r /= total;
	g /= total;
	b /= total;
	if (v >= BrightnessThreshold) {
		r *= 1 / RedGreenRate;
		b *= 1 / BlueGreenRate;
	} else {
		r *= 0.90 / RedGreenRate + (RedGreenRateStep * (BrightnessThreshold - v) / BrightnessGap);
		b *= 1.15 / BlueGreenRate + (BlueGreenRateStep * (BrightnessThreshold - v) / BrightnessGap);
	}
	total = r + g + b;
	r = r * v / total;
	g = g * v / total;
	b = b * v / total;

	r *= 0.95 / 1.0;
	g *= 0.95 / 1.0;
	b *= 0.95 / 1.0;

	colorStruct->red = (UINT16) (r * PWM_FRQ_CONST);
	colorStruct->blue = (UINT16) (b * PWM_FRQ_CONST);
	colorStruct->green = (UINT16) (g * PWM_FRQ_CONST);
	if (colorStruct->red && colorStruct->red < 8)
		colorStruct->red = 8;
	if (colorStruct->green && colorStruct->green < 8)
		colorStruct->green = 8;
	if (colorStruct->blue && colorStruct->blue < 8)
		colorStruct->blue = 8;
}
/*****************************************************************
 *
 * 关闭色温灯通道
 *
 ******************************************************************/
void turnOffColorTempLamp(void){
	setWarmData(0);				//暖光
	setCoolData(0);				//冷光
}
void	turnOffRGBLamp(void){
	setRedData(0);
	setGreenData(0);
	setBlueData(0);
}
//void updateColorTemp(displayParamsStruct * disParams) {
//	u16 vtCoolData=0, vtWarmData=0;
//	if(disParams->brightness){
//		vtCoolData = colorTempArray[(disParams->colorTemperature - MIN_ColorTemp)][0];
//		vtWarmData = colorTempArray[(disParams->colorTemperature - MIN_ColorTemp)][1];
//	}
//	if(vtCoolData){
//		vtCoolData-=LED_REAL_START_DATA;
//		setCoolData((vTemperatureCoe*vtCoolData *(((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)+LED_REAL_START_DATA));				//从4%起步
//	}else{
//		setCoolData(0);
//	}
//	if(vtWarmData){
//		vtWarmData-=LED_REAL_START_DATA;
//		setWarmData((vTemperatureCoe*vtWarmData *(((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)+LED_REAL_START_DATA));			//从4%起步
//	}else{
//		setWarmData(0);
//	}
//}

//void	setTargeColorTempData(displayParamsStruct * disParams,PcaDataStruct *targetData){
//	u16 vtCoolData=0, vtWarmData=0;
//	if(disParams->brightness){
//		vtCoolData = colorTempArray[(disParams->colorTemperature - MIN_ColorTemp)][0];
//		vtWarmData = colorTempArray[(disParams->colorTemperature - MIN_ColorTemp)][1];
//	}
//	if(vtCoolData){
//		vtCoolData-=LED_REAL_START_DATA;
//		targetData->valueOfCw=(vTemperatureCoe*vtCoolData *(((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)+LED_REAL_START_DATA);			//从4%起步
//	}else{
//		targetData->valueOfCw=0;
//	}
//	if(vtWarmData){
//		vtWarmData-=LED_REAL_START_DATA;
//		targetData->valueOfMw= (vTemperatureCoe*vtWarmData *(((float) disParams->brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)+LED_REAL_START_DATA);			//从4%起步
//	}else{
//		targetData->valueOfMw=0;
//	}
//}
//
//void	updateRGBLamp(displayParamsStruct * disParams){
//	pUserColor->hues=disParams->hues;
//	pUserColor->saturation=(float)disParams->saturation/100;
//	if(disParams->brightness)
//		pUserColor->brightness=((float)disParams->brightness)*(100-RGBLED_REAL_START_DUTY)/(100*100)+(float)RGBLED_REAL_START_DUTY/100;			//从9%起步
//	else
//		pUserColor->brightness=0;
//	hsb2Rgb(pUserColor);
//	updateColor(pUserColor);
//}



void updatePWMDuty(PWMN_e pwmN,float duty){
	hal_pwm_set_count_val(pwmN, PWM_FREQ_CONST*(1-duty), PWM_FREQ_CONST);
}
