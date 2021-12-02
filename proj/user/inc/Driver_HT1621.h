/**************************************************************************************************
Filename:       Driver_HT1621.h
Revised:        Date: 2016.05.20
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
contact Bouth R&D at www.bough.com.
**************************************************************************************************/


/*-----------------------------------------------------------------------------------------------*/
/* Copyright(c) 2012 Bough Technology Corp. All rights reserved.                                 */
/*-----------------------------------------------------------------------------------------------*/
#ifndef DRIVER_HT1621_H
#define DRIVER_HT1621_H

/*********************************************************************
* INCLUDES
*/
#include "bcomdef.h"

/*********************************************************************
* MACROS
*/

#define HT1620 0
#define HT1621 1
#define HT1622 2
#define DriverType HT1621

/* 命令摘要 */
//命令模式，ID=100
#define SYSDIS    0x00        //0b100 0000-0000-X     关系统振荡器和LCD偏压发生器（默认）
#define SYSEN     0x02        //0b100 0000-0001-X     打开系统振荡器
#define LCDOFF    0x04        //0b100 0000-0010-X     关闭LCD偏压发生器（默认）
#define LCDON     0x06        //0b100 0000-0011-X     打开LCD偏压发生器
#define TIMERDIS  0x08        //0b100 0000-0100-X     禁止时基输出
#define TIMEREN   0x0C        //0b100 0000-0110-X     使能时基输出
//#define WDTDIS    0x0A        //0b100 0000-0101-X     禁止WDT溢出标志输出
//#define WDTEN     0x0E        //0b100 0000-0111-X     使能WDT溢出标志输出

/* HT1620,HT1621有打开音频输出命令，HT1622无 */
#define TONEOFF   0x10        //0b100 0000-1000-X     关闭音频输出（默认）
#if DriverType != HT1622
#define TONEON  0x12        //0b100 0000-1001-X     打开音频输出
#endif
#define TONE4K    0x80        //0b100 010X-XXXX-X     音调频率4KHz
#define TONE2K    0xC0        //0b100 011X-XXXX-X     音调频率2KHz

#define CLRTIMER  0x1A        //0b100 0000-1101-X     清零时基发生器内容
//#define CLRWDT    0x1E        //0b100 0000-1111-X     清零WDT

/* HT1620必须使用外部时钟源，HT1621,HT1622可选片内或片外 */
#if DriverType == HT1621
#define RC256K    0x30      //0b100 0001-10XX-X     系统时钟源，内部RC振荡（默认）
#define EXT256K   0x38      //0b100 0001-11XX-X     系统时钟源，外部时钟源
#elif DriverType == HT1622
#define RC32K     0x30      //0b100 0001-10XX-X     系统时钟源，内部RC振荡（默认）
#define EXT32K    0x38      //0b100 0001-11XX-X     系统时钟源，外部时钟源
#endif

/* HT1620,HT1621偏压可选，HT1622固定 */
#if DriverType != HT1622
#define BIAS1_2   0x50      //0b100 0010-abX0-X     LCD 1/2偏压选项：ab=00,2COM;ab=01,3COM;ab=10,4COM.
#define BIAS1_3   0x52      //0b100 0010-abX1-X     LCD 1/3偏压选项：ab=00,2COM;ab=01,3COM;ab=10,4COM.
#endif


#define LCD_ON      0
#define LCD_REFRESH 1
#define LCD_OFF     2


/*********************************************************************
* CONSTANTS
*/
/* define peripheral to IO */
#define HT162X_CS								GPIO_DISPLAY_CS
#define HT162X_WR								GPIO_DISPLAY_CLK
#define HT162X_RD								GPIO_DISPLAY_MISO
#define HT162X_DAT								GPIO_DISPLAY_MOSI

/*********************************************************************
* TYPEDEFS
*/
typedef struct
{
	unsigned char ucStartSeg;
	unsigned char ucEndSeg;
	unsigned char ucDataMark;
	unsigned char ucSegX;
	unsigned char ucData;
	unsigned char ucClear;
} lcddata_t;
/*********************************************************************
* FUNCTIONS
*/

/*
* @fn      Driver_HT162x_Init
*
* @brief   Init driver for the HT162x
*
* @param   none
*
* @return  none
*/
void Driver_HT162x_Init(unsigned char cmd);

/**
* @fn		Driver_HT162x_WrData
*
* @brief	Write one byte data into address(Seg) by write mode
*
* @param	ucSegX - address(Seg)
*			ucData - data(com)
*
* @return      none
*
*/
void Driver_HT162x_WrData(lcddata_t lcddata);

/*
* @fn      Driver_HT162x_FillScreen
*
* @brief   open or clear LCD
*
* @param   srcaddr,dstaddr,dat
*
* @return  none
*/
void Driver_HT162x_FillScreen(lcddata_t lcddata);

/*********************************************************************
*********************************************************************/
#endif /* DRIVER_HT1621_H */