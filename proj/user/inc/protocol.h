/**************************************************************************************************
Filename:       protocol.h
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

#ifndef PROTOCOL_H
#define PROTOCOL_H

/*********************************************************************
* INCLUDES
*/
#include "types.h"
#include "adc.h"

/*********************************************************************
* MACROS
*/


// Define moduel for Power manager
#define 	PMM_MOD_RFPHY								MOD_USR0	//cannot change
#define 	PMM_MOD_UART								MOD_USR1

#define 	BATT_COULOMB_MAX							1150		//mAh
#define 	BATT_VOLTAGE_MIX								(BATT_VOLT_MIN*1000)
#define 	ADC_CHANNEL_BATT								ADC_CH2N_P13
#define 	ADC_CHANNEL_TEMP								ADC_CH3P_P20

#define 	BATT_VOLT_MIN									3.30
#define	BATT_LV1_THESHOLD								3440	//3.5
#define	BATT_LV2_THESHOLD								3600	//3.6
#define	BATT_LV3_THESHOLD								3720	//3.7
#define	BATT_LV4_THESHOLD								3900	//4.05
#define 	BATT_VOLTAGE_MAX								4150	//mv
//#define	CHARGE_FINAL_VOLT							414		//恒压模式电压阈值

#define CURRENT_LED_MAX								860		//mA
#define CURRENT_LED_SAVE								26		//mA
#define CURRENT_SYSTEMS_STANDBY						15		//mA

// Define for Analog, P11,12,13,14,15,20
#define ADC_BATT_DET									P13
#define ADC_TEMP_DET									P20
#define ADC_TEMP_POWER								P18
// Define for GPIO
#define GPIO_UART_WAKEUP								P10
#define GPIO_UART_TX									P9
#define GPIO_UART_RX									GPIO_UART_WAKEUP

#define GPIO_DISPLAY_RST							  	P25//P32
#define GPIO_DISPLAY_DC							    	P22//P33
#define GPIO_DISPLAY_CS								  	P26//P31
#define GPIO_DISPLAY_CLK							  	P23//P34
#define GPIO_DISPLAY_MISO							  	GPIO_DUMMY
#define GPIO_DISPLAY_MOSI							  	P24//P0

#define GPIO_KEY_POWER								  	P6
#define GPIO_KEY_FUNC_UP							  	P3//PGPIO_DUMMY//P14
#define GPIO_KEY_FUNC_DOWN						  	P4
#define	GPIO_KEY_SWITCH								P5

#define SW_RESET_MCU									P19
#define GPIO_CHARGE_DET							    	P11
#define GPIO_CHARGE_FULL							  	P12

#define 	GPIO_LIGHT_POWER								P1
#define	GPIO_LCD_BACKLIGHT						  	P21
#define	GPIO_VLCD										P27
#define 	GPIO_LIGHT_CW								  	P0
#define 	GPIO_LIGHT_MW								  	P34
#define 	GPIO_LIGHT_RED									P33
#define 	GPIO_LIGHT_GREEN								P32
#define 	GPIO_LIGHT_BLUE							  	P31

// Define for GPIO Hi-Low
#define 	GPIO_HL_LIGHT_POWER_ON						1
#define 	GPIO_HL_LIGHT_POWER_OFF						0

#define 	GPIO_HL_LIGHT_EN_ON						  	1
#define 	GPIO_HL_LIGHT_EN_OFF							0

#define 	LCD_BACKLIGHT_HIGH              						1
#define 	LCD_BACKLIGHT_LOW               						0
#define	HI_STTS											1
#define	LOW_STTS										0
//=========================================================
#define	Max_Batt_level								  	4
#define	Min_Arrow_Index								0
#define	Max_Arrow_Index							    	4
#define  USER_DOG_CONST							    	1600				//1600*5ms=8s

#define  MAX_CD_TIMER								    	100

#define	MAX_Brightness								  	100
#define MAX_Hues									      	360
#define Min_Hues									      	1
#define MAX_Saturation								  	100
#define MIN_Saturation								  	1

