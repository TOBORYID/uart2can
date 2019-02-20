/**
  ******************************************************************************
  * @file    ./src/TOF.c 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Infrared TOF Module Driver.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "TOF.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t _tof_data_update = 0;
static TOF_ORIGIN_DATA TOF_DATA = {0};
/* Private function prototypes -----------------------------------------------*/
static void usart_config(void);
static void _rc_data_decode(uint8_t data);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the UART to communicate with TOF module.
  * @param  None
  * @retval None
  */
void TOFDriverInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(TOF_USART_GPIO_CLK, ENABLE);

	/* Connect pin to Periph */
	GPIO_PinAFConfig(TOF_USART_GPIO, TOF_USART_TxPin_AF, TOF_USART_AF);
	GPIO_PinAFConfig(TOF_USART_GPIO, TOF_USART_RxPin_AF, TOF_USART_AF);

	/* Configure TOF_USART pins as AF pushpull */
	GPIO_InitStructure.GPIO_Pin = TOF_USART_RxPin | TOF_USART_TxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(TOF_USART_GPIO, &GPIO_InitStructure);

	usart_config();
}

/**
  * @brief  Configure the UART.
  * @param  None
  * @retval None
  */
static void usart_config(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable TOF_USART Clock */
	RCC_APB2PeriphClockCmd(TOF_USART_CLK, ENABLE);

	/* TOF USART configuration -------------------------------------------*/
  /* TOF USART configured as follow:
        - BaudRate = 115200
        - Word Length = 8 Bits
        - Two Stop Bit
        - Even parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Only Receive enabled
  */

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	/* Configure TOF_USART */
	USART_Init(TOF_USART, &USART_InitStructure);

	/* Enable the TOF_USART Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TOF_USART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the TOF_USART Receive Interrupt */
	USART_ITConfig(TOF_USART, USART_IT_RXNE, ENABLE);

	/* Enable TOF_USART */
	USART_Cmd(TOF_USART, ENABLE);
}

/**
  * @brief  Read a data from module.
  * @param  *d: pointer to a memory to store the TOF distance.
  * @retval data update flag.
  */
uint8_t GetNewTOFData(float *d)
{
	if(_tof_data_update) {
		_tof_data_update = 0;
		*d = (((uint16_t)TOF_DATA.HighByte << 8) | TOF_DATA.LowByte) / 10.0f;
		return 1;
	}
	return 0;
}

/**
  * @brief  com package decode function.
  * @param  data: byte read from module.
  * @retval None
  */
static uint8_t _data_step = 0;
static TOF_ORIGIN_DATA _tof_recv = {0};
static void _rc_data_decode(uint8_t data)
{
	switch(_data_step) {
		case 0: {
			if(data == 0xA5) {
				_tof_recv.Header1 = data;
				_data_step ++;
			}
		} break;
		case 1: {
			if(data == 0x5A) {
				_tof_recv.Header2 = data;
				_data_step ++;
			} else {
				_data_step = 0;
			}
		} break;
		case 2: {
			_tof_recv.HighByte = data;
			_data_step ++;
		} break;
		case 3: {
			_tof_recv.LowByte = data;
			_data_step ++;
		} break;
		case 4: {
			_tof_recv.LowCRC = data;
			_data_step ++;
		} break;
		case 5: {
			_tof_recv.HighCRC = data;
			if((((uint16_t)_tof_recv.HighCRC << 8) | _tof_recv.LowCRC) == crc16((uint8_t *)&_tof_recv, 4)) {
				TOF_DATA = _tof_recv;
				_tof_data_update = 1;
			}
			_data_step = 0;
		} break;
		default: _data_step = 0; break;
	}
}

/**
  * @brief  This function handles the interrupt request from TOF port.
  * @param  None
  * @retval None
  */
void TOF_USART_IRQHandler(void)
{
	USART_ClearFlag(TOF_USART, USART_FLAG_ORE | USART_FLAG_PE);
	if(USART_GetITStatus(TOF_USART, USART_IT_RXNE) != RESET) {
		USART_ClearFlag(TOF_USART, USART_FLAG_RXNE);
		USART_ClearITPendingBit(TOF_USART, USART_IT_RXNE);
		_rc_data_decode(USART_ReceiveData(TOF_USART));
	}
}

/******************************** END OF FILE *********************************/
