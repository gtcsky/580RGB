/**************************************************************************************************
Filename:       display.c
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


/*********************************************************************
* INCLUDES
*/
#include "types.h"
#include "OSAL.h"
#include "OSAL_Tasks.h"
#include "common.h"

/* Application */
#include "protocol.h"
#include "display.h"
#include "user_character.h"
#include "pwrmgr.h"
#include "gpio.h"
#include "spi.h"
#include "command_center.h"
#include "systems_parameters.h"
#include "log.h"
#include "user_color.h"
#include "clock.h"
#include <stdio.h>
#include <string.h>

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
static hal_spi_t lcd_spi_handle = {SPI0};

static bool fIsChargeDisplaying = false;
static bool fIsHotDisplayOn = false;
displayParamsStruct displayParams;

/*********************************************************************
* FUNCTIONS
*/
static int lcd_bus_init(void);
static void write_cmd(uint8_t cmd);
static void write_data(uint8_t Data,uint8 set_DC);
static void lcd_config(void);
//static void OLED_Set_Pos(unsigned char x, unsigned char y);
//static void OLED_ShowChar(uint8 x,uint8 y,uint8 chr);
//static void OLED_ShowOneSegment(uint8 x,uint8 y,uint8  data1,uint8 data2);
//static uint32 oled_pow(uint8 m,uint8 n);
//static uint8 getNumLen(uint32 num);
//static uint8 OLED_ShowChineseNum(uint8 x, uint8 y, uint32 num);
//static void OLED_ShowCHinese(uint8 x,uint8 y,uint8 no);
//static void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
//static void HexDigitDis(uint8 x,uint8 y,uint8 value);
static void oledDisplayChineseString(uint8 x,uint8 y,const uint8 *str,uint8 len);

static const uint16 arrowArray[][2]={{ARROW_X,LEVEL_ADDRESS_Y},{ARROW_X,TEMP_ADDRESS_Y},{ARROW_X,COLOR_ADDRESS_Y},{ARROW_X,SCENE_ADDRESS_Y},{ARROW_X,STROBE_ADDRESS_Y}};

static 	bool fIsBrightnessOffDisplaying=false;
static 	char charArray[10]={0};
/*********************************************************************
 * @fn      lcd_bus_init
 *
 * @brief   lcd bus(SPI) initial
 *
 * @param   data
 *
 * @return  none
 */
static int lcd_bus_init(void){
	int ret;
	spi_Cfg_t cfg =
	{
      .sclk_pin = GPIO_DISPLAY_CLK,
//      .ssn_pin = GPIO_DUMMY,
      .ssn_pin = GPIO_DISPLAY_CS,
      .MOSI = GPIO_DISPLAY_MOSI,
      .MISO = GPIO_DISPLAY_MISO,
      .baudrate = 4800000,
      .spi_tmod = SPI_TXD,//SPI_TRXD,
      .force_cs = false,
      .int_mode = false,
    };
  
    ret = hal_spi_bus_init(&lcd_spi_handle,cfg);//spi init
    hal_gpio_DS_control(GPIO_DISPLAY_RST,Bit_ENABLE);
    hal_gpio_DS_control(cfg.sclk_pin,Bit_ENABLE);
    hal_gpio_DS_control(cfg.MOSI,Bit_ENABLE);
    hal_gpio_pin_init(GPIO_DISPLAY_DC, OEN);
    return ret;

}
void lcdBusDeinit(void){

	hal_spi_bus_deinit(&lcd_spi_handle);

}

void userSetOutputPin(GPIO_Pin_e pin, uint8_t en) {
//	hal_gpio_fmux(pin, Bit_DISABLE);     				//disable fullmux function

	    uint32_t  bit = BIT(pin & 0x1f);
	   BM_CLR(REG_FMUX_EN_FUC(pin), bit);      	 //clear bit

//	if (pin < 18) {
//		bit = BIT(pin);
//		if (en) {
//			BM_SET(reg_gpio_swporta_dr, bit);       	//set pin output(set bit)
//		} else {
//			BM_CLR(reg_gpio_swporta_dr, bit);       	//set pin input(clear bit)
//		}
//	} else {
		bit = BIT(pin - 18);
		if (en) {
			BM_SET(reg_gpio_swportb_dr, bit);       	//set pin output(set bit)
		} else {
			BM_CLR(reg_gpio_swportb_dr, bit);       	//set pin input(clear bit)
		}
//	}
}
/**************************************************************************************
 * @fn      lcd_bus_deinit
 *
 * @brief   lcd bus(SPI) deinitial
 *
 * @param   data
 *
 * @return  none
 */
//static int lcd_bus_deinit(void)
//{
//    return hal_spi_bus_deinit(&lcd_spi_handle);
//}
//

/**************************************************************************************
 * @fn      write_cmd
 *
 * @brief   write cmd to lcd
 *
 * @param   data
 *
 * @return  none
 */
static void  write_cmd(uint8 cmd){

	hal_gpio_fast_write(GPIO_DISPLAY_DC,0);
	hal_spi_send_byte(&lcd_spi_handle,cmd);
	hal_gpio_fast_write(GPIO_DISPLAY_DC,1);

}
/*********************************************************************
 * @fn      write_data
 *
 * @brief   write uint8 data to lcd
 *
 * @param   data
 *
 * @return  none
 */
static void write_data(uint8_t Data, uint8 set_DC) {
	if (set_DC) {
		hal_gpio_fast_write(GPIO_DISPLAY_DC, 1);
	}
	hal_spi_send_byte(&lcd_spi_handle, Data);
}

void Address_set(uint16 x1, uint16 y1, uint16 x2, uint16 y2) {

		write_cmd(0x2a);
		write_data(x1 >> 8,1);
		write_data(x1,0);
		write_data(x2 >> 8,0);
		write_data(x2,0);

		write_cmd(0x2b);
		write_data(y1 >> 8,1);
		write_data(y1,0);
		write_data(y2 >> 8,0);
		write_data(y2,0);

		write_cmd(0x2C);

}

/*!
 \brief      set the start display point of lcd
 \param[in]  x: the x position of the start point
 \param[in]  y: the y position of the start point
 \param[out] none
 \retval     none
 */
void lcd_set_xy(uint16_t x, uint16_t y) {
	/* write the register address 0x2A*/
	write_cmd(0x2A);
//	write_data_uint16(x);
	write_data((x>>8)&0xFF,1);
	write_data(x&0xFF,0);

	/* write the register address 0x2B*/
	write_cmd(0x2B);
//	write_data_uint16(y);
	write_data((y>>8)&0xFF,1);
	write_data(y&0xFF,0);

	/* write the register address 0x2C*/
	write_cmd(0x2C);
}


/*!
 \brief      draw a point on the lcd
 \param[in]  x: the x position of the point
 \param[in]  y: the y position of the point
 \param[in]  data: write the register data
 \param[out] none
 \retval     none
 */
void lcdDrawPoint(uint16_t x, uint16_t y, uint16_t data,uint8 includePos) {
	if(includePos){
		lcd_set_xy(x, y);
		write_data(data >> 8,1);
	}else{
		write_data(data >> 8,0);
	}
	write_data(data,0);
}

/*!
 \brief      set lcd display region
 \param[in]  x_start: the x position of the start point
 \param[in]  y_start: the y position of the start point
 \param[in]  x_end: the x position of the end point
 \param[in]  y_end: the y position of the end point
 \param[out] none
 \retval     none
 */
void lcdSetRegion(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end) {
	write_cmd(0x2A);
	write_data(x_start >> 8,1);
	write_data(x_start,0);
	write_data(x_end >> 8,0);
	write_data(x_end,0);

	write_cmd(0x2B);
	write_data(y_start >> 8,1);
	write_data(y_start,0);
	write_data(y_end >> 8,0);
	write_data(y_end,0);

	write_cmd(0x2C);
}
/*********************************************************************
 * @fn      lcd_config
 *
 * @brief   lcd initial
 *
 * @param   data
 *
 * @return  none
 */