#define	MAX_ColorTemp								    	56//75//56
#define	MIN_ColorTemp								    	32//32
	 
//#define	MAX_LightEffect								  8
#define	MAX_SCENE_INDEX								4
#define	MAX_STROBE_INDEX								10
#define	ON_LED_DELAY_CONST						  	20
	 
#define	DEFAULT_HUES								    	MAX_Hues
#define	DEFAULT_BRIGHTNESS						  	5
#define	DEFAULT_SATURATION						  	100
#define	DEFAULT_ARROR_INDEX						  	0//3
#define	DEFAULT_MODE_INDEX						  	1//3
#define	DEFAULT_COLOR_TEMP						  	43
#define	DEFAULT_STYLE1_VALUE							0
#define	DEFAULT_STYLE2_VALUE							0
#define	DEFAULT_STYLE3_VALUE							0
#define	DEFAULT_ADC_GAP							    	0.1

#define 	OVER_TEMPERATURE_VOLT_Lv1		  			66
#define 	OVER_TEMPERATURE_VOLT_Lv0		  			60
#define 	NORMAL_TEMPERATURE_VOLT		    			52
#define	TEMPERATURE_COE_NORMAL			    			1.0
#define	TEMPERATURE_COE_LOW				      			0.8


#define	TEMPEARTURE_DET_PERIOD_2S			2013
#define	TEMPERATURE_DET_PERIOD_HOT_1S		1011
#define	BATT_DET_PERIOD_1S						994


/*********************************************************************
* CONSTANTS
*/

/*********************************************************************
* TYPEDEFS
*/

typedef struct {
	uint8 event;
}PROTOCOL_Evt_t;

typedef struct{
  uint8 task_id;
  uint16 events;
  uint16 restartEvents;
}protocol_CBs_t;

typedef struct{
	uint8	DisplayModeIndex;			//	参考值: displayModeEnum
	uint8	arrowIndex;					//	光标序号	(0~6)		参考值:settingIndexEnum
	uint8	battLv;						//	电量标志	(0~6)
	uint8	command;
	uint8	mode;
	uint16	hues;						//	色调值	(0~360)
	uint8	saturation;					//	饱和值	(0~100)
	uint8	brightness;					//	亮度值	(0~100)%
	uint8	colorTemperature;				//	33~65(00K)
	uint8	style1Value;					//	0~2对应闪光灯标志A,B,C 三种模式
	uint8	style2Value;					//	0~2对应循环标志A,B,C 三种模式
	uint8	style3Value;					//	0~2对应锁标志A,B,C 三种模式
	uint8	countDownTimer;				//	倒数模式计时
	uint8	effectmode;					//	自定义特效模式  1/2:呼吸灯/爆闪
	uint8	times;
	uint16	freq;
	uint8	trigger_brightness;
	uint8	trigger_ontime;
	uint16	remainingTime;
	UINT8	preinstallEffectNo;			//	预设特效编号
	UINT8	customizeEffectMode;		//	自定义特效模式  1/2:呼吸灯/爆闪
	UINT8	customizeEffectTimes;		//	自定义特效循环次数 1~99:次数   100:无限循环
	UINT8	customizeEffectFreq;		//	自定义特效频率
	UINT8	customizeOneShot;		//	自定义特效单次闪烁
	UINT8	fIsEffectMode;			//	自定义特效模式+预设特效模式=1.普通模式=0
	UINT8	fIsFromRGBMode;			//	0/1 :从RGB/色温模式进入自定义特效模式
	UINT8	backupArrowIndex;
	float	adcGap;					//	ADC  测量误差
	float 	rRate;
	float	gRate;
	float	bRate;
	uint8      vModeIndex;				//
}displayParamsStruct;
#pragma pack(1)

