/**
  ******************************************************************************
  * @file    ./src/Delay.c 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Delay Module Driver.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t _ticks = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Configure System tick timer update per 1ms.
	* @param  None
	* @retval None
	*/
void Delay_Init(void)
{
	SysTick_Config(SystemCoreClock / 1000);
}

/**
  * @brief  Insert any ticks in program.
  * @param  ticks: time ticks to delay. (unit: ms)
  * @retval None
  */
void DelayTicks(uint32_t ticks)
{
	_ticks = ticks;
	while(_ticks);
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	if(_ticks > 0)
		_ticks --;
}

/******************************** END OF FILE *********************************/
