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

void  USART_TxBufInit(void);     //���ͻ����ʼ��
void  USART_RxBufInit(void);     //���ջ����ʼ��


/*******************************************************************************
* Function Name  : UART0Getch
  �õ����ڽ������� 

  ����:  < 0: ����������
         >=0: ���յ��Ĵ�������
*******************************************************************************/
int   UART0Getch(void);         

/*******************************************************************************
* Function Name  : UartOutFrame
  �������֡����
         pdata:  �����͵�����ͷ��ַ
         size:   �����͵����ݳ���
         USARTx: ��ѡ�еĴ���
  ����:  TRUE:   ���ͳɹ�
        FALSE��  ����ʧ��
*******************************************************************************/
int   UartOutFrame(INT8U *pdata, INT16U size, USART_TypeDef *USARTx);

#ifdef DMA_USE_UART1
void  USART1_IRQHandler(void);
void  DMAChannel5_IRQHandler(void);	//DMA5�жϴ��� (USART1 RX)
void  DMAChannel4_IRQHandler(void);	//DMA4�жϴ���	   (USART1 Tx)
#endif

#ifdef DMA_USE_UART2
void  USART2_IRQHandler(void); 
void  DMAChannel6_IRQHandler(void);     //DMA6�жϴ���    (USART2 Rx)
void  DMAChannel7_IRQHandler(void);     //DMA7�жϴ���    (USART2 Tx)
#endif

#ifdef DMA_USE_UART3
void  USART3_IRQHandler(void);
void  DMAChannel3_IRQHandler(void);     //DMA3�жϴ���    (USART3 Rx)
void  DMAChannel2_IRQHandler(void);     //DMA2�жϴ���    (USART3 Tx)
#endif

//�ⲿ�ӿں���, ��������485���ջ�ر�485����.
extern void  Receive485Enable(void);
extern void  Receive485Disable(void);

#endif
