/**************************************************************************************************
Filename:       systems_parameters.c
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
#include "types.h"
#include <string.h>
#include "OSAL.h"
#include "bcomdef.h"
#include "ll.h"
#include "gap.h"
#include "gatt_profile_uuid.h"

/* Application */
#include "flash.h"
#include "osal_snv.h"
#include "error.h"
#include "systems_parameters.h"
#include "devinfoservice.h"
#include "simpleGATTprofile.h"
#include	 "user_flash.h"
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
* VARIABLES
*/
//static const uint8 oadskey[12] = { 0xfe,0x0a,0x42,0x6F,0x75,0x67,0x68,0x20,0x4F,0x41,0x44,0x53 };

static uint8 SystemsID[] = {0x00, 0x00, 0x00, 0x42, 0x48, 0x00, 0x00, 0x00};

static const uint8 defaultLocalName[] ={"LTF_MWRGB"};// {"LTF_MWRGB"};
//static const uint8 defaultLocalName[] ={"BH-CWRGB"};// {"LTF_MWRGB"};
//static const uint8 defaultLocalName[] = {"LTF_MWRGB"};
static uint8 LocalName[SYSPARAM_MAX_ATTR_SIZE+2] = {0};

static const uint8 defaultDeviceName[] = {"Bough_Systems"};
static uint8 DeviceName[SYSPARAM_MAX_ATTR_SIZE+2] = {0};
uint8  macAscii[12]={0};
static const uint8 defaultAdvertData[] =
{
	0x02,   // length of this data
	GAP_ADTYPE_FLAGS,
	DEFAULT_DISCOVERABLE_MODE | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

	// service
	0x03,
	GAP_ADTYPE_16BIT_MORE,
	LO_UINT16( DEVINFO_SERV_UUID ),
	HI_UINT16( DEVINFO_SERV_UUID ),

	// manufacturer specific
	0x09,   // length of this data
	GAP_ADTYPE_MANUFACTURER_SPECIFIC,
	'B',
	'H',
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06
};

static uint8 AdvertData[B_MAX_ADV_LEN+2] = {0};

static const uint8 defaultScanRspData[] =
{
	//connect inteval
	0x05,
	GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE,
	LO_UINT16(DEFAULT_DESIRED_CONN_INTERVAL_100ms-16),
	HI_UINT16(DEFAULT_DESIRED_CONN_INTERVAL_100ms-16),
	LO_UINT16(DEFAULT_DESIRED_CONN_INTERVAL_100ms),
	HI_UINT16(DEFAULT_DESIRED_CONN_INTERVAL_100ms),

	//Tx power
	0x02,
	GAP_ADTYPE_POWER_LEVEL,
	0,
};
static uint8 ScanRspData[B_MAX_ADV_LEN+2] = {0};

static const uint8 defaultIntervalTime[] =
{
	DEFAULT_ADVERTISING_INTERVAL_default_t | DEFAULT_DESIRED_CONN_INTERVAL_default_t,
	HAL_UART_BR_57600
};
static uint8 IntervalTime[sizeof(defaultIntervalTime)+2] = {0};

static const uint8 defaultModelNumber[] = {"BG580T"};
static uint8 ModelNumber[SYSPARAM_MAX_MODELNUMBER_SIZE+2] = {0};

static const uint8 defaultSerialNumber[] = {"00000001"};
static uint8 SerialNumber[SYSPARAM_MAX_SERIALNUMBER_SIZE+2] = {0};

