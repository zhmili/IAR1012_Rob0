/*
*********************************************************************************************************
* Filename    : stm32_can_drv.c
* Version     : V1.00
* Programmer(s) : shijiangxiao    2008-04-01
*********************************************************************************************************
*/

#include <includes.h>
#include "os_can_flag.h"
#include "stm32_can_drv.h"


/* CAN Mailbox Transmit Request */
#define CAN_TMIDxR_TXRQ  ((u32)0x00000001) /* Transmit mailbox request */

/*******************************************************************************
* Function Name  : CAN_COMsgTransmit
                 pmsg:  特发送的信息帧
      TransmitMailbox:  从该邮箱发送
*******************************************************************************/
BOOLEAN CAN_COMsgTransmit(CO_MSG* pmsg, INT32U TransmitMailbox)
{
  INT32U dwCtrl;
  INT32U dwCobId;
  INT8U  err;
#if OS_CRITICAL_METHOD == 3             /* Allocate storage for CPU status register    */
  OS_CPU_SR  cpu_sr = 0;
#endif

  if (TransmitMailbox != CAN_NO_MB)
  {
    dwCtrl = CAN_TMIDxR_TXRQ;

    if (COMacIsExtended(pmsg))
    {
      dwCtrl |= 0x04;
      dwCobId = COMacGetExtId(pmsg);
      dwCtrl |= (dwCobId << 3);
    }  
    else
    {
      dwCobId = COMacGetStdId(pmsg);
      dwCtrl |= (dwCobId << 21);
    }

    if (COMacIsRemote(pmsg))
    {
      dwCtrl |= 0x02;
    }

    /* Set up the DLC */
    CAN->sTxMailBox[TransmitMailbox].TDTR &= (u32)0xFFFFFFF0;
    CAN->sTxMailBox[TransmitMailbox].TDTR |= COMacGetDlc(pmsg);

    /* Set up the data field */
    CAN->sTxMailBox[TransmitMailbox].TDLR = *(INT32U *)&pmsg->COMsgData[0];
    CAN->sTxMailBox[TransmitMailbox].TDHR = *(INT32U *)&pmsg->COMsgData[4];


    //每次启动传输后, 根据硬件动作信息, 一些标记自动清零, 需同时反应在软件上
    OS_ENTER_CRITICAL();
    switch(TransmitMailbox)
    {
     case 0:
       CAN_FlagPost(CAN_FLAG_TME0 | CAN_FLAG_TRANSMIT_RQCP0 | CAN_FLAG_TRANSMIT_TXOK0 | 
         CAN_FLAG_TRANSMIT_ALOST0 | CAN_FLAG_TRANSMIT_TERR0, OS_FLAG_CLR, &err);
       break;
     case 1:
       CAN_FlagPost(CAN_FLAG_TME1 | CAN_FLAG_TRANSMIT_RQCP1 | CAN_FLAG_TRANSMIT_TXOK1 | 
         CAN_FLAG_TRANSMIT_ALOST1 | CAN_FLAG_TRANSMIT_TERR1, OS_FLAG_CLR, &err);
       break;
     case 2:
       CAN_FlagPost(CAN_FLAG_TME2 | CAN_FLAG_TRANSMIT_RQCP2 | CAN_FLAG_TRANSMIT_TXOK2 | 
         CAN_FLAG_TRANSMIT_ALOST2 | CAN_FLAG_TRANSMIT_TERR2, OS_FLAG_CLR, &err);
       break;
    }
    // 请求进行传输
    CAN->sTxMailBox[TransmitMailbox].TIR = dwCtrl;
    OS_EXIT_CRITICAL();
  }

  return TRUE;
}


/*******************************************************************************
* Function Name  : CAN_COMsgReceive
       FIFONumber:  从该FIFO中接收帧信息 
             pmsg:  待接收的信息帧
  返回:  TRUE: 接收到有效帧
         FALSE: 没有接收到有效帧
*******************************************************************************/
BOOLEAN CAN_COMsgReceive(u8 FIFONumber, CO_MSG* RxMessage)
{
  INT8U  byDLC;
  INT32U dwCtrl;
  INT8U  byFMI;

  /* Check the parameters */
  assert_param(IS_CAN_FIFO(FIFONumber));
  if (CAN_MessagePending(FIFONumber))
  {
    return FALSE; 
  }
  
  /* Get the Id */
  dwCtrl = CAN->sFIFOMailBox[FIFONumber].RIR;
  if (dwCtrl & 0x04)
  {
    COMacSetExtId(RxMessage, (dwCtrl >> 3));
  }
  else
  {
    COMacSetStdId(RxMessage, (dwCtrl >> 21));
  }

  if (dwCtrl & 0x02)
  {
    COMacSetRemote(RxMessage);
  }

  /* Get the DLC */
  byDLC = (u32)0x0000000F & CAN->sFIFOMailBox[FIFONumber].RDTR;
  COMacSetDlc(RxMessage, byDLC);
  
  /* Get the FMI */
  byFMI = (u32)0x000000FF & (CAN->sFIFOMailBox[FIFONumber].RDTR >> 8);
  COMacSetUsrFlag(RxMessage, byFMI);

  /* Get the data field */
  *(INT32U *)&RxMessage->COMsgData[0] = CAN->sFIFOMailBox[FIFONumber].RDLR;
  *(INT32U *)&RxMessage->COMsgData[4] = CAN->sFIFOMailBox[FIFONumber].RDHR;

  /* Release the FIFO */
  CAN_FIFORelease(FIFONumber);

  return TRUE;
}
