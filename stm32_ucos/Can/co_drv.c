#include <includes.h>

#include "co_drv.h"
#include "os_can_buf.h"

#ifndef Fcanclk
#error "Fcanclk not defined in advance"
#endif

#ifndef CO_MAX_FILTER_NUMS
#error  "CO_MAX_FILTER_NUMS  not defined in advance"
#endif

#ifndef CO_AUTO_BPS_SUPPORT
#error  "CO_AUTO_BPS_SUPPORT not defined in advance"
#endif



#define SJW_DEFAULT       0          //default resync jump width
#define CAN_SAMPLE_POINT  0.875      //CAN sample point

#define TSEG1(n)  (INT32U)(n * CAN_SAMPLE_POINT + 0.5 - 2)
#define TSEG2(n)  (INT32U)(n - TSEG1(n) - 3)

//set CanBitrateXXK  to 0 for unsupported baud rate
//Can Baud rate = Fcanclk / ((BRP+1) * ( TSEG1 + 1 + 1 + TSEG2 + 1))
//                           TSEG2         +  TSEG1            +  SJW                + BPR
#if Fcanclk == 60000000

#define NRT   15    //number in can bit time
#define CANBitrate10K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (10000  * NRT) - 1))
#define CANBitrate20K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (20000  * NRT) - 1))
#define CANBitrate50K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (50000  * NRT) - 1))
#define CANBitrate100K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (100000  * NRT) - 1))
#define CANBitrate125K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (125000  * NRT) - 1))
#define CANBitrate250K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (250000  * NRT) - 1))
#define CANBitrate500K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (500000  * NRT) - 1))
#define CANBitrate800K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (800000  * NRT) - 1))
#define CANBitrate1000K ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (1000000 * NRT) - 1))

#elif Fcanclk == 48000000

#define NRT   16
#define CANBitrate10K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (10000  * NRT) - 1))
#define CANBitrate20K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (20000  * NRT) - 1))
#define CANBitrate50K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (50000  * NRT) - 1))
#define CANBitrate100K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (100000  * NRT) - 1))
#define CANBitrate125K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (125000  * NRT) - 1))
#define CANBitrate250K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (250000  * NRT) - 1))
#define CANBitrate500K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (500000  * NRT) - 1))
#define CANBitrate800K  ((TSEG2(15) << 20)  + (TSEG1(15) << 16)  + (SJW_DEFAULT << 14) + (Fcanclk / (800000  * 15) - 1))
#define CANBitrate1000K ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (1000000 * NRT) - 1))

#elif Fcanclk == 72000000 || Fcanclk == 36000000

#define NRT   18
#define CANBitrate10K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (10000  * NRT) - 1))
#define CANBitrate20K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (20000  * NRT) - 1))
#define CANBitrate50K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (50000  * NRT) - 1))
#define CANBitrate100K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (100000  * NRT) - 1))
#define CANBitrate125K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (125000  * NRT) - 1))
#define CANBitrate250K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (250000  * NRT) - 1))
#define CANBitrate500K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (500000  * NRT) - 1))
#define CANBitrate800K  ((TSEG2(15) << 20)  + (TSEG1(15) << 16)  + (SJW_DEFAULT << 14) + (Fcanclk / (800000  * 15) - 1))
#define CANBitrate1000K ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (1000000 * NRT) - 1))

#elif Fcanclk == 24000000 || Fcanclk == 12000000

#define NRT   12
#define CANBitrate10K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (10000  * NRT) - 1))
#define CANBitrate20K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (20000  * NRT) - 1))
#define CANBitrate50K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (50000  * NRT) - 1))
#define CANBitrate100K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (100000  * NRT) - 1))
#define CANBitrate125K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (125000  * NRT) - 1))
#define CANBitrate250K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (250000  * NRT) - 1))
#define CANBitrate500K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (500000  * NRT) - 1))
#define CANBitrate800K  ((TSEG2(15) << 20)  + (TSEG1(15) << 16)  + (SJW_DEFAULT << 14) + (Fcanclk / (800000  * 15) - 1))
#define CANBitrate1000K ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (1000000 * NRT) - 1))

