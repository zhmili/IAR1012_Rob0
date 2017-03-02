/*
*********************************************************************************************************
* Filename    : stm32_can.c  CAN发送任务与错误处理任务
* Version     : V1.00
* Programmer(s) : shijiangxiao    2008-02-20
*********************************************************************************************************
*/
#include <includes.h>
#include "stm32_can.h"
#include "stm32_can_drv.h"
#include "os_can_flag.h"
#include "os_can_buf.h"

#define CAN_FLAG_TXOK(n)   (CAN_FLAG_TRANSMIT_TXOK0 <<  (4 * n))
#define CAN_FLAG_ALOST(n)  (CAN_FLAG_TRANSMIT_ALOST0 <<  (4 * n))
#define CAN_FLAG_TERR(n)   (CAN_FLAG_TRANSMIT_TERR0 << (4 * n))
#define CAN_FLAG_RQCP(n)   (CAN_FLAG_TRANSMIT_RQCP0 << (4 * n))
#define CAN_FLAG_TME(n)    (CAN_FLAG_TME0 << n) 
#define CAN_TSR_TME(n)     (CAN_TSR_TME0 << n)

#define CAN_TSR_TME0     ((u32)0x04000000)    /* Transmit mailbox 0 empty */
#define CAN_TSR_TME1     ((u32)0x08000000)    /* Transmit mailbox 1 empty */
#define CAN_TSR_TME2     ((u32)0x10000000)    /* Transmit mailbox 2 empty */

static void WaitTransmitMailBoxEmpty(INT8U TransmitMailbox)
{ //等待缓冲空
  while ( (CAN->TSR & CAN_TSR_TME(TransmitMailbox) ) == 0)
  {}; 
}

static void WaitForSendAbort(INT8U TransmitMailbox)
{
  INT8U err;
  CAN_CancelTransmit(TransmitMailbox);
  WaitTransmitMailBoxEmpty(TransmitMailbox);
  CAN_FlagPost(CAN_FLAG_TME(TransmitMailbox) , OS_FLAG_SET, &err);
}


static void AppCanTransmit(void *p_arg, INT8U  TransmitMailbox)
{
  CO_MSG msg;
  EBufErrCode eBufErrCode;
  INT8U  err;
  BOOLEAN bWait;
  INT32U dwFlag;
  INT32U dwPendFlag;
  INT16U wTimeOut;
  INT32U eCanErrCode;
    
  while (1)
  {
    bWait = TRUE;
    eBufErrCode = CAN_MsgFromTxBuf(&msg, bWait, 0); //从发送缓冲中取出一帧数据, 没数据时则处于等待状态
    
    if (eBufErrCode ==    BUF_OPERATTION_OK)
    { //已经取出有效数据
     CAN_FlagPend(CAN_FLAG_TME(TransmitMailbox), OS_FLAG_WAIT_SET_ANY, 0, &err);  //软件等待缓冲空
     WaitTransmitMailBoxEmpty(TransmitMailbox);       //硬件等待缓冲空
    
     CAN_COMsgTransmit(&msg, TransmitMailbox);        //从设定邮箱发送数据
     //此处等待数据发送完成
     dwPendFlag = (CAN_FLAG_TRANSMIT_TXOK0 | CAN_FLAG_TRANSMIT_ALOST0 | 
                   CAN_FLAG_TRANSMIT_TERR0 | CAN_FLAG_TRANSMIT_RQCP0) << (4 * TransmitMailbox);
     dwPendFlag |= CAN_FLAG_ACK_ERROR;

     if (msg.COUsrData == 0)
     {
       wTimeOut = CAN_TX_TIMEOUT_DEFAULT;
     }
     else
     {
       wTimeOut = COMacGetUsrFlag(&msg);   //用户标记为延时时间, 为0时表示无限等待
     }
     
     dwFlag = CAN_FlagPend(dwPendFlag,  OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME, 
             wTimeOut, &err);

     eCanErrCode = CO_ERR_TRANS_FAILED;
     if (dwFlag == 0)
     { //传输超时, 取消此次传输, 并等待缓冲空, 然后设置缓冲空标记
       eCanErrCode = CO_ERR_TRANS_TIMEOUT;
     }
     else if (dwFlag & CAN_FLAG_ACK_ERROR)
     { //总线无回应响应, 则停止发送
       eCanErrCode = CO_ERR_TRANS_ACK;
     }
     else if (dwFlag & CAN_FLAG_ERROR)
     {    
       eCanErrCode = CO_ERR_TRANS_FAILED;
     }
     else if (dwFlag & CAN_FLAG_TXOK(TransmitMailbox))
     { //传输成功
       eCanErrCode = CO_TRANS_SUCESS; 
     } 
     else if (dwFlag & CAN_FLAG_TERR(TransmitMailbox))
     { //错误传送
        eCanErrCode = CO_ERR_TRANS_FAILED;
     }
     else if (dwFlag & CAN_FLAG_ALOST(TransmitMailbox))
     {    //仲裁丢失
        eCanErrCode = CO_ERR_TRANS_ALOST;
     }

     if (eCanErrCode != CO_NO_ERR)
     {    //取消此次发送
       WaitForSendAbort(TransmitMailbox);
     }

     if (msg.COUsrData != 0)
     {
       OSMboxPost((OS_EVENT *)msg.COUsrData, (void *)eCanErrCode);
     }
    }
  }
}


/********************************************************************************************************
**    0号邮箱发送任务
********************************************************************************************************/
void  AppCanTransmit0(void *p_arg)
{
  AppCanTransmit(p_arg, 0);
}

/********************************************************************************************************
**    1号邮箱发送任务
********************************************************************************************************/
void  AppCanTransmit1(void *p_arg)
{
  AppCanTransmit(p_arg, 1);
}

/********************************************************************************************************
**    2号邮箱发送任务
********************************************************************************************************/
void  AppCanTransmit2(void *p_arg)
{
  AppCanTransmit(p_arg, 2);
}

/********************************************************************************************************
**    AppCanErrorHandle
**    CAN通讯出错处理, 
********************************************************************************************************/
void  AppCanErrorHandle(void *p_arg)
{
  INT8U  err;
  INT32U dwFlag;
  while(1)
  {
    dwFlag = CAN_FlagPend( CAN_FLAG_ESR_EWGF | CAN_FLAG_ESR_EPVF | CAN_FLAG_ESR_BOFF | 
             CAN_FLAG_STUFF_ERROR | CAN_FLAG_FORM_ERROR | CAN_FLAG_ACK_ERROR | 
             CAN_FLAG_BIT_RECESSIVE_ERROR | CAN_FLAG_BIT_DOMINANT_ERROR | 
             CAN_FLAG_CRC_ERROR | CAN_FLAG_ERROR, OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME, 0, &err);
    if (dwFlag & CAN_FLAG_ESR_BOFF)
    { //总线离线状态
      
    }
    if (dwFlag & CAN_FLAG_ESR_EWGF)
    { //Error warning flag
  
    }
    if (dwFlag & CAN_FLAG_ESR_BOFF)
    { //Error passive flag
      
    }
  }
}
