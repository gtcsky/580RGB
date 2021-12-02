/**************************************************************************************************
Filename:       devinfoservice.c
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
contact Bouth R&D at www.bough.com.cn
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
#include "gatt_profile_uuid.h"
#include "gattservapp.h"

#include "devinfoservice.h"
#include "systems_parameters.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * CONST
 */
 
// Device information service
CONST uint8 devInfoServUUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(DEVINFO_SERV_UUID), HI_UINT16(DEVINFO_SERV_UUID)
};

// System ID
CONST uint8 devInfoSystemIdUUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(SYSTEM_ID_UUID), HI_UINT16(SYSTEM_ID_UUID)
};

// Model Number String
CONST uint8 devInfoModelNumberUUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(MODEL_NUMBER_UUID), HI_UINT16(MODEL_NUMBER_UUID)
};

// Serial Number String
CONST uint8 devInfoSerialNumberUUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(SERIAL_NUMBER_UUID), HI_UINT16(SERIAL_NUMBER_UUID)
};

// Firmware Revision String
CONST uint8 devInfoFirmwareRevUUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(FIRMWARE_REV_UUID), HI_UINT16(FIRMWARE_REV_UUID)
};

// Hardware Revision String
CONST uint8 devInfoHardwareRevUUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(HARDWARE_REV_UUID), HI_UINT16(HARDWARE_REV_UUID)
};

// Software Revision String
CONST uint8 devInfoSoftwareRevUUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(SOFTWARE_REV_UUID), HI_UINT16(SOFTWARE_REV_UUID)
};

// Manufacturer Name String
CONST uint8 devInfoMfrNameUUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(MANUFACTURER_NAME_UUID), HI_UINT16(MANUFACTURER_NAME_UUID)
};

// IEEE 11073-20601 Regulatory Certification Data List
CONST uint8 devInfo11073CertUUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(IEEE_11073_CERT_DATA_UUID), HI_UINT16(IEEE_11073_CERT_DATA_UUID)
};

// PnP ID
CONST uint8 devInfoPnpIdUUID[ATT_BT_UUID_SIZE] =
{
	LO_UINT16(PNP_ID_UUID), HI_UINT16(PNP_ID_UUID)
};

/*********************************************************************
 * VARIABLES
 */
static uint8 devInfoValue[SYSPARAM_MAX_ATTR_SIZE] = {0};

/*********************************************************************
 * Profile Attributes - variables
 */

// Device Information Service attribute
static CONST gattAttrType_t devInfoService = { ATT_BT_UUID_SIZE, devInfoServUUID };

// System ID characteristic
static uint8 devInfoSystemIdProps = GATT_PROP_READ;

// Model Number String characteristic
static uint8 devInfoModelNumberProps = GATT_PROP_READ;

// Serial Number String characteristic
static uint8 devInfoSerialNumberProps = GATT_PROP_READ;

// Firmware Revision String characteristic
static uint8 devInfoFirmwareRevProps = GATT_PROP_READ;

// Hardware Revision String characteristic
static uint8 devInfoHardwareRevProps = GATT_PROP_READ;

// Software Revision String characteristic
static uint8 devInfoSoftwareRevProps = GATT_PROP_READ;

// Manufacturer Name String characteristic
static uint8 devInfoMfrNameProps = GATT_PROP_READ;

// IEEE 11073-20601 Regulatory Certification Data List characteristic
static uint8 devInfo11073CertProps = GATT_PROP_READ;

