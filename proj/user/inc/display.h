/**************************************************************************************************
Filename:       display.h
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
contact Bouth R&D at www.bough.com.hk
**************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/* Copyright(c) 2012 Bough Technology Corp. All rights reserved.                                 */
/*-----------------------------------------------------------------------------------------------*/

#ifndef DISPLAY_H
#define DISPLAY_H

/*********************************************************************
* INCLUDES
*/
#include "types.h"
#include "protocol.h"

/*********************************************************************
* MACROS
*/

/*********************************************************************
* CONSTANTS
*/
#define LCD_W 							130

#define	LCD_DISPLAY_REVERSE			0

#if(LCD_DISPLAY_REVERSE)
	#define LCD_H 							320
	#define	LCD_Y_START					80
	#define	LCD_X_START					0
#else
	#define 	LCD_H 							160
	#define	LCD_Y_START					3
	#define	LCD_X_START					0
#endif

//画笔颜色
#define WHITE         	 					0xFFFF
#define BLACK         	 					0x0000
#define BLUE         	 					0x001F
#define BRED             						0XF81F
#define GRED 			 				0XFFE0
#define GBLUE			 				0X07FF
#define RED           	 					0xF800
#define MAGENTA       	 				0xF81F
#define GREEN         	 					0x07E0
#define CYAN          	 					0x7FFF
#define YELLOW        	 					0xFFE0
#define BROWN 			 				0XBC40 //棕色
#define BRRED 			 				0XFC07 //棕红色
#define GRAY  			 				0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 				0X01CF	//深蓝色
#define LIGHTBLUE      	 				0X7D7C	//浅蓝色
#define GRAYBLUE       			 		0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	 				0X841F //浅绿色
#define LGRAY 			 				0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        					0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           						0X2B12 //浅棕蓝色(选择条目的反色)


#define	CHAR_COLOR					0X7D7C//WHITE
#define	BACKGROUND_COLOR			BLACK
#define	BATTERY_COLOR					0X7D7C//WHITE
#define	OFF_CHAR_COLOR				GRAY

#define SIZE 								16
#define XLevelL							0x02
#define XLevelH							0x10
#define Max_Column						128
#define Max_Row							64
#define	Brightness						0xFF
#define X_WIDTH 						128
#define Y_WIDTH 						64
#define OLED_CMD  						0			//写命令
#define OLED_DATA 						1			//写数据
#define OLED_MODE 						0

#define	ICON_Degree_ADDRESS				29+32//59+32
#define	ICON_Arrow_ADDRESS				30+32//60+32
#define	ICON_Colon_ADDRESS				31+32//61+32			

#define	ICON_HUES_X						16
#define	ICON_HUES_Y						3
#define	ICON_HUES_ADDRESS				10
#define	Value_Hues_X					ICON_HUES_X+24

#define	HSI_X							0
#define	HSI_Y							0
#define	ICON_HUES_ADDRESS				10

#define	Icon_Clock_X					ICON_HUES_X
#define	Icon_Clock_Y					0
#define	Icon_Clock_Table_Addr			20//30
#define	ValueOfClock_X					ICON_HUES_X+16+4


#define	ICON_Saturation_X				ICON_HUES_X
#define	ICON_Saturation_Y				3
#define	ICON_Saturation_ADDRESS			11
#define	Value_Saturation_X				ICON_HUES_X+24

#define	ICON_BRIGHTNESS_X				ICON_HUES_X
#define	ICON_BRIGHTNESS_Y				6
#define	ICON_BRIGHTNESS_ADDRESS			12
//#define	Value_Brightness_X				ICON_BRIGHTNESS_X+24
#define	Value_Percent_Addr				35

#define	ICON_ColorTemp_X				ICON_HUES_X
#define	ICON_ColorTemp_Y				3
#define	ICON_ColorTemp_ADDRESS			13
#define	Value_ColorTemp_X				ICON_ColorTemp_X+24
#define	CHINESE_K						33

#define	ICON_batt_X0					96
#define	ICON_batt_X0_systemOff			50
#define	ICON_batt_X1					ICON_batt_X0+16
#define	ICON_batt_X1_systemOff			ICON_batt_X0_systemOff+16
#define	ICON_batt_Y						0
#define	ICON_batt_Y_systemOff			4
#define	ICON_batt_lv0_Addr				21
#define	ICON_batt_lv1_Addr				23
#define	ICON_batt_lv2_Addr				25
#define	ICON_batt_lv3_Addr				27
#define	ICON_batt_lv4_Addr				29

#define	ICON_Hot_X						ICON_batt_X0
#define	ICON_Hot_X1						ICON_batt_X1
#define	ICON_Hot_Y						ICON_batt_Y
#define	ICON_Hot_Addr					39

#define	ICON_Percent_X					ICON_HUES_X
#define	ICON_Percent_Y					0


#define	ICON_charge_X0					ICON_batt_X0-32
#define	ICON_charge_X1					ICON_charge_X0+16
#define	ICON_charge_Y					ICON_batt_Y
#define	ICON_charge_TableAddr			36