#elif Fcanclk == 4000000

#define NRT   16
#define CANBitrate10K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (10000  * NRT) - 1))
#define CANBitrate20K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (20000  * NRT) - 1))
#define CANBitrate50K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (50000  * NRT) - 1))
#define CANBitrate100K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (100000  * NRT) - 1))
#define CANBitrate125K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (125000  * NRT) - 1))
#define CANBitrate250K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (250000  * NRT) - 1))
#define CANBitrate500K  ((TSEG2(8) << 20) + (TSEG1(8) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (500000  * 8) - 1))
#define CANBitrate800K  0       //not support 800K baud
#define CANBitrate1000K 0       //not support 1000K baud

#elif Fcanclk == 6000000

#define NRT   12
#define CANBitrate10K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (10000  * NRT) - 1))
#define CANBitrate20K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (20000  * NRT) - 1))
#define CANBitrate50K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (50000  * NRT) - 1))
#define CANBitrate100K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (100000  * NRT) - 1))
#define CANBitrate125K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (125000  * NRT) - 1))
#define CANBitrate250K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (250000  * NRT) - 1))
#define CANBitrate500K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (500000  * NRT) - 1))
#define CANBitrate800K  0       //not support 800K baud
#define CANBitrate1000K 0       //not support 1000K baud


#elif Fcanclk == 8000000

#define NRT   16
#define CANBitrate10K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (10000  * NRT) - 1))
#define CANBitrate20K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (20000  * NRT) - 1))
#define CANBitrate50K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (50000  * NRT) - 1))
#define CANBitrate100K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (100000  * NRT) - 1))
#define CANBitrate125K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (125000  * NRT) - 1))
#define CANBitrate250K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (250000  * NRT) - 1))
#define CANBitrate500K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (500000  * NRT) - 1))
#define CANBitrate800K  ((TSEG2(10) << 20)  + (TSEG1(10) << 16)  + (SJW_DEFAULT << 14) + (Fcanclk / (800000  * 10) - 1))
#define CANBitrate1000K ((TSEG2(8) << 20) + (TSEG1(8) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (1000000 * 8) - 1))

#elif Fcanclk == 16000000 || Fcanclk == 32000000 || Fcanclk == 64000000

#define NRT   16
#define CANBitrate10K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (10000  * NRT) - 1))
#define CANBitrate20K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (20000  * NRT) - 1))
#define CANBitrate50K   ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (50000  * NRT) - 1))
#define CANBitrate100K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (100000  * NRT) - 1))
#define CANBitrate125K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (125000  * NRT) - 1))
#define CANBitrate250K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (250000  * NRT) - 1))
#define CANBitrate500K  ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (500000  * NRT) - 1))
#define CANBitrate800K  ((TSEG2(10) << 20)  + (TSEG1(10) << 16)  + (SJW_DEFAULT << 14) + (Fcanclk / (800000  * 10) - 1))
#define CANBitrate1000K ((TSEG2(NRT) << 20) + (TSEG1(NRT) << 16) + (SJW_DEFAULT << 14) + (Fcanclk / (1000000 * NRT) - 1))

#else
#error "please add different baud rate table according Fcanclk, 0 for unsupport baud"
#endif

//将philips的LPC2368的CAN波特率设置对应到STM32F10X中
#define CAN_BS1_TQ(dwBaudBtr)    ((dwBaudBtr >> 16) & 0xF) 
#define CAN_BS2_TQ(dwBaudBtr)    ((dwBaudBtr >> 20) & 0x7) 
#define CAN_SJW_TQ(dwBaudBtr)    ((dwBaudBtr >> 14) & 0x3) 
#define CAN_PRESCALE(dwBaudBtr)  ((dwBaudBtr & 0x3FF) + 1) 

