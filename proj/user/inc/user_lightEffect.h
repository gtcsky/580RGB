/*
 * user_lightEffect.h
 *
 *  Created on: 2020年10月23日
 *      Author: Sky
 */

#ifndef SOURCE_INC_USER_LIGHTEFFECT_H_
#define SOURCE_INC_USER_LIGHTEFFECT_H_
#include "types.h"
#include "protocol.h"

typedef struct{
	UINT16	indicatorMaxIndex;
	UINT16	currentIndex;
	UINT8	indicatorStyle;
	UINT16	customizeLoopTimes;				//自定义模式循环次数
	UINT8	customizeLoopIndex;				//自定义模式循环已执行次数
	const 	UINT8 *	timeArray;
	const	UINT8 *	sttsArray;
//	func_ptr_t  pfnLedFuncCallBack;
//	func_ptr_t  pfncustomizeEffectOverCallBack;	//循环结束时回调
	void 	(* pfnLedFuncCallBack)(void);
	void 	(* pfncustomizeEffectOverCallBack)(void);	//循环结束时回调
	UINT8	fIsGradualMode;					//0/1 闪烁模式/渐变模式
}LEDParametersStruct;

extern	LEDParametersStruct	LedStruct;
extern	LEDParametersStruct	*pLedStruct;
extern	u8 	fIsLightEffectOn;		//是否开启光效模式
extern	u8  	fIsGradientMode;		// 是否渐变模

void flashMode1StyleCfunc(void);
void initLEDStyleParams(void);
void lightEffectFunc(void);
void turnOffAllLightEffect(void);
void set2FlashMode1StyleA(void);
void setLEDFunc(LEDParametersStruct * ledStruct);
void startLightEffect(displayParamsStruct * disParams);
void lightning1Process(void);
void fireProcess(void);
#endif /* SOURCE_INC_USER_LIGHTEFFECT_H_ */
