/**
  ******************************************************************************
  * @file    ./src/UltraSonic.c 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Ultrasonic Driver module.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "UltraSonic.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if (USC_MODE == USC_MODE_ECHO)
static uint8_t _detect_step = 0;
static uint8_t _utl_data_update = 0;
static uint32_t _start_time = 0, _high_level_time = 0;
#else
static KYLINK_CORE_HANDLE USC_PORT_HANDLE;
static kyLinkPackageDef *pRx = 0;

static uint8_t read_len;
static uint8_t read_buf[8];
#endif

/* Private function prototypes -----------------------------------------------*/
#if (USC_MODE == USC_MODE_ECHO)
static void TIM_Config(void);
static void GPIO_Config(void);
#endif
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize Ultrasonic module driver.
	* @param  None
	* @retval None
	*/
void UltDriverInit(void)
{
#if (USC_MODE == USC_MODE_ECHO)
  TIM_Config();
  GPIO_Config();
#else
  uart2_init();
  kyLinkInit(&USC_PORT_HANDLE, NULL);
  pRx = GetRxPackage(&USC_PORT_HANDLE);
#endif
}

/**
  * @brief  Compute Sonic Distance in cm.
  * @param  *d: pointer to a data used to store the distance data.
  * @retval data update status.
  */
uint8_t GetNewUltData(float *d)
{
#if (USC_MODE == USC_MODE_ECHO)
	if(_utl_data_update) {
		_utl_data_update = 0;
		*d = _high_level_time * 0.0172f;
		return 1;
	}
#else
	if((read_len = uart2_pullBytes(read_buf, 8)) > 0) {
		for(uint8_t idx = 0; idx < read_len; idx ++) {
			kylink_decode(&USC_PORT_HANDLE, read_buf[idx]);
		}
		if(kyLinkCheckUpdate(&USC_PORT_HANDLE) == kyTRUE) {
			*d = pRx->FormatData.PacketData.TypeData.USC_DIST_MM * 0.1f;
			return 1;
		}
	}
#endif
	return 0;
}

#if (USC_MODE == USC_MODE_ECHO)
/**
  * @brief  Configure the TIM2 Pins.
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* TIM2_CH3 pin (PA.02) configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Connect TIM pins to AF2 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_2);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 5999;
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock) / 100000) - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM2 PWM2 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_Pulse = 1;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

  TIM_OC3Init(TIM2, &TIM_OCInitStructure);

  /* TIM2 counter enable */
  TIM_Cmd(TIM2, ENABLE);
}

/**
  * @brief  Configure GPIO port.
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure PA.03 pins as input mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Connect EXTI3 Line to PA.03 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource3);

  /* Configure EXTI3 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI2_3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  This function handles External lines 2 to 3 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI2_3_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line3) != RESET) {
    /* Clear the EXTI line 3 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line3);

    if(_detect_step == 0) {
      /* Rising Falling edge */
      EXTI->RTSR |= EXTI_Line3;
      EXTI->FTSR |= EXTI_Line3;

      _start_time = _Get_Micros();

      _detect_step = 1;
    } else if(_detect_step == 1) {
      *(__IO uint32_t *)((uint32_t)EXTI_BASE + EXTI_Trigger_Rising) |= EXTI_Line3;
      _high_level_time = _Get_Micros() - _start_time;
      _utl_data_update = 1;

      _detect_step = 0;
    }
  }
}
#endif

/******************************** END OF FILE *********************************/