static const uint8 defaultFirmwareVersion[] = {"21120201"};
static const uint8 defaultHardwareVersion[] = {"BG334.00"};
static const uint8 defaultSoftwareVersion[] = {"G580TT2.10"};
static const uint8 defaultManufacturerName[] = {"Bough Tech"};
static const uint8	defaultUserManual[]={"Bough User Manual"};
static const uint8	defaultOtaInfo[]={"Bough User OTA"};
static const uint8 defaultUserManualInfo[]={"BGG580TA001B001C001"};
uint8 UserManualInfo[SYSPARAM_MAX_USER_MANUAL_SIZE+2]={0};
static const uint8 defaultOtaFile[]={"BGG580TD001E001F001"};
uint8 OtaFile[SYSPARAM_MAX_OTA_FILE_SIZE+2]={0};
static const uint8 default11073CertData[] ={
	DEVINFO_11073_BODY_EXP,      // authoritative body typeSYS
	0x00,                       // authoritative body structure type
	// authoritative body data follows below:
	'e', 'x', 'p', 'e', 'r', 'i', 'm', 'e', 'n', 't', 'a', 'l'
};
static const uint8 defaultPNPID[] ={
	1,                                      // Vendor ID source (1=Bluetooth SIG)
	LO_UINT16(0x0000), HI_UINT16(0xBB00),   // Vendor ID (Phyplus)
	LO_UINT16(0x0003), HI_UINT16(0x1200),   // Product ID (vendor-specific)
	LO_UINT16(0x0000), HI_UINT16(0x0200)    // Product version (JJ.M.N)
};
/***********************************************************************************************************
  *  @brief
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void resetDeviceInfoParams(void) {

	memset(&deviceInfoParams, 0, sizeof(deviceInfoStruct));
	memcpy(&deviceInfoParams.modelNoLen, &ModelNumber, ModelNumber[0] + 1);
	memcpy(&deviceInfoParams.serialNoLen, &SerialNumber, SerialNumber[0] + 1);
	memcpy(&deviceInfoParams.deviceNameLen, &DeviceName, DeviceName[0] + 1);
	deviceInfoParams.hwVersionLen = sizeof(defaultHardwareVersion) - 1;
	memcpy(&deviceInfoParams.hwVersion, &defaultHardwareVersion, sizeof(defaultHardwareVersion) - 1);
	deviceInfoParams.firmwareVersionLen = sizeof(defaultFirmwareVersion) - 1;
	memcpy(&deviceInfoParams.firmwareVersion, &defaultFirmwareVersion, sizeof(defaultFirmwareVersion) - 1);
	deviceInfoParams.swVersionLen = sizeof(defaultSoftwareVersion) - 1;
	memcpy(&deviceInfoParams.swVersion, &defaultSoftwareVersion, sizeof(defaultSoftwareVersion) - 1);
//	memcpy(&deviceInfoParams.userManualLen, &UserManualInfo, UserManualInfo[0] + 1);
//	memcpy(&deviceInfoParams.OtaFileInfolLen, &OtaFile, OtaFile[0] + 1);
	memcpy(&deviceInfoParams.userManual, &defaultUserManualInfo, sizeof(defaultUserManualInfo) - 1);
	deviceInfoParams.userManualLen= sizeof(defaultUserManualInfo) - 1;
	memcpy(&deviceInfoParams.OtaFileInfo, &defaultOtaFile, sizeof(defaultOtaFile) - 1);
	deviceInfoParams.OtaFileInfolLen= sizeof(defaultOtaFile) - 1;
	storeDeviceInfo();

}
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
  ************************************************************************************************************/
uint8 updateDeviceInfo2Flash(void) {
	if (readDeviceInfo()) {
		resetDeviceInfoParams();
		return 1;
	} else {
		if (memcmp(&deviceInfoParams.deviceName, &DeviceName[1], DeviceName[0] - 1)) {
			resetDeviceInfoParams();
			return 2;
		}
		if (memcmp(&deviceInfoParams.modelNum, &ModelNumber[1], ModelNumber[0] - 1)) {
			resetDeviceInfoParams();
			return 3;
		}
		if (memcmp(&deviceInfoParams.serialNum, &SerialNumber[1], SerialNumber[0] - 1)) {
			resetDeviceInfoParams();
			return 4;
		}
		//		if (memcmp(&deviceInfoParams.userManual, &UserManualInfo[1], UserManualInfo[0] - 1)) {
		//			resetDeviceInfoParams();
		//			return 5;
		//		}
		//		if (memcmp(&deviceInfoParams.OtaFileInfo, &OtaFile[1], OtaFile[0] - 1)) {
		//			resetDeviceInfoParams();
		//			return 6;
		//		}
		if (memcmp(&deviceInfoParams.userManual, &defaultUserManualInfo, sizeof(defaultUserManualInfo) - 1)) {
			resetDeviceInfoParams();
			return 5;
		}
		if (memcmp(&deviceInfoParams.OtaFileInfo, &defaultOtaFile, sizeof(defaultOtaFile) - 1)) {
			resetDeviceInfoParams();
			return 6;
		}
		if (memcmp(&deviceInfoParams.swVersion, &defaultSoftwareVersion, sizeof(defaultSoftwareVersion) - 1)) {
			resetDeviceInfoParams();
			return 7;
		}
		if (memcmp(&deviceInfoParams.firmwareVersion, &defaultFirmwareVersion, sizeof(defaultFirmwareVersion) - 1)) {
			resetDeviceInfoParams();
			return 8;
		}
		if (memcmp(&deviceInfoParams.hwVersion, &defaultHardwareVersion, sizeof(defaultHardwareVersion) - 1)) {
			resetDeviceInfoParams();
			return 9;
		}
		return 0;
	}
}
/*********************************************************************
* FUNCTIONS
*/

