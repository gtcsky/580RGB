/**************************************************************************************************
Filename:       comdef.h
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

#ifndef COMDEF_H
#define COMDEF_H

/*********************************************************************
 * INCLUDES
 */
//#include "precompile_cfg.h"
	
/* HAL */
	
#include "types.h"	
#include "hal_defs.h"	
//#include "log.h"	

	
/*********************************************************************
 * Lint Keywords
 */
#define VOID (void)

#define NULL_OK
#define INP
#define OUTP
#define UNUSED
#define ONLY
#define READONLY
#define SHARED
#define KEEP
#define RELAX

/*********************************************************************
 * CONSTANTS
 */

#ifndef false
  #define false 0
#endif

#ifndef true
  #define true 1
#endif

#ifndef CONST
  #define CONST const
#endif

#ifndef GENERIC
  #define GENERIC
#endif

/*** Generic Status Return Values ***/
#define SUCCESS                   0x00
#define FAILURE                   0x01
#define INVALIDPARAMETER          0x02
#define INVALID_TASK              0x03
#define MSG_BUFFER_NOT_AVAIL      0x04
#define INVALID_MSG_POINTER       0x05
#define INVALID_EVENT_ID          0x06
#define INVALID_INTERRUPT_ID      0x07
#define NO_TIMER_AVAIL            0x08
#define NV_ITEM_UNINIT            0x09
#define NV_OPER_FAILED            0x0A
#define INVALID_MEM_SIZE          0x0B
#define NV_BAD_ITEM_LEN           0x0C

/*********************************************************************
 * TYPEDEFS
 */

// Generic Status return
typedef uint8 Status_t;

// Data types
typedef int32   int24;
typedef uint32  uint24;

/*********************************************************************
 * Global System Events
 */

#define SYS_EVENT_MSG               0x8000  // A message is waiting event

/*********************************************************************
 * Global Generic System Messages
 */

#define KEY_CHANGE                0xC0    // Key Events

// OSAL System Message IDs/Events Reserved for applications (user applications)
// 0xE0 � 0xFC

/*********************************************************************
 * MACROS
 */
// What is the advertising interval when device is discoverable (units of 625us, 160=100ms)
#define DEFAULT_ADVERTISING_INTERVAL_30ms			48		//0x01
#define DEFAULT_ADVERTISING_INTERVAL_50ms			80		//0x02
#define DEFAULT_ADVERTISING_INTERVAL_100ms			160		//0x00&0x03
#define DEFAULT_ADVERTISING_INTERVAL_200ms			320		//0x04
#define DEFAULT_ADVERTISING_INTERVAL_500ms			800		//0x05
#define DEFAULT_ADVERTISING_INTERVAL_1s				1600	//0x06
#define DEFAULT_ADVERTISING_INTERVAL_2s				3200	//0x07
#define DEFAULT_ADVERTISING_INTERVAL_4s				6400	//0x08
#define DEFAULT_ADVERTISING_INTERVAL_10s			16000	//0x09

#define DEFAULT_ADVERTISING_INTERVAL_default_t		0x00
#define DEFAULT_ADVERTISING_INTERVAL_30ms_t			0x01
#define DEFAULT_ADVERTISING_INTERVAL_50ms_t			0x02
#define DEFAULT_ADVERTISING_INTERVAL_100ms_t		0x03
#define DEFAULT_ADVERTISING_INTERVAL_200ms_t		0x04
#define DEFAULT_ADVERTISING_INTERVAL_500ms_t		0x05
#define DEFAULT_ADVERTISING_INTERVAL_1s_t			0x06
#define DEFAULT_ADVERTISING_INTERVAL_2s_t			0x07
#define DEFAULT_ADVERTISING_INTERVAL_4s_t			0x08
#define DEFAULT_ADVERTISING_INTERVAL_10s_t			0x09

