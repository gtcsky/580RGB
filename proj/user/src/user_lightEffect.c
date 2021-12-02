/*
 * user_lightEffect.c
 *
 *  Created on: 2020年10月23日
 *      Author: Sky
 */


#include "string.h"
#include "user_lightEffect.h"
#include "user_color.h"
#include "command_center.h"
#include "light.h"
#include "log.h"
#include "gpio.h"
#include "protocol.h"

u8 	fIsLightEffectOn=0;					//是否开启光效模式
//u8  	ledStruct.fIsGradualMode=0;		// 是否渐变模式
u8	vLEDStepCtrlTimeCnt=0;
LEDParametersStruct	LedStruct;
LEDParametersStruct	*pLedStruct=&LedStruct;

colorStructType  userColor;
colorStructType  *pUserColor=&userColor;

PcaDataStruct		pcaDataStruct;
PcaDataStruct		*pPcaDataStruct=&pcaDataStruct;

extern	displayParamsStruct displayParams;
PcaDataStruct		oriCustomizePwmDataStruct;				//自定义渐变模式调用时,原始值
u8  fIsRisingNow=0;
//SttsArray 参数
//0x00		所有灯灭
//0x01		R亮
//0x02		G亮
//0x04		B亮
//0x08		C亮
//0x10		W亮
#define	MAX_FLASH_MODE1_STYLE_A_INDEX	9
const u8 flashMode1StyleATimerArray[MAX_FLASH_MODE1_STYLE_A_INDEX+1]={40,15,20,15,120,60,20,14,200,200};
const u8 flashMode1StyleASttsArray[MAX_FLASH_MODE1_STYLE_A_INDEX+1]={0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x00};

#define	MAX_FLASH_MODE1_STYLE_B_INDEX	15
const u8 flashMode1StyleBTimerArray[MAX_FLASH_MODE1_STYLE_B_INDEX+1]={40,20,20,15,10,   10,6,10,20,   40,20,15,20,    15,200,200};
const u8 flashMode1StyleBSttsArray[MAX_FLASH_MODE1_STYLE_B_INDEX+1]={0x00,0x08,0x00,0x08,0x00, 0x08,0x00,0x08,0x00,  0x08,0x00,0x08,0x00,  0x08,0x00,0x00};

const u8	GradualEffect1StyleCTimeArray[1]={5};
const u8	GradualEffect2StyleATimeArray[1]={10};
const u8	GradualEffect2StyleBTimeArray[1]={2};
const u8	GradualEffect2StyleCTimeArray[1]={5};
u8	customizeFlashTimeArray[1]={0};
u8	customizeGradualTimeArray[1]={1};
const uint8 Lightning1TimeArray[1]={0};
const uint8 fireTimeArray[1]={4};

const	u8	customizeOneShotSttsArray[2]={1,0};
#define	MAX_ONE_SHOT_FREQ_INDEX	13
#define	ONE_SHOT_FREQ_START_OFFSET	6
//const	u8   	customizeOneShotFreqArray[MAX_ONE_SHOT_FREQ_INDEX-ONE_SHOT_FREQ_START_OFFSET+1]={125,60,30,15,8,4,2,1};
const	u8   	customizeOneShotFreqArray[MAX_ONE_SHOT_FREQ_INDEX-ONE_SHOT_FREQ_START_OFFSET+1]={2,4,6,13,25,50,100,200};
//#define	Gradual_EFFECT1_STYLEC_STEP	10
//#define	Gradual_EFFECT2_STYLEA_STEP	10
//#define	Gradual_EFFECT2_STYLEB_STEP	2
//#define	Gradual_EFFECT2_STYLEC_STEP	10


//#define	MAX_SIMULATION_MODE_STYLE_A_INDEX	20
//const u8 simulationModeStyleATimerArray[MAX_SIMULATION_MODE_STYLE_A_INDEX+1]={20,20,20,20,20,   20,20,20,20,   20,20,20,20,    20,20,20,20,  20,20,20,20};
//const u8 simulationModeStyleASttsArray[MAX_SIMULATION_MODE_STYLE_A_INDEX+1]={0x00,0x01,0x00,0x01,0x00,   0x01,0x00,0x01,0x00,   0x01,0x00,0x04,0x00,    0x04,0x00,0x04,0x00, 0x04,0x00,0x04,0x00};
#define	MAX_SIMULATION_MODE_STYLE_A_INDEX	7
const u8 simulationModeStyleATimerArray[MAX_SIMULATION_MODE_STYLE_A_INDEX+1]=	{17,23,17,23,18,	26,0,23};
const u8 simulationModeStyleASttsArray[MAX_SIMULATION_MODE_STYLE_A_INDEX+1]=	{	0,1,0,4,0, 	4,0,1};

#define	MAX_SIMULATION_MODE_STYLE_B_INDEX	12
const u8 simulationModeStyleBTimerArray[MAX_SIMULATION_MODE_STYLE_B_INDEX+1]={40,40,40,40,40,   40,40,40,40,   40,40,40,40};
const u8 simulationModeStyleBSttsArray[MAX_SIMULATION_MODE_STYLE_B_INDEX+1]={0x00,0x04,0x00,0x04,0x00,   0x04,0x00,0x08,0x00,   0x08,0x00,0x08,0x00};

#define	MAX_SIMULATION_MODE_STYLE_C_INDEX	12
const u8 simulationModeStyleCTimerArray[MAX_SIMULATION_MODE_STYLE_C_INDEX+1]={40,40,50,40,50,   40,50,40,50,   40,50,40,50};
const u8 simulationModeStyleCSttsArray[MAX_SIMULATION_MODE_STYLE_C_INDEX+1]={0x00,0x01,0x00,0x01,0x00,   0x01,0x00,0x08,0x00,   0x08,0x00,0x08,0x00};