//对于未支持的波特率, 请将对应项设置为0
static const  INT32U s_adwCanBitTime[] = 
{
  CANBitrate1000K,
  CANBitrate800K,
  CANBitrate500K,
  CANBitrate250K,
  CANBitrate125K,  
  CANBitrate100K,
  CANBitrate50K,
  CANBitrate20K,
  CANBitrate10K,
};


/********************************************************************************************************
*                               CAN驱动库初始化
* 描述 : 初始化CAN驱动库, 中止所有帧的发送与接收, 清空缓冲. 调用完成后CAN中断禁止
* 参数:       : 无
* 返回:       : 无
*********************************************************************************************************/
void CODrv_Init(void)
{
   CAN_DeInit();
}


/********************************************************************************************************
* 描述 : 进行波特率支持情况校验, 检查波特率的可行性，但不进行实际设置
*
* 参数:       : eCOBaud:    需要设置的Canopen波特率
* 返回:       : FALSE:  相应的波特率无法设置或无法执行自动波特率校验算法
                TRUE:   支持新的波特率或支持自动波特率校验算法
*********************************************************************************************************/
BOOLEAN CODrv_BaudCheck(ECOBaudRate eCOBaud)
{
  if (eCOBaud > CO_BPS_AUTO)
  {
    return FALSE;
  }
  else if (eCOBaud == CO_BPS_AUTO)
  {
 #if CO_AUTO_BPS_SUPPORT != 0
    return TRUE;
 #else
    return FALSE;
 #endif
  }
  else if (s_adwCanBitTime[eCOBaud] == 0)
  {
    return FALSE;
  }
  return TRUE;
}



static const ECOBaudRate s_arrAutoBpsSeqence[]= {CO_BPS_125K,  CO_BPS_250K, CO_BPS_1000K, CO_BPS_10K, 
                                        CO_BPS_20K, CO_BPS_50K,  CO_BPS_100K, CO_BPS_500K, CO_BPS_800K};

/********************************************************************************************************
* 描述 :      自动波特率校验
*
* 参数:       : eCOBaud:    需要设置的Canopen波特率
* 返回:       : 实际所采用的波特
* 注意        : 在自动波特率校验中, 标准指示灯的闪动需要在函数内部等待时间中 调用CO_TimeoutDispatch()实现
********************************************************************************************************/
ECOBaudRate CODrv_AutoBaud(void)
{
  INT32U  i = 0;
  ECOBaudRate eCOBaud;
  INT32U dwBaudBtr;
  INT32U j;
  INT32U dwStatus;
  CAN_InitTypeDef        CAN_InitStructure;

  while(1)
  {
    eCOBaud = s_arrAutoBpsSeqence[i];   //自动波持率顺序表, 按实现波特率出现概率选择
    dwBaudBtr = s_adwCanBitTime[eCOBaud];
    if (i++ >= ARRAY_SIZE(s_arrAutoBpsSeqence))
    { 
      i = 0;  
    }
    if (dwBaudBtr == 0)
    { //unsupport baud, jump to next
      continue; 
    }

    //Enter into listen only mode
    CAN_DeInit();
    CAN_StructInit(&CAN_InitStructure);
    CAN_InitStructure.CAN_Mode=CAN_Mode_Silent;
    CAN_InitStructure.CAN_SJW=CAN_SJW_TQ(dwBaudBtr);
    CAN_InitStructure.CAN_BS1=CAN_BS1_TQ(dwBaudBtr);
    CAN_InitStructure.CAN_BS2=CAN_BS2_TQ(dwBaudBtr);
    CAN_InitStructure.CAN_Prescaler= CAN_PRESCALE(dwBaudBtr);
    CAN_Init(&CAN_InitStructure);
    CODrv_FilterSetAll();
  

      //需重复接收2次, 以防止出现接收到半个帧之类情况
    for (j = 0; j < 2; j++)
    {  
      CAN->ESR |= (0x7 << 4);       //软件设置状态位用于检测接收状态是否改变
      do
      { //加入用户等待事件
        dwStatus = ((CAN->ESR >> 4) & 0x7);
      } while (dwStatus == 0x7);     //没有接收到错误帧或正确帧, 即接收状态未改变
 
      if (dwStatus == 0)               //接收到正确的CAN帧, 退出自动波特率
      {
        return eCOBaud;
      }
    }
  }
}