static void lcd_config(void) {
//	hal_gpio_write(GPIO_DISPLAY_RST,0);
//	WaitMs(1);
	hal_gpio_pin_init(GPIO_LCD_BACKLIGHT, OEN);
	hal_gpio_pin_init(GPIO_VLCD, OEN);
	hal_gpio_write(GPIO_LCD_BACKLIGHT, LOW_STTS);
//	hal_gpio_write(GPIO_LCD_BACKLIGHT, HI_STTS);
	hal_gpio_write(GPIO_VLCD, HI_STTS);
	hal_gpio_write(GPIO_DISPLAY_RST, 1);
	WaitMs(1);
	hal_gpio_write(GPIO_DISPLAY_RST, 0);
	WaitMs(1);
	hal_gpio_write(GPIO_DISPLAY_RST, 1);
	WaitMs(120);
	write_cmd(0x11); /*display off*/
	WaitMs(120);

	//------------------------------  display and color format setting--------------------------------//
	write_cmd(0x36);

//#if(LCD_DISPLAY_REVERSE==1)
//	write_data(0xc0,1);
//#else
//	write_data(0x00,1);
//#endif
//	write_cmd(0x3a);
//	write_data(0x05,1);
	//--------------------------------ST7789V Frame rate setting-----------------------------------//
//	write_cmd(0xb2);
//	write_data(0x0c,1);
//	write_data(0x0c,0);
//	write_data(0x00,0);
//	write_data(0x33,0);
//	write_data(0x33,0);
//	write_cmd(0xb7);
//	write_data(0x35,1);
	write_cmd(0xb1);
	write_data(0x01,1);
	write_data(0x2c,0);
	write_data(0x2d,0);

	write_cmd(0xb2);
	write_data(0x01,1);
	write_data(0x2c,0);
	write_data(0x2d,0);
	write_data(0x01,0);
	write_data(0x2c,0);
	write_data(0x2d,0);

	write_cmd(0xb3);
	write_data(0x01,1);
	write_data(0x2c,0);
	write_data(0x2d,0);

	write_cmd(0xb4);
	write_data(0x07,1);
	//---------------------------------ST7789V Power setting---------------------------------------//
	write_cmd(0xC0);
	write_data(0xA2,1);
	write_data(0x02,0);
	write_data(0x84,0);

	write_cmd(0xc1);
	write_data(0xC5,1);

	write_cmd(0xC2);
	write_data(0xA0,1);
	write_data(0x00,0);

	write_cmd(0xC3);
	write_data(0x8A,1);
	write_data(0x2A,0);

	write_cmd(0xc4);
	write_data(0x8A,1);
	write_data(0xEE,0);

	write_cmd(0xc5);
	write_data(0x0E,1);

	write_cmd(0x36);
	write_data(0xC8,1);			//横屏 A8
	//--------------------------------ST7789V gamma setting---------------------------------------//
	write_cmd(0xE0);
	write_data(0x0f,1);
	write_data(0x1a,0);
	write_data(0x0f,0);
	write_data(0x18,0);
	write_data(0x2f,0);
	write_data(0x28,0);
	write_data(0x20,0);
	write_data(0x22,0);
	write_data(0x1f,0);
	write_data(0x1b,0);
	write_data(0x23,0);
	write_data(0x37,0);
	write_data(0x00,0);
	write_data(0x07,0);
	write_data(0x02,0);
	write_data(0x10,0);

	write_cmd(0xE1);
	write_data(0x0f,1);
	write_data(0x1b,0);
	write_data(0x0f,0);
	write_data(0x17,0);
	write_data(0x33,0);
	write_data(0x2c,0);
	write_data(0x29,0);
	write_data(0x2e,0);
	write_data(0x30,0);
	write_data(0x30,0);
	write_data(0x39,0);
	write_data(0x3f,0);
	write_data(0x00,0);
	write_data(0x07,0);
	write_data(0x03,0);
	write_data(0x10,0);


	write_cmd(0x2a);
	write_data(0x00,1);
	write_data(0x00+2,0);
	write_data(0x00,0);
	write_data(0x80+2,0);

	write_cmd(0x2b);
	write_data(0x00,1);
	write_data(0x00+3,0);
	write_data(0x00,0);
	write_data(0x80+3,0);


	write_cmd(0xF0); //Enable test command
	write_data(0x01,1);
	write_cmd(0xF6); //Disable ram power save mode
	write_data(0x00,1);

	write_cmd(0x3A); //65k mode
	write_data(0x05,1);

//	write_cmd(0x21);
//	write_cmd(0x11);
	//Delay (120);
	write_cmd(0x29);
	WaitMs(1);
	//WaitUs(1000); // 15 us
}
/*********************************************************************
 * @fn      OLED_Set_Pos
 *
 * @brief   定位OLED 显示坐标
 *
 * @param   data
 *
 * @return  none
 */
//static void OLED_Set_Pos(unsigned char x, unsigned char y)
//{
//	write_cmd(0xb0 + y);
//	write_cmd(((x & 0xf0) >> 4) | 0x10);
//	write_cmd((x & 0x0f) | 0x01);
//}

/*********************************************************************
 * @fn		OLED_ShowChar
 *
 * @brief	在指定位置显示一个字符,包括部分字符
 *
 * @param   x:0~127, y:0~63, 
 *			mode:0=反白显示;1=正常显示
 *			size:选择字体 16/12
 *
 * @return  none
 */
//static void OLED_ShowChar(uint8 x, uint8 y, uint8 chr) {
//	unsigned char c = 0, i = 0;
//	c = chr - ' ';    //得到偏移后的值
//	if (x > Max_Column - 1) {
//		x = 0;
//		y = y + 2;
//	}
//	//		if(SIZE ==16)
//	//			{
//	OLED_Set_Pos(x, y);
//	for (i = 0; i < 8; i++) {
//		write_data(F8X16[c * 16 + i]);
//	}

//	OLED_Set_Pos(x, y + 1);
//	for (i = 0; i < 8; i++) {
//		write_data(F8X16[c * 16 + i + 8]);
//	}
//}

/*********************************************************************
 * @fn		OLED_ShowOneSegment
 *
 * @brief	写单条Segment 数据(16条com线)数据
 *
 * @param   
 *
 * @return  none
 */
//static void OLED_ShowOneSegment(uint8 x,uint8 y,uint8  data1,uint8 data2)
//{
//	OLED_Set_Pos(x,y);
//	write_data(data1);
//	OLED_Set_Pos(x,y+1);
//	write_data(data2);
//}

/*********************************************************************
 * @fn		oled_pow
 *
 * @brief	m^n函数
 *
 * @param   
 *
 * @return  none
 */
//static uint32 oled_pow(uint8 m,uint8 n){

//	uint32 result=1;
//	while(n--)result*=m;
//	return result;
//}

/*********************************************************************
 * @fn		getNumLen
 *
 * @brief	
 *
 * @param   
 *
 * @return  none
 */
//static uint8 getNumLen(uint32 num){

//	uint8 len=1;
//	while (1)
//	{
//		if (num < 10)
//		{
//			return	len;
//		}
//		else
//		{
//			num /= 10;
//		}
//		len++;
//	}
//}

/*********************************************************************
 * @fn		OLED_ShowNum
 *
 * @brief	显示数字 并返回数字长度
 *
 * @param   
 *
 * @return  none
 */
