/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : stm32_uart.h
* Version       : V1.00
* Programmer(s) : shijiangxiao
*********************************************************************************************************
*/

#ifndef STM32_UART_H
#define STM32_UART_H

#ifndef UART_DMA_BUF_SIZE
#define UART_DMA_BUF_SIZE 64
#endif

#ifndef UART_DMA_ITEMS
#define UART_DMA_ITEMS    16
#endif

#ifndef MAX_TX_FRAME_NUMS
#define MAX_TX_FRAME_NUMS   16
#endif

#ifndef UART_DMA_TX_BUF_SIZE
#define UART_DMA_TX_BUF_SIZE   64
#endif


#define UART_RX_BUF_SIZE  (UART_DMA_BUF_SIZE * UART_DMA_ITEMS) 
#define DMA_ALIGN_SIZE(size) ((size / UART_DMA_BUF_SIZE) * UART_DMA_BUF_SIZE)

typedef struct TUartRxBuf
{
  INT8U  RxBuf[UART_RX_BUF_SIZE];
  INT16U RxBufRd;
  INT16U RxBufWr;
  INT16S DMAItems;
} TUartRxBuf;	

typedef struct TUartTxFrame
{
  INT8U  data[UART_DMA_TX_BUF_SIZE];
  INT32U size;
} TUartTxFrame;

void  USART_TxBufInit(void);     //发送缓冲初始化
void  USART_RxBufInit(void);     //接收缓冲初始化


/*******************************************************************************
* Function Name  : UART0Getch
  得到串口接收数据 

  返回:  < 0: 串口无数据
         >=0: 接收到的串口数据
*******************************************************************************/
int   UART0Getch(void);         

/*******************************************************************************
* Function Name  : UartOutFrame
  串口输出帧函数
         pdata:  待发送的数据头地址
         size:   待发送的数据长度
         USARTx: 所选中的串口
  返回:  TRUE:   发送成功
        FALSE：  发送失败
*******************************************************************************/
int   UartOutFrame(INT8U *pdata, INT16U size, USART_TypeDef *USARTx);

#ifdef DMA_USE_UART1
void  USART1_IRQHandler(void);
void  DMAChannel5_IRQHandler(void);	//DMA5中断处理 (USART1 RX)
void  DMAChannel4_IRQHandler(void);	//DMA4中断处理	   (USART1 Tx)
#endif

#ifdef DMA_USE_UART2
void  USART2_IRQHandler(void); 
void  DMAChannel6_IRQHandler(void);     //DMA6中断处理    (USART2 Rx)
void  DMAChannel7_IRQHandler(void);     //DMA7中断处理    (USART2 Tx)
#endif

#ifdef DMA_USE_UART3
void  USART3_IRQHandler(void);
void  DMAChannel3_IRQHandler(void);     //DMA3中断处理    (USART3 Rx)
void  DMAChannel2_IRQHandler(void);     //DMA2中断处理    (USART3 Tx)
#endif

//外部接口函数, 用于允许485接收或关闭485接收.
extern void  Receive485Enable(void);
extern void  Receive485Disable(void);

#endif
