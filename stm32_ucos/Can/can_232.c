/*
*********************************************************************************************************
*
*                                              uC/Probe
*
*                                      Communication: RS-232
*
* Filename      : probe_rs232.c
* Version       : V1.20
* Programmer(s) : Brian Nagel
* Note(s)       : (1) The abbreviations RX and TX refer to communication from the target's perspective.
*
*                 (2) The abbreviations RD and WR refer to reading data from the target memory and
*                     writing data to the target memory, respectively.
*********************************************************************************************************
*/

#include <includes.h>
#include <can_232.h>



/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
#define WIRTE_INT16U(pdata, val)   pdata[0] = ((val) & 0xFF);		   \
                                   pdata[1] = (((val) >> 8) & 0xff)

#define WIRTE_INT32U(pdata, val)   pdata[0] = ((val) & 0xFF);		   \
                                   pdata[1] = (((val) >> 8) & 0xff);   \
								   pdata[2] = (((val) >> 16) & 0xff);  \
								   pdata[3] = (((val) >> 24) & 0xff)

#define LOAD_INT8U(pdata)          (pdata[0])
#define LOAD_INT16U(pdata)         (pdata[0] + (pdata[1] << 8))
#define LOAD_INT32U(pdata)         (pdata[0] + (pdata[1] << 8) + (pdata[2] << 8) + (pdata[3] << 8))
/*
*********************************************************************************************************
*                                            RS-232 PACKET FORMAT
*
* Note(s):  (1) All packets include the following parts:
*
    SD LE LEr SD DA SA FC DSAP SSAP DU FCS ED
    SD:开始符（68H） LE、Ler：长度（从DA到DU）
    
    DA：目的地址 SA：源地址
    FC：功能码 （6CH） DSAP：目的服务存取点
    SSAP：源服务存取点 DU：数据单元
    FCS：校验和 ED：结束符（16H）
*********************************************************************************************************
*/

                                                                    /* ------------- INBOUND PACKET DELIMITERS ------------ */
#define  PROBE_RS232_PROTOCOL_RX_SD0            0x68                /* (A) Start delimiters                                 */
#define  PROBE_RS232_PROTOCOL_RX_SD3            0x68
#define  PROBE_RS232_PROTOCOL_RX_ED             0x16                /* (E) End   delimiter                                  */

                                                                    /* ------------ OUTBOUND PACKET DELIMITERS ------------ */
#define  PROBE_RS232_PROTOCOL_TX_SD0            0x68                /* (A) Start delimiters                                 */
#define  PROBE_RS232_PROTOCOL_TX_SD3            0x68
#define  PROBE_RS232_PROTOCOL_TX_ED             0x16                /* (E) End   delimiter                                  */

                                                                    /* ----------- RECEIVE STATE MACHINE STATES ----------- */
#define  PROBE_RS232_RX_STATE_SD0                  0                /*  (0) waiting for start first  start delimiter (SD0)  */
#define  PROBE_RS232_RX_STATE_LEN                  1                /*  (1) waiting for length                              */
#define  PROBE_RS232_RX_STATE_REPEAT_LEN           2                /*  (2) waiting for repeat length                       */
#define  PROBE_RS232_RX_STATE_SD3                  3                /*  (3) waiting for start fourth start delimiter (SD3)  */
#define  PROBE_RS232_RX_STATE_DATA                 4                /*  (4) waiting for data                                */
#define  PROBE_RS232_RX_STATE_CHKSUM               5                /*  (5) waiting for checksum                            */
#define  PROBE_RS232_RX_STATE_ED                   6                /*  (6) waiting for end delimiter                       */

                                                                    /* ---------- TRANSMIT STATE MACHINE STATES ----------- */
#define  PROBE_RS232_TX_STATE_SD0                  0                /*  (0) waiting to send start first  start delim. (SD0) */
#define  PROBE_RS232_TX_STATE_LEN                  1                /*  (1) waiting to send start second start delim. (SD1) */
#define  PROBE_RS232_TX_STATE_REPEAT_LEN           2                /*  (2) waiting to send start third  start delim. (SD2) */
#define  PROBE_RS232_TX_STATE_SD3                  3                /*  (3) waiting to send start fourth start delim. (SD3) */
#define  PROBE_RS232_TX_STATE_DATA                 4                /*  (4) waiting to send data                            */
#define  PROBE_RS232_TX_STATE_CHKSUM               5                /*  (5) waiting to send checksum                        */
#define  PROBE_RS232_TX_STATE_ED                   6                /*  (6) waiting to send end delimiter                   */

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