// Minimum connection interval (units of 1.25ms, 80=100ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_CONN_INTERVAL_30ms			32		//0x01
#define DEFAULT_DESIRED_CONN_INTERVAL_50ms			40		//0x02
#define DEFAULT_DESIRED_CONN_INTERVAL_100ms			80		//0x00&0x03
#define DEFAULT_DESIRED_CONN_INTERVAL_200ms			160		//0x04
#define DEFAULT_DESIRED_CONN_INTERVAL_500ms			400		//0x05
#define DEFAULT_DESIRED_CONN_INTERVAL_1s			800		//0x06
#define DEFAULT_DESIRED_CONN_INTERVAL_2s			1600	//0x07
#define DEFAULT_DESIRED_CONN_INTERVAL_4s			3200	//0x08
#define DEFAULT_DESIRED_CONN_INTERVAL_10ms			16		//0x09

#define DEFAULT_DESIRED_CONN_INTERVAL_default_t		0x00
#define DEFAULT_DESIRED_CONN_INTERVAL_30ms_t		0x01
#define DEFAULT_DESIRED_CONN_INTERVAL_50ms_t		0x02
#define DEFAULT_DESIRED_CONN_INTERVAL_100ms_t		0x03
#define DEFAULT_DESIRED_CONN_INTERVAL_200ms_t		0x04
#define DEFAULT_DESIRED_CONN_INTERVAL_500ms_t		0x05
#define DEFAULT_ADVERTISING_INTERVAL_1s_t			0x06
#define DEFAULT_ADVERTISING_INTERVAL_2s_t			0x07
#define DEFAULT_ADVERTISING_INTERVAL_4s_t			0x08
#define DEFAULT_ADVERTISING_INTERVAL_10s_t			0x09

// Slave latency to use if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SLAVE_LATENCY				0

// Supervision timeout value (units of 10ms, 1000=10s) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_CONN_TIMEOUT				600//1000

// Whether to enable automatic parameter update request when a connection is formed
#define DEFAULT_ENABLE_UPDATE_REQUEST				true//FALSE

// Limited discoverable mode advertises for 30.72s, and then stops
// General discoverable mode advertises indefinitely
#define DEFAULT_DISCOVERABLE_MODE					GAP_ADTYPE_FLAGS_GENERAL

// Connection Pause Peripheral time value (in seconds)
#define DEFAULT_CONN_PAUSE_PERIPHERAL				6

//define all command list of UART data only in hear
#define HAL_UART_BR_9600							0x00
#define HAL_UART_BR_4800							0x01
#define HAL_UART_BR_19200							0x03
#define HAL_UART_BR_38400							0x04
#define HAL_UART_BR_57600							0x05
#define HAL_UART_BR_115200							0x06
#define HAL_UART_BR_230400							0x07
#define HAL_UART_BR_2400							0x08

#define HAL_UART_TBR_9600							9600
#define HAL_UART_TBR_4800							4800
#define HAL_UART_TBR_19200							19200
#define HAL_UART_TBR_38400							38400
#define HAL_UART_TBR_57600							57600
#define HAL_UART_TBR_115200							115200
#define HAL_UART_TBR_230400							230400
#define HAL_UART_TBR_2400							2400

// SerialApp SOP
#define UART_SOP									0x02

// SerialApp Command HI of uint16
//------------------------------------------
#define CMD_HI_10									0x10
#define CMD_HI_20									0x20
#define CMD_HI_30									0x30
#define CMD_HI_40									0x40
#define CMD_HI_50									0x50
#define CMD_HI_60									0x60
#define CMD_HI_70									0x70
#define CMD_HI_80									0x80
#define CMD_HI_90									0x90
#define CMD_HI_a0									0xa0
//------------------------------------------


// SerialApp Command LO of uint16
//------------------------------------------
#define CMD_LO_ACK									0x01
#define CMD_LO_NACK									0x02
//------------------------------------------

// Serial command for login
#define CMD_LO_LOGIN_PASSWORD_WRITE					0x05
#define CMD_LO_LOGIN_PASSWORD_SEND					0x06
#define CMD_LO_READ_ID								0x07
#define CMD_LO_LOGIN								0x08
#define CMD_LO_LOGIN_PASSWORD_RESET					0x09
#define CMD_LO_PAIRINGMODE_INTO						0x0c
#define CMD_LO_PAIRINGMODE_EXIT						0x0d
#define CMD_LO_DISCONNECT							0x0f

