#ifndef CANDRIVER_H
#define CANDRIVER_H

#include "CO_Error.h"

#ifndef CP_MSG_TIME
#define CP_MSG_TIME  0
#endif

#ifndef CP_USR_DATA
#define CP_USR_DATA  0
#endif

#ifndef DEFAULT_BAUD
#define DEFAULT_BAUD     CO_BPS_125K
#endif

#ifndef DEFAULT_NODE_ID
#define DEFAULT_NODE_ID  10
#endif

#ifndef CAN_RECEIVE_BUF_SIZE
#define CAN_RECEIVE_BUF_SIZE  20
#endif

#ifndef CO_MSG_USER_DATA_EN
#define CO_MSG_USER_DATA_EN  1
#endif


typedef enum CP_CHANNEL {
   CP_CHANNEL_1 = 0,
   CP_CHANNEL_2,
   CP_CHANNEL_3,
   CP_CHANNEL_4,
   CP_CHANNEL_5,
   CP_CHANNEL_6,
   CP_CHANNEL_7,
   CP_CHANNEL_8
} ECpChannel;

/*
*********************************************************************************************************
*                            CANOPEN ��׼�����ʱ�
*********************************************************************************************************
*/
typedef enum ECOBaudRate
{
  CO_BPS_1000K = 0,
  CO_BPS_800K,
  CO_BPS_500K,
  CO_BPS_250K,
  CO_BPS_125K,
  CO_BPS_100K,
  CO_BPS_50K,
  CO_BPS_20K,
  CO_BPS_10K,               
  CO_BPS_AUTO,               //�����Զ�������У��
  CO_BPS_INVALID           //��Ч������
} ECOBaudRate;
    

typedef enum
{
  FUNC_ENABLE,              //�����Ӧ����
  FUNC_DISABLE              //��ֹ��Ӧ����
} EFuncSeletion;

/*
*********************************************************************************************************
*                                        CAN MESSAGE STRUCTURE
*********************************************************************************************************
*/
typedef struct {
   INT32U  COMsgId;                       /*!   The identifier field may have 11 bits for standard frames
                                          **    (CAN specification 2.0A) or 29 bits for extended frames
                                          **    (CAN specification 2.0B). The three most significant bits
                                          **    are reserved for special functionality (the LSB is Bit 0,
                                          **    the MSB is Bit 31 ).<p>
                                          **    <ul>
                                          **    <li>Bit 31: Bit value 1 marks the identifier as an
                                          **                extended frame. Bit value 0 marks the
                                          **                identifier as a standard frame.
                                          **    <li>Bit 30: Bit value 1 marks the identifier
                                          **                as an remote transmission (RTR).
                                          **    <li>Bit 29: Reserved for future use
                                          **    </ul>
                                          */
                                          
   INT16U  COMsgFlags;                    /*!   The message flags field contains the data length code
                                          **    (DLC) of the CAN message and the buffer number when
                                          **    using a FullCAN controller.<p>
                                          **    The data length code (<b>Bit 0 - Bit 3</b>) contains the
                                          **    number of data bytes which are transmitted by a message.
                                          **    The possible value range for the data length code is
                                          **    from 0 to 8 (bytes).<p>
                                          **    A FullCAN controller (e.g. AN82527) has more than only
                                          **    one transmit and one receive buffer and offers more
                                          **    sophisticated message filtering. The field message
                                          **    buffer (<b>Bit 4 - Bit 7</b>) specifies the buffer for
                                          **    message transmission or reception.<p>
                                          **    The high word (<b>Bit 16 - Bit 31</b>)
                                          **    is reserved for user defined data.
                                          */
   INT16U COUsrFlag;                      /*    0xFFFF     for software Flag Search
                                                != 0xFFFF  User flag to accelerate Msg Handle speed,
                                                           see CODrv_FilterSet function for detail
                                          */
   INT8U  COMsgData[8];                   /*!   The data fields contain up to eight bytes for a CAN
                                          **    message. If the data length code is less than 8, the
                                          **    value of the unused data bytes will be undefined.
                                          */
#if CO_MSG_TIME_EN
   INT32U  COMsgTime;                     /*!   The time stamp field defines the time when a CAN message
                                          **    was received by the CAN controller. The time stamp is a
                                          **    relative value, which is created by a free running timer.
                                          **    The time base is one microsecond (1 us). This means a
                                          **    maximum time span of 4294,96 seconds (1 hour 11 minutes)
                                          **    between two messages can be measured. This is an optional
                                          **    field (available if CO_MSG_TIME_EN is set to 1).
                                          */
#endif
#if CO_MSG_USER_DATA_EN
   INT32U  COUsrData;                     /*!   The field user data can hold a 32 bit value, which is
                                          **    defined by the user. This is an optional field
                                          **    (available if CO_MSG_USER_DATA_EN is set to 1).
                                          */
#endif

} CO_MSG;