/********************************************************************************************************
* 描述 :      波特率初始化，进行新的波特率设置, 或进入自动波特率校验
*
* 参数:       : eCOBaud:    需要设置的Canopen波特率
* 返回:       : 实际所采用的波特? 若无法设置，返回CO_BPS_INVALID?    波特率设置正常, 则返回eCOBaud,
                  若为自动波特, 则返回实际采用的波特率,

* 注意        : 在自动波特率校验中, 标准指示灯的闪动需要在函数内部等待时间中 调用CO_TimeoutDispatch()实现
********************************************************************************************************/
ECOBaudRate CODrv_BaudSet(ECOBaudRate eCOBaud)
{
  INT32U dwBaudBtr;
  CAN_InitTypeDef        CAN_InitStructure;

  if (!CODrv_BaudCheck(eCOBaud))
  {
     return CO_BPS_INVALID;
  }
  
  if (eCOBaud == CO_BPS_AUTO)
  { //自动波特率校验  
    eCOBaud = CODrv_AutoBaud();
  }

  dwBaudBtr = s_adwCanBitTime[eCOBaud];
  CAN_DeInit();
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM=DISABLE;
  CAN_InitStructure.CAN_ABOM=DISABLE;
  CAN_InitStructure.CAN_AWUM=DISABLE;
  CAN_InitStructure.CAN_NART=DISABLE;
  CAN_InitStructure.CAN_RFLM=DISABLE;
  CAN_InitStructure.CAN_TXFP=DISABLE;
  /* CAN cell init */
  CAN_InitStructure.CAN_Mode= CAN_Mode_LoopBack;   //CAN_Mode_Normal;  //
  CAN_InitStructure.CAN_SJW=CAN_SJW_TQ(dwBaudBtr);
  CAN_InitStructure.CAN_BS1=CAN_BS1_TQ(dwBaudBtr);
  CAN_InitStructure.CAN_BS2=CAN_BS2_TQ(dwBaudBtr);
  CAN_InitStructure.CAN_Prescaler= CAN_PRESCALE(dwBaudBtr);
  CAN_Init(&CAN_InitStructure);
  return eCOBaud;
}


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
INT32U CODrv_MsgWaitSend(CO_MSG *pTxMsg, int timeout)
{
  INT8U err;
  INT32U errCode;

  errCode = CO_NO_ERR;
  if (timeout >= 0)
  {
    pTxMsg->COUsrData = (INT32U)OSMboxCreate(NULL);
    COMacSetUsrFlag(pTxMsg, timeout);          //将UserFlag设置成timeout, 以便发送任务作延时判断
    
    while (BUF_OPERATTION_OK != CAN_MsgToTxBuf(pTxMsg)) 
    { //缓冲满, 等待缓冲空
      OSTimeDly(1);
    }

    errCode = (INT32U)(INT32U)OSMboxPend((OS_EVENT *)pTxMsg->COUsrData, 0, &err);
    OSMboxDel((OS_EVENT *)pTxMsg->COUsrData, OS_DEL_ALWAYS, &err);
  }
  else
  {
    pTxMsg->COUsrData = 0;
    if (BUF_OPERATTION_OK != CAN_MsgToTxBuf(pTxMsg))
    {
      errCode = CO_ERR_TRANS_BUF_FULL;
    }
  }
  return errCode;
}


//过滤设置目前只支持11位标准帧格式
static INT8U  s_FilterMaskNums;              //    带掩码的帧计数
static INT8U  s_FilterNums;                  //不带掩码的帧计数
static INT8S  s_abyFMI[56];                  //索引值为接收到帧的FMI，存储值为累计调用CODrv_FilterSet函数的次数 
                                              //调用CODrv_FilterSetStart后, 累计值清0, 值为0xFF时, 表示需软件自已做比较判断
static INT16U s_awCobID[4]; 
static INT16U s_awCobIDWithMask[4];


