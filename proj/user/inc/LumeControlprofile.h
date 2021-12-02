/**************************************************************************************************
Filename:       LumeControlprofile.h
Revised:        Date: 2020-01-18
Revision:       Revision: 0.0

Description:    This file contains the LumeControl GATT profile definitions and
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

#ifndef LUMECONTROLPROFILE_H
#define LUMECONTROLPROFILE_H

/*********************************************************************
* INCLUDES
*/
#include "types.h"
#include "bcomdef.h"

/*********************************************************************
* CONSTANTS
*/

// Profile Parameters
#define LUMECONTROLROFILE_CHAR1                   0  // RW uint8 - Profile Characteristic 1 value

// LumeControl Keys Profile Services bit fields
#define LUMECONTROLROFILE_SERVICE               0x00000001

// Max Length of Characteristic in bytes
#define MAX_LUMECONTROLROFILE_CHAR_LEN         20

#define UUID_SIZE							16

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* MACROS
*/

//#define TI_UUID(uuid)       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0, 0x00, 0x40, 0x51, 0x04, LO_UINT16(uuid), HI_UINT16(uuid), 0x00, 0xF0
#define BEL_LUMECONTROL_SERVICES	0x50,0xea,0xda,0x30,0x88,0x83,0xb8,0x9f,0x60,0x4f,0x15,0xf3,0x01,0x00,0x40,0x8e
#define BEL_LUMECONTROL_CHARACTERISTIC		0x50,0xea,0xda,0x30,0x88,0x83,0xb8,0x9f,0x60,0x4f,0x15,0xf3,0x01,0x00,0x40,0x8e

/*********************************************************************
* Profile Callbacks
*/

// Callback when a characteristic value has changed
typedef NULL_OK void (*lumecontrolProfileChange_t)( uint8 paramID );

typedef struct
{
	lumecontrolProfileChange_t        pfnLumeControlProfileChange;  // Called when characteristic value changes
} lumecontrolProfileCBs_t;



/*********************************************************************
* API FUNCTIONS
*/


/*
* LumeControlProfile_AddService- Initializes the LumeControl GATT Profile service by registering
*          GATT attributes with the GATT server.
*
* @param   services - services to add. This is a bit map and can
*                     contain more than one service.
*/

extern bStatus_t LumeControlProfile_AddService( uint32 services );

/*
* LumeControlProfile_RegisterAppCBs - Registers the application callback function.
*                    Only call this function once.
*
*    appCallbacks - pointer to application callbacks.
*/
extern bStatus_t LumeControlProfile_RegisterAppCBs( lumecontrolProfileCBs_t *appCallbacks );

/*
* LumeControlProfile_SetParameter - Set a LumeControl GATT Profile parameter.
*
*    param - Profile parameter ID
*    len - length of data to right
*    value - pointer to data to write.  This is dependent on
*          the parameter ID and WILL be cast to the appropriate
*          data type (example: data type of uint16 will be cast to
*          uint16 pointer).
*/
extern bStatus_t LumeControlProfile_SetParameter( uint8 param, uint8 len, void *value );

/*
* LumeControlProfile_GetParameter - Get a LumeControl GATT Profile parameter.
*
*    param - Profile parameter ID
*    value - pointer to data to write.  This is dependent on
*          the parameter ID and WILL be cast to the appropriate
*          data type (example: data type of uint16 will be cast to
*          uint16 pointer).
*/
extern bStatus_t LumeControlProfile_GetParameter( uint8 param, void *value, uint8 *len );

/*********************************************************************
* @fn          LumeControlProfile_GetParaLength
*
* @brief       get the data length
*
* @param       none
*
* @return      none
*/
extern uint16 LumeControlProfile_GetParaLength( uint8 param );


/*
* @fn      SetNotifyStatus
*
* @brief   set notify status
*
* @param   inf: true of false
*
* @return  none
*/
void LumeControl_SetNotifyStatus( bool inf);

/*********************************************************************
* @fn      GetNotifyStatus
*
* @brief   return notify status
*
* @param   none
*
* @return  Notifyredied_f: true or false.
*/
bool LumeControl_GetNotifyStatus(void);

/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
#endif /* SIMPLEGATTPROFILE_H */
