/*
 * uart2.h
 *
 *  Created on: Feb 19, 2019
 *      Author: kychu
 */

#ifndef UART2_H_
#define UART2_H_

#include "SysConfig.h"

#define UART2                               USART2
#define UART2_GPIO                          GPIOA
#define UART2_AF                            GPIO_AF_1
#define UART2_GPIO_CLK                      RCC_AHBPeriph_GPIOA
#define UART2_CLK                           RCC_APB1Periph_USART2
#define UART2_CLK_CMD                       RCC_APB1PeriphClockCmd
#define UART2_RxPin                         GPIO_Pin_3
#define UART2_TxPin                         GPIO_Pin_2
#define UART2_RxPin_AF                      GPIO_PinSource3
#define UART2_TxPin_AF                      GPIO_PinSource2
#define UART2_IRQn                          USART2_IRQn
#define UART2_IRQHandler                    USART2_IRQHandler

typedef void (*PortRecvByteCallback)(uint8_t Data);

void uart2_init(void);
void uart2_TxChar(uint8_t c);
void uart2_TxBuffer(uint8_t *p, uint32_t l);
void uart2_set_callback(PortRecvByteCallback p);

#endif /* UART2_H_ */
