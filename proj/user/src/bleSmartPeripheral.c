/**************************************************************************************************
Filename:	   bleSmartPeripheral.c
Revised:		Date: 2020.8.25
Revision:	   1.0

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
/* Copyright(c) 2012 Bough Technology Corp. All rights reserved.								 */
/*-----------------------------------------------------------------------------------------------*/


/*********************************************************************
* INCLUDES
*/
#include "rf_phy_driver.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"
//#include "OnBoard.h"
#include "gatt.h"
#include "hci.h"

#include "gatt_profile_uuid.h"
#include "gapgattserver.h"
#include "gattservapp.h"
#include "battservice.h"
#include "devinfoservice.h"
#include "simpleGATTprofile.h"
#include "LumeControlprofile.h"
#include "LumeCubeprofile.h"

#include "peripheral.h"
#include "gapbondmgr.h"

#ifdef OTA_MODE
#include "ota_app_service.h"
#endif	//

#include "bleSmartPeripheral.h"
#include "systems_parameters.h"
#include "command_center.h"

#if (defined HAL_KEY) && (HAL_KEY == TRUE)
#include "keys.h"
#endif	//#if (defined HAL_KEY) && (HAL_KEY == TRUE)

#include "log.h"

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
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
uint8 bleSmartPeripheral_TaskID;   // Task ID for internal task/event processing

static gaprole_States_t gapProfileState = GAPROLE_INIT;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void bleSmartPeripheral_ProcessOSALMsg(osal_event_hdr_t* pMsg);
static void peripheralStateNotificationCB(gaprole_States_t newState);
static void simpleProfileChangeCB(uint8 paramID);
//static void LumeControlProfileChangeCB(uint8 paramID);
//static void LumeCubeProfileChangeCB(uint8 paramID);


/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesCBs_t bleSmartPeripheral_PeripheralCBs =
{
	peripheralStateNotificationCB,  // Profile State Change Callbacks
	NULL							// When a valid RSSI is read from controller (not used by application)
};

// Simple GATT Profile Callbacks
static simpleProfileCBs_t bleSmartPeripheral_SimpleProfileCBs =
{
	simpleProfileChangeCB	// Charactersitic value change callback
};

//// LumeControl GATT Profile Callbacks
//static lumecontrolProfileCBs_t bleSmartPeripheral_LumeControlProfileCBs =
//{
//	LumeControlProfileChangeCB	// Charactersitic value change callback
//};
//
//// LumeCube GATT Profile Callbacks
//static lumecubeProfileCBs_t bleSmartPeripheral_LumeCubeProfileCBs =
//{
//	LumeCubeProfileChangeCB	// Charactersitic value change callback
//};