/*
*********************************************************************************************************
*                         CAN���ջ���
*********************************************************************************************************
*/
typedef struct TCanReceiveBuf
{
  OS_EVENT  *BufQueue;
  OS_MEM    *FrameMem;
  void     *MessageStorage[CAN_RECEIVE_BUF_SIZE];
  CO_MSG RxMsg[CAN_RECEIVE_BUF_SIZE];
} TCanReceiveBuf;

/*
*********************************************************************************************************
*                               CAN MESSAGE MASK USED IN MESSAGE OPERATOR MACRO
*********************************************************************************************************
*/
#define  CO_MASK_STD_FRAME 0x000007FF     /* Mask for standard frame (11 bits)                         */
#define  CO_MASK_EXT_FRAME 0x1FFFFFFF     /* Mask for extended frame (29 bits)                         */
#define  CO_MASK_EXT_BIT   0x80000000     /* Set the EXT bit in the COMsgFlags field                   */
#define  CO_MASK_RTR_BIT   0x40000000     /* Set the RTR bit in the COMsgFlags field                   */
#define  CO_MASK_DLC_BITS  0x0000000F     /* Mask for DLC field in COMsgFlags                          */
#define  CO_MASK_BUF_BITS  0x000000F0     /* Mask for buffer field in COMsgFlags                       */


/*
*********************************************************************************************************
*                                      CAN MESSAGE OPERATOR MACRO
*  PARA  DESCRIPTION
*                            MSG_PTR:  Pointer to a CO_MSG
*                                VAL:  Value used in COMacSet??? Macro
*                                DLC:  Data length of a can message [0..8]
                                 POS:  Index in COMsgData[] array   [0..7]
*  MACRO DESCRIPTION
*                     COMacGetBufNum:  Get message buffer number
*                       COMacGetData:  Get Data from message
*                        COMacGetDlc:  Get Data Length Code
*                      COMacGetExtId:  Get 29 Bit Identifier Value
*                      COMacGetStdId:  Get 11 Bit Identifier Value
*                    COMacIsExtended:  Check the frame type
*                      COMacIsRemote:  Check for remote frame
*                      CpMacMsgClear:  Clear message structure
*                     COMacSetBufNum:  Set message buffer number
*                       COMacSetData:  Set Data in message
*                        COMacSetDlc:  Set Data Length Code
*                      COMacSetExtId:  Set 29 Bit Identifier Value
*                     COMacSetRemote:  Set RTR bit
*                      COMacSetStdId:  Set 11 Bit Identifier Value
*********************************************************************************************************
*/
#define  COMacGetBufNum(MSG_PTR)          (INT8U)(((MSG_PTR)->COMsgFlags & CO_MASK_BUF_BITS) >> 4)
#define  COMacGetData(MSG_PTR, POS)       ((MSG_PTR)->COMsgData[POS])
#define  COMacGetDlc(MSG_PTR)             (INT8U)(((MSG_PTR)->COMsgFlags) & CO_MASK_DLC_BITS)

#define  COMacGetExtId(MSG_PTR)           ((MSG_PTR)->COMsgId & CO_MASK_EXT_FRAME)
#define  COMacGetStdId(MSG_PTR)           (INT16U)((MSG_PTR)->COMsgId & CO_MASK_STD_FRAME)
#define  COMacIsExtended(MSG_PTR)         (INT8U)(((MSG_PTR)->COMsgId) >> 31)
#define  COMacIsRemote(MSG_PTR)           (INT8U)(((MSG_PTR)->COMsgId) >> 30)
#define  COMacGetUsrFlag(MSG_PTR)         (INT16U)((MSG_PTR)->COUsrFlag)