/********************************************************************************************************
                       接收过滤器设置开始, 但未使能. 
注意:
   对于无自动过滤的驱动器, 则无需实现该函数, CANOPEN协议栈可实现软件过滤
   对于有自动过滤的硬件驱动器,  则直接由硬件进行接收过滤
********************************************************************************************************/
BOOLEAN CODrv_FilterSetStart(void)
{
   CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   INT32U i;

   s_FilterNums     = 0;
   s_FilterMaskNums = 0;
   memset(s_abyFMI, 0xff, sizeof(s_abyFMI));  //设置为0xFF

   CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;
   CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;
   CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
   CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
   CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
   CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FilterFIFO0;
   CAN_FilterInitStructure.CAN_FilterActivation=DISABLE;
        
   for (i = 0; i < 10; i++)
   {//前10个过滤值用于过滤不带掩码的COBID
     CAN_FilterInitStructure.CAN_FilterNumber = i;
     CAN_FilterInit(&CAN_FilterInitStructure); //禁能过滤器
   }

   CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FilterFIFO1;
   CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
   for (i = 10; i < 14; i++)
   { //剩余4个过滤值用于过滤带掩码的11位COBID
     CAN_FilterInitStructure.CAN_FilterNumber = i;
     CAN_FilterInit(&CAN_FilterInitStructure); //禁能过滤器
   }
   return TRUE;
}

static BOOLEAN CODrv__SetIdList(int iFilterIdx)
{
   CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   CAN_FilterInitStructure.CAN_FilterNumber = iFilterIdx;
   CAN_FilterInitStructure.CAN_FilterMode   =CAN_FilterMode_IdList;
   CAN_FilterInitStructure.CAN_FilterScale  =CAN_FilterScale_16bit;
   CAN_FilterInitStructure.CAN_FilterIdHigh =(s_awCobID[2] << 5);
   CAN_FilterInitStructure.CAN_FilterIdLow  =(s_awCobID[0] << 5);
   CAN_FilterInitStructure.CAN_FilterMaskIdHigh      =(s_awCobID[3] << 5);
   CAN_FilterInitStructure.CAN_FilterMaskIdLow       =(s_awCobID[1] << 5);
   CAN_FilterInitStructure.CAN_FilterFIFOAssignment  =CAN_FilterFIFO0;
   CAN_FilterInitStructure.CAN_FilterActivation      =ENABLE;   
   CAN_FilterInit(&CAN_FilterInitStructure); //使能过滤器
   return TRUE;
}

static BOOLEAN CODrv__SetIdMask(int iFilterIdx)
{  //带掩码16位ID设置从FIFO1进行过滤, 从FilterIdx10开始计数
   CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   CAN_FilterInitStructure.CAN_FilterNumber = iFilterIdx + 10;
   CAN_FilterInitStructure.CAN_FilterMode   =CAN_FilterMode_IdMask;
   CAN_FilterInitStructure.CAN_FilterScale  =CAN_FilterScale_16bit;
   CAN_FilterInitStructure.CAN_FilterIdHigh = (s_awCobIDWithMask[2] << 5);
   CAN_FilterInitStructure.CAN_FilterIdLow  = (s_awCobIDWithMask[0] << 5);
   CAN_FilterInitStructure.CAN_FilterMaskIdHigh      =(s_awCobIDWithMask[3] << 5);
   CAN_FilterInitStructure.CAN_FilterMaskIdLow       =(s_awCobIDWithMask[1] << 5);
   CAN_FilterInitStructure.CAN_FilterFIFOAssignment  =CAN_FilterFIFO1;
   CAN_FilterInitStructure.CAN_FilterActivation      =ENABLE;   
   CAN_FilterInit(&CAN_FilterInitStructure); //使能过滤器
   return TRUE;
}
  

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
BOOLEAN CODrv_FilterSet(INT32U dwCobID, INT32U dwMask)
{
   if (dwMask == 0xFFFFFFFF)
   { //单帧滤波
     s_awCobID[s_FilterNums % 4]     = dwCobID;
     s_abyFMI[s_FilterNums] = s_FilterNums + s_FilterMaskNums; 
     s_FilterNums++;
     if (s_FilterNums % 4 == 0 && s_FilterNums <= 40)
     {
       CODrv__SetIdList(s_FilterNums / 4 - 1);
     }
     return (s_FilterNums <= 40);
   }
   else
   {
     s_awCobIDWithMask[(s_FilterMaskNums * 2) % 4]     = dwCobID;
     s_awCobIDWithMask[(s_FilterMaskNums * 2) % 4 + 1] = dwMask;
     s_abyFMI[8 + s_FilterMaskNums] = s_FilterNums + s_FilterMaskNums;
     s_FilterMaskNums++;
     if (s_FilterMaskNums % 2 == 0 && s_FilterMaskNums <= 8)
     {
       CODrv__SetIdMask(s_FilterMaskNums / 2 - 1);
     }
     return (s_FilterMaskNums <= 8);
   }
}

