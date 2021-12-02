/**************************************************************************************************
Filename:       simpleGATTprofile.c
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
#include "battery.h"
#include "switch.h"
#include "pwm_ctrl.h"
#include "temp.h"

#include "simpleGATTprofile.h"
#include "command_center.h"
#include "systems_parameters.h"
#include "log.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define SERVAPP_NUM_ATTR_SUPPORTED        20

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Simple GATT Profile Service UUID: 0xFFF0
CONST uint8 simpleProfileServUUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(SIMPLEPROFILE_SERV_UUID), HI_UINT16(SIMPLEPROFILE_SERV_UUID)
};

// Characteristic 1 UUID: 0xFFF1
CONST uint8 simpleProfilechar1UUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(SIMPLEPROFILE_CHAR1_UUID), HI_UINT16(SIMPLEPROFILE_CHAR1_UUID)
};

// Characteristic 2 UUID: 0xFFF2
CONST uint8 simpleProfilechar2UUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(SIMPLEPROFILE_CHAR2_UUID), HI_UINT16(SIMPLEPROFILE_CHAR2_UUID)
};

// Characteristic 3 UUID: 0xFFF3
CONST uint8 simpleProfilechar3UUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(SIMPLEPROFILE_CHAR3_UUID), HI_UINT16(SIMPLEPROFILE_CHAR3_UUID)
};

// Characteristic 4 UUID: 0xFFF4
CONST uint8 simpleProfilechar4UUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(SIMPLEPROFILE_CHAR4_UUID), HI_UINT16(SIMPLEPROFILE_CHAR4_UUID)
};

// Characteristic 5 UUID: 0xFFF5
CONST uint8 simpleProfilechar5UUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(SIMPLEPROFILE_CHAR5_UUID), HI_UINT16(SIMPLEPROFILE_CHAR5_UUID)
};

// Characteristic 6 UUID: 0xFFF6
CONST uint8 simpleProfilechar6UUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(SIMPLEPROFILE_CHAR6_UUID), HI_UINT16(SIMPLEPROFILE_CHAR6_UUID)
};

/*********************************************************************
 * VARIABLES
 */

static simpleProfileCBs_t* simpleProfile_AppCBs = NULL;
static bool Notifyedied_f = false;

/*********************************************************************
 * Profile Attributes - variables
 */

// Simple Profile Service attribute
static CONST gattAttrType_t simpleProfileService = { ATT_BT_UUID_SIZE, simpleProfileServUUID };

// Simple Profile Characteristic 1 Properties
static uint8 simpleProfileChar1Props = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic 1 Value
static uint8 simpleProfileChar1[GATT_MAX_ATTR_SIZE] = {0};

// Simple Profile Characteristic 1 User Description
static uint8 simpleProfileChar1UserDesp[] = "System Setting";

// Simple Profile Characteristic 2 Properties
static uint8 simpleProfileChar2Props = GATT_PROP_READ;

// Characteristic 2 Value
static uint8 simpleProfileChar2[GATT_MAX_ATTR_SIZE] = {0};

// Simple Profile Characteristic 2 User Description
static uint8 simpleProfileChar2UserDesp[] = "System Setting";

// Simple Profile Characteristic 3 Properties
static uint8 simpleProfileChar3Props = GATT_PROP_READ | GATT_PROP_WRITE | GATT_PROP_WRITE_NO_RSP;

// Characteristic 3 Value
static uint8 simpleProfileChar3[GATT_MAX_ATTR_SIZE] = {0};

// Simple Profile Characteristic 3 User Description
static uint8 simpleProfileChar3UserDesp[] = "Function Control";

// Simple Profile Characteristic 4 Properties
static uint8 simpleProfileChar4Props = GATT_PROP_READ | GATT_PROP_NOTIFY;

// Characteristic 4 Value
static uint8 simpleProfileChar4[GATT_MAX_ATTR_SIZE] = {0};

// Simple Profile Characteristic 4 User Description
static uint8 simpleProfileChar4UserDesp[] = "Measurement";

