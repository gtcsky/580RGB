/**************************************************************************************************
Filename:       LumeCubeprofile.c
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

/*********************************************************************
* INCLUDES
*/
#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"
#include "LumeCubeprofile.h"

/*********************************************************************
* MACROS
*/

/*********************************************************************
* CONSTANTS
*/

#define SERVAPP_NUM_ATTR_SUPPORTED        5

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* GLOBAL VARIABLES
*/
// LumeCube GATT Profile Service UUID: 0xFFF0
CONST uint8 lumecubeProfileServUUID[ATT_UUID_SIZE] =
{
	BEL_LUMECUBE_SERVICES//TI_UUID(LUMECUBEROFILE_SERV_UUID)
};

// Characteristic 1 UUID: 0xFFF1
CONST uint8 lumecubeProfilechar1UUID[ATT_UUID_SIZE] =
{
	BEL_LUMECUBE_CHARACTERISTIC//TI_UUID(LUMECUBEROFILE_CHAR1_UUID)
};

/*********************************************************************
* EXTERNAL VARIABLES
*/

/*********************************************************************
* EXTERNAL FUNCTIONS
*/

/*********************************************************************
* LOCAL VARIABLES
*/
static lumecubeProfileCBs_t *lumecubeProfile_AppCBs = NULL;
static bool Notifyredied_f = false;

/*********************************************************************
* Profile Attributes - variables
*/

// LumeCube Profile Service attribute
static CONST gattAttrType_t lumecubeProfileService = { UUID_SIZE, lumecubeProfileServUUID };


// LumeCube Profile Characteristic 1 Properties
static uint8 lumecubeProfileChar1Props = GATT_PROP_READ | GATT_PROP_WRITE | GATT_PROP_WRITE_NO_RSP | GATT_PROP_NOTIFY;

// Characteristic 1 Value
static uint8 lumecubeProfileChar1[MAX_LUMECUBEROFILE_CHAR_LEN] = {0};

// LumeCube Profile Characteristic 1 Configuration Each client has its own
// instantiation of the Client Characteristic Configuration. Reads of the
// Client Characteristic Configuration only shows the configuration for
// that client and writes only affect the configuration of that client.
static gattCharCfg_t lumecubeProfileChar1Config[GATT_MAX_NUM_CONN];

// LumeCube Profile Characteristic 1 User Description
static uint8 lumecubeProfileChar1UserDesp[] = "Device data";

static uint8 char1lenth = 0;

/*********************************************************************
* Profile Attributes - Table
*/

static gattAttribute_t lumecubeProfileAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] =
{
	// 0 LumeCube Profile Service
	{
		{ ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
		GATT_PERMIT_READ,                         /* permissions */
		0,                                        /* handle */
		(uint8 *)&lumecubeProfileService            /* pValue */
	},

    // 1 Characteristic 1 Declaration
    {
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&lumecubeProfileChar1Props
    },

	// 2 Characteristic Value 1
	{
        { UUID_SIZE, lumecubeProfilechar1UUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        lumecubeProfileChar1
	},
	
	// 3 Characteristic 1 configuration
	{
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)lumecubeProfileChar1Config
	},

	// 4 Characteristic 1 User Description
	{
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        lumecubeProfileChar1UserDesp
	},
};


/*********************************************************************
* LOCAL FUNCTIONS
*/
static uint8 lumecubeProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
									  uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t lumecubeProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
										   uint8 *pValue, uint16 len, uint16 offset );

static void lumecubeProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType );


/*********************************************************************
* PROFILE CALLBACKS
*/
// LumeCube Profile Service Callbacks
CONST gattServiceCBs_t lumecubeProfileCBs =
{
	lumecubeProfile_ReadAttrCB,  // Read callback function pointer
	lumecubeProfile_WriteAttrCB, // Write callback function pointer
	NULL                       // Authorization callback function pointer
};

/*********************************************************************
* PUBLIC FUNCTIONS
*/

/*********************************************************************
* @fn      LumeCubeProfile_AddService
*
* @brief   Initializes the LumeCube Profile service by registering
*          GATT attributes with the GATT server.
*
* @param   services - services to add. This is a bit map and can
*                     contain more than one service.
*
* @return  Success or Failure
*/
bStatus_t LumeCubeProfile_AddService( uint32 services )
{
	uint8 status = SUCCESS;

	// Initialize Client Characteristic Configuration attributes
	GATTServApp_InitCharCfg( INVALID_CONNHANDLE, lumecubeProfileChar1Config );

	// Register with Link DB to receive link status change callback
	VOID linkDB_Register( lumecubeProfile_HandleConnStatusCB );

	if ( services & LUMECUBEROFILE_SERVICE )
	{
		// Register GATT attribute list and CBs with GATT Server App
		status = GATTServApp_RegisterService( lumecubeProfileAttrTbl,
											 GATT_NUM_ATTRS( lumecubeProfileAttrTbl ),
											 &lumecubeProfileCBs );
	}

	return ( status );
}