/********************************************************************************************************
*                          设置过滤ID结束, 并使能所有先前所设的滤波值
********************************************************************************************************/
BOOLEAN CODrv_FilterSetFinish(void)
{
  INT32U i;
  for (i = s_FilterNums % 4; i != 0 && i < 4; i++)
  {    //将剩余的不带掩码的List过滤区填充
    CODrv_FilterSet(0xFFFF, (INT32U)(-1));
  } 
  for (i = s_FilterMaskNums % 2; i != 0 && i < 2; i++)
  {    //将剩余带掩码的过滤区填充
    CODrv_FilterSet(0xFFFF, 0xFFFF);
  }  
  return TRUE;
}


/********************************************************************************************************
                       接收所有帧的过滤
********************************************************************************************************/
BOOLEAN CODrv_FilterSetAll(void)
{
  CODrv_FilterSetStart();
  CODrv_FilterSet(0x0000, 0x0000);
  CODrv_FilterSetFinish();
  return TRUE;
}


/********************************************************************************************************
*                               CAN发送与接收功能选择
* 参数:       : eFuncSel:
                  FUNC_ENABLE:   允许CAN数据接收与发送
                  FUNC_DISABLE:  禁止CAN数据接收与发送
********************************************************************************************************/
void CODrv_OperationSet(EFuncSeletion eFuncSel)
{
   if (eFuncSel == FUNC_DISABLE)
   {
      CAN_ITConfig (CAN_IT_TME | CAN_IT_FMP0 | CAN_IT_FMP1 | CAN_IT_ERR |
                    CAN_IT_BOF | CAN_IT_EWG  | CAN_IT_LEC | CAN_IT_EPV , DISABLE);
   }
   else
   {
      CAN_ITConfig (CAN_IT_TME | CAN_IT_FMP0 | CAN_IT_FMP1 | CAN_IT_ERR |
                CAN_IT_BOF | CAN_IT_EWG  | CAN_IT_LEC | CAN_IT_EPV , ENABLE);
   }
}



/********************************************************************************************************
**    发送一个帧到CAN总线
**    参数:   pTxMsg:  需要发送的帧
            timeout:   等待超时时间, 为0时直接放入缓冲, 并返回
**     返回:  TRUE: 帧已经被发送, 或帧已存入到发送缓冲中
             FALSE: 帧发送出错, 或帧无法存入到发送缓冲中
* 注意     : 在等待发送的过程中, 标准指示灯的闪动需要在函数内部等待时间中 调用CO_TimeoutDispatch()实现
********************************************************************************************************/
BOOLEAN CODrv_SendMessage(CO_MSG *pTxMsg, BOOLEAN bWait)
{
  INT32U errType;
  INT16U timeout;

  timeout = COMacGetUsrFlag(pTxMsg);  
  errType = CODrv_MsgWaitSend(pTxMsg, timeout);
  if (CO_NO_ERR == errType || CO_TRANS_SUCESS == errType)
  {
    return TRUE;
  }
  else
  { 
    return FALSE;
  }
}