uint8 OLED_ShowNum(uint8 x, uint8 y, uint32 num){
	return 0;
//	uint8 t, temp;
//	uint8 enshow = 0;
//	uint8 len = getNumLen(num);
//	for (t = 0; t < len; t++)
//	{
//		temp = (num / oled_pow(10, len - t - 1)) % 10;
//		if (enshow == 0 && t < (len - 1))
//		{
//			if (temp == 0)
//			{
//				OLED_ShowChar(x + 8 * t, y, ' ');
//				continue;
//			} else
//				enshow = 1;
//		}
//		OLED_ShowChar(x + 8 * t, y, temp + '0');
//	}
//	return len;
}

/*********************************************************************
 * @fn		OLED_ShowChineseNum
 *
 * @brief	显示字符 并返回字符长度
 *
 * @param   
 *
 * @return  none
 */
//static uint8 OLED_ShowChineseNum(uint8 x, uint8 y, uint32 num) {

//	uint8 t, temp;
//	uint8 enshow = 0;
//	uint8 len = getNumLen(num);
//	for (t = 0; t < len; t++) {
//		temp = (num / oled_pow(10, len - t - 1)) % 10;
//		if (enshow == 0 && t < (len - 1)) {
//			if (temp == 0) {
//				OLED_ShowCHinese(x + 16 * t, y, CHINESE_SPACE);
//				continue;
//			} else {
//				enshow = 1;
//			}
//		}
//		OLED_ShowCHinese(x + 16 * t, y, temp);
//	}
//	return len;
//}

/*********************************************************************
 * @fn		OLED_ShowString
 *
 * @brief	显示一个字符号串
 *
 * @param   
 *
 * @return  none
 */
void OLED_ShowString(uint8 x,uint8 y,uint8 *chr){
//	unsigned char j = 0;
//	while (chr[j] != '\0')
//	{
//		OLED_ShowChar(x, y, chr[j]);
//		x += 8;
//		if (x > 120)
//		{
//			x = 0;
//			y += 2;
//		}
//		j++;
//	}
}

/*********************************************************************
 * @fn		OLED_ShowCHinese
 *
 * @brief	显示汉字
 *
 * @param   
 *
 * @return  none
 */
//static void OLED_ShowCHinese(uint8 x,uint8 y,uint8 no)
//{
//	uint8 t, adder = 0;
//	OLED_Set_Pos(x, y);
//	for (t = 0; t < 16; t++)
//	{
//		write_data(Hzk[2 * no][t]);
//		adder += 1;
//	}
//	OLED_Set_Pos(x, y + 1);
//	for (t = 0; t < 16; t++)
//	{
//		write_data(Hzk[2 * no + 1][t]);
//		adder += 1;
//	}
//}

/*********************************************************************
 * @fn		OLED_DrawBMP
 *
 * @brief	显示显示BMP图片128×64
 *
 * @param   起始点坐标(x,y),x的范围0～127，y为页的范围0～7
 *
 * @return  none
 */
//static void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
//{
//	unsigned int j = 0;
//	unsigned char x, y;
//	
//	if (y1 % 8 == 0)
//	{
//		y = y1 / 8;
//	}
//	else
//	{
//		y = y1 / 8 + 1;
//	}
//	
//	for (y = y0; y < y1; y++)
//	{
//		OLED_Set_Pos(x0, y);
//		for (x = x0; x < x1; x++)
//		{
//			write_data(BMP[j++]);
//		}
//	}
//}

/*********************************************************************
 * @fn		HexDigitDis
 *
 * @brief	
 *
 * @param   
 *
 * @return  none
 */
//static void HexDigitDis(uint8 x,uint8 y,uint8 value)
//{
//	uint8 vtTemp;
//	vtTemp=(value>>4)&0x0F;
//	if(vtTemp<=9)
//	{
//		OLED_ShowChar(x, y,vtTemp+'0');
//	}
//	else
//	{
//		OLED_ShowChar(x, y,vtTemp+'A'-10);
//	}
//	
//	vtTemp=value&0x0F;
//	if(vtTemp<=9)
//	{
//		OLED_ShowChar(x+8, y,vtTemp+'0');
//	}
//	else
//	{
//		OLED_ShowChar(x+8, y,vtTemp+'A'-10);
//	}
//}

/*********************************************************************
 * @fn		oledDisplayChineseString
 *
 * @brief	
 *
 * @param   
 *
 * @return  none
 */
//static void oledDisplayChineseString(uint8 x,uint8 y,const uint8 *str,uint8 len)
//{
//	uint8 i=0;
//	for(;i<len;i++)
//	{
//		OLED_ShowCHinese(x+16*i,y,*(str+i));
//	}
//}

/*********************************************************************
 * @fn      display_Init
 *
 * @brief   Initialization display
 *
 * @param   none
 *
 * @return  none
 */
void display_Init( void ){
//	lcd_bus_init();
//	lcd_config();
//	lcd_clear(BLACK);

	displayParams.command = CCS_LIGHT_MODE_HSI;
	displayParams.mode = 0;
	displayParams.hues=DEFAULT_HUES;
	displayParams.rRate=1.0f;
	displayParams.DisplayModeIndex = IdleIamgeDisplay;
	displayParams.brightness = DEFAULT_BRIGHTNESS;
	displayParams.saturation = DEFAULT_SATURATION;
	displayParams.arrowIndex = DEFAULT_ARROR_INDEX;
	displayParams.vModeIndex = DEFAULT_MODE_INDEX;
	displayParams.colorTemperature = DEFAULT_COLOR_TEMP;
	displayParams.style1Value = DEFAULT_STYLE1_VALUE;
	displayParams.style2Value = DEFAULT_STYLE2_VALUE;
	displayParams.style3Value = DEFAULT_STYLE3_VALUE;
	displayParams.battLv = Max_Batt_level;
	displayParams.countDownTimer = MAX_CD_TIMER;
	displayParams.adcGap = DEFAULT_ADC_GAP;
	displayParams.effectmode = 1;							//	自定义特效模式  1/2:呼吸灯/爆闪
	displayParams.times = 0xff;
	displayParams.freq = 1;
	displayParams.preinstallEffectNo = 0;					//	预设特效编号
	displayParams.customizeEffectMode = 1;					//	自定义特效模式  1/2:呼吸灯/爆闪
	displayParams.customizeEffectTimes = 0xFF;				//	自定义特效循环次数 1~99:次数   100:无限循环
	displayParams.customizeEffectFreq = 0;					//	自定义特效频率
	displayParams.customizeOneShot = 0;					//	自定义特效单次闪烁
	displayParams.fIsEffectMode = 0;						//	自定义特效模式+预设特效模式=1.普通模式=0
	displayParams.fIsFromRGBMode = 0;						//	0/1 :从RGB/色温模式进入自定义特效模式

}
void reInitialLCD(void) {

	lcd_bus_init();
	lcd_config();
	hal_pwrmgr_lock(MOD_LCD_On);
}
/*********************************************************************
 * @fn      lcd_on
 *
 * @brief   true on lcd
 *
 * @param   data
 *
 * @return  none
 */
void lcd_on(void)
{
	lcd_config();
	reInitialLCD();
//	lcd_clear(RED);
//	write_cmd(0xAE); /*display off*/
//	write_cmd(0x8d); /*set charge pump disable*/
//	write_cmd(0x14);		//使用内置DC
//	write_cmd(0xAF); /*display ON*/
	
	WaitMs(1);
}

/*********************************************************************
 * @fn      lcd_off
 *
 * @brief   true off lcd
 *
 * @param   data
 *
 * @return  none
 */
void lcd_off(void) {
//	write_cmd(0xAE); /*display off*/
//	write_cmd(0x8d); /*set charge pump disable*/
//	write_cmd(0x10);		//使用外置DC
//	WaitMs(1);
	lcdEntrySleep();
	fIsChargeDisplaying = false;
	lcdBusDeinit();
	hal_pwrmgr_unlock(MOD_LCD_On);
}

