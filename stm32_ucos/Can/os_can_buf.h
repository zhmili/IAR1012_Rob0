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
            发送与接收缓冲初始化
*******************************************************************************/
BOOLEAN      CAN_RxBufInit(void);    //接收缓冲初始化
BOOLEAN      CAN_TxBufInit(void);    //发送缓冲初始化

/*******************************************************************************
            发送帧与接收帧保存到缓冲
            pmsg : 待保存的帧地址
      EBufErrCode: 缓冲状态
*******************************************************************************/
EBufErrCode  CAN_MsgToRxBuf(CO_MSG *pmsg);        //CAN消息存储到接收缓冲
EBufErrCode  CAN_MsgToTxBuf(CO_MSG *pmsg);        //CAN消息存储到发送缓冲

/*******************************************************************************
            从缓冲中读取发送帧与接收帧
            pmsg : 帧接收地址
           bWait : 是否等待接收
         timeout : bWait = TRUE时有效, 为等待时间(ms)
      EBufErrCode: 缓冲状态
*******************************************************************************/
EBufErrCode  CAN_MsgFromRxBuf(CO_MSG *pmsg, BOOLEAN bWait, INT32U timeout);
EBufErrCode  CAN_MsgFromTxBuf(CO_MSG *pmsg, BOOLEAN bWait, INT32U timeout);


/*******************************************************************************
            清空发送与接收缓冲
            fnProcess:  对所清除帧的处理函数
*******************************************************************************/
typedef void PROCESS_MSG(CO_MSG *pmsg);
void         CAN_RxBufFlush(PROCESS_MSG *fnProcess);	 //清除接收缓冲中所有的帧
void         CAN_TxBufFlush(PROCESS_MSG *fnProcess);	 //清除接收缓冲中所有的帧


#endif
