/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : stm32_uart.c
* Version       : V1.00
* Programmer(s) : shijiangxiao	 2008-02-20
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include "stm32_uart.h"
#include "ucosii_buf.h"

#define min(a,b) ((a) < (b)) ? (a): (b);


#define USART1_DR_Base  0x40013804
#define USART2_DR_Base  0x40004404
#define USART3_DR_Base  0x40004804

static TUartRxBuf    s_stcRxBuf;   //����ױ�ջ���
static TUcosiiBuf    s_stcTxBuf;   //���ڷ��ͻ���

//���ͻ������軺������
static TUartTxFrame s_stcTxUart[MAX_TX_FRAME_NUMS];
static void   *s_pstcTxQueueBuf[MAX_TX_FRAME_NUMS];


static TUartTxFrame  s_stcUartTxFrame;
static volatile BOOLEAN s_bUartTxBufEmpty = TRUE;  //���ڷ��ͻ���ձ��



static void DMA_TxBufStart(TUartTxFrame *pUartTxFrame, USART_TypeDef *USARTx);
static void DMA_RxBufStart(void);
static int  DMA_UartOutFrame(INT8U *pdata, INT16U size, USART_TypeDef *USARTx);


void  USART_RxBufInit(void)
{
  DMA_RxBufStart();
#ifdef DMA_USE_UART1
  DMA_ClearITPendingBit(DMA_IT_GL4);
#endif
#ifdef DMA_USE_UART2
  DMA_ClearITPendingBit(DMA_IT_GL7);
#endif
#ifdef DMA_USE_UART3
  DMA_ClearITPendingBit(DMA_IT_GL2);
#endif
}

void USART_TxBufInit(void)
{
  UCOS_BufCreate(&s_stcTxBuf, &s_pstcTxQueueBuf[0], &s_stcTxUart[0], MAX_TX_FRAME_NUMS, sizeof(TUartTxFrame));

#ifdef DMA_USE_UART1
  DMA_ClearITPendingBit(DMA_IT_GL5);
#endif

#ifdef DMA_USE_UART2
  DMA_ClearITPendingBit(DMA_IT_GL6);
#endif

#ifdef DMA_USE_UART3
  DMA_ClearITPendingBit(DMA_IT_GL3);
#endif
}

/*******************************************************************************
* Function Name  : UART0Getch
  �õ����ڽ������� 

  ����:  < 0: ����������
         >=0: ���յ��Ĵ�������
*******************************************************************************/
int UART0Getch()
{
  int remains;
  INT16U count;
  INT8U  ret;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
  OS_CPU_SR  cpu_sr = 0;
#endif
  DMA_Channel_TypeDef *DMA_Select;

#ifdef DMA_USE_UART1
  DMA_Select = DMA_Channel5;
#elif defined(DMA_USE_UART2)
  DMA_Select = DMA_Channel6;
#else
  DMA_Select = DMA_Channel3;
#endif


  OS_ENTER_CRITICAL();
  if (s_stcRxBuf.DMAItems > 0)
  {
    ret = s_stcRxBuf.RxBuf[s_stcRxBuf.RxBufRd++];
  }
  else
  {
    remains = DMA_GetCurrDataCounter(DMA_Select);
    count = UART_DMA_BUF_SIZE - remains;
    if ((s_stcRxBuf.RxBufRd  % UART_DMA_BUF_SIZE) < count)
    {
      ret = s_stcRxBuf.RxBuf[s_stcRxBuf.RxBufRd++];
    }
    else
    {
      OS_EXIT_CRITICAL();
      return -1;
    }
  }
  
  if (s_stcRxBuf.RxBufRd >= UART_RX_BUF_SIZE)
  {
    s_stcRxBuf.RxBufRd = 0;
  }
  
  if (s_stcRxBuf.RxBufRd % UART_DMA_BUF_SIZE == 0)
  {
     if (s_stcRxBuf.DMAItems-- == UART_DMA_ITEMS)
     { //�������¿ռ�����ַ�, �����������ڽ���
       DMA_RxBufStart();
     }
  }

  OS_EXIT_CRITICAL();

  return ret;
}