/*********************************************************************
 * @fn      systems_parameters_Init
 *
 * @brief   Initialization systems parameters
 *
 * @param   none
 *
 * @return  none
 */
void systems_parameters_Init( void )
{
	uint8 index;
	
//	osal_snv_read( SYSTEMS_PARAM_SYSTEMID, sizeof(SystemsID), SystemsID );
	osal_snv_read( SYSTEMS_PARAM_LOCALNAME, SYSPARAM_MAX_LOCALNAME_SIZE+2, LocalName );
	osal_snv_read( SYSTEMS_PARAM_DEVICENAME, SYSPARAM_MAX_DEVICENAME_SIZE+2, DeviceName );
	osal_snv_read( SYSTEMS_PARAM_ADVERTDATA, SYSPARAM_MAX_ADVERTDATA_SIZE+2, &AdvertData[18] );
	osal_snv_read( SYSTEMS_PARAM_INTVALER, sizeof(IntervalTime)+2, IntervalTime );
	osal_snv_read( SYSTEMS_PARAM_MODELNAME, SYSPARAM_MAX_MODELNUMBER_SIZE+2, ModelNumber );
	osal_snv_read( SYSTEMS_PARAM_SERIALNUMBER, SYSPARAM_MAX_SERIALNUMBER_SIZE+2, SerialNumber );
	osal_snv_read( SYSTEM_GET_USER_MENU, SYSPARAM_MAX_USER_MANUAL_SIZE+2, UserManualInfo );
	osal_snv_read( SYSTEM_GET_OTA_FILE, SYSPARAM_MAX_OTA_FILE_SIZE+2, OtaFile );
	
	//start check data
	//SYSINFO_SYSTEM_ID, generate by system, write new data when it is empty or data out of range.
//	if ( SystemsID[0] > DEVINFO_SYSTEM_ID_LEN || SystemsID[0] == 0xff || SystemsID[0] == 0 )
	{
//		LL_ReadBDADDR( SystemsID );		
//		if ( SystemsID[0] == 0xff && SystemsID[1] == 0xff && SystemsID[2] == 0xff &&
//				SystemsID[3] == 0xff && SystemsID[4] == 0xff && SystemsID[5] == 0xff )
//		{
//			uint32 mac1 = 0;
//			mac1 = osal_rand();
//			mac1 |= osal_rand()<<8;
//			mac1 |= osal_rand()<<16;
//			mac1 |= osal_rand()<<24;
//			uint32 mac2 = 0;
//			mac2 = osal_rand();
//			mac2 |= osal_rand()<<8;
//			WriteFlash(0x10004000, mac1);
//			WriteFlash(0x10004004, mac2);
////			ProgramWord(0x4004, &SystemsID[4], 4);
//		}
		
		LL_ReadBDADDR( SystemsID );
		SystemsID[7] = SystemsID[5];
		SystemsID[6] = SystemsID[4];
		SystemsID[5] = SystemsID[3];
		SystemsID[4] = 0x48;
		SystemsID[3] = 0x42;
	}
	
	//SYSINFO_LOCALNAME, it is maybe changed by usrer, write new data when it is empty or data out of range.
	// LocalName[0] =0XFF;
	if ( LocalName[0] > SYSPARAM_MAX_LOCALNAME_SIZE || LocalName[0] == 0xff || LocalName[0] == 0 )
	{
		osal_memcpy(&LocalName[1], defaultLocalName, sizeof(defaultLocalName));
		LocalName[0] = sizeof(defaultLocalName)-1;
		LocalName[LocalName[0]+1] = 0;
		for ( index = 0; index < LocalName[0]; index++ )
		{
			LocalName[LocalName[0]+1] ^= LocalName[index];
		}
		
		osal_snv_write( SYSTEMS_PARAM_LOCALNAME, LocalName[0]+2, LocalName );
	}
	
	//scan rsp data
	osal_memcpy(&ScanRspData[1], defaultScanRspData, sizeof(defaultScanRspData));
	ScanRspData[0] = sizeof(defaultScanRspData);
	ScanRspData[10] = LocalName[0]+1;
	ScanRspData[11] = GAP_ADTYPE_LOCAL_NAME_COMPLETE;
	osal_memcpy(&ScanRspData[12], &LocalName[1], LocalName[0]);
	ScanRspData[0] += LocalName[0]+2;

	//SYSINFO_DEVICENAME, it is maybe changed by usrer, write new data when it is empty or data out of range.
	if ( DeviceName[0] > SYSPARAM_MAX_DEVICENAME_SIZE || DeviceName[0] == 0xff || DeviceName[0] == 0 )
	{
		osal_memcpy(&DeviceName[1], defaultDeviceName, sizeof(defaultDeviceName));
		DeviceName[0] = sizeof(defaultDeviceName)-1;
		DeviceName[DeviceName[0]+1] = 0;
		for ( index = 0; index < DeviceName[0]; index++ )
		{
			DeviceName[DeviceName[0]+1] ^= DeviceName[index];
		}
		
		osal_snv_write( SYSTEMS_PARAM_DEVICENAME, DeviceName[0]+2, DeviceName );
	}
	
	//AdvertData = default advertdata+user broadcasedata.
	osal_memcpy(&AdvertData[1], defaultAdvertData, sizeof(defaultAdvertData));
	AdvertData[0] = sizeof(defaultAdvertData);
	if ( AdvertData[18] <= SYSPARAM_MAX_ADVERTDATA_SIZE )
	{
		AdvertData[0] += AdvertData[18];
	}
	LL_ReadBDADDR(&AdvertData[12]);
	
	//ModelNumber, it is maybe changed by usrer, write new data when it is empty or data out of range.
	if ( ModelNumber[0] > SYSPARAM_MAX_MODELNUMBER_SIZE || ModelNumber[0] == 0xff || ModelNumber[0] == 0 )
	{
//						LOG("  len=%d   len=%d",ModelNumber[0],sizeof(defaultModelNumber) - 1);
		osal_memcpy(&ModelNumber[1], defaultModelNumber, sizeof(defaultModelNumber));
		ModelNumber[0] = sizeof(defaultModelNumber)-1;
		ModelNumber[ModelNumber[0]+1] = 0;
		for ( index = 0; index < ModelNumber[0]; index++ )
		{
			ModelNumber[ModelNumber[0]+1] ^= ModelNumber[index];
		}
		
		osal_snv_write( SYSTEMS_PARAM_MODELNAME, ModelNumber[0]+2, ModelNumber );
	}

	//SerialNumber, it is maybe changed by usrer, write new data when it is empty or data out of range.
	if ( SerialNumber[0] > SYSPARAM_MAX_SERIALNUMBER_SIZE || SerialNumber[0] == 0xff || SerialNumber[0] == 0 ){
		osal_memcpy(&SerialNumber[1], defaultSerialNumber, sizeof(defaultSerialNumber));
		SerialNumber[0] = sizeof(defaultSerialNumber)-1;
		SerialNumber[SerialNumber[0]+1] = 0;
		for ( index = 0; index < SerialNumber[0]; index++ )
		{
			SerialNumber[SerialNumber[0]+1] ^= SerialNumber[index];
		}
		
		osal_snv_write( SYSTEMS_PARAM_SERIALNUMBER, SerialNumber[0]+2, SerialNumber );
	}
//	if (UserManualInfo[0] > SYSPARAM_MAX_USER_MANUAL_SIZE || UserManualInfo[0] == 0xff || UserManualInfo[0] == 0) {
	if (UserManualInfo[0] > SYSPARAM_MAX_USER_MANUAL_SIZE || UserManualInfo[0] == 0xff || UserManualInfo[0] == 0||(sizeof(defaultUserManualInfo) - 1!=UserManualInfo[0])||memcmp(&defaultUserManualInfo, &UserManualInfo[1], UserManualInfo[0])) {
//				LOG("  len=%d   len=%d",UserManualInfo[0],sizeof(defaultUserManualInfo) - 1);
		osal_memcpy(&UserManualInfo[1], defaultUserManualInfo, sizeof(defaultUserManualInfo));
		UserManualInfo[0] = sizeof(defaultUserManualInfo) - 1;
		UserManualInfo[UserManualInfo[0] + 1] = 0;
		for (index = 0; index < UserManualInfo[0]; index++) {
			UserManualInfo[UserManualInfo[0] + 1] ^= UserManualInfo[index];
		}
		osal_snv_write( SYSTEM_GET_USER_MENU, UserManualInfo[0] + 2, UserManualInfo);
	}
	if (OtaFile[0] > SYSPARAM_MAX_OTA_FILE_SIZE || OtaFile[0] == 0xff || OtaFile[0] == 0||(sizeof(defaultOtaFile) - 1!=OtaFile[0])||memcmp(&defaultOtaFile, &OtaFile[1], OtaFile[0])) {
//	if (OtaFile[0] > SYSPARAM_MAX_OTA_FILE_SIZE || OtaFile[0] == 0xff || OtaFile[0] == 0) {

		osal_memcpy(&OtaFile[1], defaultOtaFile, sizeof(defaultOtaFile));
		OtaFile[0] = sizeof(defaultOtaFile) - 1;
		OtaFile[OtaFile[0] + 1] = 0;
		for (index = 0; index < OtaFile[0]; index++) {
			OtaFile[OtaFile[0] + 1] ^= OtaFile[index];
		}
		osal_snv_write( SYSTEM_GET_OTA_FILE, OtaFile[0] + 2, OtaFile);
	}

	//SYSINFO_INTERVALTIME, it is maybe changed by usrer, write new data when it is empty or data out of range.
	if (IntervalTime[0] > 2 || IntervalTime[0] == 0xff || IntervalTime[0] == 0) {
		osal_memcpy(&IntervalTime[1], defaultIntervalTime, sizeof(defaultIntervalTime));
		IntervalTime[0] = sizeof(defaultIntervalTime);
		IntervalTime[IntervalTime[0] + 1] = 0;
		for (index = 0; index < IntervalTime[0]; index++) {
			IntervalTime[IntervalTime[0] + 1] ^= IntervalTime[index];
		}
		osal_snv_write( SYSTEMS_PARAM_INTVALER, IntervalTime[0] + 2, IntervalTime);
	}
//	updateDeviceInfo2Flash();
}
/***********************************************************************************************************
  *  @brief
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
static bool UserInfo_NotifyStatus(uint8 cmd) {
	if (cmd != SYSTEM_GET_USER_MENU && cmd != SYSTEM_GET_OTA_FILE)
		return false;
	uint8 databuf[23] = { 0 };
	memset(databuf, 0xff, sizeof(databuf));
	uint8 len = 0;
	if (cmd == SYSTEM_GET_USER_MENU) {
		databuf[0] = cmd;
		len = sizeof(defaultUserManualInfo) ;
		memcpy(&databuf[1], &defaultUserManualInfo, len);
//		LOG("%02x,%02x,%02x,%02x,%02x, %02x,%02x,%02x,%02x,%02x, %02x,%02x,%02x,%02x,%02x, %02x,%02x,%02x,%02x,%02x\n",databuf[0],databuf[1],databuf[2],databuf[3],databuf[4],databuf[5],databuf[6],databuf[7],databuf[8],databuf[9],databuf[10],databuf[11],databuf[12],databuf[13],databuf[14],databuf[15],databuf[16],databuf[17],databuf[18],databuf[19]);
//			LOG("len=%d\n",len);
//		LOG(defaultUserManualInfo);
	} else if (cmd == SYSTEM_GET_OTA_FILE) {
		databuf[0] = cmd;
		len = sizeof(defaultOtaFile) ;
		memcpy(&databuf[1], &defaultOtaFile, len);
//		LOG(defaultOtaFile);
//		LOG("%02x,%02x,%02x,%02x,%02x, %02x,%02x,%02x,%02x,%02x, %02x,%02x,%02x,%02x,%02x, %02x,%02x,%02x,%02x,%02x\n",databuf[0],databuf[1],databuf[2],databuf[3],databuf[4],databuf[5],databuf[6],databuf[7],databuf[8],databuf[9],databuf[10],databuf[11],databuf[12],databuf[13],databuf[14],databuf[15],databuf[16],databuf[17],databuf[18],databuf[19]);

	}
	if ( SUCCESS == SimpleProfile_SetParameter(SIMPLEPROFILE_CHAR6, len, databuf)) {
		return true;
	} else {
		return false;
	}
}

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
void updateDeviceDefaultInfo(uint8* value, uint8 exceptNotify) {

	uint8 index = 0;
//	uint16	test=0xffff;
	value[0] -= 1;
	switch (value[1]) {
	case SYSTEM_UPDATE_LOCALNAME:
		if (value[0] <= (SYSPARAM_MAX_LOCALNAME_SIZE+1) && (value[0] == value[2] + 1)) {		//valid data
			osal_memcpy(&LocalName[1], &value[3], value[0]);
			LocalName[0] = value[2];
			LocalName[LocalName[0] + 1] = 0;
			for (index = 0; index < LocalName[0]; index++) {
				LocalName[LocalName[0] + 1] ^= LocalName[index];
			}
			osal_snv_write( SYSTEMS_PARAM_LOCALNAME, LocalName[0] + 2, LocalName);
//			LOG("update local Name");
			storeExceptionStts();
			NVIC_SystemReset();
		}

		break;
	case SYSTEM_UPDATE_DEVICENAME:
		if (value[0] <= (SYSPARAM_MAX_DEVICENAME_SIZE+1) && (value[0] == value[2] + 1)) {		//valid data
			osal_memcpy(&DeviceName[1], &value[3], value[0]);
			DeviceName[0] = value[2];
			DeviceName[DeviceName[0] + 1] = 0;
			for (index = 0; index < DeviceName[0]; index++) {
				DeviceName[DeviceName[0] + 1] ^= DeviceName[index];
			}
			osal_snv_write( SYSTEMS_PARAM_DEVICENAME, DeviceName[0] + 2, DeviceName);
//			LOG("update Device Name");
			storeExceptionStts();
			NVIC_SystemReset();
		}
		break;

	case SYSTEM_UPDATE_SERIALNUMBER:
		if (value[0] <= (SYSPARAM_MAX_SERIALNUMBER_SIZE+1) && (value[0] == value[2] + 1)) {		//valid data
			osal_memcpy(&SerialNumber[1], &value[3], value[0]);
			SerialNumber[0] = value[2];
			SerialNumber[SerialNumber[0] + 1] = 0;
			for (index = 0; index < SerialNumber[0]; index++) {
				SerialNumber[SerialNumber[0] + 1] ^= SerialNumber[index];
			}
			osal_snv_write( SYSTEMS_PARAM_SERIALNUMBER, SerialNumber[0] + 2, SerialNumber);
//			LOG("update SerialNo");
			storeExceptionStts();
			NVIC_SystemReset();
		}
		break;
	case SYSTEM_UPDATE_MODELNAME:
		if (value[0] <= (SYSPARAM_MAX_MODELNUMBER_SIZE+1) && (value[0] == value[2] + 1)) {		//valid data
			osal_memcpy(&ModelNumber[1], &value[3], value[0] - 1);
			ModelNumber[0] = value[2];
			ModelNumber[ModelNumber[0] + 1] = 0;
			for (index = 0; index < ModelNumber[0]; index++) {
				ModelNumber[ModelNumber[0] + 1] ^= ModelNumber[index];
			}
			osal_snv_write( SYSTEMS_PARAM_MODELNAME, ModelNumber[0] + 2, ModelNumber);
//			LOG("update ModelNo");
			storeExceptionStts();
			NVIC_SystemReset();
		}
		break;
	case SYSTEM_DEFAULT_SETTTING:
		if (value[0] <= (SYSPARAM_MAX_DEFAULT_SETTING_SIZE+1) && (value[0] == value[2] + 1)) {		//valid data
			if (value[3] == 0x24 && value[4] == 0x29 && value[5] == 0x40 && value[6] == 0x3F && value[7] == 0x22) {
//				LOG("load Default Setting\n");
				uint8 temp[1] = { 0xff };
				osal_snv_write( SYSTEMS_PARAM_MODELNAME, 1, temp);
				osal_snv_write( SYSTEMS_PARAM_SERIALNUMBER, 1, temp);
				osal_snv_write( SYSTEMS_PARAM_DEVICENAME, 1, temp);
				osal_snv_write( SYSTEMS_PARAM_LOCALNAME, 1, temp);
			}
			storeExceptionStts();
			NVIC_SystemReset();
		}
		break;

	case SYSTEM_GET_USER_MENU:
		if (!exceptNotify) {
			if (value[0] <= USER_MENUAL_MAX_SIZE) {		//valid data
				if (!memcmp(&value[2], defaultUserManual, USER_MENUAL_MAX_SIZE)) {
					UserInfo_NotifyStatus(SYSTEM_GET_USER_MENU);
				}

			}
		}
		break;

	case SYSTEM_GET_OTA_FILE:
		if (!exceptNotify) {
			if (value[0] <= USER_OTA_NAME_MAX_SIZE) {		//valid data
				if (!memcmp(&value[2], defaultOtaInfo, USER_OTA_NAME_MAX_SIZE)) {
					UserInfo_NotifyStatus(SYSTEM_GET_OTA_FILE);
				}
			}
		}
		break;
	}
}
/*********************************************************************
 * @fn      systems_param_Get_param
 *
 * @brief   Get systems parameters
 *
 * @param   systems_param_t
 *
 * @return  bStatus_t
 */