/*********************************************************************
 * @fn      lcd_clear
 *
 * @brief   true off lcd
 *
 * @param   data
 *
 * @return  none
 **********************************************************************/
void lcd_clear(u16 Color) {
	fIsChargeDisplaying = false;
	fIsBrightnessOffDisplaying = false;
	u16 i, j;
//	Address_set(0, 0, LCD_W - 1, LCD_H - 1);
//	SET_CS_LOW;
	lcdSetRegion(0, LCD_Y_START, LCD_W - 1, LCD_H - 1);
	for (i = 0; i < LCD_W; i++) {
		for (j = 0; j < LCD_H; j++) {
//			write_data_uint16(Color);
			write_data((Color>>8)&0xFF,1);
			write_data(Color&0xFF,0);
		}
	}
//	SET_CS_HI;
}


uint8 lcdDrawFontGbk16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, char *s) {
	unsigned char i, j;
	unsigned short k, x0;
	uint8 len = 0;
	x0 = x;

	while (*s) {
		/* ASCII character table from 32 to 128 */
		k = *s;
		if (13 == k) {
			x = x0;
			y += 16;
		} else {
			if (k > ' ')
				k -= ' ';
			else
				k = 0;
			lcdSetRegion(x, y, x + 7, y + 15);
			for (i = 0; i < 16; i++) {
				for (j = 0; j < 8; j++) {
					if (asc16[k * 16 + i] & (0x80 >> j)) {
						/* draw a point on the lcd */
						lcdDrawPoint(x + j, y + i, fc,false);
					} else {
						if (fc != bc)
							/* draw a point on the lcd */
							lcdDrawPoint(x + j, y + i, bc,false);
					}
				}
			}
			x += 8;
		}
		s++;
		len++;
	}
	return len;
}


void	 lcdDrawSingleCharGbk16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8 digit){
	if(digit<0x20)
		return;
	char data=digit;
	lcdDrawFontGbk16( x,  y,  fc,  bc, &data) ;
}


void lcdDrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	/* - difference in x's
	 - difference in y's
	 - dx,dy * 2
	 - amount in pixel space to move during drawing
	 - amount in pixel space to move during drawing
	 - the discriminant i.e. error i.e. decision variable
	 - used for looping */
	int dx, dy, dx2, dy2, x_inc, y_inc, error, index;
//	SET_CS_LOW;
	lcd_set_xy(x0, y0);
	/* calculate x distance */
	dx = x1 - x0;
	/* calculate y distance */
	dy = y1 - y0;

	if (dx >= 0) {
		x_inc = 1;
	} else {
		x_inc = -1;
		dx = -dx;
	}

	if (dy >= 0) {
		y_inc = 1;
	} else {
		y_inc = -1;
		dy = -dy;
	}

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy) {
		/* initialize error */
		error = dy2 - dx;
		/* draw the line */
		for (index = 0; index <= dx; index++) {
			lcdDrawPoint(x0, y0, color,true);
			/* test if error has overflowed */
			if (0 <= error) {
				error -= dx2;
				/* move to next line */
				y0 += y_inc;
			}
			/* adjust the error term */
			error += dy2;
			/* move to the next pixel */
			x0 += x_inc;
		}
	} else {
		/* initialize error term */
		error = dx2 - dy;
		/* draw the linedraw the line*/
		for (index = 0; index <= dy; index++) {
			/* set the pixel */
			lcdDrawPoint(x0, y0, color,true);

			/* test if error overflowed */
			if (0 <= error) {
				error -= dy2;
				/* move to next line */
				x0 += x_inc;
			}
			/* adjust the error term */
			error += dx2;

			/* move to the next pixel */
			y0 += y_inc;
		}
	}
//	SET_CS_HI;
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
void lcdRectColorDraw(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t fc, bool roundRect) {
	int ix, iy;
	if (roundRect) {
		lcdDrawLine(x1 + 2, y1, x2 - 3, y1, fc);
		lcdDrawLine(x1 + 1, y1 + 1, x2 - 2, y1 + 1, fc);
		lcdDrawLine(x1 + 2, y2 - 1, x2 - 3, y2 - 1, fc);
		lcdDrawLine(x1 + 1, y2 - 2, x2 - 2, y2 - 2, fc);
		lcdSetRegion(x1, y1 + 2, x2 - 1, y2 - 2 - 1);
		for (ix = x1; ix < x2; ix++) {
			for (iy = y1; iy < y2; iy++)
				/* set the pixel */
				lcdDrawPoint(ix, iy, fc, false);
		}
	} else {
		lcdSetRegion(x1, y1, x2 - 1, y2 - 1);
		for (ix = x1; ix < x2; ix++) {
			for (iy = y1; iy < y2; iy++)
				/* set the pixel */
				lcdDrawPoint(ix, iy, fc, false);
		}
	}

}

//
//	unsigned char x, y;
//	//	fIsChargeDisplaying = false;
//	write_cmd(0x00);
//	write_cmd(0x10); /*set higher column address*/
//
//	for (y = 0; y < 8; y++)
//	{
//		write_cmd(0xB0 + y); /*set page address*/
//		write_cmd(0x00);
//		write_cmd(0x10);
//		for (x = 0; x < 128; x++)
//		{
//			write_data(0x00);
//		}
//		write_data(0x00);
//	}
//}

void logoDisplay(void) {
	lcdDrawWidthTimesHeight(LOGO_START_X , LOGO_START_Y,32 ,32,WHITE, BACKGROUND_COLOR, (uint8 *) logoIconData);
	lcdDrawWidthTimesHeight(LOGO_START_X+32, LOGO_START_Y+8,96,16, WHITE, BACKGROUND_COLOR, (uint8 *) logoData);
	hal_gpio_write(GPIO_LCD_BACKLIGHT, HI_STTS);

	int ori = hal_systick();

	while (hal_ms_intv(ori) < 1500);

	hal_gpio_write(GPIO_LCD_BACKLIGHT, LOW_STTS);
	lcdSetRegion(LOGO_START_X, LOGO_START_Y, LOGO_WIDTH, LOGO_HEITHT);
	uint8 i, j = 0;
	for (i = 0; i < LOGO_HEITHT; i++) {
		for (j = 0; j < LOGO_WIDTH; j++) {
//			write_data_uint16(BLACK);
			write_data((BLACK>>8)&0xFF,1);
			write_data(BLACK&0xFF,0);
		}
	}
}


/*********************************************************************
 * @fn		displaySystemMenu
 *
 * @brief	待机画面显示
 *
 * @param   
 *
 * @return  none
 */
