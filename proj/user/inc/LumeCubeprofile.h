/**************************************************************************************************
Filename:       LumeCubeprofile.h
Revised:        Date: 2020-01-18
Revision:       Revision: 0.0

Description:    This file contains the LumeCube GATT profile definitions and
prototypes.

Copyright 2010 Bough Technology LTD.. All rights reserved.

IMPORTANT: Your use of this Software is limited to those specific rights
granted under the terms of a software license agreement between the user
who downloaded the software, his/her employer (which must be your employer)
and Bough Technology Incorporated (the "License").  You may not use this
Software unless you agree to abide by the terms of the License. The License
limits your use, and you acknowledge, that the Software may not be modified,
copied or distributed unless embedded on a Bough Technology microcontroller
or used solely and exclusively in conjunction with a Bough Technology radio
frequency transceiver, which is integrated into your product.  Other than for
the foregoing purpose, you may not use, reproduce, copy, prepare derivative
works of, modify, distribute, perform, display or sell this Software and/or
its documentation for any purpose.

YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
PROVIDED IS IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
BOUGH TECHNOLOGY OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

Should you have any questions regarding your right to use this Software,
contact Bough Technology Incorporated at www.bough.com.cn
**************************************************************************************************/

#ifndef LUMECUBEPROFILE_H
#define LUMECUBEPROFILE_H

/*********************************************************************
* INCLUDES
*/
#include "types.h"
#include "bcomdef.h"
/*********************************************************************
* CONSTANTS
*/

// Profile Parameters
#define LUMECUBEROFILE_CHAR1                   0  // RW uint8 - Profile Characteristic 1 value

// LumeCube Keys Profile Services bit fields
#define LUMECUBEROFILE_SERVICE               0x00000001

// Max Length of Characteristic in bytes
#define MAX_LUMECUBEROFILE_CHAR_LEN         20

#define UUID_SIZE							16

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* MACROS
*/

//#define TI_UUID(uuid)       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0, 0x00, 0x40, 0x51, 0x04, LO_UINT16(uuid), HI_UINT16(uuid), 0x00, 0xF0
#define BEL_LUMECUBE_SERVICES			0xf0,0x9b,0x46,0x07,0x28,0x02,0x45,0xa5,0xe4,0x11,0x6a,0x48,0x4c,0x6a,0x82,0x33
#define BEL_LUMECUBE_CHARACTERISTIC		0xf0,0x9b,0x46,0x07,0x28,0x02,0x45,0xa5,0xe4,0x11,0x6a,0x48,0x4d,0x6a,0x82,0x33

/*********************************************************************
* Profile Callbacks
*/

// Callback when a characteristic value has changed
typedef NULL_OK void (*lumecubeProfileChange_t)( uint8 paramID );

typedef struct
{
	lumecubeProfileChange_t        pfnLumeCubeProfileChange;  // Called when characteristic value changes
} lumecubeProfileCBs_t;



/*********************************************************************
* API FUNCTIONS
*/


/*
* LumeCubeProfile_AddService- Initializes the LumeCube GATT Profile service by registering
*          GATT attributes with the GATT server.
*
* @param   services - services to add. This is a bit map and can
*                     contain more than one service.
*/

extern bStatus_t LumeCubeProfile_AddService( uint32 services );

/*
* LumeCubeProfile_RegisterAppCBs - Registers the application callback function.
*                    Only call this function once.
*
*    appCallbacks - pointer to application callbacks.
*/
extern bStatus_t LumeCubeProfile_RegisterAppCBs( lumecubeProfileCBs_t *appCallbacks );

/*
* LumeCubeProfile_SetParameter - Set a LumeCube GATT Profile parameter.
*
*    param - Profile parameter ID
*    len - length of data to right
*    value - pointer to data to write.  This is dependent on
*          the parameter ID and WILL be cast to the appropriate
*          data type (example: data type of uint16 will be cast to
*          uint16 pointer).
*/
extern bStatus_t LumeCubeProfile_SetParameter( uint8 param, uint8 len, void *value );

/*
* LumeCubeProfile_GetParameter - Get a LumeCube GATT Profile parameter.
*
*    param - Profile parameter ID
*    value - pointer to data to write.  This is dependent on
*          the parameter ID and WILL be cast to the appropriate
*          data type (example: data type of uint16 will be cast to
*          uint16 pointer).
*/
extern bStatus_t LumeCubeProfile_GetParameter( uint8 param, void *value, uint8 *len );

/*********************************************************************
* @fn          LumeCubeProfile_GetParaLength
*
* @brief       get the data length
*
* @param       none
*
* @return      none
*/
extern uint16 LumeCubeProfile_GetParaLength( uint8 param );


/*
* @fn      SetNotifyStatus
*
* @brief   set notify status
*
* @param   inf: true of false
*
* @return  none
*/
void LumeCube_SetNotifyStatus( bool inf);

/*********************************************************************
* @fn      GetNotifyStatus
*
* @brief   return notify status
*
* @param   none
*
* @return  Notifyredied_f: true or false.
*/
bool LumeCube_GetNotifyStatus(void);

/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
#endif /* SIMPLEGATTPROFILE_H */