#define  COMacSetUsrFlag(MSG_PTR, FLAG)   (MSG_PTR)->COUsrFlag = FLAG;
#define  COMacMsgClear(MSG_PTR)           (MSG_PTR)->COUsrFlag = 0xFFFF;                   \
                                          (MSG_PTR)->COMsgFlags = 0;                        \
                                          (MSG_PTR)->COMsgId= 0;

#define  COMacDataClear(MSG_PTR)          *(INT32U *)(&(MSG_PTR)->COMsgData[0]) = 0;        \
                                          *(INT32U *)(&(MSG_PTR)->COMsgData[4]) = 0
#define  COMacSetBufNum(MSG_PTR, VAL)     (MSG_PTR)->COMsgFlags     &= (~CO_MASK_BUF_BITS); \
                                          (MSG_PTR)->COMsgFlags     |= (VAL << 4)
#define  COMacSetData(MSG_PTR, POS, VAL)  (MSG_PTR)->COMsgData[POS]  = VAL
#define  COMacSetDlc(MSG_PTR, DLC)        (MSG_PTR)->COMsgFlags     &= (~CO_MASK_DLC_BITS); \
                                          (MSG_PTR)->COMsgFlags     |= DLC
#define  COMacSetExtId(MSG_PTR, VAL)      (MSG_PTR)->COMsgId         = VAL | CO_MASK_EXT_BIT
#define  COMacSetRemote(MSG_PTR)          (MSG_PTR)->COMsgId        |= CO_MASK_RTR_BIT
#define  COMacSetStdId(MSG_PTR, VAL)      (MSG_PTR)->COMsgId        = VAL


/********************************************************************************************************
*                               CAN�������ʼ��
* ���� : ��ʼ��CAN������, ��ֹ����֡�ķ��������, ��ջ���. ������ɺ�CAN�жϽ�ֹ
* ����:       : ��
* ����:       : ��
*********************************************************************************************************/
extern void CODrv_Init(void);


/********************************************************************************************************
* ���� : ���в�����֧�����У��, ��鲨���ʵĿ����ԣ���������ʵ������
*
* ����:       : eCOBaud:    ��Ҫ���õ�Canopen������
* ����:       : FALSE:  ��Ӧ�Ĳ������޷����û��޷�ִ���Զ�������У���㷨
                TRUE:   ֧���µĲ����ʻ�֧���Զ�������У���㷨
*********************************************************************************************************/
extern BOOLEAN CODrv_BaudCheck(ECOBaudRate eCOBaud);

/********************************************************************************************************
* ���� :      �����ʳ�ʼ���������µĲ���������, ������Զ�������У��
*
* ����:       : eCOBaud:    ��Ҫ���õ�Canopen������
* ����:       : ʵ�������õĲ�����
                  ����������������, �򷵻�eCOBaud,
                  ��Ϊ�Զ�����, �򷵻�ʵ�ʲ��õĲ�����,

* ע��        : ���Զ�������У����, ��׼ָʾ�Ƶ�������Ҫ�ں����ڲ��ȴ�ʱ���� ����CO_TimeoutDispatch()ʵ��
********************************************************************************************************/
extern ECOBaudRate CODrv_BaudSet(ECOBaudRate eCOBaud);

/********************************************************************************************************
* ���� :      �Զ�������У��
*
* ����:       : eCOBaud:    ��Ҫ���õ�Canopen������
* ����:       : ʵ�������õĲ���
* ע��        : ���Զ�������У����, ��׼ָʾ�Ƶ�������Ҫ�ں����ڲ��ȴ�ʱ���� ����CO_TimeoutDispatch()ʵ��
********************************************************************************************************/
extern ECOBaudRate CODrv_AutoBaud(void);

/********************************************************************************************************
*                               CAN��������չ���ѡ��
* ����:       : eFuncSel:
                  FUNC_ENABLE:   ����CAN���ݽ����뷢��
                  FUNC_DISABLE:  ��ֹCAN���ݽ����뷢��
********************************************************************************************************/
extern void CODrv_OperationSet(EFuncSeletion eFuncSel);

/********************************************************************************************************
                       ���չ��������ÿ�ʼ, ��δʹ��.
ע��:
   �������Զ����˵�������, ������ʵ�ָú���, CANOPENЭ��ջ��ʵ���������
   �������Զ����˵�Ӳ��������,  ��ֱ����Ӳ�����н��չ���
********************************************************************************************************/
extern BOOLEAN CODrv_FilterSetStart(void);