u8	vCandleIndex=0;

//#define  GROSS_INDEX	10
//const	u8	lightEffectMaxIndexArray[GROSS_INDEX+1]={0,MAX_FLASH_MODE1_STYLE_A_INDEX,MAX_FLASH_MODE1_STYLE_B_INDEX,0X00,
//												0X00,0X00,0X00,
//												MAX_SIMULATION_MODE_STYLE_A_INDEX,MAX_SIMULATION_MODE_STYLE_B_INDEX,MAX_SIMULATION_MODE_STYLE_C_INDEX,0};

//void  initLEDStyleParams(void){
//	DDL_ZERO_STRUCT(LedStruct);
//	DDL_ZERO_STRUCT(pcaDataStruct);
//}


void setLEDFunc(LEDParametersStruct * ledStruct){
	UINT8 vtCurLED;
	vLEDStepCtrlTimeCnt=*(ledStruct->timeArray+ledStruct->currentIndex);
	vtCurLED=*(ledStruct->sttsArray+ledStruct->currentIndex);
	if(displayParams.brightness)
		vtCurLED=*(pLedStruct->sttsArray+pLedStruct->currentIndex);
	if (!vtCurLED){
		 turnOffColorTempLamp();
		 turnOffRGBLamp();
	}
	else{
		if(vtCurLED&0x01){
			setRedData(PWM_FRQ_CONST*(((float)displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100));
		}
		if(vtCurLED&0x02){
			setGreenData(PWM_FRQ_CONST*(((float)displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100));
		}
		if(vtCurLED&0x04){
			setBlueData(PWM_FRQ_CONST*(((float)displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100));
		}
		if(vtCurLED&0x08){
			setCoolData(PWM_FRQ_CONST * (((float) displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100));				//从4%起步
		}
		if(vtCurLED&0x10){
			setWarmData(PWM_FRQ_CONST * (((float) displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100));				//从4%起步
		}
	}
}


void lightEffectFunc(void) {
	if (fIsLightEffectOn) {
		if (!LedStruct.fIsGradualMode) {
			if(CustomizeEffect==displayParams.vModeIndex){					//自定义模式
				if (vLEDStepCtrlTimeCnt == 0) {	//Flash Lamp function executing
					if (LedStruct.currentIndex >= LedStruct.indicatorMaxIndex) {
						LedStruct.currentIndex = 0;
					}else {
						LedStruct.currentIndex++;
					}
					LedStruct.pfnLedFuncCallBack();
				}else {
					vLEDStepCtrlTimeCnt--;
				}
			} else {
				//预设模式
				if (vLEDStepCtrlTimeCnt == 0) {	//Flash Lamp function executing
					if ((LedStruct.currentIndex >= LedStruct.indicatorMaxIndex)) {
						LedStruct.currentIndex = 0;
					} else {
						LedStruct.currentIndex++;
					}
					if (!LedStruct.fIsGradualMode)
						setLEDFunc(&LedStruct);
				} else {
					if (!LedStruct.fIsGradualMode)
						vLEDStepCtrlTimeCnt--;
				}
			}
		} else {			//渐变模式
			if (CustomizeEffect == displayParams.vModeIndex) {		//自定义模式
				if (vLEDStepCtrlTimeCnt == 0) {	//Flash Lamp function executing
					if (LedStruct.currentIndex >= LedStruct.indicatorMaxIndex) {
						LedStruct.currentIndex = 0;
						if(LedStruct.customizeLoopTimes<INFINITE_LOOP_TIMES){
							if(++LedStruct.customizeLoopIndex>=LedStruct.customizeLoopTimes){
								LedStruct.pfncustomizeEffectOverCallBack();
								return;											//预设循环次数完成
							}
						}
					}else {
						LedStruct.currentIndex++;
					}
					LedStruct.pfnLedFuncCallBack();
				}else {
					vLEDStepCtrlTimeCnt--;
				}
			} else {
				//预设模式
				if (vLEDStepCtrlTimeCnt == 0) {
					LedStruct.pfnLedFuncCallBack();
				} else {
					vLEDStepCtrlTimeCnt--;
				}
			}
		}
	}
}

void turnOffAllLightEffect(void) {
	fIsLightEffectOn = 0;
	vCandleIndex=0;
	if(displayParams.fIsEffectMode){
		stopLightEffectEvent();
		displayParams.fIsEffectMode=FALSE;
	}
	turnOffColorTempLamp();
	turnOffRGBLamp();
}
/*********************************************************************
 *
 * 	闪烁模式
 *
 **********************************************************************/
void setFlashEffectPub(LEDParametersStruct * ledStruct){
	fIsLightEffectOn = 1;
	ledStruct->currentIndex=0;
	ledStruct->fIsGradualMode=0;
	displayParams.fIsEffectMode=TRUE;
	setLEDFunc(ledStruct);
}

/*********************************************************************
 *
 * 	渐变模式
 *
 **********************************************************************/
void setGradualEffectPub(LEDParametersStruct * ledStruct){
	fIsLightEffectOn =1;
	vCandleIndex=0;
	displayParams.fIsEffectMode=TRUE;
	ledStruct->fIsGradualMode=1;
	ledStruct->sttsArray=NULL;		//不使用
	ledStruct->pfnLedFuncCallBack();
}

//void	 set2FlashMode1StyleA(void){
//	LedStruct.indicatorMaxIndex=MAX_FLASH_MODE1_STYLE_A_INDEX;
//	LedStruct.indicatorStyle=1;					//flashMode1  styleA
//	LedStruct.sttsArray=flashMode1StyleASttsArray;
//	LedStruct.timeArray=flashMode1StyleATimerArray;
//	setFlashEffectPub(&LedStruct);
//}
//void	 set2FlashMode1StyleB(void){
//	LedStruct.indicatorMaxIndex=MAX_FLASH_MODE1_STYLE_B_INDEX;
//	LedStruct.indicatorStyle=2;					//flashMode1  styleB
//	LedStruct.sttsArray=flashMode1StyleBSttsArray;
//	LedStruct.timeArray=flashMode1StyleBTimerArray;
//	setFlashEffectPub(&LedStruct);
//}
//void	set2FlashMode1StyleC(void){
//	LedStruct.indicatorStyle=3;					//flashMode1  styleC
//	LedStruct.pfnLedFuncCallBack=flashMode1StyleCfunc;
//	LedStruct.currentIndex=0;
//	LedStruct.indicatorMaxIndex=90;
//	LedStruct.timeArray=GradualEffect1StyleCTimeArray;
//	fIsRisingNow=1;
//	setGradualEffectPub(&LedStruct);
//}

void sceneLightning1(void) {
	LedStruct.pfnLedFuncCallBack = lightning1Process;
	LedStruct.currentIndex = 0;
	LedStruct.indicatorMaxIndex = 714;
	LedStruct.timeArray = Lightning1TimeArray;
	pUserColor->hues = 0;
	setGradualEffectPub(&LedStruct);
}
void sceneFire(void) {
	LedStruct.pfnLedFuncCallBack = fireProcess;
	LedStruct.currentIndex = 0;
//	LedStruct.indicatorMaxIndex = 395;
	LedStruct.indicatorMaxIndex = 80;
	LedStruct.timeArray = fireTimeArray;
	pUserColor->hues = 0;
	setGradualEffectPub(&LedStruct);
}
/****************************************************************************
 *
 * 灯光先降后升效果
 *
 *****************************************************************************/
void FallingAndRising(u8* vtIndex, u8 lowThreshold, u8 hiThreshold, u16 fallingStep5ms, u16 risingStep5ms) {
	if (!fIsRisingNow) {
		if (NULL != fallingStep5ms) {
			vLEDStepCtrlTimeCnt = fallingStep5ms;
		}
		if ((!LedStruct.currentIndex)||(--LedStruct.currentIndex <= lowThreshold)) {
			fIsRisingNow = TRUE;
		}
	} else {
		if (NULL != risingStep5ms) {
			vLEDStepCtrlTimeCnt = risingStep5ms;
		}
		if (++LedStruct.currentIndex >= hiThreshold) {
			fIsRisingNow = FALSE;
			*vtIndex += 1;
		}
	}
}
/****************************************************************************
 *
 * 灯光闪烁效果
 *
 *****************************************************************************/

void stepByStep(u8* vtIndex, u8 lowThreshold, u8 hiThreshold, u8 lowTime5ms, u8 hiTimes5ms) {
	if (!fIsRisingNow) {
		LedStruct.currentIndex = lowThreshold;
		vLEDStepCtrlTimeCnt = lowTime5ms;
		fIsRisingNow = 1;
	} else {
		LedStruct.currentIndex = hiThreshold;
		vLEDStepCtrlTimeCnt = hiTimes5ms;
		fIsRisingNow = 0;
		*vtIndex+=1;
	}
}
/****************************************************************************
 *
 * 灯光保持效果
 *
 *****************************************************************************/
void keepStep(u8* vtIndex, u8 vtStep,u16 stepTime5ms){
	LedStruct.currentIndex = vtStep;
	vLEDStepCtrlTimeCnt = stepTime5ms;
	fIsRisingNow = FALSE;
	*vtIndex+=1;
}

/*********************************************************************************
 *
 * 电视灯光效果
 *
*********************************************************************************/
//void	 flashMode1StyleCProess(void){
//	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
//	if(!vCandleIndex){
//		if(fIsRisingNow){
//			if (++LedStruct.currentIndex >=(LedStruct.indicatorMaxIndex-2)){
//						fIsRisingNow=0;
//						vCandleIndex=1;
//			}
//		}else{
//			if(LedStruct.currentIndex ==0)
//				fIsRisingNow=1;
//			else
//				LedStruct.currentIndex--;
//		}
//	}
//	else if(vCandleIndex==1){
//			FallingAndRising(&vCandleIndex,(LedStruct.indicatorMaxIndex/2),(LedStruct.indicatorMaxIndex-10),4,1);
//	}
//	else if(vCandleIndex==2){
//			FallingAndRising(&vCandleIndex,10,(LedStruct.indicatorMaxIndex-10),5,2);
//	}
//	else if(vCandleIndex==3){
//			FallingAndRising(&vCandleIndex,0,50,2,1);
//	}
//	else if(vCandleIndex==4){
//			stepByStep(&vCandleIndex, 50, 8, 2, 10);
//	}
//	else if(vCandleIndex==5){
//		FallingAndRising(&vCandleIndex,55,(LedStruct.indicatorMaxIndex-5),3,5);
//	}
//	else if(vCandleIndex==6){
//		if(!fIsRisingNow){
//			if (--LedStruct.currentIndex <=1){
//					fIsRisingNow=1;
//					vCandleIndex=0;
//			}
//		}
//	}
//}
/*********************************************************************************
 *
 * 	烛光效果
 *
*********************************************************************************/
//void	 flashMode2StyleCProess(void){
//	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
//	if(!vCandleIndex){
//		if(fIsRisingNow){
//			if (++LedStruct.currentIndex >=(LedStruct.indicatorMaxIndex-10)){
//				fIsRisingNow=0;
//				vCandleIndex=1;
//			}
//		}else{
//			if(LedStruct.currentIndex ==0)
//				fIsRisingNow=1;
//			else
//				LedStruct.currentIndex--;
//		}
//	}else if(vCandleIndex==1){
//		FallingAndRising(&vCandleIndex,5,70,10,10);
//		//vCandleIndex++;
//	}else if(vCandleIndex==2){
//		FallingAndRising(&vCandleIndex,5,40,1,1);
//		//vCandleIndex++;
//	}else if(vCandleIndex==3){
//		keepStep(&vCandleIndex,38,200);
//		//vCandleIndex++;
//	}else if(vCandleIndex==4){
//		FallingAndRising(&vCandleIndex,40,70,5,5);
//		//vCandleIndex++;
//	}else if(vCandleIndex==5){
//		stepByStep(&vCandleIndex, 60,45, 100, 80);
//		//vCandleIndex++;
//	}else if(vCandleIndex==6){
//		FallingAndRising(&vCandleIndex,2,20,5,10);
//		//vCandleIndex++;
//	}else if(vCandleIndex==7){
//		keepStep(&vCandleIndex,0,2);
//		//vCandleIndex++;
//	}else if(vCandleIndex==8){
//		FallingAndRising(&vCandleIndex,2,15,1,9);
//		//vCandleIndex++;
//	}else if(vCandleIndex==9){
//		keepStep(&vCandleIndex,0,2);
//		//vCandleIndex++;
//	}else if(vCandleIndex==10){
//		FallingAndRising(&vCandleIndex,2,20,1,10);
//		//vCandleIndex++;
//	}else if(vCandleIndex==11){
//		FallingAndRising(&vCandleIndex,20,40,1,10);
//		//vCandleIndex++;
//	}else if(vCandleIndex==12){
//		FallingAndRising(&vCandleIndex,40,60,1,5);
//		//vCandleIndex++;
//	}else if(vCandleIndex==13){
//		keepStep(&vCandleIndex,50,600);
//		//vCandleIndex++;
//	}else if(vCandleIndex==14){
//		FallingAndRising(&vCandleIndex,20,21,10,1);
//		//vCandleIndex++;
//	}else if(vCandleIndex==15){
//		keepStep(&vCandleIndex,0,1);
//		//vCandleIndex++;
//	}else if(vCandleIndex==16){
//		FallingAndRising(&vCandleIndex,20,50,1,10);
//		//vCandleIndex++;
//	}else if(vCandleIndex==17){
//		FallingAndRising(&vCandleIndex,35,55,2,10);
//		//vCandleIndex++;
//	}else if(vCandleIndex==18){
//		FallingAndRising(&vCandleIndex,2,85,5,5);
//		//vCandleIndex++;
//	}else if(vCandleIndex==19){
//		FallingAndRising(&vCandleIndex,10,80,2,NULL);
//		//vCandleIndex++;
//	}else if(vCandleIndex==20){
//		stepByStep(&vCandleIndex, 60, 45, 5, 100);
//		//vCandleIndex++;
//	}else if(vCandleIndex==21){
//		FallingAndRising(&vCandleIndex, (LedStruct.indicatorMaxIndex/3), (LedStruct.indicatorMaxIndex-10),10,NULL);
//		//vCandleIndex++;
//	}else if(vCandleIndex==22){
//		stepByStep(&vCandleIndex, 75, 45, 5, 100);
//		//vCandleIndex++;
//	}else if(vCandleIndex==23){
//		FallingAndRising(&vCandleIndex,2,20,5,10);
//		//vCandleIndex++;
//	}else if(vCandleIndex==24){
//		keepStep(&vCandleIndex,0,1);
//		//vCandleIndex++;
//	}else if(vCandleIndex==25){
//		FallingAndRising(&vCandleIndex,2,25,1,10);
//		//vCandleIndex++;
//	}else if(vCandleIndex==26){
//		FallingAndRising(&vCandleIndex,10,20,10,15);
//	}else if(vCandleIndex==27){
//		keepStep(&vCandleIndex,0,0);
//	}else if(vCandleIndex==28){
//		keepStep(&vCandleIndex,20,40);
//	}else if(vCandleIndex==28){
//		keepStep(&vCandleIndex,0,0);
//	}else if(vCandleIndex==29){
//		keepStep(&vCandleIndex,20,40);
//	}else if(vCandleIndex==30){
//		vCandleIndex++;
//		//keepStep(&vCandleIndex,0,0);
//	}else if(vCandleIndex==31){
//		vCandleIndex++;
//		//keepStep(&vCandleIndex,20,40);
//	}else if(vCandleIndex==32){
//		FallingAndRising(&vCandleIndex,10,60,1,10);
//	}else if(vCandleIndex==33){
//		if(!fIsRisingNow){
//			if (--LedStruct.currentIndex <=1){
//					fIsRisingNow=1;
//					vCandleIndex=0;
//			}
//		}
//	}
//}


/******************************************************************************
 *
 * 	光效模式1-C  渐变效果实现
 *
 *******************************************************************************/
//void flashMode1StyleCfunc(void) {
//	flashMode1StyleCProess();
//	if(displayParams.brightness){
//		setCoolData(PWM_FRQ_CONST * (((float) displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)* ((float) (LedStruct.currentIndex + 10) / 100));				//从4%起步
//	}else
//		setCoolData(0);
//}
/******************************************************************************
 *
 * 	光效模式2-A  渐变效果实现
 *
 *******************************************************************************/
//void	flashMode2StyleAfunc(void){
//	if (++pUserColor->hues >= 360){
//		pUserColor->hues = 0;
//	}
//	pUserColor->saturation=1.0;
//	if(displayParams.brightness)
//		pUserColor->brightness=((float)displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100;
//	else
//		pUserColor->brightness=0;
//	hsb2Rgb(pUserColor);
//	updateColor(pUserColor);
//	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
//}
void	flashMode2StyleAfunc(void){
	if (++pUserColor->hues >= 360){
		pUserColor->hues = 0;
	}
	pUserColor->saturation=1.0;
	if(displayParams.brightness)
		pUserColor->brightness = ((float) displayParams.brightness) * (100 - RGBLED_REAL_START_DUTY) / (100 * 100) + (float) RGBLED_REAL_START_DUTY / 100;
	else
		pUserColor->brightness=0;
	hsb2Rgb(pUserColor);
	updateColor(pUserColor);
	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
}
void	 coolUpdateByCoe(float coe){
	if (displayParams.brightness)
			setCoolData(	PWM_FRQ_CONST * (((float) displayParams.brightness) * (100 - LED_REAL_START_DUTY) / (100 * 100) + (float) LED_REAL_START_DUTY / 100)* coe);				//从4%起步
		else
			setCoolData(0);
}
/***********************************************************************************************************
  *  @brief  		fire process
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void fireProcess(void) {
	uint8 step;
	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
	if (LedStruct.currentIndex++ >= LedStruct.indicatorMaxIndex) {
		LedStruct.currentIndex = 0;
	}
	if (displayParams.brightness)
		pUserColor->brightness = ((float) displayParams.brightness) * (100 - RGBLED_REAL_START_DUTY) / (100 * 100) + (float) RGBLED_REAL_START_DUTY / 100;
	else
		pUserColor->brightness = 0;
	if (LedStruct.currentIndex <= 21) {			//phase 1		last 525ms
		step = LedStruct.currentIndex;
		pUserColor->red = (0.615 + step * 0.01) * pUserColor->brightness * PWM_FRQ_CONST;
		pUserColor->green = (0.04 + step * 0.0036) * pUserColor->brightness * PWM_FRQ_CONST;
//		pUserColor->blue = (0.007 + step * 0.0013) * pUserColor->brightness * PWM_FRQ_CONST;
		updateRgbOnly(pUserColor);
////		setWarmData((LED_REAL_START_DUTY*0.01+0.025-step*0.0005)* pUserColor->brightness * PWM_FRQ_CONST);		//
////		setWarmData((LED_REAL_START_DUTY*0.005+0.025-step*0.0005)* pUserColor->brightness * PWM_FRQ_CONST);		//
		setWarmData((LED_REAL_START_DUTY*0.005+0.025-step*0.0015)* pUserColor->brightness * PWM_FRQ_CONST);		//

	} else if (LedStruct.currentIndex <= 26) {		//phase 2		last 130ms
		pUserColor->red = 0.2 * pUserColor->brightness * PWM_FRQ_CONST;
		pUserColor->green = 0.075 * pUserColor->brightness * PWM_FRQ_CONST;
		pUserColor->blue = 0.063 * pUserColor->brightness * PWM_FRQ_CONST;
		updateColor(pUserColor);
	} else if (LedStruct.currentIndex <= 37) {		//phase 3		last 260ms
		step = LedStruct.currentIndex - 26;
		pUserColor->red = (0.39 + step * 0.005) * pUserColor->brightness * PWM_FRQ_CONST;
		if (0.11 > step * 0.005)
			pUserColor->green = (0.11 - step * 0.005) * pUserColor->brightness * PWM_FRQ_CONST;
		else
			pUserColor->green = 0;
		if (0.055 > step * 0.005)
			pUserColor->blue = (0.055 - step * 0.005) * pUserColor->brightness * PWM_FRQ_CONST;
		else
			pUserColor->blue = 0;
		updateRgbOnly(pUserColor);
////		setWarmData((LED_REAL_START_DUTY*0.01+0.001+step*0.0004)* pUserColor->brightness * PWM_FRQ_CONST);		//
////		setWarmData((LED_REAL_START_DUTY*0.005+0.001+step*0.0004)* pUserColor->brightness * PWM_FRQ_CONST);		//
		setWarmData((step*0.004)* pUserColor->brightness * PWM_FRQ_CONST);		//
	} else if (LedStruct.currentIndex <= 53) {		//phase 4		last 400ms
		step = LedStruct.currentIndex - 37;
		pUserColor->red = (0.57 + step * 0.005) * pUserColor->brightness * PWM_FRQ_CONST;
		if (0.05 > step * 0.0033)
			pUserColor->green = (0.05 - step * 0.0033) * pUserColor->brightness * PWM_FRQ_CONST;
		else
			pUserColor->green = 0;
		pUserColor->blue = 0;
		updateRgbOnly(pUserColor);
////		setWarmData((LED_REAL_START_DUTY*0.01+0.022+step*0.00032)* pUserColor->brightness * PWM_FRQ_CONST);		//
		setWarmData((LED_REAL_START_DUTY*0.005+0.022+step*0.00032)* pUserColor->brightness * PWM_FRQ_CONST);		//
	} else if (LedStruct.currentIndex <= 58) {		//phase 5		last 130ms
		if (LedStruct.currentIndex <= 55) {
			pUserColor->red = 0.615 * pUserColor->brightness * PWM_FRQ_CONST;
			pUserColor->green = 0;
			pUserColor->blue = 0;
			updateRgbOnly(pUserColor);
////			setWarmData((LED_REAL_START_DUTY*0.01+0.022)* pUserColor->brightness * PWM_FRQ_CONST);		//
			setWarmData((LED_REAL_START_DUTY*0.005+0.022)* pUserColor->brightness * PWM_FRQ_CONST);		//
		}
	} else if (LedStruct.currentIndex <= 69) {		//phase 6		last 260ms
		step = LedStruct.currentIndex - 58;
		pUserColor->red = (0.58 + step * 0.003) * pUserColor->brightness * PWM_FRQ_CONST;
		pUserColor->green = (0.01 + step * 0.003) * pUserColor->brightness * PWM_FRQ_CONST;
		pUserColor->blue = 0;
		updateRgbOnly(pUserColor);
////		setWarmData((LED_REAL_START_DUTY*0.01+0.022-step*0.00032)* pUserColor->brightness * PWM_FRQ_CONST);		//
		setWarmData((LED_REAL_START_DUTY*0.005+0.022-step*0.00032)* pUserColor->brightness * PWM_FRQ_CONST);		//
	} else if (LedStruct.currentIndex <= 80) {		//phase 7		last 270ms
		if (LedStruct.currentIndex <= 70) {
			pUserColor->red = 0.52 * pUserColor->brightness * PWM_FRQ_CONST;
			pUserColor->green = 0;
			pUserColor->blue = 0;
			updateColor(pUserColor);
		}
////		setWarmData((LED_REAL_START_DUTY*0.01+0.022-step*0.0007)* pUserColor->brightness * PWM_FRQ_CONST);		//
		setWarmData((LED_REAL_START_DUTY*0.005+0.022-step*0.0007)* pUserColor->brightness * PWM_FRQ_CONST);		//
	}
}

/***********************************************************************************************************
  *  @brief  		Lightning1 process
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void lightning1Process(void) {
	vLEDStepCtrlTimeCnt = *(LedStruct.timeArray);
	if (LedStruct.currentIndex++ >= LedStruct.indicatorMaxIndex) {
		LedStruct.currentIndex = 0;
	}
	if (LedStruct.currentIndex <= 160) {			//phase 1		last 800ms
		coolUpdateByCoe((float) (100 - 0.625 * LedStruct.currentIndex) / 100);
	} else if (LedStruct.currentIndex <= 291) {		//phase2  		last 655ms
		setCoolData(0);
	} else if (LedStruct.currentIndex <= 371) {		//phase2  		last 390
		coolUpdateByCoe(1.0);
	} else if (LedStruct.currentIndex <= 713) {		//phase3  		last 1710ms
		setCoolData(0);
	}
}
/******************************************************************************
 *
 * 	光效模式2-B  渐变效果实现
 *
 *******************************************************************************/
//void	flashMode2StyleBfunc(void){
//	flashMode2StyleAfunc();				//效果同光效模式2-A  只是每个step间隔不同
//}
/******************************************************************************
 *
 * 	光效模式2-C  渐变效果实现
 *
 *******************************************************************************/
//void	flashMode2StyleCfunc(void){
//	flashMode2StyleCProess();
//	if(displayParams.brightness){
//		setWarmData(PWM_FRQ_CONST * (((float) displayParams.brightness)*(100-LED_REAL_START_DUTY)/(100*100)+(float)LED_REAL_START_DUTY/100)* ((float) (LedStruct.currentIndex + 10) / 100));				//从4%起步
//	}else
//		setWarmData(0);
//}

/******************************************************************************
 *
 * 	自定渐变灯效果
 *
 *******************************************************************************/
void customizeGrdualCbfunc(void) {
	float vtStep = 0, vtMutiRate = 0;
	vLEDStepCtrlTimeCnt = 1;//step=5ms
	vtStep = CUSTOMIZE_GRADUAL_TOTAL_RATE * 2 / LedStruct.indicatorMaxIndex;	//在半个周期内进行80%的亮度变化.
	if (LedStruct.currentIndex <= LedStruct.indicatorMaxIndex / 2) {
		vtMutiRate = (1-CUSTOMIZE_GRADUAL_TOTAL_RATE)+LedStruct.currentIndex * vtStep;
		if(vtMutiRate>1)
			vtMutiRate=1;
	} else {
		vtMutiRate = (1 - vtStep*(LedStruct.currentIndex-LedStruct.indicatorMaxIndex / 2));
	}
	if ( displayParams.fIsFromRGBMode) {
		setRedData(oriCustomizePwmDataStruct.valueOfRed * vtMutiRate);//
		setGreenData( oriCustomizePwmDataStruct.valueOfGreen * vtMutiRate);
		setBlueData(oriCustomizePwmDataStruct.valueOfBlue * vtMutiRate);
	} else {
		setCoolData(oriCustomizePwmDataStruct.valueOfCw * vtMutiRate);//
		setWarmData(oriCustomizePwmDataStruct.valueOfMw * vtMutiRate);
	}
}
/******************************************************************************
 *
 * 	自定义呼吸灯效果结束
 *
 *******************************************************************************/
void customizeGrdualEndCbfunc(void){
	if ( displayParams.fIsFromRGBMode) {
		setRedData(0);//
		setGreenData( 0);
		setBlueData(0);
	} else {
		setCoolData(0);//
		setWarmData(0);
	}
//	LOG("end \n");
	stopLightEffectEvent();
	fIsLightEffectOn = FALSE;
	LedStruct.customizeLoopIndex=0;
	LedStruct.currentIndex=0;
	displayParams.fIsEffectMode=FALSE;
	displayParams.mode=0;
	//fIsLightActive=0;
//	LEDPowerOff ;
	displayParams.vModeIndex=displayParams.backupArrowIndex;
//	if (HuesSetting == displayParams.arrowIndex)
//		displayParams.command= CCS_LIGHT_MODE_HSI;
////	else if (SaturationSetting == displayParams.arrowIndex)
////		displayParams.command = CCS_LIGHT_MODE_HSI;
//	else if (ColorTempSetting == displayParams.arrowIndex)
//		displayParams.command = CCS_LIGHT_MODE_CCT;
//	else if (PreinstallEffect == displayParams.arrowIndex)
//		displayParams.command = CCS_LIGHT_MODE_FIXEDMODE;
	light_off();
}

void checkFlashLoopOver(void) {
	if (LedStruct.customizeLoopTimes < INFINITE_LOOP_TIMES) {
		if (++LedStruct.customizeLoopIndex >= LedStruct.customizeLoopTimes) {
		//if (++LedStruct.customizeLoopIndex >=3) {
			LedStruct.pfncustomizeEffectOverCallBack();
			return;					//预设循环次数完成
		}
	}
}
/******************************************************************************
 *
 * 	自定义闪烁效果
 *
 *******************************************************************************/
void	customizeFlashCbfunc(void){
	vLEDStepCtrlTimeCnt = 1000/(displayParams.customizeEffectFreq*2*5);		//step=5ms
	if(LedStruct.currentIndex%2==0){
		if(FALSE==displayParams.fIsFromRGBMode){
			updateColorTemp(&displayParams);
		}else{
			updateRGBLamp(&displayParams);
		}
	}else{
		if(FlashMode==displayParams.customizeEffectMode){
			checkFlashLoopOver();
		}
		if(FALSE==displayParams.fIsFromRGBMode){
			turnOffColorTempLamp();
		}else{
			turnOffRGBLamp();
		}
	}
}
///******************************************************************************
// *
// * 	自定义闪烁效果
// *
// *******************************************************************************/
//void	customizeOneShotCbfunc(void){
//	if(LedStruct.currentIndex%2==0){
//		if(FALSE==displayParams.fIsFromRGBMode){
//			updateColorTemp(&displayParams);
//		}else{
//			updateRGBLamp(&displayParams);
//		}
//	}else{
//		if((FlashMode==displayParams.customizeEffectMode)||(OneShotMode==displayParams.customizeEffectMode)){
//			checkFlashLoopOver();
//		}
//		if(FALSE==displayParams.fIsFromRGBMode){
//			turnOffColorTempLamp();
//		}else{
//			turnOffRGBLamp();
//		}
//	}
//}
/******************************************************************************
 *
 * 	自定义闪烁效果结束
 *
 *******************************************************************************/
void customizeFlashEndCbfunc(void){
//	setCoolData(0);
//	setWarmData(0);
//	fIsLightEffectOn = FALSE;
//	LedStruct.customizeLoopIndex=0;
//	LedStruct.currentIndex=0;
//	displayParams.fIsEffectMode=FALSE;
	customizeGrdualEndCbfunc();
}


void	set2FlashMode2StyleA(void){
	LedStruct.indicatorStyle=4;					//flashMode2  styleA
	LedStruct.pfnLedFuncCallBack=flashMode2StyleAfunc;
	LedStruct.currentIndex=0;
	LedStruct.indicatorMaxIndex=360;
	LedStruct.timeArray=GradualEffect2StyleATimeArray;
	pUserColor->hues=0;
	setGradualEffectPub(&LedStruct);
}
//void	set2FlashMode2StyleB(void){
//	LedStruct.indicatorStyle=5;					//flashMode2  styleB
//	LedStruct.pfnLedFuncCallBack=flashMode2StyleBfunc;
//	LedStruct.currentIndex=0;
//	LedStruct.indicatorMaxIndex=360;
//	LedStruct.timeArray=GradualEffect2StyleBTimeArray;
//	pUserColor->hues=0;
//	setGradualEffectPub(&LedStruct);
//}
//void	set2FlashMode2StyleC(void){
//	LedStruct.indicatorStyle=6;					//flashMode2  styleC
//	LedStruct.pfnLedFuncCallBack=flashMode2StyleCfunc;
//	LedStruct.currentIndex=0;
//	LedStruct.indicatorMaxIndex=90;
//	LedStruct.timeArray=GradualEffect2StyleCTimeArray;
//	setGradualEffectPub(&LedStruct);
//}

void	 set2SimulatonModeStyleA(void){
	LedStruct.indicatorMaxIndex=MAX_SIMULATION_MODE_STYLE_A_INDEX;
	LedStruct.indicatorStyle=7;					//SimulatonMode  styleA
	LedStruct.sttsArray=simulationModeStyleASttsArray;
	LedStruct.timeArray=simulationModeStyleATimerArray;
	setFlashEffectPub(&LedStruct);
}
//void	 set2SimulatonModeStyleB(void){
//	LedStruct.indicatorMaxIndex=MAX_SIMULATION_MODE_STYLE_B_INDEX;
//	LedStruct.indicatorStyle=8;					//SimulatonMode  styleC
//	LedStruct.sttsArray=simulationModeStyleBSttsArray;
//	LedStruct.timeArray=simulationModeStyleBTimerArray;
//	setFlashEffectPub(&LedStruct);
//}
//void	 set2SimulatonModeStyleC(void){
//	LedStruct.indicatorMaxIndex=MAX_SIMULATION_MODE_STYLE_C_INDEX;
//	LedStruct.indicatorStyle=9;					//SimulatonMode  styleC
//	LedStruct.sttsArray=simulationModeStyleCSttsArray;
//	LedStruct.timeArray=simulationModeStyleCTimerArray;
//	setFlashEffectPub(&LedStruct);
//}




void	 loadTargetPwmData(displayParamsStruct * disParams,PcaDataStruct * pcaData){
	if(FALSE==displayParams.fIsFromRGBMode){
		loadTargetColorTemperatureValue(disParams,pcaData);
	}else{
		loadTargetRGBValue(disParams,pcaData);
	}
}
/********************************************************
 *
 *	自定义渐变效果
 *
 ********************************************************/
void	 customizeGradualEffect(void){
	LedStruct.currentIndex=0;
//	customizeFlashCbfunc();					//仅仅用于获取PWM的值
	loadTargetPwmData(&displayParams,&pcaDataStruct);
	LedStruct.indicatorStyle=67;					//flashMode2  styleC
	LedStruct.pfnLedFuncCallBack=customizeGrdualCbfunc;
	LedStruct.pfncustomizeEffectOverCallBack=customizeGrdualEndCbfunc;
	LedStruct.customizeLoopIndex=0;
	LedStruct.indicatorMaxIndex= displayParams.customizeEffectFreq*1000/(2*5)-1;		//step=5ms2*displayParams.customizeEffectFreq-1;
//	LedStruct.indicatorMaxIndex= displayParams.customizeEffectFreq*100/(2*5)-1;		//step=5ms2*displayParams.customizeEffectFreq-1;
	LedStruct.customizeLoopTimes=displayParams.customizeEffectTimes;
	LedStruct.timeArray=customizeGradualTimeArray;
	fIsLightEffectOn = 1;
	displayParams.fIsEffectMode=TRUE;
	LedStruct.fIsGradualMode=1;
	//DDL_ZERO_STRUCT(oriCustomizePwmDataStruct);
	memset(&oriCustomizePwmDataStruct,0,sizeof(pcaDataStruct));
	oriCustomizePwmDataStruct.valueOfRed=pcaDataStruct.valueOfRed;
	oriCustomizePwmDataStruct.valueOfGreen=pcaDataStruct.valueOfGreen;
	oriCustomizePwmDataStruct.valueOfBlue=pcaDataStruct.valueOfBlue;
	oriCustomizePwmDataStruct.valueOfCw=pcaDataStruct.valueOfCw;
	oriCustomizePwmDataStruct.valueOfMw=pcaDataStruct.valueOfMw;
//	LOG("\nlightEffect.c   R:%d  C:%d  M:%d\n",pcaDataStruct.valueOfRed,pcaDataStruct.valueOfCw,pcaDataStruct.valueOfMw);
//	LOG("Start\n");
	customizeGrdualCbfunc();
}
/********************************************************
 *
 *	自定义闪烁效果
 *
 ********************************************************/
void	customizeFlashEffect(void){
	LedStruct.indicatorStyle=66;
	LedStruct.pfnLedFuncCallBack=customizeFlashCbfunc;
	LedStruct.pfncustomizeEffectOverCallBack=customizeFlashEndCbfunc;
	LedStruct.currentIndex=0;
	LedStruct.indicatorMaxIndex=2*displayParams.customizeEffectFreq-1;
	LedStruct.customizeLoopTimes=displayParams.customizeEffectTimes;
	LedStruct.customizeLoopIndex=0;
	LedStruct.timeArray=customizeFlashTimeArray;
	fIsLightEffectOn = 1;
	displayParams.fIsEffectMode=TRUE;
	LedStruct.fIsGradualMode=0;
	customizeFlashCbfunc();
}
//void	customizeOneShotEffect(void){
//	if((displayParams.customizeOneShot>MAX_ONE_SHOT_FREQ_INDEX)||(displayParams.customizeOneShot<ONE_SHOT_FREQ_START_OFFSET)){
//		customizeFlashEndCbfunc();
//		return;
//	}
//	LedStruct.indicatorStyle=69;
//	LedStruct.pfnLedFuncCallBack=customizeOneShotCbfunc;
//	LedStruct.pfncustomizeEffectOverCallBack=customizeFlashEndCbfunc;
//	LedStruct.currentIndex=0;
//	LedStruct.indicatorMaxIndex=1;
//	LedStruct.customizeLoopTimes=1;
//	LedStruct.customizeLoopIndex=0;
//	LedStruct.timeArray=customizeFlashTimeArray;
//	fIsLightEffectOn = 1;
//	displayParams.fIsEffectMode=TRUE;
//	LedStruct.fIsGradualMode=0;
//	vLEDStepCtrlTimeCnt = customizeOneShotFreqArray[displayParams.customizeOneShot-ONE_SHOT_FREQ_START_OFFSET];		//step=5ms
//	customizeOneShotCbfunc();
//}
void startLightEffect(displayParamsStruct * disParams) {
	turnOffAllLightEffect();
	if (PreinstallEffect == disParams->arrowIndex) {
		if (disParams->preinstallEffectNo == 1) {
			set2SimulatonModeStyleA();
//			set2FlashMode1StyleA();
		} else if (disParams->preinstallEffectNo == 2) {
//			set2FlashMode1StyleB();
			sceneFire();
		} else if (disParams->preinstallEffectNo == 3) {
//			set2FlashMode1StyleC();
			sceneLightning1();
		} else if (disParams->preinstallEffectNo == 4)
			set2FlashMode2StyleA();
//		else if(disParams->preinstallEffectNo==5)
//			set2FlashMode2StyleB();
//		else if(disParams->preinstallEffectNo==6)
//			set2FlashMode2StyleC();
//		else if(disParams->preinstallEffectNo==7)
//			set2SimulatonModeStyleA();
//		else if(disParams->preinstallEffectNo==8)
//			set2SimulatonModeStyleB();
//		else if(disParams->preinstallEffectNo==9)
//			set2SimulatonModeStyleC();
	} else if (CustomizeEffect == disParams->arrowIndex) {
		if (GradualMode == displayParams.customizeEffectMode) {
			customizeGradualEffect();
		} else if (FlashMode == displayParams.customizeEffectMode) {
			customizeFlashEffect();
		}
//		else{
//			customizeOneShotEffect();
//		}
	}
	startLightEffectEvent();
}
