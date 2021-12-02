/**************************************************************************************************
Filename:       simpleGATTprofile.h
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

#ifndef SIMPLEGATTPROFILE_H
#define SIMPLEGATTPROFILE_H

/*********************************************************************
 * INCLUDES
 */
#include "types.h"
#include "bcomdef.h"
#include "gatt.h"
/*********************************************************************
 * CONSTANTS
 */

// Profile Parameters
#define SIMPLEPROFILE_CHAR1						0  // RW uint8 - Profile Characteristic 1 value 
#define SIMPLEPROFILE_CHAR2						1  // RW uint8 - Profile Characteristic 2 value
#define SIMPLEPROFILE_CHAR3						2  // RW uint8 - Profile Characteristic 3 value
#define SIMPLEPROFILE_CHAR4						3  // RW uint8 - Profile Characteristic 4 value
#define SIMPLEPROFILE_CHAR5						4  // RW uint8 - Profile Characteristic 4 value
#define SIMPLEPROFILE_CHAR6						5  // RW uint8 - Profile Characteristic 4 value

// Simple Profile Service UUID
#define SIMPLEPROFILE_SERV_UUID					0xFFF0

// Key Pressed UUID
#define SIMPLEPROFILE_CHAR1_UUID				0xFFF1
#define SIMPLEPROFILE_CHAR2_UUID				0xFFF2
#define SIMPLEPROFILE_CHAR3_UUID				0xFFF3
#define SIMPLEPROFILE_CHAR4_UUID				0xFFF4
#define SIMPLEPROFILE_CHAR5_UUID				0xFFF5
#define SIMPLEPROFILE_CHAR6_UUID				0xFFF6

// Simple Keys Profile Services bit fields
#define SIMPLEPROFILE_SERVICE					0x00000001

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */

// Callback when a characteristic value has changed
typedef void (*simpleProfileChange_t)(uint8 paramID);

typedef struct
{
	simpleProfileChange_t        pfnSimpleProfileChange;  // Called when characteristic value changes
} simpleProfileCBs_t;



/*********************************************************************
 * API FUNCTIONS
 */


/*
 * SimpleProfile_AddService- Initializes the Simple GATT Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 */

extern bStatus_t SimpleProfile_AddService(uint32 services);

/*
 * SimpleProfile_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
extern bStatus_t SimpleProfile_RegisterAppCBs(simpleProfileCBs_t* appCallbacks);

/*
 * SimpleProfile_SetParameter - Set a Simple GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    len - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t SimpleProfile_SetParameter(uint8 param, uint16 len, void* value);

/*
 * SimpleProfile_GetParameter - Get a Simple GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t SimpleProfile_GetParameter(uint8 param, void* value, uint8 *len);

/**
 * @fn          simpleProfile_Get_notify_status
 *
 * @brief       Get nofify flag status.
 *
 * @param       none
 *
 * @return      bool
 */
extern bool simpleProfile_Get_notify_status( void );

/**
 * @fn          simpleProfile_Get_notify_status
 *
 * @brief       Get nofify flag status.
 *
 * @param       bool
 *
 * @return      none
 */
extern void simpleProfile_Set_notify_status( bool flag );

/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
#endif /* SIMPLEGATTPROFILE_H */