void displaySystemMenu(displayParamsStruct * disParams){
//	updateHotDisplay(false);
//	hal_gpio_write(GPIO_VLCD, HI_STTS);

	if (IdleIamgeDisplay==disParams->DisplayModeIndex ) {
		logoDisplay();
		batterDisplay(disParams->battLv);
		lcdDrawLine(LINE_ADDRESS_X,LINE_ADDRESS_Y,LCD_W - 1,LINE_ADDRESS_Y,YELLOW);
		lcdDrawLine(LINE_ADDRESS_X+4,LINE_ADDRESS_Y+1,LCD_W - 1,LINE_ADDRESS_Y+1,YELLOW);
		lcdDrawLine(LINE_ADDRESS_X+8,LINE_ADDRESS_Y+2,LCD_W - 1,LINE_ADDRESS_Y+2,YELLOW);
		lcdDrawFontGbk16(LEVEL_ADDRESS_X,LEVEL_ADDRESS_Y,CHAR_COLOR,BACKGROUND_COLOR,"Level");
		lcdDrawFontGbk16(TEMP_ADDRESS_X,TEMP_ADDRESS_Y,CHAR_COLOR,BACKGROUND_COLOR,"Te");
		lcdDrawWidthTimesHeight(TEMP_ADDRESS_X+16,TEMP_ADDRESS_Y,16,16,CHAR_COLOR,BACKGROUND_COLOR,(uint8 *)(&special16Data[32]));
		lcdDrawFontGbk16(TEMP_ADDRESS_X+16+13,TEMP_ADDRESS_Y,CHAR_COLOR,BACKGROUND_COLOR,"p.");

		lcdDrawFontGbk16(COLOR_ADDRESS_X,COLOR_ADDRESS_Y,CHAR_COLOR,BACKGROUND_COLOR,"Color");
		lcdDrawFontGbk16(SCENE_ADDRESS_X,SCENE_ADDRESS_Y,CHAR_COLOR,BACKGROUND_COLOR,"Scene");
		lcdDrawFontGbk16(STROBE_ADDRESS_X,STROBE_ADDRESS_Y,CHAR_COLOR,BACKGROUND_COLOR,"Stro");
		lcdDrawFontGbk16(STROBE_ADDRESS_X+33,STROBE_ADDRESS_Y,CHAR_COLOR,BACKGROUND_COLOR,"be");

		lcdRectColorDraw(RED_RECT_START_X,RED_RECT_START_Y,RED_RECT_END_X,RED_RECT_END_Y,RED,true);
		lcdRectColorDraw(GREEN_RECT_START_X,GREEN_RECT_START_Y,GREEN_RECT_END_X,GREEN_RECT_END_Y,GREEN,true);
		lcdRectColorDraw(BLUE_RECT_START_X,BLUE_RECT_START_Y,BLUE_RECT_END_X,BLUE_RECT_END_Y,BLUE,true);
		if(disParams->brightness){
			updateColorTempDisplay(disParams);
			updateHuesDisplay(disParams,true);
			updateSceneDisplay(disParams);
			updateStrobeDisplay(disParams);
		}
		updateBrightnessDisplay(disParams);
		hal_gpio_write(GPIO_LCD_BACKLIGHT, HI_STTS);
	}else if(ChargingAtPowerDown==disParams->DisplayModeIndex){
		LOG("\n system off Charging\n");
		lcd_clear(BLUE);
		batterDisplay(disParams->battLv);
		hal_gpio_write(GPIO_LCD_BACKLIGHT, HI_STTS);
		return;
	}
}


void exitLcdSleep(void) {
	write_cmd(0x11);
	write_cmd(0x29);
}

void lcdEntrySleep(void) {

//	write_cmd(0x01);
	write_cmd(0x28);
	write_cmd(0x10);

}
/*********************************************************************
 * @fn		batteryPercentDisplay
 *
 * @brief	待机画面电池百分比显示
 *
 * @param   
 *
 * @return  none
 */
void batteryPercentDisplay(uint8 level) {
//	if (!level) {
//		OLED_ShowChar(ICON_Percent_X, ICON_Percent_Y, ' ');
//		OLED_ShowChar(ICON_Percent_X + 8, ICON_Percent_Y, ' ');
//		OLED_ShowChar(ICON_Percent_X + 16, ICON_Percent_Y, ' ');
//		OLED_ShowChar(ICON_Percent_X + 24, ICON_Percent_Y, ' ');
//		OLED_ShowChar(ICON_Percent_X + 32, ICON_Percent_Y, ' ');
//	} else {

//		if (level != 100) {
//			OLED_ShowChar(ICON_Percent_X, ICON_Percent_Y, ' ');
//			OLED_ShowChar(ICON_Percent_X + 8, ICON_Percent_Y, level/10 + '0');
//		} else {
//			OLED_ShowChar(ICON_Percent_X, ICON_Percent_Y, '1');
//			OLED_ShowChar(ICON_Percent_X + 8, ICON_Percent_Y, '0');
//		}
//		OLED_ShowChar(ICON_Percent_X + 16, ICON_Percent_Y, level % 10 + '0');
//		OLED_ShowCHinese(ICON_Percent_X + 24, ICON_Percent_Y, Value_Percent_Addr);
//	}
}

/*****************************************************************************************
 *
 *剩余时间显示
 *
*/
void updateRemainingTimeByValue(uint16 data)
{
//	OLED_ShowChar(ValueOfClock_X, Icon_Clock_Y, ' ');
//	OLED_ShowChar(ValueOfClock_X + 8, Icon_Clock_Y, ' ');
//	OLED_ShowChar(ValueOfClock_X + 16, Icon_Clock_Y, ' ');
//	OLED_ShowChar(ValueOfClock_X + 24, Icon_Clock_Y, ' ');
//	
//	displayParams.remainingTime=data;
//	if ( data < 10 )
//	{
//		OLED_ShowNum(ValueOfClock_X,Icon_Clock_Y, data);
//		OLED_ShowChar(ValueOfClock_X+8,Icon_Clock_Y,LOW_CASE_m);
//	}
//	else if(data<60)
//	{
//		OLED_ShowNum(ValueOfClock_X,Icon_Clock_Y,data/10);
//		OLED_ShowNum(ValueOfClock_X+8,Icon_Clock_Y, data%10);
//		OLED_ShowChar(ValueOfClock_X+16,Icon_Clock_Y,LOW_CASE_m);
//	}
//	else if(data<600)	//<10h
//	{
//		OLED_ShowNum(ValueOfClock_X,Icon_Clock_Y,data/60);
//		OLED_ShowChar(ValueOfClock_X+8, Icon_Clock_Y, '.');
//		OLED_ShowNum(ValueOfClock_X+12,Icon_Clock_Y,data*10/60%10);
//		OLED_ShowChar(ValueOfClock_X+20,Icon_Clock_Y,LOW_CASE_h);
//	}
//	else if(data<6000)	//<100h
//	{
//		OLED_ShowNum(ValueOfClock_X,Icon_Clock_Y,data/60);
//		OLED_ShowChar(ValueOfClock_X+16,Icon_Clock_Y,LOW_CASE_h);
//	}
//	else if(data<59941)	//<999
//	{
//		OLED_ShowNum(ValueOfClock_X,Icon_Clock_Y,data/6000);
//		OLED_ShowNum(ValueOfClock_X+8,Icon_Clock_Y,data/600%10);
//		OLED_ShowNum(ValueOfClock_X+16,Icon_Clock_Y,data/60%10);
//		OLED_ShowChar(ValueOfClock_X+24,Icon_Clock_Y,LOW_CASE_h);
//	}
//	else
//	{
//		OLED_ShowNum(ValueOfClock_X,Icon_Clock_Y,9);
//		OLED_ShowNum(ValueOfClock_X+8,Icon_Clock_Y,9);
//		OLED_ShowNum(ValueOfClock_X+16,Icon_Clock_Y,9);
//		OLED_ShowChar(ValueOfClock_X+24,Icon_Clock_Y,LOW_CASE_h);
//	}
}

/*********************************************************************
 * @fn		batterDisplay
 *
 * @brief	电池显示
 *
 * @param   
 *
 * @return  none
 */