typedef struct{
	uint8 checkCode;
	uint8 modelNoLen;
	uint8 modelNum[22];
	uint8 serialNoLen;
	uint8 serialNum[14];
	uint8 deviceNameLen;
	uint8 deviceName[22];
	uint8 hwVersionLen;
	uint8 hwVersion[10];
	uint8 swVersionLen;
	uint8 swVersion[10];
	uint8 firmwareVersionLen;
	uint8 firmwareVersion[15];
	uint8 userManualLen;
	uint8 userManual[20];
	uint8 OtaFileInfolLen;
	uint8 OtaFileInfo[20];
//	uint8 localNameLen;
//	uint8 localName[22];
}deviceInfoStruct;

#pragma pack()
typedef enum{
	IdleIamgeDisplay=0,
	ModeTDisplay=1,
	CountDownDisplay=2,
	ChargingAtPowerDown=3
}displayModeEnum;

#define	INFINITE_LOOP_TIMES			100
#define	CUSTOMIZE_GRADUAL_TOTAL_RATE	0.95			//自定义渐变模式最大亮度变化范围

typedef enum{
	GradualMode=1,
	FlashMode=2,
	OneShotMode=3
}customizeEffectModeType;

//typedef enum{
//	HuesSetting = 0,
//	SaturationSetting = 1,
//	ColorTempSetting = 2,
//	Style1Setting = 3,
//	PreinstallEffect=Style1Setting,
//	Style2Setting = 4,
//	Style3Setting = 5,
//	CustomizeEffect=Max_Arrow_Index+1,
//}settingIndexEnum;
typedef enum{
	BrightnessSetting=0,
	ColorTempSetting,
	HuesSetting,
	PreinstallEffect,
	CustomizeEffect,
//	HuesSetting = 0,
//	SaturationSetting = 1,
//	ColorTempSetting = 2,
//	Style1Setting = 3,
//	PreinstallEffect=Style1Setting,
//	Style2Setting = 4,
//	Style3Setting = 5,
//	CustomizeEffect=Max_Arrow_Index+1,
}settingIndexEnum;

typedef void (*PROTOCOL_CB_t)(PROTOCOL_Evt_t* pev);

/*********************************************************************
* VARIABLES
*/

/*********************************************************************
* UART
*/
#define UART_TX_BUF_SIZE								512
#define UART_RX_BUF_SIZE								512
enum {
	PROTOCOL_RX_ST_IDLE = 0,
	PROTOCOL_RX_ST_DELAY_SLOT,
	PROTOCOL_RX_ST_SENDING
};

enum {
	PROTOCOL_TX_ST_IDLE = 0,
	PROTOCOL_TX_ST_DELAY_SLOT,
	PROTOCOL_TX_ST_SENDING
};

typedef struct {
	//tx
	uint8 tx_state;
	uint16 tx_size;
	uint8 tx_buf[UART_TX_BUF_SIZE];
	
	//rx
	uint8 rx_state;
	uint16 rx_size;
	uint16 rx_offset;
	uint8 rx_buf[UART_RX_BUF_SIZE];

    
}PROTOCOL_uart_t;


typedef enum{
	IDLE_STATUS=0,
	WAIT_FIRST_VOLT,
	FULLY_COUNTDOWN,
}fullyCheckDef;

#define 	VOLT_CHECK_WAIT_TIMES_3	3
#define	FULLY_TIME_OUT_60S			60
#define	FULLY_TIME_OUT_30M		1800
#define	FULLY_TIME_OUT_40M		2400
typedef struct {

	fullyCheckDef fullyCheckIndex;
	uint16 counter;

} chgSttsDef;

#define	BATT_ARRAY_SIZE					10
#define	PARAMS_DATA_RESET				0
#define	GENERATE_BATT_VOLT_TIMES			5

