/*
 * user_utils.c
 *
 *  Created on: 2021年5月13日
 *      Author: Sky
 */

#include "user_utils.h"
#include "systems_parameters.h"
#include <string.h>
#include <stdlib.h>
//#include <assert.h>

//基于查表实现BCD与Ascii之间的转换
static uint8 bcd2ascii[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
static uint8 ascii2bcd1[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
static uint8 ascii2bcd2[6] = { 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };

uint32 ASC2BCD(uint8 *bcd, const uint8 *asc, uint32 len) {
	uint8 c = 0;
	uint8 index = 0;
	uint8 i = 0;

	len >>= 1;

	for (; i < len; i++) {
		//first BCD
		if (*asc >= 'A' && *asc <= 'F') {
			index = *asc - 'A';
			c = ascii2bcd2[index] << 4;
		} else if (*asc >= '0' && *asc <= '9') {
			index = *asc - '0';
			c = ascii2bcd1[index] << 4;
		}
		asc++;

		//second BCD
		if (*asc >= 'A' && *asc <= 'F') {
			index = *asc - 'A';
			c |= ascii2bcd2[index];
		} else if (*asc >= '0' && *asc <= '9') {
			index = *asc - '0';
			c |= ascii2bcd1[index];
		}
		asc++;

		*bcd++ = c;
	}

	return 0;
}

uint32 BCD2ASC(uint8 *asc, const uint8 *bcd, uint32 len) {
	uint8 c = 0;
	uint8 i;

	for (i = 0; i < len; i++) {
		//first BCD
		c = *bcd >> 4;
		*asc++ = bcd2ascii[c];

		//second
		c = *bcd & 0x0f;
		*asc++ = bcd2ascii[c];
		bcd++;
	}

	return 0;
}
/***********************************************************************************************************
  *  @brief
  *
  *
  *  @param [in] :
  *
  *  @param [out] :		0/1 buff中所有数据均不为/均为  oriByte
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/

uint8 sameByteCheck(uint8 * buff,uint8 len,uint8 oriByte){
	for(uint8  i=0;i<len;i++){
		if(buff[i]!=oriByte)
			return 0;
	}
	return 1;
}
/***********************************************************************************************************
  *  @brief
  *
  *
  *  @param [in] :
  *
  *  @param [out] :			0/1  valid MAC/invalid MAC
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
uint8 invalidMacAddrCheck(void){
	uint8 * mac=(uint8 *)malloc(6);
	getSystemId(mac);
	if(sameByteCheck(mac,6,0xff)||sameByteCheck(mac,6,0x00)){
		BCD2ASC(macAscii,mac,6);
		LOG("%s",macAscii);
		free(mac);
		return 1;
	}else{
		free(mac);
		return 0;
	}
}