void batterDisplay(uint8 level){
	if (level > Max_Batt_level) {
		level = Max_Batt_level;
	}

	lcdDrawLine(BATT_BORDER_START_X+1,BATT_BORDER_START_Y,BATT_BORDER_END_X-1,BATT_BORDER_START_Y,BATTERY_COLOR);
//	lcdDrawLine(BATT_BORDER_START_X,BATT_BORDER_START_Y+1,BATT_BORDER_END_X,BATT_BORDER_START_Y+1,BATTERY_COLOR);
	lcdDrawLine(BATT_BORDER_START_X,BATT_BORDER_START_Y+1,BATT_BORDER_START_X,BATT_BORDER_END_Y-1,BATTERY_COLOR);
//	lcdDrawLine(BATT_BORDER_START_X+1,BATT_BORDER_START_Y+2,BATT_BORDER_START_X+1,BATT_BORDER_END_Y-2,BATTERY_COLOR);
//	lcdDrawLine(BATT_BORDER_START_X,BATT_BORDER_END_Y-1,BATT_BORDER_END_X,BATT_BORDER_END_Y-1,BATTERY_COLOR);
	lcdDrawLine(BATT_BORDER_START_X+1,BATT_BORDER_END_Y,BATT_BORDER_END_X-1,BATT_BORDER_END_Y,BATTERY_COLOR);
	lcdDrawLine(BATT_BORDER_END_X,BATT_BORDER_START_Y+1,BATT_BORDER_END_X,BATT_BORDER_END_Y-1,BATTERY_COLOR);
	lcdDrawLine(BATT_BORDER_END_X+2,BATT_BORDER_START_Y+4,BATT_BORDER_END_X+2,BATT_BORDER_END_Y-4,BATTERY_COLOR);
	lcdDrawLine(BATT_BORDER_END_X+3,BATT_BORDER_START_Y+4,BATT_BORDER_END_X+3,BATT_BORDER_END_Y-4,BATTERY_COLOR);

	if(level>0){
		lcdRectColorDraw(BATT_BORDER_START_X+2,BATT_BORDER_START_Y+2,BATT_BORDER_START_X+2+BATT_SINGLE_LV_WIDTH,BATT_BORDER_END_Y-1,BATTERY_COLOR,false);
	}else{
		lcdRectColorDraw(BATT_BORDER_START_X+2,BATT_BORDER_START_Y+2,BATT_BORDER_START_X+2+BATT_SINGLE_LV_WIDTH,BATT_BORDER_END_Y-1,BACKGROUND_COLOR,false);
	}

	if(level>1){
		lcdRectColorDraw(BATT_BORDER_START_X+3+BATT_SINGLE_LV_WIDTH,BATT_BORDER_START_Y+2,BATT_BORDER_START_X+3+2*BATT_SINGLE_LV_WIDTH,BATT_BORDER_END_Y-1,BATTERY_COLOR,false);
	}else{
		lcdRectColorDraw(BATT_BORDER_START_X+3+BATT_SINGLE_LV_WIDTH,BATT_BORDER_START_Y+2,BATT_BORDER_START_X+3+2*BATT_SINGLE_LV_WIDTH,BATT_BORDER_END_Y-1,BACKGROUND_COLOR,false);
	}

	if(level>2){
		lcdRectColorDraw(BATT_BORDER_START_X+4+2*BATT_SINGLE_LV_WIDTH,BATT_BORDER_START_Y+2,BATT_BORDER_START_X+4+3*BATT_SINGLE_LV_WIDTH,BATT_BORDER_END_Y-1,BATTERY_COLOR,false);
	}else{
		lcdRectColorDraw(BATT_BORDER_START_X+4+2*BATT_SINGLE_LV_WIDTH,BATT_BORDER_START_Y+2,BATT_BORDER_START_X+4+3*BATT_SINGLE_LV_WIDTH,BATT_BORDER_END_Y-1,BACKGROUND_COLOR,false);
	}

	if(level==Max_Batt_level){
		lcdRectColorDraw(BATT_BORDER_START_X+5+3*BATT_SINGLE_LV_WIDTH,BATT_BORDER_START_Y+2,BATT_BORDER_START_X+5+4*BATT_SINGLE_LV_WIDTH,BATT_BORDER_END_Y-1,BATTERY_COLOR,false);
	}else{
		lcdRectColorDraw(BATT_BORDER_START_X+5+3*BATT_SINGLE_LV_WIDTH,BATT_BORDER_START_Y+2,BATT_BORDER_START_X+5+4*BATT_SINGLE_LV_WIDTH,BATT_BORDER_END_Y-1,BACKGROUND_COLOR,false);
	}

//	lcdDrawSingleCharGbk16(100,4,CHAR_COLOR,BACKGROUND_COLOR,level+'0');
	return;
//	uint8	vtBattIconTableAddr = 0;
//
//	if (level > Max_Batt_level)
//	{
//		level = Max_Batt_level;
//	}
//
//	if( CCS_GET_HOTDISPLAYSTATUS() == false )
//	{
//		switch (level)
//		{
//			default:
//			case 0:
//			vtBattIconTableAddr=ICON_batt_lv0_Addr;
//			break;
//
//			case 1:
//			vtBattIconTableAddr=ICON_batt_lv1_Addr;
//			break;
//
//			case 2:
//			vtBattIconTableAddr=ICON_batt_lv2_Addr;
//			break;
//
//			case 3:
//			vtBattIconTableAddr=ICON_batt_lv3_Addr;
//			break;
//
//			case 4:
//			vtBattIconTableAddr=ICON_batt_lv4_Addr;
//			break;
//		}
//
//		OLED_ShowCHinese(ICON_batt_X0, ICON_batt_Y, vtBattIconTableAddr);
//		OLED_ShowCHinese(ICON_batt_X1, ICON_batt_Y, vtBattIconTableAddr + 1);
//	}
//
//	if( CCS_GET_ChargeStatus() )
//	{
//		if ( !fIsChargeDisplaying )
//		{
//			fIsChargeDisplaying = true;
////			OLED_ShowCHinese(ICON_charge_X0, ICON_charge_Y, ICON_charge_TableAddr);
//			OLED_ShowCHinese(ICON_charge_X0, ICON_charge_Y, ICON_charge_TableAddr);
//			OLED_ShowCHinese(ICON_charge_X0+16, ICON_charge_Y, ICON_charge_TableAddr+1);
//		}
//	}
//	else// if(!fIsCharging&&fIsChargeDisplaying)
//	{
////		if(!fIsSystemOff)
//		if ( fIsChargeDisplaying )
//		{						//关机充电指示由再次进入睡眠来完成,防止图标闪烁
//			fIsChargeDisplaying = false;
////			OLED_ShowCHinese(ICON_charge_X0, ICON_charge_Y, CHINESE_SPACE);
//			OLED_ShowCHinese(ICON_charge_X0, ICON_charge_Y, CHINESE_SPACE);
//			OLED_ShowCHinese(ICON_charge_X0+16, ICON_charge_Y, CHINESE_SPACE);
//		}
//	}
//
//	if ( CCS_GET_BLE_Status() )
//	{
//		updateBLEDisplay(true);
//	}
}

/*********************************************************************
 * @fn		updateArrowDisplay
 *
 * @brief	光标显示
 *
 * @param   
 *
 * @return  none
 */
void updateArrowDisplay(displayParamsStruct * disParams) {

	if (fIsInvalidMacAddr) {
		lcd_clear(BLACK);
		lcdDrawFontGbk16(50,100,CHAR_COLOR,BACKGROUND_COLOR,"MAC ERROR");
		lcdDrawFontGbk16(70,50,CHAR_COLOR,BACKGROUND_COLOR,(char *)macAscii);
	} else {

		for(uint8 i=0;i<5;i++){
			if(i==disParams->arrowIndex)
				lcdDrawSingleCharGbk16(arrowArray[i][0],arrowArray[i][1],CHAR_COLOR,BACKGROUND_COLOR,'>');
			else
				lcdDrawSingleCharGbk16(arrowArray[i][0],arrowArray[i][1],CHAR_COLOR,BACKGROUND_COLOR,' ');
		}
	}
}

/*********************************************************************
 * @fn		updateBLEDisplay
 *
 * @brief	BLE显示
 *
 * @param   
 *
 * @return  none
 */
void updateBLEDisplay(bool flag){
//	if ( flag )
//	{
//		OLED_ShowCHinese(ICON_BLE_X, ICON_BLE_Y, ICON_BLE_ADDRESS);
//	}
//	else
//	{
//		OLED_ShowString(ICON_BLE_X, ICON_BLE_Y, "  ");
//	}
}