// Simple Profile Characteristic 4 Configuration Each client has its own
// instantiation of the Client Characteristic Configuration. Reads of the
// Client Characteristic Configuration only shows the configuration for
// that client and writes only affect the configuration of that client.
gattCharCfg_t simpleProfileChar4Config[GATT_MAX_NUM_CONN];

// Simple Profile Characteristic 5 Properties
static uint8 simpleProfileChar5Props = GATT_PROP_READ;

// Characteristic 5 Value
static uint8 simpleProfileChar5[GATT_MAX_ATTR_SIZE] = {0};

// Simple Profile Characteristic 5 User Description
static uint8 simpleProfileChar5UserDesp[] = "Measurement";

// Simple Profile Characteristic 6 Properties
static uint8 simpleProfileChar6Props =GATT_PROP_READ | GATT_PROP_WRITE|GATT_PROP_NOTIFY;
//static uint8 simpleProfileChar6Props =GATT_PROP_READ ;

// Characteristic 6 Value
static uint8 simpleProfileChar6[GATT_MAX_ATTR_SIZE] = {0};

// Simple Profile Characteristic 6 User Description
static uint8 simpleProfileChar6UserDesp[] = "OAD Firmwrare Ver";
gattCharCfg_t simpleProfileChar6Config[GATT_MAX_NUM_CONN];

static uint16 char1lenth = 0;
static uint16 char2lenth = 0;
static uint16 char3lenth = 0;
static uint16 char4lenth = 0;
static uint16 char5lenth = 0;
static uint16 char6lenth = 0;

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t simpleProfileAttrTbl[] =
{
	// 0 Simple Profile Service
	{
		{ ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
		GATT_PERMIT_READ,                         /* permissions */
		0,                                        /* handle */
		(uint8*)& simpleProfileService            /* pValue */
	},

	// 1 Characteristic 1 Declaration,
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&simpleProfileChar1Props
	},

	// 2 Characteristic 1 Value
	{
		{ ATT_BT_UUID_SIZE, simpleProfilechar1UUID },
		GATT_PERMIT_READ | GATT_PERMIT_WRITE,
		0,
		simpleProfileChar1
	},

	// 3 Characteristic 1 User Description
	{
		{ ATT_BT_UUID_SIZE, charUserDescUUID },
		GATT_PERMIT_READ,
		0,
		simpleProfileChar1UserDesp
	},

	// 4 Characteristic 2 Declaration
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&simpleProfileChar2Props
	},

	// 5 Characteristic 2 Value
	{
		{ ATT_BT_UUID_SIZE, simpleProfilechar2UUID },
		GATT_PERMIT_READ,
		0,
		simpleProfileChar2
	},

	// 6 Characteristic 2 User Description
	{
		{ ATT_BT_UUID_SIZE, charUserDescUUID },
		GATT_PERMIT_READ,
		0,
		simpleProfileChar2UserDesp
	},

	// 7 Characteristic 3 Declaration
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&simpleProfileChar3Props
	},

	// 8 Characteristic 3 Value
	{
		{ ATT_BT_UUID_SIZE, simpleProfilechar3UUID },
		GATT_PERMIT_READ | GATT_PERMIT_WRITE,
		0,
		simpleProfileChar3
	},

	// 9 Characteristic 3 User Description
	{
		{ ATT_BT_UUID_SIZE, charUserDescUUID },
		GATT_PERMIT_READ,
		0,
		simpleProfileChar3UserDesp
	},

	// 10 Characteristic 4 Declaration
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&simpleProfileChar4Props
	},

	// 11 Characteristic Value 4
	{
		{ ATT_BT_UUID_SIZE, simpleProfilechar4UUID },
		GATT_PERMIT_READ,
		0,
		(uint8*)& simpleProfileChar4
	},

	// 12 Characteristic 4 configuration
	{
		{ ATT_BT_UUID_SIZE, clientCharCfgUUID },
		GATT_PERMIT_READ | GATT_PERMIT_WRITE,
		0,
		(uint8*)simpleProfileChar4Config
	},

	// 13 Characteristic 4 User Description
	{
		{ ATT_BT_UUID_SIZE, charUserDescUUID },
		GATT_PERMIT_READ,
		0,
		simpleProfileChar4UserDesp
	},

	// 14 Characteristic 5 Declaration
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&simpleProfileChar5Props
	},

	// 15 Characteristic 5 Value
	{
		{ ATT_BT_UUID_SIZE, simpleProfilechar5UUID },
		GATT_PERMIT_READ,
		0,
		(uint8*)& simpleProfileChar5
	},

	// 16 Characteristic 5 User Description
	{
		{ ATT_BT_UUID_SIZE, charUserDescUUID },
		GATT_PERMIT_READ,
		0,
		simpleProfileChar5UserDesp
	},

	// 17 Characteristic 6 Declaration
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&simpleProfileChar6Props
	},

	// 18 Characteristic 6 Value
	{
		{ ATT_BT_UUID_SIZE, simpleProfilechar6UUID },
//		GATT_PERMIT_READ| GATT_PERMIT_WRITE|GATT_PROP_NOTIFY,
		GATT_PERMIT_READ| GATT_PERMIT_WRITE,
		0,
		(uint8*)& simpleProfileChar6
	},

	// 19 Characteristic 6 configuration
	{
		{ ATT_BT_UUID_SIZE, clientCharCfgUUID },
//		GATT_PERMIT_READ| GATT_PERMIT_WRITE|GATT_PROP_NOTIFY,
		GATT_PERMIT_READ| GATT_PERMIT_WRITE,
		0,
		(uint8*)simpleProfileChar6Config
	},
	// 20 Characteristic 6 User Description
	{
		{ ATT_BT_UUID_SIZE, charUserDescUUID },
		GATT_PERMIT_READ,
		0,
		simpleProfileChar6UserDesp
	},
};