#ifdef DMA_USE_UART1
void  DMAChannel5_IRQHandler(void)  //DMA5�жϴ��� (USART1 RX)
#elif defined(DMA_USE_UART2)
void  DMAChannel6_IRQHandler(void)    //DMA6�жϴ���    (USART2 Rx)
#else
void  DMAChannel3_IRQHandler(void)   //DMA3�жϴ���    (USART3 Rx)
#endif
{ 
  INT32U DMA_IT_GL_SEL;

#ifdef DMA_USE_UART1
  DMA_IT_GL_SEL = DMA_IT_GL5;
#elif defined(DMA_USE_UART2)
  DMA_IT_GL_SEL = DMA_IT_GL6;
#else
  DMA_IT_GL_SEL = DMA_IT_GL3;
#endif

  DMA_ClearITPendingBit(DMA_IT_GL_SEL);
  s_stcRxBuf.RxBufWr = DMA_ALIGN_SIZE(s_stcRxBuf.RxBufWr) + UART_DMA_BUF_SIZE;
  if (s_stcRxBuf.RxBufWr >= UART_RX_BUF_SIZE)
  {
    s_stcRxBuf.RxBufWr = 0;
  }

  if (++s_stcRxBuf.DMAItems < UART_DMA_ITEMS)
  { //���ڻ���δ��ʱ�������н���, ����رմ��ڽ���
    DMA_RxBufStart();
  } 
}

static void  DMAChannel_TxIRQHandler(USART_TypeDef *USARTx)   //DMA���п�Tx�жϴ���
{ 
  if (BUF_OPERATTION_OK == UCOS_BufOut(&s_stcTxBuf, &s_stcUartTxFrame, FALSE, 0))
  { //��������֡��Ҫ����
    USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
    DMA_TxBufStart(&s_stcUartTxFrame, USARTx);
  }
  else
  { //����ʵ��õ�����, ֻҪ��DMA������ʼ�����USART_IT_TC, ��ֱ�����һ���ַ�����
    //��ɺ�(��λ�̴�����)�Ż����USART_IT_TC���Ӷ�ʹ485�Ľ���������ǡ����ִ��
    s_bUartTxBufEmpty = TRUE;
    USART_ITConfig(USARTx, USART_IT_TC, ENABLE);    //�������һ���ַ���������ж�
  }
}


static void  USART_IRQHandler(USART_TypeDef *USARTx)
{
  if (SET == USART_GetITStatus(USARTx, USART_IT_TC))
  { //�����Ѿ���ɴ���, ��������485����
    USART_ClearITPendingBit(USARTx, USART_IT_TC);   //�رմ�����λ����ж�
    Receive485Enable(); 
    USART_ITConfig(USARTx, USART_IT_TC, DISABLE);
  }
}

#ifdef DMA_USE_UART1
void  DMAChannel4_IRQHandler(void) //DMA5�жϴ��� (USART1 TX)
{
  DMA_ClearITPendingBit(DMA_IT_GL4);
  DMAChannel_TxIRQHandler(USART1);
}

void  USART1_IRQHandler(void)
{
  USART_IRQHandler(USART1);
}
#endif //#ifdef DMA_USE_UART1

#ifdef DMA_USE_UART2
void  DMAChannel7_IRQHandler(void) //DMA6�жϴ��� (USART2 TX)
{
  DMA_ClearITPendingBit(DMA_IT_GL7);
  DMAChannel_TxIRQHandler(USART2);
}

void  USART2_IRQHandler(void)
{
  USART_IRQHandler(USART2);
}
#endif //#ifdef DMA_USE_UART2

#ifdef DMA_USE_UART3
void  DMAChannel2_IRQHandler(void) //DMA2�жϴ��� (USART3 TX)
{
  DMA_ClearITPendingBit(DMA_IT_GL2);
  DMAChannel_TxIRQHandler(USART3);
}

void  USART3_IRQHandler(void)
{
  USART_IRQHandler(USART3);
}
#endif //#ifdef DMA_USE_UART3

