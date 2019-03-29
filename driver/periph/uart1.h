/*
 * uart1.h
 *
 *  Created on: Feb 19, 2019
 *      Author: kychu
 */

#ifndef UART1_H_
#define UART1_H_

#include "SysConfig.h"

#define UART1_DMA_ENABLE                    (1)

#define UART1                               USART1
#define UART1_GPIO                          GPIOB
#define UART1_AF                            GPIO_AF_0
#define UART1_GPIO_CLK                      RCC_AHBPeriph_GPIOB
#define UART1_CLK                           RCC_APB2Periph_USART1
#define UART1_CLK_CMD                       RCC_APB2PeriphClockCmd
#define UART1_RxPin                         GPIO_Pin_7
#define UART1_TxPin                         GPIO_Pin_6
#define UART1_RxPin_AF                      GPIO_PinSource7
#define UART1_TxPin_AF                      GPIO_PinSource6
#define UART1_IRQn                          USART1_IRQn
#define UART1_IRQHandler                    USART1_IRQHandler

#if UART1_DMA_ENABLE
#define UART1_TX_DMA                        DMA1_Channel2
#define UART1_RX_DMA                        DMA1_Channel3
#define UART1_DMA_CLK                       RCC_AHBPeriph_DMA1
#define UART1_DMA_CLK_CMD                   RCC_AHBPeriphClockCmd
#define UART1_TX_DMA_IT_TC_FLAG             DMA1_IT_TC2
#define UART1_RX_DMA_IT_HT_FLAG             DMA1_IT_HT3
#define UART1_RX_DMA_IT_TC_FLAG             DMA1_IT_TC3
#define UART1_DMA_IRQn                      DMA1_Channel2_3_IRQn   /*!< DMA1 Channel 2, Channel 3 Interrupts */
#define UART1_DMA_IRQHandler                DMA1_Channel2_3_IRQHandler
#endif /* UART1_DMA_ENABLE */

void uart1_init(
#if UART1_DMA_ENABLE
		void
#else
		PortRecvByteCallback p
#endif
		);

void uart1_TxByte(uint8_t c);
void uart1_TxBytes(uint8_t *p, uint32_t l);
#if UART1_DMA_ENABLE
void uart1_flush(void);
void uart1_TxBytesDMA(uint8_t *p, uint32_t l);
uint8_t uart1_pullByte(uint8_t *p);
uint32_t uart1_pullBytes(uint8_t *p, uint32_t l);
#else
#define uart1_TxBytesDMA uart1_TxBytes
void uart1_set_callback(PortRecvByteCallback p);
#endif /* UART1_DMA_ENABLE */

#endif /* UART1_H_ */