//extern switch_cfg_t sw[];

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 simpleProfile_ReadAttrCB(uint16 connHandle, gattAttribute_t* pAttr,
                                      uint8* pValue, uint8* pLen, uint16 offset, uint8 maxLen);
static bStatus_t simpleProfile_WriteAttrCB(uint16 connHandle, gattAttribute_t* pAttr,
        uint8* pValue, uint16 len, uint16 offset);

static void simpleProfile_HandleConnStatusCB(uint16 connHandle, uint8 changeType);


/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t simpleProfileCBs =
{
	simpleProfile_ReadAttrCB,  // Read callback function pointer
	simpleProfile_WriteAttrCB, // Write callback function pointer
	NULL                       // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SimpleProfile_AddService
 *
 * @brief   Initializes the Simple Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t SimpleProfile_AddService(uint32 services){
	uint8 status = SUCCESS;
	// Initialize Client Characteristic Configuration attributes
//	GATTServApp_InitCharCfg(INVALID_CONNHANDLE, simpleProfileChar4Config);
	GATTServApp_InitCharCfg(INVALID_CONNHANDLE, simpleProfileChar6Config);
	// Register with Link DB to receive link status change callback
	VOID linkDB_Register(simpleProfile_HandleConnStatusCB);

	if(services & SIMPLEPROFILE_SERVICE){
		// Register GATT attribute list and CBs with GATT Server App
		status = GATTServApp_RegisterService(simpleProfileAttrTbl,
		                                     GATT_NUM_ATTRS(simpleProfileAttrTbl),
		                                     &simpleProfileCBs);
	}

	return (status);
}


/*********************************************************************
 * @fn      SimpleProfile_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t SimpleProfile_RegisterAppCBs(simpleProfileCBs_t* appCallbacks) {
	if (appCallbacks) {
		simpleProfile_AppCBs = appCallbacks;
		return (SUCCESS);
	}

	else {
		return (bleAlreadyInRequestedMode);
	}
}
/*********************************************************************
 * @fn      SimpleProfile_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
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
bStatus_t SimpleProfile_SetParameter(uint8 param, uint16 len, void* value)
{
	bStatus_t ret = SUCCESS;
//	LOG("\n__SimpleProfile_SetParameter@simpleGATTprofile.c__\n");
	if(len > GATT_MAX_ATTR_SIZE)
	{
		return bleInvalidRange;
	}

	switch(param)
	{
		case SIMPLEPROFILE_CHAR1:
			char1lenth = len;
			osal_memcpy(simpleProfileChar1, value, len);
			break;

		case SIMPLEPROFILE_CHAR2:
			char2lenth = len;
			osal_memcpy(simpleProfileChar2, value, len);
			break;

		case SIMPLEPROFILE_CHAR3:
			char3lenth = len;
			osal_memcpy(simpleProfileChar3, value, len);
			break;

		case SIMPLEPROFILE_CHAR4:
			char4lenth = len;
			osal_memcpy(simpleProfileChar4, value, len);
//			LOG("1=%08x /t 2=%02x,%02x,%02x \t 3=%08x	\n",simpleProfileChar4Config[0],simpleProfileChar4[1],simpleProfileChar4[2],GATT_NUM_ATTRS(simpleProfileAttrTbl));
			GATTServApp_ProcessCharCfg(simpleProfileChar4Config, simpleProfileChar4, FALSE,
			                           simpleProfileAttrTbl, GATT_NUM_ATTRS(simpleProfileAttrTbl),
			                           INVALID_TASK_ID);
			break;

		case SIMPLEPROFILE_CHAR5:
			char5lenth = len;
			osal_memcpy(simpleProfileChar5, value, len);
			break;

		case SIMPLEPROFILE_CHAR6:
			char6lenth = len;
			osal_memcpy(simpleProfileChar6, value, len);
//			simpleProfileChar6Config[0]=simpleProfileChar4Config[0];
//			LOG("1=%02x /t 2=%02x,%02x,%02x \t 3=%02x",simpleProfileChar6[0],simpleProfileChar6[1],simpleProfileChar6[2],GATT_NUM_ATTRS(simpleProfileAttrTbl));
			GATTServApp_ProcessCharCfg(simpleProfileChar6Config, simpleProfileChar6, FALSE,
			                           simpleProfileAttrTbl, GATT_NUM_ATTRS(simpleProfileAttrTbl),
			                           INVALID_TASK_ID);
			break;
		default:
			ret = INVALIDPARAMETER;
			break;
	}

	return (ret);
}

/*********************************************************************
 * @fn      SimpleProfile_GetParameter
 *
 * @brief   Get a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SimpleProfile_GetParameter(uint8 param, void* value, uint8* len)
{
	bStatus_t ret = SUCCESS;
//	LOG("\n__SimpleProfile_GetParameter@simpleGATTprofile.c__\n");
	switch(param)
	{
		case SIMPLEPROFILE_CHAR1:
			*len = char1lenth;
			osal_memcpy(value, simpleProfileChar1, char1lenth);
			break;

		case SIMPLEPROFILE_CHAR3:
			*len = char3lenth;
			osal_memcpy(value, simpleProfileChar3, char3lenth);
			break;
		case SIMPLEPROFILE_CHAR6:
			*len = char6lenth;
			osal_memcpy(value, simpleProfileChar6, char6lenth);
			break;
		default:
			ret = INVALIDPARAMETER;
			break;
	}

	return (ret);
}

/*********************************************************************
 * @fn          simpleProfile_ReadAttrCB
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
static uint8 simpleProfile_ReadAttrCB(uint16 connHandle, gattAttribute_t* pAttr,
                                      uint8* pValue, uint8* pLen, uint16 offset, uint8 maxLen)
 {
	bStatus_t status = SUCCESS;
	uint8 temperature = 0;
//	uint8 userManualInfoLen = sizeof(UserManualInfo) - 1;
//	uint8 otaFileLen = sizeof(OtaFile) - 1;
//	LOG("\n__simpleProfile_ReadAttrCB@simpleGATTprofile.c__\n");
	// If attribute permissions require authorization to read, return error
	if (gattPermitAuthorRead(pAttr->permissions)) {
		// Insufficient authorization
		return (ATT_ERR_INSUFFICIENT_AUTHOR);
	}

	// Make sure it's not a blob operation (no attributes in the profile are long)
	if (offset > 0) {
		return (ATT_ERR_ATTR_NOT_LONG);
	}

	if (pAttr->type.len == ATT_BT_UUID_SIZE) {
		uint16 uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);

		switch (uuid) {
		default:
			*pLen = 0;
			status = ATT_ERR_ATTR_NOT_FOUND;
			break;

		case SIMPLEPROFILE_CHAR1_UUID:
			*pLen = char1lenth;
			osal_memcpy(pValue, pAttr->pValue, char1lenth);
			char1lenth = 0;
			break;

		case SIMPLEPROFILE_CHAR2_UUID:
			*pLen = char2lenth;
			osal_memcpy(pValue, pAttr->pValue, char2lenth);
			char2lenth = 0;
			break;

		case SIMPLEPROFILE_CHAR3_UUID:
			*pLen = char3lenth;
			osal_memcpy(pValue, pAttr->pValue, char3lenth);
			char3lenth = 0;
			break;

		case SIMPLEPROFILE_CHAR4_UUID:
			*pLen = char4lenth;
			osal_memcpy(pValue, pAttr->pValue, char4lenth);
			//			char4lenth = 0;
			break;

		case SIMPLEPROFILE_CHAR5_UUID:
			*pLen = 1;
			temperature = (uint8) temp_get_value();
			osal_memcpy(pValue, &temperature, *pLen);
			char5lenth = 0;
			break;

		case SIMPLEPROFILE_CHAR6_UUID:
			//				*pLen = 3;
			//				uint8 ver[] = {"TBD"};
			//				osal_memcpy(pValue, ver, 3);
			//				LOG("char6lenth=%d\t otaFileLen=%d \t userManualInfoLen=%d",char6lenth,OtaFile[0]+1,UserManualInfo[0]+1);
			if (char6lenth == OtaFile[0] + 1 || char6lenth == UserManualInfo[0] + 1) {
				*pLen = char6lenth;
				osal_memcpy(pValue, pAttr->pValue, char6lenth);
			} else {
				char6lenth=UserManualInfo[0]+1;
				*pLen = char6lenth;
				osal_memcpy(pValue, UserManualInfo, char6lenth);
				//					LOG("\n%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x\n",pValue[0],pValue[1],pValue[2],pValue[3],pValue[4],pValue[5],pValue[6],pValue[7],pValue[8],pValue[9],pValue[10],pValue[11],pValue[12],pValue[13],pValue[14],pValue[15],pValue[16],pValue[17],pValue[18],pValue[19]);
				*pValue = SYSTEM_GET_USER_MENU;
				osal_memcpy(pAttr->pValue, pValue, char6lenth);
			}
			break;
		}
	}

	else {
		// 128-bit UUID
		*pLen = 0;
		status = ATT_ERR_INVALID_HANDLE;
	}

	return (status);
}

/*********************************************************************
 * @fn      simpleProfile_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 *
 * @return  Success or Failure
 */
static bStatus_t simpleProfile_WriteAttrCB(uint16 connHandle, gattAttribute_t* pAttr, uint8* pValue, uint16 len, uint16 offset) {
	bStatus_t status = SUCCESS;
	uint8 notifyApp = 0xFF;
	// If attribute permissions require authorization to write, return error
//	LOG("\n__simpleProfile_WriteAttrCB@simpleGATTprofile.c__\n");
	if (gattPermitAuthorWrite(pAttr->permissions)) {
		// Insufficient authorization
		return (ATT_ERR_INSUFFICIENT_AUTHOR);
	}

	if (pAttr->type.len == ATT_BT_UUID_SIZE) {
		// 16-bit UUID
		uint16 uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);
#if(BLE_RECEIVED_SHOW==1)
		LOG("\n__uuid=%04x__\n",uuid);
		user_print_hex(pValue, len);
#endif
		switch (uuid) {
		default:
			status = ATT_ERR_ATTR_NOT_FOUND;
			break;

		case SIMPLEPROFILE_CHAR1_UUID: {
			if (offset == 0) {
				if (len > GATT_MAX_ATTR_SIZE) {
					status = ATT_ERR_INVALID_VALUE_SIZE;
				}
			}

			else {
				status = ATT_ERR_ATTR_NOT_LONG;
			}

			if (status == SUCCESS) {
				char1lenth = len;
				osal_memcpy(pAttr->pValue, pValue, char1lenth);
				notifyApp = SIMPLEPROFILE_CHAR1;
			}
		}
			break;

		case SIMPLEPROFILE_CHAR3_UUID: {
			if (offset == 0) {
				if (len > GATT_MAX_ATTR_SIZE) {
					status = ATT_ERR_INVALID_VALUE_SIZE;
				}
			}

			else {
				status = ATT_ERR_ATTR_NOT_LONG;
			}

			if (status == SUCCESS) {
				char3lenth = len;
				osal_memcpy(pAttr->pValue, pValue, char3lenth);
				notifyApp = SIMPLEPROFILE_CHAR3;
			}
		}
			break;

		case SIMPLEPROFILE_CHAR6_UUID: {
			if (offset == 0) {
				if (len > GATT_MAX_ATTR_SIZE) {
					status = ATT_ERR_INVALID_VALUE_SIZE;
				}
			}

			else {
				status = ATT_ERR_ATTR_NOT_LONG;
			}

			if (status == SUCCESS) {
//				LOG("\n__UUID6@simpleGATTprofile.c__\n");
				char6lenth = len;
				osal_memcpy(pAttr->pValue, pValue, char6lenth);
				notifyApp = SIMPLEPROFILE_CHAR6;
			}
		}
			break;

		case GATT_CLIENT_CHAR_CFG_UUID: {
//			LOG("simpleProfile_WriteAttrCB     uuid=%04x@gattservapp.c \n",uuid);
			status = GATTServApp_ProcessCCCWriteReq(connHandle, pAttr, pValue, len, offset, GATT_CLIENT_CFG_NOTIFY);

			if (*(pValue)) {
				Notifyedied_f = true;
				osal_start_timerEx(command_center_TaskID, CCS_GATT_NOTIFY_EVT, 500);
			}

			else {
				Notifyedied_f = false;
				osal_stop_timerEx(command_center_TaskID, CCS_GATT_NOTIFY_EVT);
			}
		}
			break;
		}
	}

	else {
		// 128-bit UUID
		status = ATT_ERR_INVALID_HANDLE;
	}

	// If a charactersitic value changed then callback function to notify application of change
	if ((notifyApp != 0xFF) && simpleProfile_AppCBs && simpleProfile_AppCBs->pfnSimpleProfileChange) {
		simpleProfile_AppCBs->pfnSimpleProfileChange(notifyApp);
	}

	return (status);
}

