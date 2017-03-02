/*
*********************************************************************************************************
* Filename    : stm32_can.c  CAN�������������������
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
{ //�ȴ������
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
    eBufErrCode = CAN_MsgFromTxBuf(&msg, bWait, 0); //�ӷ��ͻ�����ȡ��һ֡����, û����ʱ���ڵȴ�״̬
    
    if (eBufErrCode ==    BUF_OPERATTION_OK)
    { //�Ѿ�ȡ����Ч����
     CAN_FlagPend(CAN_FLAG_TME(TransmitMailbox), OS_FLAG_WAIT_SET_ANY, 0, &err);  //����ȴ������
     WaitTransmitMailBoxEmpty(TransmitMailbox);       //Ӳ���ȴ������
    
     CAN_COMsgTransmit(&msg, TransmitMailbox);        //���趨���䷢������
     //�˴��ȴ����ݷ������
     dwPendFlag = (CAN_FLAG_TRANSMIT_TXOK0 | CAN_FLAG_TRANSMIT_ALOST0 | 
                   CAN_FLAG_TRANSMIT_TERR0 | CAN_FLAG_TRANSMIT_RQCP0) << (4 * TransmitMailbox);
     dwPendFlag |= CAN_FLAG_ACK_ERROR;

     if (msg.COUsrData == 0)
     {
       wTimeOut = CAN_TX_TIMEOUT_DEFAULT;
     }
     else
     {
       wTimeOut = COMacGetUsrFlag(&msg);   //�û����Ϊ��ʱʱ��, Ϊ0ʱ��ʾ���޵ȴ�
     }
     
     dwFlag = CAN_FlagPend(dwPendFlag,  OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME, 
             wTimeOut, &err);

     eCanErrCode = CO_ERR_TRANS_FAILED;
     if (dwFlag == 0)
     { //���䳬ʱ, ȡ���˴δ���, ���ȴ������, Ȼ�����û���ձ��
       eCanErrCode = CO_ERR_TRANS_TIMEOUT;
     }
     else if (dwFlag & CAN_FLAG_ACK_ERROR)
     { //�����޻�Ӧ��Ӧ, ��ֹͣ����
       eCanErrCode = CO_ERR_TRANS_ACK;
     }
     else if (dwFlag & CAN_FLAG_ERROR)
     {    
       eCanErrCode = CO_ERR_TRANS_FAILED;
     }
     else if (dwFlag & CAN_FLAG_TXOK(TransmitMailbox))
     { //����ɹ�
       eCanErrCode = CO_TRANS_SUCESS; 
     } 
     else if (dwFlag & CAN_FLAG_TERR(TransmitMailbox))
     { //������
        eCanErrCode = CO_ERR_TRANS_FAILED;
     }
     else if (dwFlag & CAN_FLAG_ALOST(TransmitMailbox))
     {    //�ٲö�ʧ
        eCanErrCode = CO_ERR_TRANS_ALOST;
     }

     if (eCanErrCode != CO_NO_ERR)
     {    //ȡ���˴η���
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
**    0�����䷢������
********************************************************************************************************/
void  AppCanTransmit0(void *p_arg)
{
  AppCanTransmit(p_arg, 0);
}

/********************************************************************************************************
**    1�����䷢������
********************************************************************************************************/
void  AppCanTransmit1(void *p_arg)
{
  AppCanTransmit(p_arg, 1);
}

/********************************************************************************************************
**    2�����䷢������
********************************************************************************************************/
void  AppCanTransmit2(void *p_arg)
{
  AppCanTransmit(p_arg, 2);
}

/********************************************************************************************************
**    AppCanErrorHandle
**    CANͨѶ������, 
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
    { //��������״̬
      
    }
    if (dwFlag & CAN_FLAG_ESR_EWGF)
    { //Error warning flag
  
    }
    if (dwFlag & CAN_FLAG_ESR_BOFF)
    { //Error passive flag
      
    }
  }
}