static void DMA_RxBufStart(void)
{
  INT32U RxBufOff;
  DMA_InitTypeDef DMA_InitStructure;
  DMA_Channel_TypeDef *DMA_Select;
  INT32U USART_DR_Base;

#ifdef DMA_USE_UART1
  DMA_Select    = DMA_Channel5;
  USART_DR_Base = USART1_DR_Base;
#elif defined(DMA_USE_UART2)
  DMA_Select = DMA_Channel6;
  USART_DR_Base = USART2_DR_Base;
#else
  DMA_Select    = DMA_Channel3;
  USART_DR_Base = USART3_DR_Base;
#endif

  RxBufOff = DMA_ALIGN_SIZE(s_stcRxBuf.RxBufWr);

  DMA_DeInit(DMA_Select);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr =  (u32)&s_stcRxBuf.RxBuf[RxBufOff];
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = UART_DMA_BUF_SIZE;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

  DMA_Init(DMA_Select, &DMA_InitStructure);
  DMA_Cmd(DMA_Select, ENABLE);
  DMA_ITConfig(DMA_Select, DMA_IT_TC, ENABLE);
}

static void DMA_TxBufStart(TUartTxFrame *pUartTxFrame, USART_TypeDef *USARTx)
{
  DMA_InitTypeDef DMA_InitStructure;
  DMA_Channel_TypeDef *DMA_Select;
  INT32U USART_DR_Base;

#ifdef DMA_USE_UART1
  if (USARTx == USART1)
  {
    DMA_Select = DMA_Channel4;
    USART_DR_Base = USART1_DR_Base;
  }
#endif
#ifdef DMA_USE_UART2
  if (USARTx == USART2)
  {
    DMA_Select = DMA_Channel7;
    USART_DR_Base = USART2_DR_Base;
  }
#endif
#ifdef DMA_USE_UART3
  if (USARTx == USART3)
  {
    DMA_Select = DMA_Channel2;
    USART_DR_Base = USART3_DR_Base;
  }
#endif

  DMA_DeInit(DMA_Select);
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)pUartTxFrame->data;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = pUartTxFrame->size;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA_Select, &DMA_InitStructure);

  DMA_ITConfig(DMA_Select, DMA_IT_TC | DMA_IT_TE, ENABLE);
  USART_ClearITPendingBit(USARTx, USART_IT_TC); //���USARTx������ɱ��
  DMA_Cmd(DMA_Select, ENABLE);
}


static int DMA_UartOutFrame(INT8U *pdata, INT16U size, USART_TypeDef *USARTx)
{
  TUartTxFrame stcUartTxFrame;
  TUartTxFrame *pUartTxFrame;
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
  OS_CPU_SR  cpu_sr = 0;
#endif

  if (size > UART_DMA_TX_BUF_SIZE)
  {
    return -1;
  }

  USART_ITConfig(USARTx, USART_IT_TC, DISABLE);
  Receive485Disable();

  OS_ENTER_CRITICAL();
  if (s_bUartTxBufEmpty)
  {
    s_bUartTxBufEmpty = FALSE;
    pUartTxFrame = &s_stcUartTxFrame;
    memcpy(pUartTxFrame->data, pdata, size);
    pUartTxFrame->size   = size;
    DMA_TxBufStart(pUartTxFrame, USARTx);
  }
  else
  {
    pUartTxFrame = &stcUartTxFrame;
    memcpy(pUartTxFrame->data, pdata, size);
    pUartTxFrame->size   = size;
    if (UCOS_BufIn(&s_stcTxBuf, pUartTxFrame) != BUF_OPERATTION_OK)
    {
      OS_EXIT_CRITICAL();	
      return -1;
    }
  }
  OS_EXIT_CRITICAL();

  return 0;
}

/*******************************************************************************
* Function Name  : UartOutFrame
  �������֡����
         pdata:  �����͵�����ͷ��ַ
         size:   �����͵����ݳ���
         USARTx: ��ѡ�еĴ���
  ����:  TRUE:   ���ͳɹ�
        FALSE��  ����ʧ��
*******************************************************************************/
int   UartOutFrame(INT8U *pdata, INT16U size, USART_TypeDef *USARTx)
{
  int ret;
  int frameSize;

  while (size >  0)
  { 
    frameSize = min(size, UART_DMA_TX_BUF_SIZE);

    ret = DMA_UartOutFrame(pdata, frameSize, USARTx);
    if (ret < 0)
    {
      OSTimeDly(OS_TICKS_PER_SEC / 100);
    }
    else
    {
      pdata += frameSize;
      size -= frameSize;
    }
  }
  return TRUE;
}

