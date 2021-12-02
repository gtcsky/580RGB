/*
 * user_utils.h
 *
 *  Created on: 2021年5月13日
 *      Author: Sky
 */

#ifndef BG571_USER_INC_USER_UTILS_H_
#define BG571_USER_INC_USER_UTILS_H_
#include "protocol.h"

uint32 BCD2ASC(uint8 *asc, const uint8 *bcd, uint32 len);
uint32 ASC2BCD(uint8 *bcd, const uint8 *asc, uint32 len);
uint8 invalidMacAddrCheck(void);
#endif /* BG571_USER_INC_USER_UTILS_H_ */
