/*
 * user_flash.h
 *
 *  Created on: 2020年10月27日
 *      Author: Sky
 */

#ifndef BG571_USER_INC_USER_FLASH_H_
#define BG571_USER_INC_USER_FLASH_H_
#include "types.h"
#include "protocol.h"
//basic address 0x10005000
uint8_t		readFlashData(uint16 startOffset,uint16 len,uint8 * pRBuff);
uint8_t		writeFlashData(uint16 startOffset,uint16 len,uint8 * pWBuff);

#define		CHECK_INFO_ADDR					0x00000000
#define  		CHECK_INFO_DATA					0x5A
#define		DISPLAY_PARAMS_BACKUP_ADDR		0x0001


#define		SYSTEM_INFO_CHECK_INFO_ADDR		0X00001000
#define		DEVICE_CHECK_INFO_DATA			0xA9
#define		DEVICE_INFO_BACKUP_ADDR			0X00001001


typedef enum{
	ACTION_SUCCUSS=0,
	ACTION_FAIL,
	ACTION_OUT_OF_MEM,
}userException;

extern	deviceInfoStruct deviceInfoParams;

/***********************************************************************************************************
  *  @brief  			读取设备异常重启相关信息
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
userException readExceptionStts(void);

/***********************************************************************************************************
  *  @brief 			存储异常发生时的状态
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
userException storeExceptionStts(void);

/***********************************************************************************************************
  *  @brief			读取Flash中已存储的Device Info相关信息
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
userException readDeviceInfo(void);

/***********************************************************************************************************
  *  @brief			读取Flash中已存储的Device Info相关信息
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
userException storeDeviceInfo(void);
#endif /* BG571_USER_INC_USER_FLASH_H_ */