// GAP Bond Manager Callbacks
//static gapBondCBs_t bleSmartPeripheral_BondMgrCBs =
//{
//    NULL,                     // Passcode callback (not used by application)
//    NULL                      // Pairing / Bonding state Callback (not used by application)
//};
/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn	  bleSmartPeripheral_Init
 *
 * @brief   Initialization function for the Simple BLE Peripheral App Task.
 *		  This is called during initialization and should contain
 *		  any application specific initialization (ie. hardware
 *		  initialization/setup, table initialization, power up
 *		  notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *					used to send messages and set timers.
 *
 * @return  none
 */
void bleSmartPeripheral_Init(uint8 task_id)
{
	bleSmartPeripheral_TaskID = task_id;
	
	// Setup advert data
	uint8 advLen = 0;
	uint8 advertdata[B_MAX_ADV_LEN] = {0};
	systems_param_Get_param(ITEM_ADVERTDATA, &advLen, advertdata);

	GAPRole_SetParameter(GAPROLE_ADVERT_DATA, advLen, advertdata);
	
	// Setup scanRsp data
	uint8 scanRspLen = 0;
	uint8 scanRspData[B_MAX_ADV_LEN] = {0};
	systems_param_Get_param(ITEM_SCANRSPDATA, &scanRspLen, scanRspData);
	GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, scanRspLen, scanRspData);
	
	// Set GAP advertising paramters
	{
		uint16 advInt = GAP_GetadvInt();   // actual time = advInt * 625us
		GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MIN, advInt);
		GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MAX, advInt);
		GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, advInt);
		GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, advInt);
		
		uint16 gapRole_AdvertOffTime = 0;
		GAPRole_SetParameter(GAPROLE_ADVERT_OFF_TIME, sizeof(uint16), &gapRole_AdvertOffTime);
		
		uint8 advType = LL_ADV_CONNECTABLE_UNDIRECTED_EVT;
		GAPRole_SetParameter(GAPROLE_ADV_EVENT_TYPE, sizeof(uint8), &advType);
		uint8 initial_advertising_enable = false;
		GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8), &initial_advertising_enable);
	}
	
	// Set GAP connectting paramters
	{
		uint16 conInt = GAP_GetConnetInt();
		GAPRole_SetParameter(GAPROLE_MIN_CONN_INTERVAL, sizeof(uint16), &conInt-16);
		GAPRole_SetParameter(GAPROLE_MAX_CONN_INTERVAL, sizeof(uint16), &conInt);
		
		uint16 desired_slave_latency = DEFAULT_DESIRED_SLAVE_LATENCY;
		GAPRole_SetParameter(GAPROLE_SLAVE_LATENCY, sizeof(uint16), &desired_slave_latency);
		
		uint16 desired_conn_timeout = DEFAULT_DESIRED_CONN_TIMEOUT;
		GAPRole_SetParameter(GAPROLE_TIMEOUT_MULTIPLIER, sizeof(uint16), &desired_conn_timeout);
		
		uint8 enable_update_request = DEFAULT_ENABLE_UPDATE_REQUEST;
		GAPRole_SetParameter(GAPROLE_PARAM_UPDATE_ENABLE, sizeof(uint8), &enable_update_request);
		
		GAP_SetParamValue(TGAP_CONN_PAUSE_PERIPHERAL, DEFAULT_CONN_PAUSE_PERIPHERAL);
	}
	
	// Set the GAP device name
	{
		uint8 devicenameLen = 0;
		uint8 devicename[GAP_DEVICE_NAME_LEN] = {0};
		systems_param_Get_param(ITEM_DEVICENAME, &devicenameLen, devicename);
		GGS_SetParameter(GGS_DEVICE_NAME_ATT, devicenameLen, devicename);
	}
	
	{
//        uint32 passkey = 0; // passkey "000000"
//        uint8 pairMode = GAPBOND_PAIRING_MODE_NO_PAIRING;
//        uint8 mitm = true;
//        uint8 ioCap = GAPBOND_IO_CAP_NO_INPUT_NO_OUTPUT;
//        uint8 bonding = false;
//        
//        GAPBondMgr_SetParameter( GAPBOND_DEFAULT_PASSCODE, sizeof ( uint32 ), &passkey );
//        GAPBondMgr_SetParameter( GAPBOND_PAIRING_MODE, sizeof ( uint8 ), &pairMode );
//        GAPBondMgr_SetParameter( GAPBOND_MITM_PROTECTION, sizeof ( uint8 ), &mitm );
//        GAPBondMgr_SetParameter( GAPBOND_IO_CAPABILITIES, sizeof ( uint8 ), &ioCap );
//        GAPBondMgr_SetParameter( GAPBOND_BONDING_ENABLED, sizeof ( uint8 ), &bonding );
    }
	
	// Initialize GATT services
	{
		GGS_AddService(GATT_ALL_SERVICES);				// GAP
//		GATTServApp_AddService(GATT_ALL_SERVICES);	  	// GATT attributes
		Batt_AddService();								//
		DevInfo_AddService();								// Device Information Service
//		LumeControlProfile_AddService(GATT_ALL_SERVICES);
//		LumeCubeProfile_AddService(GATT_ALL_SERVICES);
		SimpleProfile_AddService(GATT_ALL_SERVICES);	// Simple GATT Profile
		
		// Register callback with profile
//		LumeControlProfile_RegisterAppCBs(&bleSmartPeripheral_LumeControlProfileCBs);
//		LumeCubeProfile_RegisterAppCBs(&bleSmartPeripheral_LumeCubeProfileCBs);
		SimpleProfile_RegisterAppCBs(&bleSmartPeripheral_SimpleProfileCBs);
		
#ifdef OTA_MODE
		ota_app_AddService();
#endif	//#ifdef OTA_MODE
	}
	
#if (defined HAL_KEY) && (HAL_KEY == TRUE)
	protocol_CBs_t cbs;
	cbs.task_id = bleSmartPeripheral_TaskID;
	cbs.events = SBP_KEY_DEBOUNCE_EVT;
	keys_RegisterDebounceCBs(cbs);
#endif	//#if (defined HAL_KEY) && (HAL_KEY == TRUE)
	
	// Setup a delayed profile startup
	osal_set_event(bleSmartPeripheral_TaskID, SBP_START_DEVICE_EVT);
}

/*********************************************************************
 * @fn	  bleSmartPeripheral_ProcessEvent
 *
 * @brief   Simple BLE Peripheral Application Task event processor.  This function
 *		  is called to process all events for the task.  Events
 *		  include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *				   contain more than one event.
 *
 * @return  events not processed
 */