#define	ICON_Flash_X					ICON_HUES_X
#define	ICON_Flash_Y					3
#define	ICON_Flash_ADDRESS				14

#define	ICON_Loop_X						ICON_Flash_X
#define	ICON_Loop_Y						4
#define	ICON_Loop_ADDRESS				15

#define	ICON_Lock_X						ICON_Flash_X
#define	ICON_Lock_Y						6
#define	ICON_Lock_ADDRESS				16

#define	ICON_Style1_X					ICON_Flash_X+24
#define	ICON_Style1_Y					ICON_Flash_Y
#define	ICON_Style2_X					ICON_Style1_X
#define	ICON_Style3_X					ICON_Style1_X
#define	ICON_Style2_Y					ICON_Style1_Y+2
#define	ICON_Style3_Y					6
#define	ICON_StyleA_ADDRESS				7
#define	ICON_StyleB_ADDRESS				8
#define	ICON_StyleC_ADDRESS				9

#define	ICON_T_X						ICON_HUES_X+16*3
#define	ICON_T_Y						2
#define	ICON_T_Addr						20

#define	Value_CD_X						ICON_HUES_X+16*2+8
#define	Value_CD_Y						4

#define	Counting_X						ICON_HUES_X
#define	Counting_Y						4

#define	TimerBarX						Counting_X
#define	TimerBarY						6
#define	TimerBarStartAddr				62+32
#define	TimerBarEndX					Counting_X+4+100
#define	TimerBarEndAddr				63+32

#define	LOW_CASE_m					28+32
#define	LOW_CASE_h						27+32

#define	CHINESE_SPACE					32

#define	ICON_BLE_X						54
#define	ICON_BLE_Y						0
#define	ICON_BLE_ADDRESS				38


#define	LINE_ADDRESS_X					(LCD_X_START+11)
#define	LINE_ADDRESS_Y					(LCD_Y_START+20)

#define	LEVEL_ADDRESS_X				(LCD_X_START+10)
#define	LEVEL_ADDRESS_Y				(LINE_ADDRESS_Y+5)

#define	Value_Brightness_X				(LCD_X_START+82)
#define	Value_Brightness_Y				LEVEL_ADDRESS_Y

#define	ARROW_X						(Value_Brightness_X-16)

#define	TEMP_ADDRESS_X				LEVEL_ADDRESS_X
#define	TEMP_ADDRESS_Y				(LEVEL_ADDRESS_Y+16)

#define	VALUE_CCT_X					(Value_Brightness_X)
#define	VALUE_CCT_Y					(TEMP_ADDRESS_Y)

#define	COLOR_ADDRESS_X				LEVEL_ADDRESS_X
#define	COLOR_ADDRESS_Y				(LEVEL_ADDRESS_Y+32)

#define	VALUE_COLOR_X					(Value_Brightness_X)
#define	VALUE_COLOR_Y					(COLOR_ADDRESS_Y)

#define	SCENE_ADDRESS_X				LEVEL_ADDRESS_X
#define	SCENE_ADDRESS_Y				(LEVEL_ADDRESS_Y+48)
#define	VALUE_SCENE_X					(Value_Brightness_X)

#define	STROBE_ADDRESS_X				LEVEL_ADDRESS_X
#define	STROBE_ADDRESS_Y				(LEVEL_ADDRESS_Y+64)
#define	VALUE_STROBE_X				(Value_Brightness_X)

#define	RED_RECT_START_X				(LEVEL_ADDRESS_X-1)
#define	RED_RECT_END_X					(RED_RECT_START_X+32)
#define	RED_RECT_START_Y				(STROBE_ADDRESS_Y+20)
#define	RED_RECT_END_Y					(RED_RECT_START_Y+19)

#define	GREEN_RECT_START_X			(RED_RECT_END_X+10)
#define	GREEN_RECT_END_X				(GREEN_RECT_START_X+32)
#define	GREEN_RECT_START_Y			(RED_RECT_START_Y)
#define	GREEN_RECT_END_Y				(RED_RECT_START_Y+19)

#define	BLUE_RECT_START_X				(GREEN_RECT_END_X+10)
#define	BLUE_RECT_END_X				(BLUE_RECT_START_X+32)
#define	BLUE_RECT_START_Y				(RED_RECT_START_Y)
#define	BLUE_RECT_END_Y				(RED_RECT_START_Y+19)


#define	BATT_BORDER_START_X			(LCD_X_START+102)
#define	BATT_BORDER_END_X			(BATT_BORDER_START_X+22)
#define	BATT_BORDER_START_Y			(LCD_Y_START+3)
#define	BATT_BORDER_END_Y			(BATT_BORDER_START_Y+13)

#define	BATT_SINGLE_LV_WIDTH			4


#define	LOGO_START_X					(LCD_X_START)
#define	LOGO_START_Y					(LCD_Y_START+50)
#define	LOGO_WIDTH					230
#define	LOGO_HEITHT					32
/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* VARIABLES
*/

/*********************************************************************
* FUNCTIONS
*/

