/**************************************************************************************************
Filename:       devinfoservice.h
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

#ifndef DEVINFOSERVICE_H
#define DEVINFOSERVICE_H

/*********************************************************************
 * INCLUDES
 */
#include "types.h"
#include "bcomdef.h"

/*********************************************************************
 * CONSTANTS
 */

// Device Information Service Parameters
#define DEVINFO_SYSTEM_ID                 0
#define DEVINFO_MODEL_NUMBER              1
#define DEVINFO_SERIAL_NUMBER             2
#define DEVINFO_FIRMWARE_REV              3
#define DEVINFO_HARDWARE_REV              4
#define DEVINFO_SOFTWARE_REV              5
#define DEVINFO_MANUFACTURER_NAME         6
#define DEVINFO_11073_CERT_DATA           7
#define DEVINFO_PNP_ID                    8

// IEEE 11073 authoritative body values
#define DEVINFO_11073_BODY_EMPTY          0
#define DEVINFO_11073_BODY_IEEE           1
#define DEVINFO_11073_BODY_CONTINUA       2
#define DEVINFO_11073_BODY_EXP            254

// System ID length
#define DEVINFO_SYSTEM_ID_LEN             8

  // PnP ID length
#define DEVINFO_PNP_ID_LEN                7

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */


/*********************************************************************
 * API FUNCTIONS
 */

/*
 * DevInfo_AddService- Initializes the Device Information service by registering
 *          GATT attributes with the GATT server.
 *
 */

extern bStatus_t DevInfo_AddService( void );

/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
#endif /* DEVINFOSERVICE_H */