INT32U  ProbeRS232_RxCtr;
INT32U  ProbeRS232_RxPktCtr;

                                                                     /* -------------- RX state variables ------------------ */
static  CPU_INT08U   ProbeRS232_RxState;                            /*  ... Current state of RX state machine               */
static  CPU_INT16U   ProbeRS232_RxRemainLen;                        /*  ... Remaining bytes of data to read                 */

                                                                     /* -------------- RX packet variables ----------------- */
static  CPU_INT16U   ProbeRS232_RxLen;                              /*  ... Length  of data in current packet               */
static  CPU_INT08U   ProbeRS232_RxBuf[PROBE_RS232_RX_BUF_SIZE];     /*  ... Data    of current packet                       */
static  CPU_INT08U   ProbeRS232_RxChkSumRecv;                       /*  ... Checksum of current packet                      */


                                                                     /* ------------ RX data buffer variable --------------- */
static  CPU_INT16U   ProbeRS232_RxBufWrIx;                          /*  ... Index of next write; also number of bytes in buf*/


#define  ProbeRS232_RxBufClr()   ProbeRS232_RxBufWrIx = 0


#define ProbeRS232_RxStoINT8U(data)  		                     \
   if (ProbeRS232_RxBufWrIx < PROBE_RS232_RX_BUF_SIZE) {		 \
       ProbeRS232_RxBuf[ProbeRS232_RxBufWrIx++] = rx_data; 		 \
    }



/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  CPU_INT16U  ProbeRS232_RxPkt (void);
static int Format232ToCanMsg(CO_MSG *pmsg, INT8U *pbyFrame, INT8U frameLen);

/*
*********************************************************************************************************
*                                              Rx Handler
*
* Description: This routine is called from the Rx interrupt service handler.
*
* Argument(s): rx_data  is a received data byte.
*
* Returns    : None
*********************************************************************************************************
*/

void  ProbeRS232_RxHandler (CPU_INT08U rx_data)
{
    ProbeRS232_RxCtr++;

    switch (ProbeRS232_RxState) {

        case PROBE_RS232_RX_STATE_SD0:                              /* Receive the start first  start delimiter (SD0)       */
             if (rx_data == PROBE_RS232_PROTOCOL_RX_SD0) {
                 ProbeRS232_RxState = PROBE_RS232_RX_STATE_LEN;
                 ProbeRS232_RxBufClr();
             }
             break;

        case PROBE_RS232_RX_STATE_LEN:                               /* Receive the start second start delimiter (SD1)      */
             ProbeRS232_RxRemainLen = rx_data;
             if (ProbeRS232_RxRemainLen > PROBE_RS232_RX_BUF_SIZE) {
                 ProbeRS232_RxState     = PROBE_RS232_RX_STATE_SD0;  /* ... Cannot handle a packet of this size             */
             } else { 
                 ProbeRS232_RxState     = PROBE_RS232_RX_STATE_REPEAT_LEN; 
             }           
             break;
             
        case PROBE_RS232_RX_STATE_REPEAT_LEN:                       /* Receive the second length byte                       */
             if (rx_data != ProbeRS232_RxRemainLen) {                
                 ProbeRS232_RxState  = PROBE_RS232_RX_STATE_SD0;    /* ... Repeat size not correct                          */
             } else {
                 ProbeRS232_RxLen    = ProbeRS232_RxRemainLen;
                 ProbeRS232_RxState  = PROBE_RS232_RX_STATE_SD3;
             }
             break;

        case PROBE_RS232_RX_STATE_SD3:                              /* Receive the start fourth start delimiter (SD3)       */
             if (rx_data == PROBE_RS232_PROTOCOL_RX_SD3) {
                 ProbeRS232_RxState  = PROBE_RS232_RX_STATE_DATA;
             } else {
                 ProbeRS232_RxState  = PROBE_RS232_RX_STATE_SD0;
             }
             break;

        case PROBE_RS232_RX_STATE_DATA:                             /* Receive the data                                     */
             ProbeRS232_RxStoINT8U(rx_data);
             if (--ProbeRS232_RxRemainLen == 0) {
                 ProbeRS232_RxState = PROBE_RS232_RX_STATE_CHKSUM;
             }
             break;

        case PROBE_RS232_RX_STATE_CHKSUM:                           
             ProbeRS232_RxChkSumRecv = rx_data;
             ProbeRS232_RxState   = PROBE_RS232_RX_STATE_ED;        /* Receive the checksum                                 */
             break;

        case PROBE_RS232_RX_STATE_ED:                               /* Receive the end delimiter                            */
             if (rx_data == PROBE_RS232_PROTOCOL_RX_ED) {
                 ProbeRS232_RxPktCtr++;                             /* ... Increment Rx packet counter                      */
                 ProbeRS232_RxPkt();                                /* ... Parse received packet                            */
             }
             ProbeRS232_RxState = PROBE_RS232_RX_STATE_SD0;
             break;

        default:
             ProbeRS232_RxState = PROBE_RS232_RX_STATE_SD0;
             break;
    }
}