/*********************************************************************
 * @fn		updateSceneDisplay
 *
 * @brief		特效模式显示
 *
 * @param
 *
 * @return  none
 */
void updateSceneDisplay(displayParamsStruct * disParams) {
	uint16 fontColor = CHAR_COLOR;
	if (!disParams->brightness)
		fontColor = OFF_CHAR_COLOR;
	if (disParams->preinstallEffectNo > MAX_SCENE_INDEX)
		return;
	lcdDrawFontGbk16(VALUE_SCENE_X, SCENE_ADDRESS_Y, fontColor, BACKGROUND_COLOR, (char *) scene[disParams->preinstallEffectNo]);

}
/***********************************************************************************************************
  *  @brief 				updateStrobeDisplay
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void updateStrobeDisplay(displayParamsStruct * disParams) {
	uint16 fontColor = CHAR_COLOR;
	if (!disParams->brightness)
		fontColor = OFF_CHAR_COLOR;
	if (disParams->customizeEffectFreq > 10)
		return;
	if (disParams->customizeEffectFreq == 0) {
		lcdDrawFontGbk16(VALUE_STROBE_X, STROBE_ADDRESS_Y, fontColor, BACKGROUND_COLOR, "OFF ");
	} else {
		memset(charArray,0,sizeof(charArray));
		sprintf(charArray, "%dHz ", disParams->customizeEffectFreq);
		lcdDrawFontGbk16(VALUE_STROBE_X, STROBE_ADDRESS_Y, fontColor, BACKGROUND_COLOR, charArray);
	}
}
/*********************************************************************
 * @fn		updateHuesDisplay
 *
 * @brief	色调值显示
 *
 * @param   
 *
 * @return  none
 */
void updateHuesDisplay(displayParamsStruct * disParams,bool updateRGB) {
	uint16 fontColor = CHAR_COLOR;
	if (!disParams->brightness)
		fontColor = OFF_CHAR_COLOR;
	if (disParams->hues > MAX_Hues) {
		disParams->hues = 0;
	}
	memset(charArray,0,sizeof(charArray));
	sprintf(charArray,"%d",disParams->hues);  
	uint8 len=lcdDrawFontGbk16(VALUE_COLOR_X,VALUE_COLOR_Y,fontColor,BACKGROUND_COLOR,charArray);
	if(len<3){
		lcdDrawSingleCharGbk16(VALUE_COLOR_X+16,VALUE_COLOR_Y,fontColor,BACKGROUND_COLOR,' ');
	}
	if(len<2){
		lcdDrawSingleCharGbk16(VALUE_COLOR_X+8,VALUE_COLOR_Y,fontColor,BACKGROUND_COLOR,' ');
	}
	memset(charArray,0,sizeof(charArray));
	if(updateRGB||disParams->hues<=120||disParams->hues>=240){
		sprintf(charArray,"%3d",(uint8)(disParams->rRate*255));
		lcdDrawFontGbk16(RED_RECT_START_X+4,RED_RECT_START_Y+2,BACKGROUND_COLOR,RED,charArray);
	}
	if(updateRGB||disParams->hues<=240||disParams->hues==MAX_Hues){
		sprintf(charArray,"%3d",(uint8)(disParams->gRate*255));
		lcdDrawFontGbk16(GREEN_RECT_START_X+4,GREEN_RECT_START_Y+2,BACKGROUND_COLOR,GREEN,charArray);
	}
	if(updateRGB||disParams->hues<=360||disParams->hues>=120){
		sprintf(charArray,"%3d",(uint8)(disParams->bRate*255));
		lcdDrawFontGbk16(BLUE_RECT_START_X+4,BLUE_RECT_START_Y+2,BACKGROUND_COLOR,BLUE,charArray);
	}
}
/*********************************************************************
 * @fn		updateSaturationDisplay
 *
 * @brief	饱和度值显示
 *
 * @param   
 *
 * @return  none
 */
//void updateSaturationDisplay(displayParamsStruct * disParams) {
//	uint8 len = 0;
//	OLED_ShowCHinese(Value_Saturation_X + 16 * 3, ICON_HUES_Y, CHINESE_SPACE);
//	len = OLED_ShowChineseNum(Value_Saturation_X, ICON_Saturation_Y, disParams->saturation);
//	if (len < 3)
//		OLED_ShowCHinese(Value_Saturation_X + 16 * 2, ICON_HUES_Y, CHINESE_SPACE);
//	if (len < 2)
//		OLED_ShowCHinese(Value_Saturation_X + 16 * 1, ICON_HUES_Y, CHINESE_SPACE);
//}

//void	 clearSecondLineDisplay(void){
//	for(uint8 i=0;i<7;i++)
//		OLED_ShowCHinese(ICON_HUES_X+16*i, ICON_HUES_Y,CHINESE_SPACE);
//}
/*********************************************************************
 * @fn		updateBrightnessDisplay
 *
 * @brief	亮度值显示
 *
 * @param   
 *
 * @return  none
 */
void updateBrightnessDisplay(displayParamsStruct * disParams) {
	if (disParams->brightness >= MAX_Brightness) {
		disParams->brightness = MAX_Brightness;
	}
	if (disParams->brightness) {
		memset(charArray, 0, sizeof(charArray));
		sprintf(charArray, "%d", disParams->brightness);
		uint8 len = lcdDrawFontGbk16(Value_Brightness_X, Value_Brightness_Y, CHAR_COLOR, BACKGROUND_COLOR, charArray);
		lcdDrawWidthTimesHeight(Value_Brightness_X + len * 8, Value_Brightness_Y, 16, 16, CHAR_COLOR, BACKGROUND_COLOR, (uint8 *) (&special16Data[0]));
		if (len < 4) {
			lcdDrawSingleCharGbk16(Value_Brightness_X + 32 + 5, Value_Brightness_Y, CHAR_COLOR, BACKGROUND_COLOR, ' ');
		}
		if (len < 3) {
			lcdDrawSingleCharGbk16(Value_Brightness_X + 24 + 5, Value_Brightness_Y, CHAR_COLOR, BACKGROUND_COLOR, ' ');
		}
		if (len < 2) {
			lcdDrawSingleCharGbk16(Value_Brightness_X + 16 + 5, Value_Brightness_Y, CHAR_COLOR, BACKGROUND_COLOR, ' ');
		}
		if (fIsBrightnessOffDisplaying) {
			updateColorTempDisplay(disParams);
			updateHuesDisplay(disParams, true);
			updateSceneDisplay(disParams);
			updateStrobeDisplay(disParams);
			fIsBrightnessOffDisplaying = false;
		}
	} else {
		lcdDrawFontGbk16(Value_Brightness_X, Value_Brightness_Y, CHAR_COLOR, BACKGROUND_COLOR, "OFF ");
		if (!fIsBrightnessOffDisplaying) {
			updateColorTempDisplay(disParams);
			updateHuesDisplay(disParams, true);
			updateSceneDisplay(disParams);
			updateStrobeDisplay(disParams);
			fIsBrightnessOffDisplaying = true;
		}
	}
}

/*********************************************************************
 * @fn		updateColorTempDisplay
 *
 * @brief	色温值显示
 *
 * @param   
 *
 * @return  none
 */
void updateColorTempDisplay(displayParamsStruct * disParams) {
	uint16 fontColor = CHAR_COLOR;
	if (!disParams->brightness)
		fontColor = OFF_CHAR_COLOR;
	if (disParams->colorTemperature >= MAX_ColorTemp) {
		disParams->colorTemperature = MAX_ColorTemp;
	}
	memset(charArray, 0, sizeof(charArray));
	sprintf(charArray, "%d00k", disParams->colorTemperature);
	lcdDrawFontGbk16(VALUE_CCT_X, VALUE_CCT_Y, fontColor, BACKGROUND_COLOR, charArray);
}