/*********************************************************************
* @fn      LumeCubeProfile_RegisterAppCBs
*
* @brief   Registers the application callback function. Only call
*          this function once.
*
* @param   callbacks - pointer to application callbacks.
*
* @return  SUCCESS or bleAlreadyInRequestedMode
*/
bStatus_t LumeCubeProfile_RegisterAppCBs( lumecubeProfileCBs_t *appCallbacks )
{
	if ( appCallbacks )
	{
		lumecubeProfile_AppCBs = appCallbacks;

		return ( SUCCESS );
	}
	else
	{
		return ( bleAlreadyInRequestedMode );
	}
}


/*********************************************************************
* @fn      LumeCubeProfile_SetParameter
*
* @brief   Set a LumeCube Profile parameter.
*
* @param   param - Profile parameter ID
* @param   len - length of data to right
* @param   value - pointer to data to write.  This is dependent on
*          the parameter ID and WILL be cast to the appropriate
*          data type (example: data type of uint16 will be cast to
*          uint16 pointer).
*
* @return  bStatus_t
*/
bStatus_t LumeCubeProfile_SetParameter( uint8 param, uint8 len, void *value )
{
	bStatus_t ret = SUCCESS;

	if ( len > MAX_LUMECUBEROFILE_CHAR_LEN )
	{
		return bleInvalidRange;
	}

	switch ( param )
	{
		case LUMECUBEROFILE_CHAR1:
		char1lenth = len;
		osal_memcpy( lumecubeProfileChar1, value, len );
		
		// See if Notification has been enabled
		GATTServApp_ProcessCharCfg( lumecubeProfileChar1Config, lumecubeProfileChar1, FALSE,
								   lumecubeProfileAttrTbl, GATT_NUM_ATTRS( lumecubeProfileAttrTbl ),
								   INVALID_TASK_ID );
		break;

		default:
		ret = INVALIDPARAMETER;
		break;
	}

	return ( ret );
}

/*********************************************************************
* @fn      LumeCubeProfile_GetParameter
*
* @brief   Get a LumeCube Profile parameter.
*
* @param   param - Profile parameter ID
* @param   value - pointer to data to put.  This is dependent on
*          the parameter ID and WILL be cast to the appropriate
*          data type (example: data type of uint16 will be cast to
*          uint16 pointer).
* @param   len - Profile parameter length
*
* @return  bStatus_t
*/
bStatus_t LumeCubeProfile_GetParameter( uint8 param, void *value, uint8 *len )
{
	bStatus_t ret = SUCCESS;
	switch ( param )
	{
		case LUMECUBEROFILE_CHAR1:
		*len = char1lenth;
		osal_memcpy( value, lumecubeProfileChar1, char1lenth );
		break;

		default:
		ret = INVALIDPARAMETER;
		break;
	}

	return ( ret );
}

/*********************************************************************
* @fn          lumecubeProfile_ReadAttrCB
*
* @brief       Read an attribute.
*
* @param       connHandle - connection message was received on
* @param       pAttr - pointer to attribute
* @param       pValue - pointer to data to be read
* @param       pLen - length of data to be read
* @param       offset - offset of the first octet to be read
* @param       maxLen - maximum length of data to be read
*
* @return      Success or Failure
*/
static uint8 lumecubeProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
									  uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen )
{
	bStatus_t status = SUCCESS;

	// If attribute permissions require authorization to read, return error
	if ( gattPermitAuthorRead( pAttr->permissions ) )
	{
		// Insufficient authorization
		return ( ATT_ERR_INSUFFICIENT_AUTHOR );
	}

	// Make sure it's not a blob operation (no attributes in the profile are long)
	if ( offset > 0 )
	{
		return ( ATT_ERR_ATTR_NOT_LONG );
	}

	if ( pAttr->type.len == UUID_SIZE )
	{
		uint8 uuid[ATT_UUID_SIZE];
		osal_memcpy(uuid, pAttr->type.uuid, ATT_UUID_SIZE);
		if ( osal_memcmp(uuid, lumecubeProfilechar1UUID, ATT_UUID_SIZE) )
		{
			*pLen = char1lenth;
			osal_memcpy( pValue, pAttr->pValue, char1lenth );
			char1lenth = 0;
		}
		else
		{
			// Should never get here!
			*pLen = 0;
			status = ATT_ERR_ATTR_NOT_FOUND;
		}
	}

	return ( status );
}

