/*
*********************************************************************************************************
* Filename    : stm32_can_irq.c
* Version     : V1.00
* Programmer(s) : shijiangxiao    2008-04-01    
*********************************************************************************************************
*/
#include <includes.h>
#include "co_drv.h"
#include "os_can_flag.h"
#include "os_can_buf.h"
#include "stm32_can_drv.h"

/* CAN Mailbox Transmit Request */
#define CAN_TMIDxR_TXRQ  ((u32)0x00000001)    /* Transmit mailbox request */
#define CAN_MSR_ERRI     ((u32)0x00000004)    /* Error interrupt */


static void CAN__ReceiveFifoMsg(INT8U Can_Fifo)
{
  CO_MSG stcMsg;
  INT8U  err;
  OS_CPU_SR  cpu_sr;

  OS_ENTER_CRITICAL();                 /* Tell uC/OS-II that we are starting an ISR         */
  OSIntNesting++;
  OS_EXIT_CRITICAL();

  if (CAN_COMsgReceive(Can_Fifo, &stcMsg))
  {
    CAN_MsgToRxBuf(&stcMsg);
    CAN_FlagPost(CAN_FLAG_RECEIVE_VALID, OS_FLAG_SET, &err);
  }

  OSIntExit();
}


/*******************************************************************************
* Function Name  : USB_HP_CAN_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts 
*         requests.
* Input        : None
* Output       : None
* Return       : None
*******************************************************************************/
void USB_HP_CAN_TX_IRQHandler(void)
{ 
  INT32U dwTSR;
  INT32U dwFlag, dwFlagClear;
  INT8U  err;
  OS_CPU_SR  cpu_sr;

  OS_ENTER_CRITICAL();                 /* Tell uC/OS-II that we are starting an ISR         */
  OSIntNesting++;
  OS_EXIT_CRITICAL();

  dwFlag    = 0;
  dwTSR    = CAN->TSR;

  dwFlag = ((dwTSR & 0xF) * CAN_FLAG_TRANSMIT_RQCP0);
  dwFlag |= (((dwTSR >> 8) & 0xF) * CAN_FLAG_TRANSMIT_RQCP1);
  dwFlag |= (((dwTSR >> 16) & 0xF) * CAN_FLAG_TRANSMIT_RQCP2);
  dwFlag |= (((dwTSR >> 26) & 0x7) * CAN_FLAG_TME0);

  dwFlagClear = (((~dwTSR >> 26) & 0x7) * CAN_FLAG_TME0);
  CAN->TSR = dwTSR;  //清除相关的中断请求

  CAN_FlagPost(dwFlag, OS_FLAG_SET, &err);
  CAN_FlagPost(dwFlagClear, OS_FLAG_CLR, &err);

  OSIntExit();
}

/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*         requests.
* Input        : None
* Output       : None
* Return       : None
*******************************************************************************/
void USB_LP_CAN_RX0_IRQHandler(void)
{   
  CAN__ReceiveFifoMsg(CAN_FIFO0);
}

/*******************************************************************************
* Function Name  : CAN_RX1_IRQHandler
* Description    : This function handles CAN RX1 interrupt request.
* Input        : None
* Output       : None
* Return       : None
*******************************************************************************/
void CAN_RX1_IRQHandler(void)
{ 
  CAN__ReceiveFifoMsg(CAN_FIFO1);
}

/*******************************************************************************
* Function Name  : CAN_SCE_IRQHandler
* Description    : This function handles CAN SCE interrupt request.
* Input        : None
* Output       : None
* Return       : None
*******************************************************************************/
void CAN_SCE_IRQHandler(void)
{
  INT32U dwErrCode;
  INT8U  LastErrCode;
  INT8U  err;
  OS_CPU_SR  cpu_sr;

  INT32U dwFlag = 0;

  OS_ENTER_CRITICAL();                 /* Tell uC/OS-II that we are starting an ISR         */
  OSIntNesting++;
  OS_EXIT_CRITICAL();

  dwErrCode = CAN->ESR;

  dwFlag |= (dwErrCode & 0x7);  //CAN_FLAG_ESR_EWGF     ((u32)0x00000001)    /* Error warning flag */
                                //CAN_FLAG_ESR_EPVF     ((u32)0x00000002)    /* Error passive flag */
                                //CAN_FLAG_ESR_BOFF ((u32)0x00000004)        /* Bus-off flag */

  LastErrCode = ((dwErrCode & 0x70) >> 4); 
  if (LastErrCode != 0 && LastErrCode != 7)
  {
    dwFlag |=  (CAN_FLAG_STUFF_ERROR << (LastErrCode - 1));
  } 
  if ((dwErrCode >> 24) >= 96)
  {
    dwFlag |= CAN_FLAG_RX_ERR_REACH;
  } 
  if (((dwErrCode >> 16) & 0xFF) >= 96)
  {
    dwFlag |= CAN_FLAG_TX_ERR_REACH;
  }

  if (dwFlag & CAN_FLAG_ESR_EWGF)
  {
    CAN_ITConfig (CAN_IT_EWG, DISABLE);
  }
  if (dwFlag & CAN_FLAG_ESR_EPVF)
  {
    CAN_ITConfig (CAN_IT_EPV, DISABLE);
  }
  if (dwFlag & CAN_FLAG_ESR_BOFF)
  {
    CAN_ITConfig (CAN_IT_BOF, DISABLE);
  }

  CAN->ESR  = (dwErrCode | (0x7 << 4));       //清除各类错误中断源
  CAN->MSR  = CAN_MSR_ERRI;                   //清错误中断源rc_w1

  CAN_FlagPost(dwFlag, OS_FLAG_SET, &err);
  OSIntExit();              /* Tell uC/OS-II that we are leaving the ISR         */
}