///*********************************************************************
// * @fn		updateLightEffectDisplay
// *
// * @brief	灯效样式显示
// *
// * @param
// *
// * @return  none
// */
//void updateLightEffectDisplay(displayParamsStruct * disParams)
//{
//	if(PreinstallEffect==disParams->arrowIndex)
//		OLED_ShowCHinese(ICON_Style1_X, ICON_Style1_Y,1+disParams->style1Value);
////	else if(Style2Setting==disParams->arrowIndex)
////		OLED_ShowCHinese(ICON_Style1_X, ICON_Style1_Y,1+disParams->style2Value);
////	else if(Style3Setting==disParams->arrowIndex)
////		OLED_ShowCHinese(ICON_Style1_X, ICON_Style1_Y,1+disParams->style3Value);
//}

/*********************************************************************
 * @fn		updateHotDisplay
 *
 * @brief	过热显示
 *
 * @param   
 *
 * @return  none
 */
void updateHotDisplay(bool flag)
{
//	fIsHotDisplayOn = flag;

////	if(fIsHotNow)
////	{
//		if(fIsHotDisplayOn)
//		{
//			OLED_ShowCHinese(ICON_Hot_X, ICON_Hot_Y,ICON_Hot_Addr);
//			OLED_ShowCHinese(ICON_Hot_X1, ICON_Hot_Y,ICON_Hot_Addr+1);
//			fIsHotDisplayOn=TRUE;
//		}
//		else
//		{
//			OLED_ShowCHinese(ICON_Hot_X, ICON_Hot_Y,ICON_batt_lv0_Addr);
//			OLED_ShowCHinese(ICON_Hot_X1, ICON_Hot_Y,ICON_batt_lv0_Addr+1);
//			fIsHotDisplayOn=FALSE;
//		}
////	}
}

/*********************************************************************
 * @fn		temperatureDisplay
 *
 * @brief	测试显示
 *
 * @param   
 *
 * @return  none
 */
void temperatureDisplay(uint8 x, uint8 y, uint32 value) {
//	//return;
//	uint8 vtTemperature_X = 0;
//	uint8 vtTemperature_Y = 0;
//	uint8 vtVolt_X = 0;
//	uint8 vtVolt_Y = 0;
//	uint8 vtCVolt_X = 0;
//	uint8 vtCVolt_Y = 0;
//	if (fIsSystemTempGot)
//	{
//		fIsSystemTempGot = 0;
//		if (IdleIamgeDisplay == displayParams.DisplayModeIndex)
//		{
//			vtTemperature_X = 52;
//			vtTemperature_Y = 0;
//			vtVolt_X = 52;
//			vtVolt_Y = 2;
//			vtCVolt_X =vtVolt_X+8 ;
//			vtCVolt_Y = 4;
//		}
//		else
//		{
//			vtTemperature_X = 8;
//			vtTemperature_Y = 2;
//			vtVolt_X = 80;
//			vtVolt_Y = 2;
//			vtCVolt_X =vtVolt_X ;
//			vtCVolt_Y = 4;
//		}
//		OLED_ShowChar(vtTemperature_X, vtTemperature_Y, vSystemTemperature / 10 + '0');
//		OLED_ShowChar(vtTemperature_X+8, vtTemperature_Y, vSystemTemperature % 10 + '0');
//		OLED_ShowChar(vtTemperature_X + 16, vtTemperature_Y, ICON_Degree_ADDRESS);
//		//---------------------------------------------------------------------
//		OLED_ShowChar(vtVolt_X, vtVolt_Y, ((uint8) vTestBatt) + '0');
//		OLED_ShowChar(vtVolt_X + 8, vtVolt_Y, '.');
//		OLED_ShowChar(vtVolt_X + 13, vtVolt_Y, ((uint8) (vTestBatt * 10)) % 10 + '0');
//		OLED_ShowChar(vtVolt_X + 21, vtVolt_Y, ((uint16) (vTestBatt * 100)) % 10 + '0');
//		OLED_ShowChar(vtVolt_X + 29, vtVolt_Y, 'V');
//		//---------------------------------------------------------------------
//		OLED_ShowChar(vtCVolt_X, vtCVolt_Y, ((uint8) vTestCompBatt) + '0');
//		OLED_ShowChar(vtCVolt_X + 8, vtCVolt_Y, '.');
//		OLED_ShowChar(vtCVolt_X + 13, vtCVolt_Y, ((uint8) (vTestCompBatt * 10)) % 10 + '0');
//		OLED_ShowChar(vtCVolt_X + 21, vtCVolt_Y, ((uint16) (vTestCompBatt * 100)) % 10 + '0');
//		//---------------------------------------------------------------------
//		OLED_ShowChar(vtCVolt_X, vtCVolt_Y+2, ((uint8) vSystemVdd) + '0');
//		OLED_ShowChar(vtCVolt_X + 8, vtCVolt_Y+2, '.');
//		OLED_ShowChar(vtCVolt_X + 13, vtCVolt_Y+2, ((uint8) (vSystemVdd * 10)) % 10 + '0');
//		OLED_ShowChar(vtCVolt_X + 21, vtCVolt_Y+2, ((uint16) (vSystemVdd * 100)) % 10 + '0');
//	}
//	
//	OLED_ShowString(x, y,"    ");
//	OLED_ShowChineseNum(x, y, value);
}

void temperatureDisplay_char(uint8 x, uint8 y, uint8 value) {
//	OLED_ShowString(x,y,"    ");
//	OLED_ShowCHinese(x, y , value);
}

/************************************************************************
 *
 * 浮点小数显示
 *	x,y:起始坐标
 *	size: 小数点后显示几位
 *	per:显示单位
 */
void displayFloat(UINT8 x,UINT8 y,float fNum,UINT8 size,UINT8 per){
//	UINT16 vtInt=0;
//	UINT8 len=0,dotPos=0,i=0;
//	vtInt=(UINT16)fNum;
////	OLED_ShowString(x,y,"    ");
//	len=OLED_ShowNum(x,y,vtInt);
//	dotPos=x+8*len;
//	if(size){
//		OLED_ShowChar(dotPos, y, '.');
//		for(i=1;i<=size;i++){
//			OLED_ShowChar(dotPos+5+(i-1)*8, y, ((u16) (fNum * oled_pow(10,i))) % 10 + '0');
//		}
//		OLED_ShowChar(dotPos +5+(size)*8, y, per);
//	}else{
//		OLED_ShowChar(dotPos , y, per);
//		OLED_ShowChar(dotPos+8 , y, ' ');
//	}
}
void displayFactoryInfo(u8 item) {
//	switch (item) {
//	case NonError:
//		break;
//	case TemperatureError:
//		oledDisplayChineseString(32, 2,TEMP_ERROR , 4);
//		break;
//	case VoltageError:
//		lcd_clear(RED);
//		oledDisplayChineseString(32, 4, VOLT_ERROR, 4);
//		break;
//	case ClearAllInfo:
//		lcd_clear(RED);
//		break;
//	}
}
//

void lcdDrawWidthTimesHeight(uint16_t x, uint16_t y, uint16 width, uint16 heitht, uint16_t fc, uint16_t bc, uint8 * data) {
	u16 i, j;
	uint8 displayData = 0;
	uint8 remainer = 0;
	lcdSetRegion(x, y, x + width - 1, y + heitht - 1);
	for (i = 0; i < heitht; i++) {
		for (j = 0; j < width; j++) {
			remainer = j % 8;
			if (!remainer) {
				displayData = *(data++);
			}
			if (displayData & (0x80 >> remainer)) {
				/* draw a point on the lcd */
				lcdDrawPoint(x + j, y + i, fc, false);
			} else {
				if (fc != bc)
					/* draw a point on the lcd */
					lcdDrawPoint(x + j, y + i, bc, false);
			}
		}
	}

}

/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
