/**
  ******************************************************************************
  * @file    ./inc/TimerCounter.h 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Header for TimerCounter.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMERCOUNTER_H
#define __TIMERCOUNTER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void _TimeTicksInit(void);

uint16_t _Get_Ticks(void);
uint32_t _Get_Micros(void);
uint32_t _Get_Millis(void);
uint32_t _Get_Secnds(void);

#endif /* __TIMERCOUNTER_H */

/******************************** END OF FILE *********************************/
