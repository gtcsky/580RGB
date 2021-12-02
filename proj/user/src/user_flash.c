/*
 * user_flash.c
 *
 *  Created on: 2020年10月27日
 *      Author: Sky
 */

#include	"user_flash.h"
#include	"flash.h"
#include "log.h"
#include "stdlib.h"
#include "string.h"
#include "command_center.h"
extern	displayParamsStruct displayParams;

deviceInfoStruct deviceInfoParams={0};
/***********************************************************
 *
 *
 *    startOffset :地址需要是4的倍数
 *
 */
uint8_t readFlashData(uint16 startOffset, uint16 len, uint8 * pRBuff) {

	return (flash_read_ucds_block_byte((uint32_t) startOffset, (uint32_t) len, pRBuff));

}
/***********************************************************
 *
 *
 *    startOffset :地址需要是4的倍数
 *
 */
uint8_t writeFlashData(uint16 startOffset, uint16 len, uint8 * pWBuff) {
	flash_erase_ucds((uint32_t) startOffset);
	return (flash_write_ucds_block_byte((uint32_t) startOffset, (uint32_t) len, pWBuff));
}
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

userException storeExceptionStts(void) {

	uint8 len = sizeof(displayParamsStruct);
	uint8 * pData = (uint8 *) malloc(len + 2);
	if (NULL == pData) {
		return ACTION_OUT_OF_MEM;
	}
	*pData = CHECK_INFO_DATA;
	memcpy(pData + 1, &displayParams, len);
	*(pData + len + 1) = getSystemStts();
	writeFlashData( CHECK_INFO_ADDR, (uint16) len + 2, pData);
	free(pData);
	return ACTION_SUCCUSS;

}

void	 clearExceptionStts(void){

	flash_erase_ucds((uint32)0x00);

}
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
userException readExceptionStts(void) {

	uint8 len = sizeof(displayParamsStruct);
	uint8 checkInfo=0;
	uint8 * pData = (uint8 *) malloc(len + 2);
	uint8 ret=0xff;
	ret=readFlashData(CHECK_INFO_ADDR,1,&checkInfo);
	if(ret){
		return	ACTION_FAIL;
	}
	if(checkInfo!=CHECK_INFO_DATA)
		return ACTION_SUCCUSS;
	if (NULL == pData) {
		return ACTION_OUT_OF_MEM;
	}
	ret=readFlashData(CHECK_INFO_ADDR,(uint16)(len+2),pData);
	if(ret){
		free(pData);
		return	ACTION_FAIL;
	}
	memcpy(&displayParams,pData+1,len);
	updateSystemStts(*(pData+len+1));
	flash_erase_ucds(CHECK_INFO_ADDR);
	free(pData);
	return ACTION_SUCCUSS;

}

/***********************************************************************************************************
  *  @brief			保存Device Info服务相关的设备信息,用于OTA失败时广播
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/


userException storeDeviceInfo(void) {

	uint8 len = sizeof(deviceInfoParams);
	deviceInfoParams.checkCode = DEVICE_CHECK_INFO_DATA;
	writeFlashData( SYSTEM_INFO_CHECK_INFO_ADDR, (uint16) len , (uint8 *)(&deviceInfoParams));
	return ACTION_SUCCUSS;

}

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
userException readDeviceInfo(void) {

	uint8 len = sizeof(deviceInfoStruct);
	uint8 checkInfo=0;
	uint8 * pData = (uint8 *) malloc(len );
	uint8 ret=0xff;
	ret=readFlashData(SYSTEM_INFO_CHECK_INFO_ADDR,1,&checkInfo);
	if(ret){
		return	ACTION_FAIL;
	}
	if(checkInfo!=DEVICE_CHECK_INFO_DATA)
		return ACTION_FAIL;
	ret=readFlashData(SYSTEM_INFO_CHECK_INFO_ADDR,sizeof(deviceInfoStruct),(uint8 *)&deviceInfoParams);
	if(ret){
		return	ACTION_FAIL;
	}
	return ACTION_SUCCUSS;

}


