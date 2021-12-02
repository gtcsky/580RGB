/**************************************************************************************************
Filename:       Driver_HT1621.c
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

/*********************************************************************
* INCLUDES
*/
#include "protocol.h"
#include "Driver_HT1621.h"

/*********************************************************************
* MACROS
*/

/*********************************************************************
* GLOBAL FUNCTIONS
*/


/*********************************************************************
* CONSTANTS
*/
#if DriverType == HT1622
const FillScreen[] = {};
#else
#endif
/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* VARIABLES
*/
uint8 databuf[32] = {0};

/*********************************************************************
* EXTERNAL FUNCTIONS
*/
extern void Delay_WaitUs(uint16 microSecs);
/*********************************************************************
* LOCAL VARIABLES
*/

/*********************************************************************
* LOCAL FUNCTIONS
*/
static void Driver_HT162x_WrCmd(unsigned char ucCmd);
static void Driver_HT162x_WrAddr(lcddata_t lcddata);

/*********************************************************************
* FUNCTIONS
*/

/******************************************************************************
* @fn      Driver_HT162x_Init
*
* @brief   Init driver for the HT162x
*
* @param   none
*
* @return  none
*/
void Driver_HT162x_Init(unsigned char cmd)
{
	lcddata_t lcddata;
	
	switch(cmd)
	{
		case LCD_ON:
		//---------------------------端口控制总线初始化-----------------------------
		//    PE_DDR_DDR2 = 1;//CS控制端口配置为输出
		//    PE_CR1_C12 = 1;//推挽输出
		//    PE_CR2_C22 = 0;//最大输出速度2MHz
		//
		//    PG_DDR_DDR1 = 1;//WR控制端口配置为输出
		//    PG_CR1_C11 = 1;//推挽输出
		//    PG_CR2_C21 = 0;//最大输出速度2MHz
		//
		//    PG_DDR_DDR0 = 1;//DAT控制端口配置为输出
		//    PG_CR1_C10 = 1;//推挽输出
		//    PG_CR2_C20 = 0;//最大输出速度2MHz
		//
		// 	P0M1 &= ~(1 << 4);	//P0.4 设置为推挽输出
		//	P0M0 |=  (1 << 4);
		//
		//	P0M1 &= ~(1 << 2);	//P0.2 设置为推挽输出
		//	P0M0 |=  (1 << 2);
		//
		//	P0M1 &= ~(1 << 1);	//P0.1 设置为推挽输出
		//	P0M0 |=  (1 << 1);

		//输出状态初始化
		hal_gpio_write(HT162X_CS, 1);
		hal_gpio_write(HT162X_WR, 1);
		hal_gpio_write(HT162X_DAT, 1);
		

		//    //输入,HT162X_RD/HT162X_IRQ,未用!
		//    PE_DDR_DDR3 = 0;//RD控制端口配置为输入
		//    PE_CR1_C13 = 1;//上拉输入
		//    PE_CR2_C23 = 0;//禁止中断
		//
		//    PC_DDR_DDR4 = 0;//IRQ控制端口配置为输入
		//    PC_CR1_C14 = 1;//上拉输入
		//    PC_CR2_C24 = 0;//禁止中断
		//--------------------------------------------------------------------------

		//--------------------------------------------------------------------------
#if DriverType != HT1622
		Driver_HT162x_WrCmd(BIAS1_3);
#endif

#if DriverType == HT1621
		Driver_HT162x_WrCmd(RC256K);    //使用内部256K RC振荡器,若是HT1620则只能用外部晶振
#elif DriverType == HT1622
		Driver_HT162x_WrCmd(EXT32K);    //使用内部32K RC振荡器,若是HT1620则只能用外部晶振
#endif

		Driver_HT162x_WrCmd(SYSEN);
		Driver_HT162x_WrCmd(LCDON);
		//--------------------------------------------------------------------------

		//--------------------------------------------------------------------------
#if DriverType == HT1622	//6位地址，地址范围0~63
		lcddata.ucStartSeg = 0;
		lcddata.ucEndSeg = 191;
		lcddata.ucData = 0xff;
		lcddata.ucClear = 0xff;
		Driver_HT162x_FillScreen(lcddata);
#else	//6位地址，地址范围0~31
		lcddata.ucStartSeg = 0;
		lcddata.ucEndSeg = 31;
		lcddata.ucData = 0xff;
		lcddata.ucClear = 0xff;
		Driver_HT162x_FillScreen(lcddata);
#endif
		//--------------------------------------------------------------------------
		break;

		case LCD_REFRESH:
#if DriverType != HT1622
		Driver_HT162x_WrCmd(BIAS1_3);
#endif

#if DriverType == HT1621
		Driver_HT162x_WrCmd(RC256K);    //使用内部256K RC振荡器,若是HT1620则只能用外部晶振
#elif DriverType == HT1622
		Driver_HT162x_WrCmd(EXT32K);    //使用内部32K RC振荡器,若是HT1620则只能用外部晶振
#endif

		Driver_HT162x_WrCmd(SYSEN);
		Driver_HT162x_WrCmd(LCDON);
		break;

		case LCD_OFF:
#if DriverType == HT1622
		lcddata.ucStartSeg = 0;
		lcddata.ucEndSeg = 191;
		lcddata.ucData = 0;
		lcddata.ucClear = 0;
		Driver_HT162x_FillScreen(lcddata);
#else
		lcddata.ucStartSeg = 0;
		lcddata.ucEndSeg = 31;
		lcddata.ucData = 0;
		lcddata.ucClear = 0;
		Driver_HT162x_FillScreen(lcddata);
#endif

		Driver_HT162x_WrCmd(LCDOFF);//关闭液晶
		Driver_HT162x_WrCmd(SYSDIS);//系统待机
		break;

		default:
		break;
	}
}