uint16 bleSmartPeripheral_ProcessEvent(uint8 task_id, uint16 events) {
	VOID task_id; // OSAL required parameter that isn't used in this function

	if (events & SYS_EVENT_MSG) {
		uint8* pMsg;

		if ((pMsg = osal_msg_receive(bleSmartPeripheral_TaskID)) != NULL) {
			bleSmartPeripheral_ProcessOSALMsg((osal_event_hdr_t*) pMsg);

			// Release the OSAL message
			VOID osal_msg_deallocate(pMsg);
		}

		// return unprocessed events
		return (events ^ SYS_EVENT_MSG);
	}

	if (events & SBP_START_DEVICE_EVT) {
		// Start the Device
		VOID GAPRole_StartDevice(&bleSmartPeripheral_PeripheralCBs);
		return (events ^ SBP_START_DEVICE_EVT);
	}

#if (defined HAL_KEY) && (HAL_KEY == TRUE)
	if( events & SBP_KEY_DEBOUNCE_EVT )
	{
		keys_Debounce_Handle();
		return (events ^ SBP_KEY_DEBOUNCE_EVT);
	}
#endif	//#if (defined HAL_KEY) && (HAL_KEY == TRUE)	

	// Discard unknown events
	return 0;
}

/*********************************************************************
 * @fn	  bleSmartPeripheral_ProcessOSALMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void bleSmartPeripheral_ProcessOSALMsg(osal_event_hdr_t* pMsg)
{
	switch(pMsg->event)
	{
		default:
			// do nothing
			break;
	}
}

/*********************************************************************
 * @fn	  peripheralStateNotificationCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void peripheralStateNotificationCB(gaprole_States_t newState)
{
	switch(newState)
	{
		case GAPROLE_STARTED:
		{
			uint8 systemId[DEVINFO_SYSTEM_ID_LEN];
			uint8 ownAddress[B_ADDR_LEN] = {0};
			
			GAPRole_GetParameter(GAPROLE_BD_ADDR, ownAddress);
			
			// use 6 bytes of device address for 8 bytes of system ID value
			systemId[0] = ownAddress[0];
			systemId[1] = ownAddress[1];
			systemId[2] = ownAddress[2];
			// set middle bytes to zero
			systemId[4] = 0x48;
			systemId[3] = 0x42;
			// shift three bytes up
			systemId[7] = ownAddress[5];
			systemId[6] = ownAddress[4];
			systemId[5] = ownAddress[3];
			systems_param_Set_param(ITEM_SYSTEMID, DEVINFO_SYSTEM_ID_LEN, systemId);
			
			uint8 advLen = 0;
			uint8 advertdata[B_MAX_ADV_LEN] = {0};
			systems_param_Get_param(ITEM_ADVERTDATA, &advLen, advertdata);
			GAPRole_SetParameter(GAPROLE_ADVERT_DATA, advLen, advertdata);
	
			uint8 initial_advertising_enable = true;
			GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8), &initial_advertising_enable);
		}
		break;

		case GAPROLE_CONNECTED:
			osal_start_timerEx( command_center_TaskID, CCS_BLEIOC_EVT, 100 );
			break;

		case GAPROLE_ADVERTISING:		
		case GAPROLE_WAITING:
		case GAPROLE_WAITING_AFTER_TIMEOUT:
#if(BLE_RECEIVED_SHOW==1)
			LOG("\n___Disconnected!____\n");
#endif
		case GAPROLE_ERROR:
			simpleProfile_Set_notify_status(false);
			osal_start_timerEx( command_center_TaskID, CCS_BLEIOC_EVT, 100 );
			break;

		case GAPROLE_CONNECTED_ADV:
		default:
			break;
	}

	gapProfileState = newState;
	VOID gapProfileState;
}

/*********************************************************************
 * @fn	  GetgapProfileState
 *
 * @brief   get gatt status
 *
 * @param   none.
 *
 * @return  GetgapProfileState
 */
gaprole_States_t GetgapProfileState(void)
{
	return gapProfileState;
}

/*********************************************************************
 * @fn	  simpleProfileChangeCB
 *
 * @brief   Callback from SimpleBLEProfile indicating a value change
 *
 * @param   paramID - parameter ID of the value that was changed.
 *
 * @return  none
 */
static void simpleProfileChangeCB(uint8 paramID) {
	uint8 newValue[32];
	switch (paramID) {
	case SIMPLEPROFILE_CHAR1:
		SimpleProfile_GetParameter(SIMPLEPROFILE_CHAR1, &newValue[4], (uint8*) &newValue[3]);
		updateDeviceDefaultInfo((uint8*) &newValue[3],TRUE);
		break;

	case SIMPLEPROFILE_CHAR3:
		SimpleProfile_GetParameter(SIMPLEPROFILE_CHAR3, &newValue[4], (uint8*) &newValue[3]);
		CCS_SIMPLEGATT_DATA_PROCESS(&newValue[3]);
		break;
	case SIMPLEPROFILE_CHAR6:
		SimpleProfile_GetParameter(SIMPLEPROFILE_CHAR6, &newValue[4], (uint8*) &newValue[3]);
//		if (newValue[3] == newValue[4] + 2) {		//valid length
//			updateDeviceDefaultInfo((uint8*) &newValue[4]);
//		}
		updateDeviceDefaultInfo((uint8*) &newValue[3],FALSE);
		break;
	default:
		// not process other attribute change
		break;
	}
}

