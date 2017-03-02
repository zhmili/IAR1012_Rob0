#ifndef __USART_H
#define	__USART_H


#include "stm32f10x.h"
#include <stdio.h>


/********************USART参数定义**************************/
#define             macUSART_BAUD_RATE                       115200

#define             macUSARTx                                UART5
#define             macUSART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             macUSART_CLK                             RCC_APB1Periph_UART5
#define             macUSART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macUSART_GPIO_CLK                        RCC_APB2Periph_GPIOA     
#define             macUSART_TX_PORT                         GPIOC   
#define             macUSART_TX_PIN                          GPIO_Pin_12
#define             macUSART_RX_PORT                         GPIOD 
#define             macUSART_RX_PIN                          GPIO_Pin_2

#define	RX_485 		GPIO_ResetBits(GPIOA,GPIO_Pin_15);			//初始化定位低电平接收状态
#define	TX_485 		GPIO_SetBits(GPIOA,GPIO_Pin_15);			//初始化定位低电平接收状态

/**************************函数声明********************************/
void UXART_Init(USART_TypeDef* UXARTx,u32 USART_BaudRate,u16 USART_WordLength,u16 USART_StopBits,u16 USART_Parity,u8 IT_RX_EN,u8 IT_TX_EN);

void UART485_Config(int baud_rate);	//UART4

void USARTx_Config(int baud_rate);		//UART5


#endif /* __USART1_H */
