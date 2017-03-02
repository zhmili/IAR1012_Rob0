#ifndef CAN232_H
#define CAN232_H

#include "co_drv.h"

typedef enum ECan232Command
{
	CCM_FILTER_RESET,
	CCM_FILTER_ALL  = 0x01,			
	CCM_FILTER_ADD  = 0x03,			
	CCM_FILTER_REMOVE        = 0x05,
	CCM_SEND_REPEAT          = 0x07,
	CCM_SEND_SINGLE          = 0x09,
	CCM_RECV_FRAME           = 0x0B,
	CCM_BAUD_SET             = 0x0C,
	CCM_RESET                = 0x0D,
	CCM_RECV_ERR,
}ECan232Command;


typedef struct TCommFrame
{
	ECan232Command cmd;
	union	cmdpara
	{
	  INT16U timeout;
	  INT16U repeat_time;
	  INT8U  baud;
	  INT16U errCode;
	} para;
    CO_MSG stcCanMsg;
} TCommFrame;

typedef struct TCanStandard
{
  INT32U fRtr :1;
  INT32U fExt :1;
  INT32U     :3;
  INT32U COBID :11;
  INT32U       :16;
} TCanStandard;

typedef struct TCanExtended
{
  INT32U fRtr :1;
  INT32U fExt :1;
  INT32U     :1;
  INT32U COBID :29;
} TCanExtended;

#define PROBE_RS232_RX_BUF_SIZE  100  //最大接收通讯帧长度, 为静态分配内存
#define PROBE_RS232_TX_BUF_SIZE  100  //最大发送帧长度, 为动态分配内存或堆栈内存


int  ProbeRS232_TxSendCommand (TCommFrame *pstcCommFrame);


#endif //#ifndef CAN232_H