bStatus_t systems_param_Get_param( systems_param_paramID_t paramID, uint8 *len, uint8 *param )
{
	switch ( paramID )
	{
		case ITEM_SYSTEMID:
			*len = sizeof(SystemsID);
			osal_memcpy( param, SystemsID, *len );
			break;

		case ITEM_LOCALNAME:
			*len = LocalName[0];
			osal_memcpy( param, &LocalName[1], *len );
			break;

		case ITEM_DEVICENAME:
			*len = DeviceName[0];
			osal_memcpy( param, &DeviceName[1], *len );
			break;

		case ITEM_ADVERTDATA:
			*len = AdvertData[0];
			osal_memcpy( param, &AdvertData[1], *len );
			break;

		case ITEM_SCANRSPDATA:
			*len = ScanRspData[0];
			osal_memcpy( param, &ScanRspData[1], *len );
			break;

		case ITEM_INTERVALTIME:
			*len = IntervalTime[0];
			osal_memcpy( param, &IntervalTime[1], *len );
			break;

		case ITEM_MODELNUMBER:
			*len = ModelNumber[0];
			osal_memcpy( param, &ModelNumber[1], *len );
			break;

		case ITEM_SERIAL_NUMBER:
			*len = SerialNumber[0];
			osal_memcpy( param, &SerialNumber[1], *len );
			break;

		case ITEM_FIRMWARE_REV:
			*len = sizeof(defaultFirmwareVersion)-1;
			osal_memcpy( param, defaultFirmwareVersion, *len );
			break;

		case ITEM_HARDWARE_REV:
			*len = sizeof(defaultHardwareVersion)-1;
			osal_memcpy( param, defaultHardwareVersion, *len );
			break;

		case ITEM_SOFTWARE_REV:
			*len = sizeof(defaultSoftwareVersion)-1;
			osal_memcpy( param, defaultSoftwareVersion, *len );
			break;

		case ITEM_MANUFACTURER_NAME:
			*len = sizeof(defaultManufacturerName)-1;
			osal_memcpy( param, defaultManufacturerName, *len );
			break;

		case ITEM_11073_CERT_DATA:
			*len = sizeof(default11073CertData);
			osal_memcpy( param, default11073CertData, *len );
			break;

		case ITEM_PNP_ID:
			*len = sizeof(defaultPNPID);
			osal_memcpy( param, defaultPNPID, *len );
			break;

		default:
			return false;
	}

	return SUCCESS;
}

