/*
 * temp.h
 *
 *  Created on: 2020年10月13日
 *      Author: Sky
 */

#ifndef BG589_SOURCE_TEMP_H_
#define BG589_SOURCE_TEMP_H_
/*********************************************************************
* INCLUDES
*/
#include "types.h"
#include "protocol.h"

/*********************************************************************
* MACROS
*/
#define TEMP_POWER_VOLTAGE						3300//3412	//mv
#define TEMP_RES_PULLUP							1000	//kΩ

/*********************************************************************
* CONSTANTS
*/

/*********************************************************************
* TYPEDEFS
*/
//typedef void (* batt_evt_hdl_t)(batt_evt_t batt_evt);

/*********************************************************************
* VARIABLES
*/

/*********************************************************************
* FUNCTIONS
*/

/**
 * @fn      temp_init
 *
 * @brief   Init temp, set "emset". Only call once at systems init.
 *
 * @param   none
 *
 * @return  none
 */
extern int temp_init(void);

/**
 * @fn      temp_RegisterCBs_Value
 *
 * @brief   Register for temp change callback.
 *
 * @param   task_id, event
 *
 * @return  none
 */
extern void temp_RegisterCBs_Value(protocol_CBs_t cbs);

/**
 * @fn      temp_measure
 *
 * @brief   Start temp measure once. Call back by register when done.
 *
 * @param   none
 *
 * @return  none
 */
extern void temp_measure(void);

/**
 * @fn      temp_get_value
 *
 * @brief   return temp value.
 *
 * @param   none
 *
 * @return  float
 */
extern float temp_get_value(void);
extern	void setTemperature(float temp);
/*************************** (C) COPYRIGHT 2012 Bough*****END OF FILE*****************************/

#endif /* BG589_SOURCE_TEMP_H_ */