// System ID characteristic
static uint8 devInfoPnpIdProps = GATT_PROP_READ;

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t devInfoAttrTbl[] =
{
	// Device Information Service
	{
		{ ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
		GATT_PERMIT_READ,                         /* permissions */
		0,                                        /* handle */
		(uint8*)& devInfoService                /* pValue */
	},

	// System ID Declaration
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&devInfoSystemIdProps
	},

	// System ID Value
	{
		{ ATT_BT_UUID_SIZE, devInfoSystemIdUUID },
		GATT_PERMIT_READ,
		0,
		(uint8*) devInfoValue
	},

	// Model Number String Declaration
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&devInfoModelNumberProps
	},

	// Model Number Value
	{
		{ ATT_BT_UUID_SIZE, devInfoModelNumberUUID },
		GATT_PERMIT_READ,
		0,
		(uint8*) devInfoValue
	},

	// Serial Number String Declaration
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&devInfoSerialNumberProps
	},

	// Serial Number Value
	{
		{ ATT_BT_UUID_SIZE, devInfoSerialNumberUUID },
		GATT_PERMIT_READ,
		0,
		(uint8*) devInfoValue
	},

	// Firmware Revision String Declaration
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&devInfoFirmwareRevProps
	},

	// Firmware Revision Value
	{
		{ ATT_BT_UUID_SIZE, devInfoFirmwareRevUUID },
		GATT_PERMIT_READ,
		0,
		(uint8*) devInfoValue
	},

	// Hardware Revision String Declaration
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&devInfoHardwareRevProps
	},

	// Hardware Revision Value
	{
		{ ATT_BT_UUID_SIZE, devInfoHardwareRevUUID },
		GATT_PERMIT_READ,
		0,
		(uint8*) devInfoValue
	},

	// Software Revision String Declaration
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&devInfoSoftwareRevProps
	},

	// Software Revision Value
	{
		{ ATT_BT_UUID_SIZE, devInfoSoftwareRevUUID },
		GATT_PERMIT_READ,
		0,
		(uint8*) devInfoValue
	},

	// Manufacturer Name String Declaration
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&devInfoMfrNameProps
	},

	// Manufacturer Name Value
	{
		{ ATT_BT_UUID_SIZE, devInfoMfrNameUUID },
		GATT_PERMIT_READ,
		0,
		(uint8*) devInfoValue
	},

	// IEEE 11073-20601 Regulatory Certification Data List Declaration
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&devInfo11073CertProps
	},

	// IEEE 11073-20601 Regulatory Certification Data List Value
	{
		{ ATT_BT_UUID_SIZE, devInfo11073CertUUID },
		GATT_PERMIT_READ,
		0,
		(uint8*) devInfoValue
	},

	// PnP ID Declaration
	{
		{ ATT_BT_UUID_SIZE, characterUUID },
		GATT_PERMIT_READ,
		0,
		&devInfoPnpIdProps
	},

	// PnP ID Value
	{
		{ ATT_BT_UUID_SIZE, devInfoPnpIdUUID },
		GATT_PERMIT_READ,
		0,
		(uint8*) devInfoValue
	}
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 devInfo_ReadAttrCB(uint16 connHandle, gattAttribute_t* pAttr,
                                uint8* pValue, uint8* pLen, uint16 offset, uint8 maxLen);

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Device Info Service Callbacks
CONST gattServiceCBs_t devInfoCBs =
{
	devInfo_ReadAttrCB, // Read callback function pointer
	NULL,               // Write callback function pointer
	NULL                // Authorization callback function pointer
};

/*********************************************************************
 * NETWORK LAYER CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      DevInfo_AddService
 *
 * @brief   Initializes the Device Information service by registering
 *          GATT attributes with the GATT server.
 *
 * @return  Success or Failure
 */
bStatus_t DevInfo_AddService(void)
{
	// Register GATT attribute list and CBs with GATT Server App
	return GATTServApp_RegisterService(devInfoAttrTbl,
	                                   GATT_NUM_ATTRS(devInfoAttrTbl),
	                                   &devInfoCBs);
}

/*********************************************************************
 * @fn          devInfo_ReadAttrCB
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
static uint8 devInfo_ReadAttrCB(uint16 connHandle, gattAttribute_t* pAttr,
                                uint8* pValue, uint8* pLen, uint16 offset, uint8 maxLen)
{
	bStatus_t status = SUCCESS;
	uint16 uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);

	switch(uuid)
	{
		case SYSTEM_ID_UUID:
			systems_param_Get_param(ITEM_SYSTEMID, pLen, pValue);
			break;

		case MODEL_NUMBER_UUID:
			systems_param_Get_param(ITEM_MODELNUMBER, pLen, pValue);
			break;

		case SERIAL_NUMBER_UUID:
			systems_param_Get_param(ITEM_SERIAL_NUMBER, pLen, pValue);
			break;

		case FIRMWARE_REV_UUID:
			systems_param_Get_param(ITEM_FIRMWARE_REV, pLen, pValue);
			break;

		case HARDWARE_REV_UUID:
			systems_param_Get_param(ITEM_HARDWARE_REV, pLen, pValue);
			break;

		case SOFTWARE_REV_UUID:
			systems_param_Get_param(ITEM_SOFTWARE_REV, pLen, pValue);
			break;

		case MANUFACTURER_NAME_UUID:
			systems_param_Get_param(ITEM_MANUFACTURER_NAME, pLen, pValue);
			break;

		case IEEE_11073_CERT_DATA_UUID:
			systems_param_Get_param(ITEM_11073_CERT_DATA, pLen, pValue);
			break;

		case PNP_ID_UUID:
			systems_param_Get_param(ITEM_PNP_ID, pLen, pValue);
			break;

		default:
			pLen = 0;
			status = ATT_ERR_ATTR_NOT_FOUND;
			break;
	}

	return (status);
}


/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
