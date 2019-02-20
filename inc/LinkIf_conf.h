/*
 * LinkIf_conf.h
 *
 *  Created on: Feb 19, 2019
 *      Author: kychu
 */

#ifndef LINKIF_CONF_H_
#define LINKIF_CONF_H_

#include "SysConfig.h"
#include "board_config.h"

#define MAIN_DATA_CACHE                (32) /* 16 * n */
#define MSG_QUEUE_DEPTH                (3)

#define HARD_DEV_ID                    (0x11) /* for UART2CAN board */

#define COM_USER_TYPE                  TYPE_USC_INFO_Resp = 0xC1,

#define COM_USER_TYPE_DATA             uint16_t USC_DIST_MM;

#endif /* LINKIF_CONF_H_ */