typedef struct{
	uint8 vDisplayBattLv;
	uint8 fBattFullyWaiting;
	uint8 vLightEfftectVoltDectCnt;
	float battArray[BATT_ARRAY_SIZE];
	uint8 battRecordIndex;
	uint8 vCurrentBattLv;
	uint8 vBattMinTimes;
	uint8 fIsPowerOnFirstVoltGot;				//	0/1      下次获取的电压不经过/经过平均使用
	uint8 fIsFirst5TimesVoltGot;					//     0/1      尚未/已记录到5次电压数据
	uint8 vRecordCnt;							//
	uint8 vBattPercent;							//
}battInfoDef;

typedef enum{
	NonError=0,
	TemperatureError,
	VoltageError,
	ClearAllInfo,
}FactoryInfoDef;





//define all command list of UART data only in hear
#define HAL_UART_BR_9600							0x00
#define HAL_UART_BR_4800							0x01
#define HAL_UART_BR_19200							0x03
#define HAL_UART_BR_38400							0x04
#define HAL_UART_BR_57600							0x05
#define HAL_UART_BR_115200							0x06
#define HAL_UART_BR_230400							0x07
#define HAL_UART_BR_2400							0x08

#define HAL_UART_TBR_9600							9600
#define HAL_UART_TBR_4800							4800
#define HAL_UART_TBR_19200							19200
#define HAL_UART_TBR_38400							38400
#define HAL_UART_TBR_57600							57600
#define HAL_UART_TBR_115200							115200
#define HAL_UART_TBR_230400							230400
#define HAL_UART_TBR_2400							2400

// SerialApp SOP
#define UART_SOP									0x02
#define UART_CMD_SOP								0x02

// SerialApp Command HI of uint16
//------------------------------------------
#define CMD_HI_10									0x10
#define CMD_HI_20									0x20
#define CMD_HI_30									0x30
#define CMD_HI_40									0x40
#define CMD_HI_50									0x50
#define CMD_HI_60									0x60
#define CMD_HI_70									0x70
#define CMD_HI_80									0x80
#define CMD_HI_90									0x90
#define CMD_HI_a0									0xa0
//------------------------------------------


// SerialApp Command LO of uint16
//------------------------------------------
#define CMD_LO_ACK									0x01
#define CMD_LO_NACK									0x02
//------------------------------------------

// Serial command for login
#define CMD_LO_LOGIN_PASSWORD_WRITE					0x05
#define CMD_LO_LOGIN_PASSWORD_SEND					0x06
#define CMD_LO_READ_ID								0x07
#define CMD_LO_LOGIN								0x08
#define CMD_LO_LOGIN_PASSWORD_RESET					0x09
#define CMD_LO_PAIRINGMODE_INTO						0x0c
#define CMD_LO_PAIRINGMODE_EXIT						0x0d
#define CMD_LO_DISCONNECT							0x0f

// SerialApp Command only for 0xa0
#define CMD_LO_MOD_DEVICE_NAME						0xa1
#define CMD_LO_SET_DATATIME							0xa2
#define CMD_LO_MOD_MEMORYDATA						0xa9
#define CMD_LO_MOD_BAUDRATE_INVTIME					0xaa
#define CMD_LO_MOD_MODEL_NAME						0xab
#define CMD_LO_MOD_SERIAL_NAME						0xac
#define CMD_LO_MOD_LOCAL_NAME						0xad
#define CMD_LO_MOD_FIREWALL							0xae
#define CMD_LO_MOD_DEFAULT							0xaf

#define CMD_LO_READ_DeviceName						0xb1
#define CMD_LO_READ_DATATIME						0xb2
#define CMD_LO_READ_BATT							0xb3
#define CMD_LO_READ_SystemID						0xb4
#define CMD_LO_READ_FirmwareRev						0xb5
#define CMD_LO_READ_HardwareRev						0xb6
#define CMD_LO_READ_SoftwareRev						0xb7
#define CMD_LO_READ_ManufacturerName				0xb8
#define CMD_LO_READ_MemoryData						0xb9
#define CMD_LO_READ_TimeUART						0xba
#define CMD_LO_READ_ModelNumber						0xbb
#define CMD_LO_READ_SerialNumber					0xbc
#define CMD_LO_READ_LocalName						0xbd
#define CMD_LO_READ_MacAddress						0xbe
#define CMD_LO_READ_AllInformation					0xbf

