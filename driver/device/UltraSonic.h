/**
  ******************************************************************************
  * @file    ./inc/UltraSonic.h 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Header for UltraSonic.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

/* Includes ------------------------------------------------------------------*/
#include "SysConfig.h"
#include "ifconfig.h"

#if (USC_MODE == USC_MODE_UART)
#include "kyLink.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void UltDriverInit(void);
uint8_t GetNewUltData(float *d);

#endif /* __ULTRASONIC_H */

/******************************** END OF FILE *********************************/