/*********************************************************************
 * @fn          simpleProfile_HandleConnStatusCB
 *
 * @brief       Simple Profile link status change handler function.
 *
 * @param       connHandle - connection handle
 * @param       changeType - type of change
 *
 * @return      none
 */
static void simpleProfile_HandleConnStatusCB(uint16 connHandle, uint8 changeType) {
	// Make sure this is not loopback connection
	if (connHandle != LOOPBACK_CONNHANDLE) {
		// Reset Client Char Config if connection has dropped
		if ((changeType == LINKDB_STATUS_UPDATE_REMOVED) || ((changeType == LINKDB_STATUS_UPDATE_STATEFLAGS) && (!linkDB_Up(connHandle)))) {
//			GATTServApp_InitCharCfg(connHandle, simpleProfileChar4Config);
			GATTServApp_InitCharCfg(connHandle, simpleProfileChar6Config);
		}
	}
}

/*********************************************************************
 * @fn          simpleProfile_Get_notify_status
 *
 * @brief       Get nofify flag status.
 *
 * @param       none
 *
 * @return      bool
 */
bool simpleProfile_Get_notify_status( void )
{
	return Notifyedied_f;
}

/*********************************************************************
 * @fn          simpleProfile_Get_notify_status
 *
 * @brief       Get nofify flag status.
 *
 * @param       none
 *
 * @return      none
 */
void simpleProfile_Set_notify_status( bool flag )
{
	Notifyedied_f = flag;
}

/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