/******************************************************************************
* @fn      Driver_HT162x_WrCmd
*
* @brief   Write command(100)
*
* @param   none
*
* @return  none
*/
static void Driver_HT162x_WrCmd(unsigned char ucCmd)
{
	unsigned char i,ucCmdMark = 0x80;//写命令模式标志100-X,部分命令X=0,部分命令X=1,此时ucCmdMark = 0x90

	hal_gpio_write(HT162X_CS, 0);
	WaitUs(10);

	for(i=0;i<4;i++)
	{
		if(ucCmdMark & 0x80)
		{
			hal_gpio_write(HT162X_DAT, 1);
		}
		else
		{
			hal_gpio_write(HT162X_DAT, 0);
		}
		hal_gpio_write(HT162X_WR, 0);
		WaitUs(10);
		hal_gpio_write(HT162X_WR, 1);
		WaitUs(1);
		ucCmdMark <<= 1;
	}

	for(i=0;i<8;i++)//写入命令
	{
		if(ucCmd & 0x80)
		{
			hal_gpio_write(HT162X_DAT, 1);
		}
		else
		{
			hal_gpio_write(HT162X_DAT, 0);
		}
		hal_gpio_write(HT162X_WR, 0);
		WaitUs(10);
		hal_gpio_write(HT162X_WR, 1);
		WaitUs(1);
		ucCmd <<= 1;
	}
	hal_gpio_write(HT162X_CS, 1);
}

/******************************************************************************
* @fn      Driver_HT162x_WrAddr
*
* @brief   Write address(101)
*
* @param   none
*
* @return  none
*/
static void Driver_HT162x_WrAddr(lcddata_t lcddata)
{
	unsigned char i;

	for(i=0;i<3;i++)//写入地址模式标志,3 bit
	{
		if(lcddata.ucDataMark & 0x80)
		{
			hal_gpio_write(HT162X_DAT, 1);
		}
		else
		{
			hal_gpio_write(HT162X_DAT, 0);
		}
		hal_gpio_write(HT162X_WR, 0);
		WaitUs(10);
		hal_gpio_write(HT162X_WR, 1);
		WaitUs(1);
		lcddata.ucDataMark <<= 1;
	}

	lcddata.ucSegX <<= 2;//地址仅取低6位，先送高位
	for(i=0;i<6;i++)
	{
		if(lcddata.ucSegX & 0x80)
		{
			hal_gpio_write(HT162X_DAT, 1);
		}
		else
		{
			hal_gpio_write(HT162X_DAT, 0);
		}
		hal_gpio_write(HT162X_WR, 0);
		WaitUs(10);
		hal_gpio_write(HT162X_WR, 1);
		WaitUs(1);
		lcddata.ucSegX <<= 1;
	}
}

/**************************************************************************************************
* @fn		Driver_HT162x_ReData
*
* @brief	Read one byte data from address
*
* @param	ucSegX - address(Seg)
*			ucData - data(com)
*			ucClear - data filter
*
* @return      none
*
*/
unsigned char Driver_HT162x_ReData(lcddata_t lcddata)
{
	// Enable CS
	HT162X_CS = 0;
	WaitUs(10);

	P1SEL &= ~0x04;	//P1.2 as GPIO
	P1DIR |= 0x04;	//P1.2 initialize output

	// Write address(110)
	lcddata.ucDataMark = 0xc0;
	Driver_HT162x_WrAddr(lcddata);

	// Read data
	P1SEL &= ~0x04;	//P1.2 as GPIO
	P1DIR &= ~0x04;	//P1.2 initialize input

	for ( unsigned char daCnt = 0; daCnt < 4; daCnt++ )
	{
		HT162X_RD = 0;
		WaitUs(10);
		lcddata.ucData = HT162X_DAT<<4;
		lcddata.ucData >>= 1;
	}
	//updata buff
	lcddata.ucData &= ~lcddata.ucClear;
	HT162X_CS = 1;

	return lcddata.ucData;
}