/*
*********************************************************************************************************
*                                         Parse Receive Packet
*
* Description: This routine is called after a complete packet has been received.
*
* Argument(s): None.
*
* Returns    : The number of bytes in the data segment of the packet to transmit in response.
*********************************************************************************************************
*/

static  CPU_INT16U  ProbeRS232_RxPkt (void)
{
    INT8U accChk;
	INT8U resp[PROBE_RS232_TX_BUF_SIZE];
	INT16U respLen;
	INT32U errCode;
	INT8U  baud;
	CO_MSG stcMsg;
	INT8U  *pbyFrame;
	INT16U id, timeout, repeat;


    respLen = 0;

	errCode = CO_NO_ERR;

	accChk     = CheckAccLRC(ProbeRS232_RxBuf, ProbeRS232_RxLen);
    if (accChk != 	ProbeRS232_RxChkSumRecv)
	{ //校验码出错
	  errCode =  CO_ERR_232_ACC_CHK;
	}

	pbyFrame = &ProbeRS232_RxBuf[0];

	switch(pbyFrame[0])
	{
	  case CCM_RESET:
	    CODrv_Init();
	    break; 
	  case CCM_FILTER_RESET:
	    CODrv_FilterSetStart();    
		break;
	  case CCM_FILTER_ALL:
	    CODrv_FilterSetAll();    
		break;
	  case CCM_FILTER_ADD:
		Format232ToCanMsg(&stcMsg, &pbyFrame[1], ProbeRS232_RxLen - 1);
        id = COMacGetStdId(&stcMsg);
		CODrv_FilterSet(id, 0);
		break;
	  case CCM_BAUD_SET:
	    if (ProbeRS232_RxLen != 2)
		{
		  errCode =  PROBE_ERR_DATA_FORMAT;
		}
		baud = ProbeRS232_RxBuf[1];
		if (!CODrv_BaudCheck(baud))
		{
		  errCode =  PROBE_ERR_BAUD_FORMAT;
		}
      
	    if (baud != CODrv_BaudSet(baud))
		{
		  errCode =  PROBE_ERR_BAUD_FORMAT;	  
		}
		break;
      case CCM_SEND_SINGLE:
        timeout = LOAD_INT16U(pbyFrame);
		Format232ToCanMsg(&stcMsg, &pbyFrame[3], ProbeRS232_RxLen - 3);
        errCode = CODrv_MsgWaitSend(&stcMsg, timeout);
		break;
      
	  case CCM_SEND_REPEAT:
        repeat = LOAD_INT16U(pbyFrame);
		Format232ToCanMsg(&stcMsg, &pbyFrame[3], ProbeRS232_RxLen - 3);
        errCode = CODrv_MsgWaitSend(&stcMsg, repeat);
		break;  
	}

	return (errCode);
}


static int Format232ToCanMsg(CO_MSG *pmsg, INT8U *pbyFrame, INT8U frameLen)
{
   TCanStandard  std;
   TCanExtended  ext;

  *(INT32U *)&ext = LOAD_LONG(pbyFrame);
  *(INT32U *)&std = *(INT32U *)&ext;

  if (ext.fExt)
  {
    COMacSetExtId(pmsg, ext.COBID);
    COMacSetDlc(pmsg, frameLen - 4);
    memcpy(pmsg->COMsgData,  pbyFrame + 4, frameLen - 4);
  }
  else
  {
    COMacSetStdId(pmsg, std.COBID);
    COMacSetDlc(pmsg, frameLen - 2);
    memcpy(pmsg->COMsgData,  pbyFrame + 2, frameLen - 2);
  }
  
  if (ext.fRtr)
  {
    COMacSetRemote(pmsg);
  }
  return TRUE;  
}



