/*
 * user_color.h
 *
 *  Created on: 2019��9��26��
 *      Author: Bough_004
 */

#ifndef USER_INC_USER_COLOR_H_
#define USER_INC_USER_COLOR_H_

#include "types.h"
#include "protocol.h"
#include "light.h"
#include "pwm.h"
extern	void hsb2Rgb(colorStructType * colorStruct);
extern	void updateColor(colorStructType * colorStruct);
//extern	void updateColorTemp(displayParamsStruct * disParams);
extern	void turnOffRGBLamp(void);
extern	void turnOffColorTempLamp(void);
extern	void superFlashMode(uint8 index);
extern	void updateBrightnessByTemperature(void);
void	 setCoolData(u16 pwmData);
void	 setWarmData(u16 pwmData);
void	 setGreenData(u16 pwmData);
void	 setRedData(u16 pwmData);
void	 setBlueData(u16 pwmData);
void updatePWMDuty(PWMN_e pwmN,float duty);
void originalHsv2Rgb(uint16 hues, float brightness, uint8 saturation, float * rData, float * gData, float * bData) ;
void updateRgbOnly(colorStructType * colorStruct);
#endif /* USER_INC_USER_COLOR_H_ */
