/*
 * uart2.c
 *
 *  Created on: Feb 19, 2019
 *      Author: kychu
 */

#include "uart2.h"

static uint8_t _uart2_init_flag = 0;
static PortRecvByteCallback pCallback = 0;

void uart2_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	if(_uart2_init_flag == 1) return; // already init.

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(UART2_GPIO_CLK, ENABLE);

	/* Connect pin to Periph */
	GPIO_PinAFConfig(UART2_GPIO, UART2_TxPin_AF, UART2_AF);
	GPIO_PinAFConfig(UART2_GPIO, UART2_RxPin_AF, UART2_AF);

	/* Configure UART2 pins as AF pushpull */
	GPIO_InitStructure.GPIO_Pin = UART2_RxPin | UART2_TxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(UART2_GPIO, &GPIO_InitStructure);

	/* Enable UART2 Clock */
	UART2_CLK_CMD(UART2_CLK, ENABLE);

	/* UART2 configuration -------------------------------------------*/
  /* UART2 configured as follow:
        - BaudRate = 115200
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive && Transmit enabled
  */

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* Configure UART2 */
	USART_Init(UART2, &USART_InitStructure);

	/* Enable the TOF_USART Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = UART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the UART2 Receive Interrupt */
	USART_ITConfig(UART2, USART_IT_RXNE, ENABLE);

	/* Enable UART2 */
	USART_Cmd(UART2, ENABLE);

	_uart2_init_flag = 1;
}

void uart2_TxChar(uint8_t c)
{
	UART2->TDR = (c & (uint16_t)0x01FF);
	while(USART_GetFlagStatus(UART2, USART_FLAG_TXE) == RESET) {}
}

void uart2_TxBuffer(uint8_t *p, uint32_t l)
{
	while( l --) {
		uart2_TxChar(*p ++);
	}
}

void uart2_set_callback(PortRecvByteCallback p)
{
	if(p != 0) {
		pCallback = p;
	}
}

void UART2_IRQHandler(void)
{
	USART_ClearFlag(UART2, USART_FLAG_ORE | USART_FLAG_PE);
	if(USART_GetITStatus(UART2, USART_IT_RXNE) != RESET) {
		USART_ClearFlag(UART2, USART_FLAG_RXNE);
		USART_ClearITPendingBit(UART2, USART_IT_RXNE);

		if(pCallback != 0) {
			pCallback(USART_ReceiveData(UART2));
		}
	}
}
