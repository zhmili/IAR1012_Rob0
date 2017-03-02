/*
*********************************************************************************************************
* Filename    : os_can_buf.c
* Version     : V1.00
* Programmer(s) : shijiangxiao    2008-02-20
                  shijiangxiao    2008-04-01  �޸ĳ���, ��ǿģ�������
*********************************************************************************************************
*/

#include <includes.h>
#include "ucosii_buf.h"
#include "os_can_buf.h"

static CO_MSG s_stcRxCanMsg[CAN_RX_BUF_SIZE];
static void *pstcRxQueueBuf[CAN_RX_BUF_SIZE];
static CO_MSG s_stcTxCanMsg[CAN_TX_BUF_SIZE];
static void *pstcTxQueueBuf[CAN_TX_BUF_SIZE];


static TUcosiiBuf  s_stcCanRxBuf;
static TUcosiiBuf  s_stcCanTxBuf;

/*******************************************************************************
            ��������ջ����ʼ��
*******************************************************************************/
BOOLEAN   CAN_RxBufInit(void)
{
  return UCOS_BufCreate(&s_stcCanRxBuf, pstcRxQueueBuf, s_stcRxCanMsg, CAN_RX_BUF_SIZE, sizeof(CO_MSG));
}

BOOLEAN   CAN_TxBufInit(void)
{
  return UCOS_BufCreate(&s_stcCanTxBuf, pstcTxQueueBuf, s_stcTxCanMsg, CAN_TX_BUF_SIZE, sizeof(CO_MSG));
}


/*******************************************************************************
            ����֡�����֡���浽����
            pmsg : �������֡��ַ
      EBufErrCode: ����״̬
*******************************************************************************/
EBufErrCode  CAN_MsgToRxBuf(CO_MSG *pmsg)
{
  return  UCOS_BufIn(&s_stcCanRxBuf, pmsg) ;
}

EBufErrCode  CAN_MsgToTxBuf(CO_MSG *pmsg)
{
  return  UCOS_BufIn(&s_stcCanTxBuf, pmsg) ;
}


/*******************************************************************************
            �ӻ����ж�ȡ����֡�����֡
            pmsg : ֡���յ�ַ
           bWait : �Ƿ�ȴ�����
         timeout : bWait = TRUEʱ��Ч, Ϊ�ȴ�ʱ��(ms)
      EBufErrCode: ����״̬
*******************************************************************************/
EBufErrCode  CAN_MsgFromRxBuf(CO_MSG *pmsg, BOOLEAN bWait, INT32U timeout)
{
  return  UCOS_BufOut(&s_stcCanRxBuf, pmsg, bWait, timeout);
}

EBufErrCode  CAN_MsgFromTxBuf(CO_MSG *pmsg, BOOLEAN bWait, INT32U timeout)
{
  return  UCOS_BufOut(&s_stcCanTxBuf, pmsg, bWait, timeout);
}



/*******************************************************************************
            ��շ�������ջ���
            fnProcess:  �������֡�Ĵ�����
*******************************************************************************/
void CAN_RxBufFlush(PROCESS_MSG *fnProcess)
{
  CO_MSG msg, *pmsg;
  pmsg = &msg;

  while (BUF_MEMORY_EMPTY != UCOS_BufOut(&s_stcCanRxBuf, pmsg, FALSE, 0))
  {
    if (fnProcess != NULL)
    {
      fnProcess(pmsg);
    }
  }
}

void CAN_TxBufFlush(PROCESS_MSG *fnProcess)
{
  CO_MSG msg, *pmsg;
  pmsg = &msg;

  while (BUF_MEMORY_EMPTY != UCOS_BufOut(&s_stcCanTxBuf, pmsg, FALSE, 0))
  {
    if (fnProcess != NULL)
    {
      fnProcess(pmsg);
    }
  }
}
