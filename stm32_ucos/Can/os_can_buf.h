/*
*********************************************************************************************************
*
* Filename      : os_can_buf
* Version       : V1.00
* Programmer(s) : shijiangxiao, 2008-04-01
*********************************************************************************************************
*/

#ifndef STM32_CAN_BUF_H
#define STM32_CAN_BUF_H

#include "co_drv.h"        //for CO_MSG definition

#ifndef CAN_RX_BUF_SIZE
#define CAN_RX_BUF_SIZE  10
#endif

#ifndef CAN_TX_BUF_SIZE
#define CAN_TX_BUF_SIZE  10
#endif

/*******************************************************************************
            ��������ջ����ʼ��
*******************************************************************************/
BOOLEAN      CAN_RxBufInit(void);    //���ջ����ʼ��
BOOLEAN      CAN_TxBufInit(void);    //���ͻ����ʼ��

/*******************************************************************************
            ����֡�����֡���浽����
            pmsg : �������֡��ַ
      EBufErrCode: ����״̬
*******************************************************************************/
EBufErrCode  CAN_MsgToRxBuf(CO_MSG *pmsg);        //CAN��Ϣ�洢�����ջ���
EBufErrCode  CAN_MsgToTxBuf(CO_MSG *pmsg);        //CAN��Ϣ�洢�����ͻ���

/*******************************************************************************
            �ӻ����ж�ȡ����֡�����֡
            pmsg : ֡���յ�ַ
           bWait : �Ƿ�ȴ�����
         timeout : bWait = TRUEʱ��Ч, Ϊ�ȴ�ʱ��(ms)
      EBufErrCode: ����״̬
*******************************************************************************/
EBufErrCode  CAN_MsgFromRxBuf(CO_MSG *pmsg, BOOLEAN bWait, INT32U timeout);
EBufErrCode  CAN_MsgFromTxBuf(CO_MSG *pmsg, BOOLEAN bWait, INT32U timeout);


/*******************************************************************************
            ��շ�������ջ���
            fnProcess:  �������֡�Ĵ�����
*******************************************************************************/
typedef void PROCESS_MSG(CO_MSG *pmsg);
void         CAN_RxBufFlush(PROCESS_MSG *fnProcess);	 //������ջ��������е�֡
void         CAN_TxBufFlush(PROCESS_MSG *fnProcess);	 //������ջ��������е�֡


#endif