/********************************************************************************************************
*                               ���ù���ID, ͬʱʹ��.
** ����:    dwCobID     :  ��Ҫ���õ�COBID��
            dwMask      :  COBID����, �����ض�λΪ1ʱ, ���յ���֡�ű�����dwCobId��Ӧλ��ֵһ�²Ž��ո�֡,
			               ���ڵ����˲�ֵ��˵, ��ֵΪ0xFFFFFFFF, ������λ��Ҫһ��,
						   dwMask != 0xFFFFFFFFʱ, ˵��Ϊ���������
** ����:   TRUE:  ���óɹ�
          FALSE:  ����ʧ��
** ע��1:  �ú����ᱻ��������, ֱ�����������е�����˵�CAN��ID, �������Ӳ����֧�ֵ���������, ����FALSE
** ע��2:  �������Զ����˵�������, ������ʵ�ָú���, CANOPENЭ��ջ��ʵ���������
           �������Զ����˵�Ӳ��������,  ��ֱ����Ӳ�����н��չ�������
********************************************************************************************************/
extern BOOLEAN CODrv_FilterSet(INT32U dwCobID, INT32U dwMask);

/********************************************************************************************************
*                               ���ù���ID, ʹ�ܽ�������֡
********************************************************************************************************/
extern BOOLEAN CODrv_FilterSetAll(void);


/********************************************************************************************************
*                          ���ù���ID����, ��ʹ��������ǰ������˲�ֵ
********************************************************************************************************/
extern BOOLEAN CODrv_FilterSetFinish(void);

/********************************************************************************************************
*                               CAN��������չ���ѡ��
* ����:       : eFuncSel:
                  FUNC_ENABLE:   ����CAN���ݽ����뷢��
                  FUNC_DISABLE:  ��ֹCAN���ݽ����뷢��
********************************************************************************************************/
extern void CODrv_OperationSet(EFuncSeletion eFuncSel);



/********************************************************************************************************
**    ����һ��֡��CAN����
**    ����:   pTxMsg:  ��Ҫ���͵�֡
            timeout:   > 0:  �ȴ����ͳɹ���ʧ��
                       =0:   ���޵ȴ�֡���ͳɹ���ʧ��
					   <0:   ���͵�����󼴷��ؽ��, ����ȴ�
**     ����:  CO_NO_ERR:           ֡�Ѿ���ɴ�����ѷ��뵽���ͻ�����
              CO_ERR_TRANS_FAIL:     ֡����ʧ��
              CO_ERR_TRANS_TIMEOUT:  ֡���䳬ʱ, ����û��ͨѶ���, �������æ�����ٲ�һֱ��ʧ,�򻺳���
			  CO_ERR_TRANS_BUF_FULL: ֡���ͻ�����
********************************************************************************************************/
INT32U CODrv_MsgWaitSend(CO_MSG *pTxMsg, int timeout);

/********************************************************************************************************
**    ����һ��֡��CAN����
**    ����:   pTxMsg:  ��Ҫ���͵�֡
              bWait:   TRUE:  һֱ�ȴ�, ֱ��֡�ѷ��ͻ��ͳ���
                       FALSE: ����Ϣ�浽���ͻ�����, ���ɷ���
**     ����:  TRUE: ���ͳɹ����ѷ��뵽���ͻ�����
             FALSE: ����ʧ��
********************************************************************************************************/
extern BOOLEAN CODrv_SendMessage(CO_MSG *pTxMsg, BOOLEAN bWait);


/********************************************************************************************************
**    ��Can�����Ͻ���һ��֡, ��û�н��յ�֡, �򷵻�FALSE�����򷵻ؽ��յ���֡
**    ����:   pRxMsg:  �洢���յ���֡
**     ����:  TRUE: ֡�Ѿ�������
             FALSE: û�н��յ�CAN֡
********************************************************************************************************/
extern BOOLEAN  CODrv_RecvMessage(CO_MSG *pRxMsg);

#undef  ARRAY_SIZE
#define ARRAY_SIZE(arr)  (sizeof(arr) / sizeof(arr[0]))

#endif //#ifndef CANDRIVER_H