///*********************************************************************
// * @fn	  LumeControlProfileChangeCB
// *
// * @brief   Callback from SimpleBLEProfile indicating a value change
// *
// * @param   paramID - parameter ID of the value that was changed.
// *
// * @return  none
// */
//static void LumeControlProfileChangeCB(uint8 paramID)
//{
//	uint8 newValue[20];
//	LOG("\n____LumeCtrl Cb______\n");
//
//	switch(paramID)
//	{
//		case LUMECONTROLROFILE_CHAR1:
//		{
//			LumeControlProfile_GetParameter(LUMECONTROLROFILE_CHAR1, &newValue[1],&newValue[0]);
//			CCS_LUMECONTROL_DATA_PROCESS(newValue);
//		}
//		break;
//
//		default:
//		// not process other attribute change
//		break;
//	}
//}
//
///*********************************************************************
// * @fn	  LumeCubeProfileChangeCB
// *
// * @brief   Callback from SimpleBLEProfile indicating a value change
// *
// * @param   paramID - parameter ID of the value that was changed.
// *
// * @return  none
// */
//static void LumeCubeProfileChangeCB(uint8 paramID)
//{
//	uint8 newValue[20];
//	LOG("\n____LumeCube Cb______\n");
//	switch(paramID)
//	{
//		case LUMECUBEROFILE_CHAR1:
//		{
//			LumeCubeProfile_GetParameter(LUMECUBEROFILE_CHAR1, &newValue[1], &newValue[0]);
//			CCS_LUMECUBE_DATA_PROCESS(newValue);
//		}
//		break;
//
//		default:
//		// not process other attribute change
//		break;
//	}
//}

/*********************************************************************
* @fn      GAP_GetadvInt
*
* @brief   set advertising interval time
*
* @param   none.
*
* @return  advInt:advertising interval time
*/
uint16 GAP_GetadvInt(void)
{
	uint8 advInttime[3] = {0};
	uint16 advInt = 0;

	systems_param_Get_param(ITEM_INTERVALTIME, &advInttime[0], &advInttime[1]);

	switch (advInttime[1] & 0x0f)
	{
		default:
		case 0x00:	advInt = DEFAULT_ADVERTISING_INTERVAL_100ms;	break;
		case 0x01:	advInt = DEFAULT_ADVERTISING_INTERVAL_30ms;		break;
		case 0x02:	advInt = DEFAULT_ADVERTISING_INTERVAL_50ms;		break;
		case 0x04:	advInt = DEFAULT_ADVERTISING_INTERVAL_200ms;	break;
		case 0x05:	advInt = DEFAULT_ADVERTISING_INTERVAL_500ms;	break;
		case 0x06:	advInt = DEFAULT_ADVERTISING_INTERVAL_1s;		break;
		case 0x07:	advInt = DEFAULT_ADVERTISING_INTERVAL_2s;		break;
		case 0x08:	advInt = DEFAULT_ADVERTISING_INTERVAL_4s;		break;
		case 0x09:	advInt = DEFAULT_ADVERTISING_INTERVAL_10s;		break;
	}
	return advInt;
}

/*********************************************************************
* @fn      GAP_GetadvInt
*
* @brief   set connection interval time
*
* @param   none.
*
* @return  cntInt:connection interval time
*/
uint16 GAP_GetConnetInt(void)
{
	uint8 cntInttime[3] = {0};
	uint16 cntInt = 0;

	systems_param_Get_param(ITEM_INTERVALTIME, &cntInttime[0], &cntInttime[1]);

	switch ((cntInttime[1] >> 4) & 0x0f)
	{
		default:
		case 0x00:	cntInt = DEFAULT_DESIRED_CONN_INTERVAL_100ms;	break;
		case 0x01:	cntInt = DEFAULT_DESIRED_CONN_INTERVAL_30ms;	break;
		case 0x02:	cntInt = DEFAULT_DESIRED_CONN_INTERVAL_50ms;	break;
		case 0x04:	cntInt = DEFAULT_DESIRED_CONN_INTERVAL_200ms;	break;
		case 0x05:	cntInt = DEFAULT_DESIRED_CONN_INTERVAL_500ms;	break;
		case 0x06:	cntInt = DEFAULT_DESIRED_CONN_INTERVAL_1s;		break;
		case 0x07:	cntInt = DEFAULT_DESIRED_CONN_INTERVAL_2s;		break;
		case 0x08:	cntInt = DEFAULT_DESIRED_CONN_INTERVAL_4s;		break;
	}
	return cntInt;
}

/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/

