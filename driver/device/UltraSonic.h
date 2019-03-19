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
#include "TimerCounter.h"
#include "board_config.h"

#include "stm32f0xx.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_syscfg.h"

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