// SerialApp Command for test
#define CMD_LO_TESTMODE_INTO						0xc0	//成品生产测试模式
	#define OPTION_PD_TEST_STANDBY						0x00	//into test mode and hold on standby
	#define OPTION_PD_TEST_CURRENT_LED_R				0x01	//current of M_led
	#define OPTION_PD_TEST_CURRENT_LED_G				0x02	//current of C_led
	#define OPTION_PD_TEST_CURRENT_LED_B				0x03	//current of R_led
	#define OPTION_PD_TEST_CURRENT_LED_C				0x04	//current of G_led
	#define OPTION_PD_TEST_CURRENT_LED_M				0x05	//current of B_led
	#define OPTION_PD_TEST_CURRENT_STANDBY			0x06	//current of standby mode
	#define OPTION_PD_TEST_CURRENT_SLEEP				0x07	//current of sleep mode
	#define OPTION_PD_TEST_BATT						0x08	//Batt test
	#define OPTION_PD_TEST_TEMP						0x09	//temp test
	#define OPTION_PD_TEST_Key							0x0A	//key Test
	#define OPTION_PD_TEST_MAC						0x0B	//Get Mac Info
#define CMD_LO_PD_TEST								0xc1
	#define OPTION_PD_TEST_MDIO							0x01 	//module<->MCU IO test
	#define OPTION_PD_TEST_XAL							0x02
	#define OPTION_PD_TEST_RF							0x03
	#define OPTION_PD_TEST_IO							0x04	//IC all IO self test
	#define OPTION_PD_TEST_RX							0x05	//test Rx mode for CE
	#define OPTION_PD_TEST_CURRENT						0x06	//Sleep mode current test
#define CMD_LO_PD_PRO								0xc5	//烧录
#define CMD_LO_PD_PRORE								0xc6	//回复烧录结果
#define CMD_LO_BCSDATA								0xcb	//updata broadcase data & save it to flash
#define CMD_LO_TEST_UART							0xcd	//UART可靠性测试
#define CMD_LO_TEST_WL								0xce	//无线可靠性测试
#define CMD_LO_TEST_REPLY							0xcf	//reply test result
#define REPLY_TEST_MDLO_PASS						0x10
#define REPLY_TEST_MDLO_FAIL						0x11
#define REPLY_TEST_XAL_PASS							0x20
#define REPLY_TEST_XAL_FAIL							0x21
#define REPLY_TEST_POWER_OK							0x30
#define REPLY_TEST_IO_PASS							0x40
#define REPLY_TEST_IO_FAIL							0x41
#define CMD_LO_T8DATA								0xd8
#define CMD_LO_TTDATA								0xd9
#define CMD_LO_TRDATA								0xdd
#define CMD_LO_BCDATA								0xdf


#define CMD_LO_TEST_WL_BACK							0xe0

#define CMD_LO_WAKEUP								0xf0
#define CMD_LO_SLEEPMODE							0xf1

//define reply command
#define SAPP_PKG_SUCCESS							0x00
#define SAPP_PKG_ERR_SOP							0x01
#define SAPP_PKG_ERR_CMD							0x02
#define SAPP_PKG_ERR_LTH							0x03	//数据包长度错误
#define SAPP_PKG_ERR_XOR							0x04
#define SAPP_PKG_ERR_LEN							0x05	//长度超过20byte
#define SAPP_PKG_ERR_DISC							0x06	//链接未建立
#define SAPP_PKG_ERR_EOP							0x07	//链接配置未准备好
#define SAPP_PKG_ERR_TMR							0x10
#define SAPP_PKG_SUCCESS1							0x80
#define SAPP_PKG_N_ACK								0x81	//收到正应答或负应答
#define SAPP_PKG_ERR_OTH							0xFF
/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
#endif // PROTOCOL_UART_H