/**
 * @fn      command_center_Init
 *
 * @brief   Initialization display
 *
 * @param   none
 *
 * @return  none
 */
extern void display_Init( void );

/**
 * @fn      lcd_on
 *
 * @brief   true on lcd
 *
 * @param   data
 *
 * @return  none
 */
extern void lcd_on(void);

/**
 * @fn      lcd_off
 *
 * @brief   true off lcd
 *
 * @param   data
 *
 * @return  none
 */
extern void lcd_off(void);

/**
 * @fn      lcd_clear
 *
 * @brief   true off lcd
 *
 * @param   data
 *
 * @return  none
 */
extern void lcd_clear(u16 Color);

/**
 * @fn		displaySystemMenu
 *
 * @brief	待机画面显示
 *
 * @param   
 *
 * @return  none
 */
extern void displaySystemMenu(displayParamsStruct * disParams);

/**
 * @fn		batteryPercentDisplay
 *
 * @brief	待机画面电池百分比显示
 *
 * @param   
 *
 * @return  none
 */
extern void batteryPercentDisplay(uint8 level);

/**
 *
 *剩余时间显示
 *
*/
extern void updateRemainingTimeByValue(uint16 data);

/**
 * @fn		batterDisplay
 *
 * @brief	电池显示
 *
 * @param   
 *
 * @return  none
 */
extern void batterDisplay(uint8 level);

/**
 * @fn		updateArrowDisplay
 *
 * @brief	光标显示
 *
 * @param   
 *
 * @return  none
 */
extern void updateArrowDisplay(displayParamsStruct * disParams);

/**
 * @fn		updateBLEDisplay
 *
 * @brief	BLE显示
 *
 * @param   
 *
 * @return  none
 */
extern void updateBLEDisplay(bool flag);

/**
 * @fn		updateHuesDisplay
 *
 * @brief	色调值显示
 *
 * @param   
 *
 * @return  none
 */
extern void updateHuesDisplay(displayParamsStruct * disParams,bool updateRGB);

/**
 * @fn		updateSaturationDisplay
 *
 * @brief	饱和度值显示
 *
 * @param   
 *
 * @return  none
 */
//extern void updateSaturationDisplay(displayParamsStruct * disParams);

/**
 * @fn		updateBrightnessDisplay
 *
 * @brief	亮度值显示
 *
 * @param   
 *
 * @return  none
 */
extern void updateBrightnessDisplay(displayParamsStruct * disParams);

/**
 * @fn		updateColorTempDisplay
 *
 * @brief	色温值显示
 *
 * @param   
 *
 * @return  none
 */
extern void updateColorTempDisplay(displayParamsStruct * disParams);

/**
 * @fn		updateLightEffectDisplay
 *
 * @brief	灯效样式显示
 *
 * @param   
 *
 * @return  none
 */
extern void updateLightEffectDisplay(displayParamsStruct * disParams);

/**
 * @fn		updateHotDisplay
 *
 * @brief	过热显示
 *
 * @param   
 *
 * @return  none
 */
extern void updateHotDisplay(bool flag);

/**
 * @fn		OLED_ShowNum
 *
 * @brief	测试显示
 *
 * @param   
 *
 * @return  none
 */
extern void OLED_ShowString(uint8 x,uint8 y,uint8 *chr);

/**
 * @fn		OLED_ShowNum
 *
 * @brief	测试显示
 *
 * @param   
 *
 * @return  none
 */
extern uint8 OLED_ShowNum(uint8 x, uint8 y, uint32 num);

/**
 * @fn		temperatureDisplay
 *
 * @brief	测试显示
 *
 * @param   
 *
 * @return  none
 */
extern void temperatureDisplay(uint8 x, uint8 y, uint32 value);

/**
 * @fn		temperatureDisplay_char
 *
 * @brief	测试显示
 *
 * @param   
 *
 * @return  none
 */
void clearSecondLineDisplay(void);
void temperatureDisplay_char(uint8 x, uint8 y, uint8 value);
void reInitialLCD(void);
void displayFloat(UINT8 x, UINT8 y, float fNum, UINT8 size, UINT8 per);
void oledDisplayChineseString(uint8 x, uint8 y, const uint8 *str, uint8 len);
void displayFactoryInfo(u8 item);
void LCD_ShowChar(u16 x, u16 y, u8 num);
uint8 lcdDrawFontGbk16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, char *s);
void lcdDrawPoint(uint16_t x, uint16_t y, uint16_t data, uint8 includePos);
void lcdDrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void lcdRectColorDraw(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t fc, bool roundRect);
void lcdDrawSingleCharGbk16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8 digit);
void updateSceneDisplay(displayParamsStruct * disParams);
void updateStrobeDisplay(displayParamsStruct * disParams);
void lcdEntrySleep(void);
void exitLcdSleep(void);
void lcdDraw160times32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8 * data);
void lcdDraw32times32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8 * data);
void lcdDrawWidthTimesHeight(uint16_t x, uint16_t y, uint16 width, uint16 heitht, uint16_t fc, uint16_t bc, uint8 * data);
/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
#endif // DISPLAY_H