/*********************************************************************
 * @fn      systems_param_Get_param
 *
 * @brief   Get systems parameters
 *
 * @param   systems_param_t
 *
 * @return  bStatus_t
 */
bStatus_t systems_param_Set_param( systems_param_paramID_t paramID, uint8 len, uint8 *param )
{
	switch ( paramID )
	{
		case ITEM_SYSTEMID:
			if ( len > DEVINFO_SYSTEM_ID_LEN )
			{
				return INVALID_MEM_SIZE;
			}

			AdvertData[12] = *param;
			AdvertData[13] = *(param+1);
			AdvertData[14] = *(param+2);
			AdvertData[15] = *(param+5);
			AdvertData[16] = *(param+6);
			AdvertData[17] = *(param+7);
			osal_memcpy( SystemsID, param, len );
			break;

		case ITEM_LOCALNAME:
			if ( len > SYSPARAM_MAX_ATTR_SIZE )
			{
				return INVALID_MEM_SIZE;
			}

			osal_memcpy( LocalName, param, len );
			break;

		case ITEM_DEVICENAME:
			if ( len > SYSPARAM_MAX_ATTR_SIZE )
			{
				return INVALID_MEM_SIZE;
			}

			osal_memcpy( DeviceName, param, len );
			break;

		case ITEM_ADVERTDATA:
			if ( len > B_MAX_ADV_LEN - sizeof(defaultAdvertData) )
			{
				return INVALID_MEM_SIZE;
			}

			osal_memcpy( &AdvertData[sizeof(defaultAdvertData)+1], param, len );
			break;

		case ITEM_SCANRSPDATA:
			if ( len > B_MAX_ADV_LEN - sizeof(defaultAdvertData) )
			{
				return INVALID_MEM_SIZE;
			}

			osal_memcpy( &ScanRspData[sizeof(defaultAdvertData)]+1, param, len );
			break;

		case ITEM_INTERVALTIME:
			if ( len > 2 )
			{
				return INVALID_MEM_SIZE;
			}

			osal_memcpy( IntervalTime, param, len );
			break;

		case ITEM_MODELNUMBER:
			if ( len > SYSPARAM_MAX_ATTR_SIZE )
			{
				return INVALID_MEM_SIZE;
			}

			osal_memcpy( ModelNumber, param, len );
			break;

		case ITEM_SERIAL_NUMBER:
			if ( len > SYSPARAM_MAX_ATTR_SIZE )
			{
				return INVALID_MEM_SIZE;
			}

			osal_memcpy( SerialNumber, param, len );
			break;
		case ITEM_USER_MANUAL:
			if ( len > SYSPARAM_MAX_ATTR_SIZE ){
				return INVALID_MEM_SIZE;
			}

			osal_memcpy( UserManualInfo, param, len );
			break;
		case ITEM_OTA_FILE:
			if ( len > SYSPARAM_MAX_ATTR_SIZE ){
				return INVALID_MEM_SIZE;
			}

			osal_memcpy( OtaFile, param, len );
			break;

		default:
			return INVALID_EVENT_ID;
	}

	return SUCCESS;
}



void getSystemId(uint8 *id){
	if(id!=NULL){
		id[0]=SystemsID[7];
		id[1]=SystemsID[6];
		id[2]=SystemsID[5];
		id[3]=SystemsID[2];
		id[4]=SystemsID[1];
		id[5]=SystemsID[0];
//		memcpy(id,SystemsID,8);
	}
}
/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