/**************************************************************************************************
* @fn		HT162X_ReModWrData
*
* @brief	Write one byte data into address(Seg) by write mode
*
* @param	ucSegX - address(Seg)
*			ucData - data(com)
*
* @return      none
*
*/
void Driver_HT162x_WrData(lcddata_t lcddata)
{
	unsigned char i;
	unsigned char data = 0;

	databuf[lcddata.ucSegX] &= ~lcddata.ucClear;
	databuf[lcddata.ucSegX] |= lcddata.ucData;
	data = databuf[lcddata.ucSegX];
	
	HT162X_CS = 0;
	WaitUs(10);

	P1DIR |= 0x04;	//P1.2 initialize ouput

	lcddata.ucDataMark = 0xa0;
	Driver_HT162x_WrAddr(lcddata);

	for(i=0;i<4;i++)
	{
		if(data & 0x01)
		{
			hal_gpio_write(HT162X_DAT, 1);
		}
		else
		{
			hal_gpio_write(HT162X_DAT, 0);
		}
		HT162X_WR = 0;
		WaitUs(10);
		HT162X_WR = 1;
		WaitUs(1);
		data >>= 1;
	}
	HT162X_CS = 1;
}

/**************************************************************************************************
* @fn		HT162X_ReModWrData
*
* @brief	Write one byte data into address(Seg) by read-modify-write mode
*
* @param	ucSegX - address(Seg)
*			ucData - data(com)
*			ucClear - data filter
*
* @return      none
*
*/
void Driver_HT162x_ReModWrData(lcddata_t lcddata)
{
	unsigned char daCnt = 0;
	unsigned char buff = 0;

	// Enable CS
	HT162X_CS = 0;
	WaitUs(10);

	P1DIR |= 0x04;	//P1.2 initialize ouput

	// Write address(101)
	lcddata.ucDataMark = 0xa0;
	Driver_HT162x_WrAddr(lcddata);

	// Read data
	P1DIR &= ~0x04;	//P1.2 initialize input
	P1INP &= ~0x04;	// Set pin P1.4,P1.5 Pullup/Pulldown, 3-state
	P2INP &= ~0x40;	// Set P1 pullup

	for ( daCnt = 0; daCnt < 4; daCnt++ )
	{
		HT162X_RD = 0;
		WaitUs(10);
		buff |= HT162X_DAT<<4;
		buff >>= 1;
		HT162X_RD = 1;
	}
	//clear buff
	buff &= lcddata.ucClear;
	//updata data
	buff |= lcddata.ucData;

	// Write data
	P1SEL &= ~0x04;	//P1.2 function to GPIO
	P1DIR |= 0x04;	//P1.2 direction to output

	for ( daCnt = 0; daCnt < 4; daCnt++ )
	{
		if(buff & 0x01)
		{
			hal_gpio_write(HT162X_DAT, 1);
		}
		else
		{
			hal_gpio_write(HT162X_DAT, 0);
		}
		HT162X_WR = 0;
		WaitUs(10);
		HT162X_WR = 1;
		WaitUs(1);
		buff >>= 1;
	}
	HT162X_CS = 1;
}

/******************************************************************************
* @fn      Driver_HT162x_FillScreen
*
* @brief   open or clear LCD
*
* @param   srcaddr,dstaddr,dat
*
* @return  none
*/
void Driver_HT162x_FillScreen(lcddata_t lcddata)
{
	unsigned char i;

	if(lcddata.ucStartSeg < lcddata.ucEndSeg)
	{
		for(i=lcddata.ucStartSeg;i<=lcddata.ucEndSeg;i++)
		{
			lcddata.ucSegX = i;
			Driver_HT162x_ReModWrData(lcddata);
			//Driver_HT162x_WrData(i,dat,4);
		}
	}
	else
	{
		for(i=lcddata.ucEndSeg;i<=lcddata.ucStartSeg;i++)
		{
			lcddata.ucSegX = i;
			Driver_HT162x_ReModWrData(lcddata);
			//Driver_HT162x_WrData(i,dat,8);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////