// SerialApp Command only for 0xa0
#define CMD_LO_MOD_DEVICE_NAME						0xa1
#define CMD_LO_SET_DATATIME							0xa2
#define CMD_LO_MOD_MEMORYDATA						0xa9
#define CMD_LO_MOD_BAUDRATE_INVTIME					0xaa
#define CMD_LO_MOD_MODEL_NAME						0xab
#define CMD_LO_MOD_SERIAL_NAME						0xac
#define CMD_LO_MOD_LOCAL_NAME						0xad
#define CMD_LO_MOD_FIREWALL							0xae
#define CMD_LO_MOD_DEFAULT							0xaf

#define CMD_LO_READ_DeviceName						0xb1
#define CMD_LO_READ_DATATIME						0xb2
#define CMD_LO_READ_BATT							0xb3
#define CMD_LO_READ_SystemID						0xb4
#define CMD_LO_READ_FirmwareRev						0xb5
#define CMD_LO_READ_HardwareRev						0xb6
#define CMD_LO_READ_SoftwareRev						0xb7
#define CMD_LO_READ_ManufacturerName				0xb8
#define CMD_LO_READ_MemoryData						0xb9
#define CMD_LO_READ_TimeUART						0xba
#define CMD_LO_READ_ModelNumber						0xbb
#define CMD_LO_READ_SerialNumber					0xbc
#define CMD_LO_READ_LocalName						0xbd
#define CMD_LO_READ_MacAddress						0xbe
#define CMD_LO_READ_AllInformation					0xbf

// SerialApp Command for test
#define CMD_LO_TESTMODE_INTO						0xc0	//成品生产测试模式
#define CMD_LO_PD_TEST								0xc1
#define OPTION_PD_TEST_MDIO							0x01 	//module<->MCU IO test
#define OPTION_PD_TEST_XAL							0x02
#define OPTION_PD_TEST_POWER						0x03
#define OPTION_PD_TEST_IO							0x04	//IC all IO self test
#define OPTION_PD_TEST_RX							0x05	//test Rx mode for CE
#define CMD_LO_PD_PRO								0xc5	//烧录
#define CMD_LO_PD_PRORE								0xc6	//回复烧录结果
#define CMD_LO_BCSDATA								0xcb	//updata broadcase data & save it to flash
#define CMD_LO_TEST_UART							0xcd	//UART可靠性测试
#define CMD_LO_TEST_WL								0xce	//无线可靠性测试
#define CMD_LO_TEST_REPLY							0xcf	//reply test result
#define REPLY_TEST_MDLO_PASS						0x10
#define REPLY_TEST_MDLO_FAIL						0x11
#define REPLY_TEST_XAL_PASS							0x20
#define REPLY_TEST_XAL_FAIL							0x21
#define REPLY_TEST_POWER_OK							0x30
#define REPLY_TEST_IO_PASS							0x40
#define REPLY_TEST_IO_FAIL							0x41
#define CMD_LO_T8DATA								0xd8
#define CMD_LO_TTDATA								0xd9
#define CMD_LO_TRDATA								0xdd
#define CMD_LO_BCDATA								0xdf


#define CMD_LO_TEST_WL_BACK							0xe0

#define CMD_LO_WAKEUP								0xf0
#define CMD_LO_SLEEPMODE							0xf1

//define reply command
#define SAPP_PKG_SUCCESS							0x00
#define SAPP_PKG_ERR_SOP							0x01
#define SAPP_PKG_ERR_CMD							0x02
#define SAPP_PKG_ERR_LTH							0x03	//数据包长度错误
#define SAPP_PKG_ERR_XOR							0x04
#define SAPP_PKG_ERR_LEN							0x05	//长度超过20byte
#define SAPP_PKG_ERR_DISC							0x06	//链接未建立
#define SAPP_PKG_ERR_EOP							0x07	//链接配置未准备好
#define SAPP_PKG_ERR_TMR							0x10
#define SAPP_PKG_SUCCESS1							0x80
#define SAPP_PKG_N_ACK								0x81	//收到正应答或负应答
#define SAPP_PKG_ERR_OTH							0xFF

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/
#endif /* COMDEF_H */
