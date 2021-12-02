/**************************************************************************************************
Filename:       systems_parameters.h
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

#ifndef SYSTEMS_PARAMETERS_H
#define SYSTEMS_PARAMETERS_H

/*********************************************************************
* INCLUDES
*/
#include "types.h"
#include "bcomdef.h"

/*********************************************************************
* MACROS
*/
#define  SYSPARAM_MAX_ATTR_SIZE						16
#define  SYSPARAM_MAX_LOCALNAME_SIZE				20
#define  SYSPARAM_MAX_DEVICENAME_SIZE				20
#define  SYSPARAM_MAX_ADVERTDATA_SIZE				20
#define  SYSPARAM_MAX_MODELNUMBER_SIZE			8
#define  SYSPARAM_MAX_SERIALNUMBER_SIZE			8
#define  SYSPARAM_MAX_USER_MANUAL_SIZE				18
#define  SYSPARAM_MAX_OTA_FILE_SIZE					18
#define  SYSPARAM_MAX_DEFAULT_SETTING_SIZE			8


#define	USER_MENUAL_MAX_SIZE							17
#define	USER_OTA_NAME_MAX_SIZE						14
/*********************************************************************
* CONSTANTS
*/

/*********************************************************************
* TYPEDEFS
*/

typedef enum
{
	ITEM_SYSTEMID = 0,
	ITEM_LOCALNAME,
	ITEM_DEVICENAME,
	ITEM_ADVERTDATA,
	ITEM_SCANRSPDATA,
	ITEM_INTERVALTIME,
	ITEM_MODELNUMBER,
	ITEM_SERIAL_NUMBER,
	ITEM_FIRMWARE_REV,
	ITEM_HARDWARE_REV,
	ITEM_SOFTWARE_REV,
	ITEM_MANUFACTURER_NAME,
	ITEM_11073_CERT_DATA,
	ITEM_PNP_ID,
	ITEM_USER_MANUAL,
	ITEM_OTA_FILE,
}systems_param_paramID_t;

/*********************************************************************
* VARIABLES
*/
extern uint8  macAscii[12];
extern	uint8 UserManualInfo[SYSPARAM_MAX_USER_MANUAL_SIZE+2];
extern	uint8 OtaFile[SYSPARAM_MAX_OTA_FILE_SIZE+2];
/*********************************************************************
* FUNCTIONS
*/
/**
 * @fn      systems_parameters_Init
 *
 * @brief   Initialization systems parameters
 *
 * @param   none
 *
 * @return  none
 */
extern void systems_parameters_Init( void );

/**
 * @fn      systems_parameters_Get_param
 *
 * @brief   Get systems parameters
 *
 * @param   systems_parameters_t
 *
 * @return  bStatus_t
 */
extern bStatus_t systems_param_Get_param( systems_param_paramID_t paramID, uint8 *len, uint8 *param );

/**
 * @fn      systems_parameters_Set_param
 *
 * @brief   Set systems parameters
 *
 * @param   systems_parameters_t
 *
 * @return  bStatus_t
 */
extern bStatus_t systems_param_Set_param( systems_param_paramID_t paramID, uint8 len, uint8 *param );

/***********************************************************************************************************
  *  @brief  					update model number or local name
  *
  *
  *  @param [in] :		value[0]=package length,value[1]=Type,value[2]=length(only valid data length except  value[0],value[1] and value[2]) ,value[3]++=data
  *					exceptNotify:  TRUE without notify function
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void updateDeviceDefaultInfo(uint8* value,uint8 exceptNotify);

/***********************************************************************************************************
  *  @brief				校验将会被用于OTA 服务的相关信息,如果不匹配,则更新.
  *
  *  @param [in] :
  *
  *  @param [out] :			1:	update over
  *						0:	nothing change
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/;
extern uint8 updateDeviceInfo2Flash(void);

extern void getSystemId(uint8 *id);
/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
#endif // SYSTEMS_PARAMETERS_H
