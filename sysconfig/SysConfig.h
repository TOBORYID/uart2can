/*
 * SysConfig.h
 *
 *  Created on: Feb 19, 2019
 *      Author: kychu
 */

#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_

#include <stdint.h>
#include "stm32f0xx.h"

#define CAN_MASTER_MODE                     (1)

#define PRINT_LOG_ENABLE                    (1)

//#if !(CAN_MASTER_MODE)
//#if !(PRINT_LOG_ENABLE)

#define USC_MODE_ECHO                       (0)
#define USC_MODE_UART                       (1)
#define USC_MODE                            USC_MODE_UART

//#endif
//#endif

#define SYSTEM_TIMER_INT_PRIORITY                (0)

#endif /* SYSCONFIG_H_ */