/*********************************************************************
* @fn      lumecubeProfile_WriteAttrCB
*
* @brief   Validate attribute data prior to a write operation
*
* @param   connHandle - connection message was received on
* @param   pAttr - pointer to attribute
* @param   pValue - pointer to data to be written
* @param   len - length of data
* @param   offset - offset of the first octet to be written
* @param   complete - whether this is the last packet
* @param   oper - whether to validate and/or write attribute value
*
* @return  Success or Failure
*/
static bStatus_t lumecubeProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
										   uint8 *pValue, uint16 len, uint16 offset )
{
	bStatus_t status = SUCCESS;
	uint8 notifyApp = 0xFF;

	// If attribute permissions require authorization to write, return error
	if ( gattPermitAuthorWrite( pAttr->permissions ) )
	{
		// Insufficient authorization
		return ( ATT_ERR_INSUFFICIENT_AUTHOR );
	}

	if ( pAttr->type.len == ATT_BT_UUID_SIZE )
	{
		// 16-bit UUID
		uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
		switch ( uuid )
		{
			case GATT_CLIENT_CHAR_CFG_UUID:
			status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
													offset, GATT_CLIENT_CFG_NOTIFY );
			if ( *(pValue) )
			{
				Notifyredied_f = true;
//				P0_7 = 1;
			}
			else
			{
				Notifyredied_f = false;
//				P0_7 = 0;
			}
			break;

			default:
			// Should never get here! (characteristics 2 and 4 do not have write permissions)
			status = ATT_ERR_ATTR_NOT_FOUND;
			break;
		}
	}
	else
	{
		// 128-bit UUID
		uint8 uuid[ATT_UUID_SIZE];
		osal_memcpy(uuid, pAttr->type.uuid, ATT_UUID_SIZE);
		if ( osal_memcmp(uuid, lumecubeProfilechar1UUID, ATT_UUID_SIZE) )
		{
			if( pAttr->pValue == lumecubeProfileChar1 )
			{
				if ( offset == 0 )
				{
					if ( len > MAX_LUMECUBEROFILE_CHAR_LEN )
					{
						status = ATT_ERR_INVALID_VALUE_SIZE;
					}
				}
				else
				{
					status = ATT_ERR_ATTR_NOT_LONG;
				}

				//Write the value
				if ( status == SUCCESS )
				{
					uint8 *pCurValue = (uint8 *)pAttr->pValue;

					osal_memcpy( pCurValue, pValue, len );
					char1lenth = len;

					notifyApp = LUMECUBEROFILE_CHAR1;
				}
			}
			else
			{
				status = ATT_ERR_ATTR_NOT_FOUND;
			}
		}
	}

	// If a charactersitic value changed then callback function to notify application of change
	if ( (notifyApp != 0xFF ) && lumecubeProfile_AppCBs && lumecubeProfile_AppCBs->pfnLumeCubeProfileChange )
	{
		lumecubeProfile_AppCBs->pfnLumeCubeProfileChange( notifyApp );
	}

	return ( status );
}

/*********************************************************************
* @fn          lumecubeProfile_HandleConnStatusCB
*
* @brief       LumeCube Profile link status change handler function.
*
* @param       connHandle - connection handle
* @param       changeType - type of change
*
* @return      none
*/
static void lumecubeProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType )
{
	// Make sure this is not loopback connection
	if ( connHandle != LOOPBACK_CONNHANDLE )
	{
		// Reset Client Char Config if connection has dropped
		if ( ( changeType == LINKDB_STATUS_UPDATE_REMOVED )      ||
			( ( changeType == LINKDB_STATUS_UPDATE_STATEFLAGS ) &&
			 ( !linkDB_Up( connHandle ) ) ) )
		{
			GATTServApp_InitCharCfg( connHandle, lumecubeProfileChar1Config );
		}
	}
}


/*********************************************************************
* @fn          LumeCubeProfile_GetParaLength
*
* @brief       get the data length
*
* @param       none
*
* @return      none
*/
uint16 LumeCubeProfile_GetParaLength( uint8 param )
{
	uint16 len = 0;
	switch (param)
	{
		case LUMECUBEROFILE_CHAR1:
		len = char1lenth;
		break;
	}

	return len;
}

/*********************************************************************
* @fn      SetNotifyStatus
*
* @brief   set notify status
*
* @param   inf: true of false
*
* @return  none
*/
void LumeCube_SetNotifyStatus( bool inf)
{
	Notifyredied_f = inf;
}

/*********************************************************************
* @fn      GetNotifyStatus
*
* @brief   return notify status
*
* @param   none
*
* @return  Notifyredied_f: true or false.
*/
bool LumeCube_GetNotifyStatus(void)
{
	return( Notifyredied_f );
}

/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
