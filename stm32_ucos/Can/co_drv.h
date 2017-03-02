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
*                            CANOPEN 标准波特率表
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
  CO_BPS_AUTO,               //进行自动波特率校验
  CO_BPS_INVALID           //无效波特率
} ECOBaudRate;
    

typedef enum
{
  FUNC_ENABLE,              //允许对应功能
  FUNC_DISABLE              //禁止对应功能
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
*                         CAN接收缓冲
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
*                               CAN驱动库初始化
* 描述 : 初始化CAN驱动库, 中止所有帧的发送与接收, 清空缓冲. 调用完成后CAN中断禁止
* 参数:       : 无
* 返回:       : 无
*********************************************************************************************************/
extern void CODrv_Init(void);


/********************************************************************************************************
* 描述 : 进行波特率支持情况校验, 检查波特率的可行性，但不进行实际设置
*
* 参数:       : eCOBaud:    需要设置的Canopen波特率
* 返回:       : FALSE:  相应的波特率无法设置或无法执行自动波特率校验算法
                TRUE:   支持新的波特率或支持自动波特率校验算法
*********************************************************************************************************/
extern BOOLEAN CODrv_BaudCheck(ECOBaudRate eCOBaud);

/********************************************************************************************************
* 描述 :      波特率初始化，进行新的波特率设置, 或进入自动波特率校验
*
* 参数:       : eCOBaud:    需要设置的Canopen波特率
* 返回:       : 实际所采用的波特率
                  若波特率设置正常, 则返回eCOBaud,
                  若为自动波特, 则返回实际采用的波特率,

* 注意        : 在自动波特率校验中, 标准指示灯的闪动需要在函数内部等待时间中 调用CO_TimeoutDispatch()实现
********************************************************************************************************/
extern ECOBaudRate CODrv_BaudSet(ECOBaudRate eCOBaud);

/********************************************************************************************************
* 描述 :      自动波特率校验
*
* 参数:       : eCOBaud:    需要设置的Canopen波特率
* 返回:       : 实际所采用的波特
* 注意        : 在自动波特率校验中, 标准指示灯的闪动需要在函数内部等待时间中 调用CO_TimeoutDispatch()实现
********************************************************************************************************/
extern ECOBaudRate CODrv_AutoBaud(void);

/********************************************************************************************************
*                               CAN发送与接收功能选择
* 参数:       : eFuncSel:
                  FUNC_ENABLE:   允许CAN数据接收与发送
                  FUNC_DISABLE:  禁止CAN数据接收与发送
********************************************************************************************************/
extern void CODrv_OperationSet(EFuncSeletion eFuncSel);

/********************************************************************************************************
                       接收过滤器设置开始, 但未使能.
注意:
   对于无自动过滤的驱动器, 则无需实现该函数, CANOPEN协议栈可实现软件过滤
   对于有自动过滤的硬件驱动器,  则直接由硬件进行接收过滤
********************************************************************************************************/
extern BOOLEAN CODrv_FilterSetStart(void);

/********************************************************************************************************
*                               设置过滤ID, 同时使能.
** 参数:    dwCobID     :  需要设置的COBID号
            dwMask      :  COBID掩码, 掩码特定位为1时, 接收到的帧号必须与dwCobId对应位的值一致才接收该帧,
			               对于单个滤波值来说, 该值为0xFFFFFFFF, 即所有位都要一致,
						   dwMask != 0xFFFFFFFF时, 说明为带掩码接收
** 返回:   TRUE:  设置成功
          FALSE:  设置失败
** 注意1:  该函数会被连续调用, 直到设置完所有的需过滤的CAN的ID, 如果超出硬件所支持的最大过滤数, 返回FALSE
** 注意2:  对于无自动过滤的驱动器, 则无需实现该函数, CANOPEN协议栈可实现软件过滤
           对于有自动过滤的硬件驱动器,  则直接由硬件进行接收过滤设置
********************************************************************************************************/
extern BOOLEAN CODrv_FilterSet(INT32U dwCobID, INT32U dwMask);

/********************************************************************************************************
*                               设置过滤ID, 使能接收所有帧
********************************************************************************************************/
extern BOOLEAN CODrv_FilterSetAll(void);


/********************************************************************************************************
*                          设置过滤ID结束, 并使能所有先前所设的滤波值
********************************************************************************************************/
extern BOOLEAN CODrv_FilterSetFinish(void);

/********************************************************************************************************
*                               CAN发送与接收功能选择
* 参数:       : eFuncSel:
                  FUNC_ENABLE:   允许CAN数据接收与发送
                  FUNC_DISABLE:  禁止CAN数据接收与发送
********************************************************************************************************/
extern void CODrv_OperationSet(EFuncSeletion eFuncSel);



/********************************************************************************************************
**    发送一个帧到CAN总线
**    参数:   pTxMsg:  需要发送的帧
            timeout:   > 0:  等待发送成功或失败
                       =0:   无限等待帧发送成功或失败
					   <0:   发送到缓冲后即返回结果, 无需等待
**     返回:  CO_NO_ERR:           帧已经完成传输或已放入到发送缓冲中
              CO_ERR_TRANS_FAIL:     帧传输失败
              CO_ERR_TRANS_TIMEOUT:  帧传输超时, 可能没有通讯结点, 或网络很忙导致仲裁一直丢失,或缓冲满
			  CO_ERR_TRANS_BUF_FULL: 帧发送缓冲满
********************************************************************************************************/
INT32U CODrv_MsgWaitSend(CO_MSG *pTxMsg, int timeout);

/********************************************************************************************************
**    发送一个帧到CAN总线
**    参数:   pTxMsg:  需要发送的帧
              bWait:   TRUE:  一直等待, 直到帧已发送或发送出错
                       FALSE: 将信息存到发送缓冲中, 即可返回
**     返回:  TRUE: 发送成功或已放入到发送缓冲区
             FALSE: 发送失败
********************************************************************************************************/
extern BOOLEAN CODrv_SendMessage(CO_MSG *pTxMsg, BOOLEAN bWait);


/********************************************************************************************************
**    从Can总线上接收一个帧, 若没有接收到帧, 则返回FALSE，否则返回接收到的帧
**    参数:   pRxMsg:  存储接收到的帧
**     返回:  TRUE: 帧已经被接收
             FALSE: 没有接收到CAN帧
********************************************************************************************************/
extern BOOLEAN  CODrv_RecvMessage(CO_MSG *pRxMsg);

#undef  ARRAY_SIZE
#define ARRAY_SIZE(arr)  (sizeof(arr) / sizeof(arr[0]))

#endif //#ifndef CANDRIVER_H