static int CanMsgTo232Format(INT8U *pbyFrame, CO_MSG *pmsg)
{
  BOOLEAN bExtend;
  INT32U  dwId;
  INT16U  wId;
  INT16U  msgLen;
  int len = 0;
  
  bExtend = COMacIsExtended(pmsg);
  if (bExtend)
  {
    dwId = COMacGetExtId(pmsg);
	if (COMacIsRemote(pmsg))
	{
	  dwId |= 0x80000000;
	}
    WIRTE_INT32U(pbyFrame, dwId); 
	len += 4; 
  }
  else
  {
    wId = COMacGetStdId(pmsg); 
	if (COMacIsRemote(pmsg))
	{
	  wId |= 0x8000;
	}
    WIRTE_INT16U(pbyFrame, wId);
	len += 2; 
  }

  msgLen = COMacGetDlc(pmsg);
  memcpy(pbyFrame + len, pmsg->COMsgData, msgLen);

  len = len + msgLen;
  return len;
}
 
int  ProbeRS232_TxSendCommand (TCommFrame *pstcCommFrame)
{
    CPU_INT08U  tx_data;
    CPU_INT16U  ProbeRS232_TxLen;                              /*  ... Length  of data in current packet               */
    CPU_INT08U  ProbeRS232_TxBuf[PROBE_RS232_TX_BUF_SIZE];     /*  ... Data    of current packet                       */
    CPU_INT08U  ProbeRS232_TxChkSum;                           /*  ... Checksum of current packet                      */
	BOOLEAN     bExtend;
	CPU_INT16U  len;
	CPU_INT08U  cmd;
    INT8U       *pbyFrame;
	INT8U       msgLen;

	cmd = (pstcCommFrame->cmd & ~CMD_FLAG_EXTEND_FRAME);
	pbyFrame = &ProbeRS232_TxBuf[5];

    switch(cmd)
	{
	  case CCM_FILTER_RESET:
	  case CCM_FILTER_ALL:
	  case 	CCM_RESET:
        break;
	  case CCM_BAUD_SET	:
	    *pbyFrame++ = pstcCommFrame->para.baud;
	    break;
	  case CCM_FILTER_ADD:
	  case	CCM_FILTER_REMOVE:
	  case	CCM_RECV_FRAME:
		pbyFrame += CanMsgTo232Format(pbyFrame, &pstcCommFrame->stcCanMsg);
		break;
	  case	CCM_SEND_REPEAT:
	    WIRTE_INT16U(pbyFrame, pstcCommFrame->para.repeat_time);
		pbyFrame += 2;
		pbyFrame += CanMsgTo232Format(pbyFrame, &pstcCommFrame->stcCanMsg);
		break;
	  case	CCM_SEND_SINGLE:
 	    WIRTE_INT16U(pbyFrame, pstcCommFrame->para.timeout);	
		pbyFrame += 2;
		pbyFrame += CanMsgTo232Format(pbyFrame, &pstcCommFrame->stcCanMsg);
		break;
	  case CCM_RECV_ERR:
   	    WIRTE_INT16U(pbyFrame, pstcCommFrame->para.errCode);
		pbyFrame += 2;
	    break;
	}

	len = (INT8U)(pbyFrame - &ProbeRS232_TxBuf[4]);

	ProbeRS232_TxBuf[0] = PROBE_RS232_PROTOCOL_TX_SD0;
    ProbeRS232_TxBuf[1] = len;
	ProbeRS232_TxBuf[2] = len;
	ProbeRS232_TxBuf[3] = PROBE_RS232_PROTOCOL_TX_SD3;
	ProbeRS232_TxBuf[4] = cmd;

	pbyFrame[0]         = CheckAccLRC(&ProbeRS232_TxBuf[4], len);	
	pbyFrame[1]         = PROBE_RS232_PROTOCOL_TX_ED;

	return UartOutFrame(ProbeRS232_TxBuf, len + 6);
}