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

//��philips��LPC2368��CAN���������ö�Ӧ��STM32F10X��
#define CAN_BS1_TQ(dwBaudBtr)    ((dwBaudBtr >> 16) & 0xF) 
#define CAN_BS2_TQ(dwBaudBtr)    ((dwBaudBtr >> 20) & 0x7) 
#define CAN_SJW_TQ(dwBaudBtr)    ((dwBaudBtr >> 14) & 0x3) 
#define CAN_PRESCALE(dwBaudBtr)  ((dwBaudBtr & 0x3FF) + 1) 

//����δ֧�ֵĲ�����, �뽫��Ӧ������Ϊ0
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
*                               CAN�������ʼ��
* ���� : ��ʼ��CAN������, ��ֹ����֡�ķ��������, ��ջ���. ������ɺ�CAN�жϽ�ֹ
* ����:       : ��
* ����:       : ��
*********************************************************************************************************/
void CODrv_Init(void)
{
   CAN_DeInit();
}


/********************************************************************************************************
* ���� : ���в�����֧�����У��, ��鲨���ʵĿ����ԣ���������ʵ������
*
* ����:       : eCOBaud:    ��Ҫ���õ�Canopen������
* ����:       : FALSE:  ��Ӧ�Ĳ������޷����û��޷�ִ���Զ�������У���㷨
                TRUE:   ֧���µĲ����ʻ�֧���Զ�������У���㷨
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
* ���� :      �Զ�������У��
*
* ����:       : eCOBaud:    ��Ҫ���õ�Canopen������
* ����:       : ʵ�������õĲ���
* ע��        : ���Զ�������У����, ��׼ָʾ�Ƶ�������Ҫ�ں����ڲ��ȴ�ʱ���� ����CO_TimeoutDispatch()ʵ��
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
    eCOBaud = s_arrAutoBpsSeqence[i];   //�Զ�������˳���, ��ʵ�ֲ����ʳ��ָ���ѡ��
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
  

      //���ظ�����2��, �Է�ֹ���ֽ��յ����֮֡�����
    for (j = 0; j < 2; j++)
    {  
      CAN->ESR |= (0x7 << 4);       //�������״̬λ���ڼ�����״̬�Ƿ�ı�
      do
      { //�����û��ȴ��¼�
        dwStatus = ((CAN->ESR >> 4) & 0x7);
      } while (dwStatus == 0x7);     //û�н��յ�����֡����ȷ֡, ������״̬δ�ı�
 
      if (dwStatus == 0)               //���յ���ȷ��CAN֡, �˳��Զ�������
      {
        return eCOBaud;
      }
    }
  }
}

/********************************************************************************************************
* ���� :      �����ʳ�ʼ���������µĲ���������, ������Զ�������У��
*
* ����:       : eCOBaud:    ��Ҫ���õ�Canopen������
* ����:       : ʵ�������õĲ���? ���޷����ã�����CO_BPS_INVALID?    ��������������, �򷵻�eCOBaud,
                  ��Ϊ�Զ�����, �򷵻�ʵ�ʲ��õĲ�����,

* ע��        : ���Զ�������У����, ��׼ָʾ�Ƶ�������Ҫ�ں����ڲ��ȴ�ʱ���� ����CO_TimeoutDispatch()ʵ��
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
  { //�Զ�������У��  
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
INT32U CODrv_MsgWaitSend(CO_MSG *pTxMsg, int timeout)
{
  INT8U err;
  INT32U errCode;

  errCode = CO_NO_ERR;
  if (timeout >= 0)
  {
    pTxMsg->COUsrData = (INT32U)OSMboxCreate(NULL);
    COMacSetUsrFlag(pTxMsg, timeout);          //��UserFlag���ó�timeout, �Ա㷢����������ʱ�ж�
    
    while (BUF_OPERATTION_OK != CAN_MsgToTxBuf(pTxMsg)) 
    { //������, �ȴ������
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


//��������Ŀǰֻ֧��11λ��׼֡��ʽ
static INT8U  s_FilterMaskNums;              //    �������֡����
static INT8U  s_FilterNums;                  //���������֡����
static INT8S  s_abyFMI[56];                  //����ֵΪ���յ�֡��FMI���洢ֵΪ�ۼƵ���CODrv_FilterSet�����Ĵ��� 
                                              //����CODrv_FilterSetStart��, �ۼ�ֵ��0, ֵΪ0xFFʱ, ��ʾ������������Ƚ��ж�
static INT16U s_awCobID[4]; 
static INT16U s_awCobIDWithMask[4];


/********************************************************************************************************
                       ���չ��������ÿ�ʼ, ��δʹ��. 
ע��:
   �������Զ����˵�������, ������ʵ�ָú���, CANOPENЭ��ջ��ʵ���������
   �������Զ����˵�Ӳ��������,  ��ֱ����Ӳ�����н��չ���
********************************************************************************************************/
BOOLEAN CODrv_FilterSetStart(void)
{
   CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   INT32U i;

   s_FilterNums     = 0;
   s_FilterMaskNums = 0;
   memset(s_abyFMI, 0xff, sizeof(s_abyFMI));  //����Ϊ0xFF

   CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;
   CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;
   CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
   CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
   CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
   CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FilterFIFO0;
   CAN_FilterInitStructure.CAN_FilterActivation=DISABLE;
        
   for (i = 0; i < 10; i++)
   {//ǰ10������ֵ���ڹ��˲��������COBID
     CAN_FilterInitStructure.CAN_FilterNumber = i;
     CAN_FilterInit(&CAN_FilterInitStructure); //���ܹ�����
   }

   CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FilterFIFO1;
   CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
   for (i = 10; i < 14; i++)
   { //ʣ��4������ֵ���ڹ��˴������11λCOBID
     CAN_FilterInitStructure.CAN_FilterNumber = i;
     CAN_FilterInit(&CAN_FilterInitStructure); //���ܹ�����
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
   CAN_FilterInit(&CAN_FilterInitStructure); //ʹ�ܹ�����
   return TRUE;
}

static BOOLEAN CODrv__SetIdMask(int iFilterIdx)
{  //������16λID���ô�FIFO1���й���, ��FilterIdx10��ʼ����
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
   CAN_FilterInit(&CAN_FilterInitStructure); //ʹ�ܹ�����
   return TRUE;
}
  

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
BOOLEAN CODrv_FilterSet(INT32U dwCobID, INT32U dwMask)
{
   if (dwMask == 0xFFFFFFFF)
   { //��֡�˲�
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
*                          ���ù���ID����, ��ʹ��������ǰ������˲�ֵ
********************************************************************************************************/
BOOLEAN CODrv_FilterSetFinish(void)
{
  INT32U i;
  for (i = s_FilterNums % 4; i != 0 && i < 4; i++)
  {    //��ʣ��Ĳ��������List���������
    CODrv_FilterSet(0xFFFF, (INT32U)(-1));
  } 
  for (i = s_FilterMaskNums % 2; i != 0 && i < 2; i++)
  {    //��ʣ�������Ĺ��������
    CODrv_FilterSet(0xFFFF, 0xFFFF);
  }  
  return TRUE;
}


/********************************************************************************************************
                       ��������֡�Ĺ���
********************************************************************************************************/
BOOLEAN CODrv_FilterSetAll(void)
{
  CODrv_FilterSetStart();
  CODrv_FilterSet(0x0000, 0x0000);
  CODrv_FilterSetFinish();
  return TRUE;
}


/********************************************************************************************************
*                               CAN��������չ���ѡ��
* ����:       : eFuncSel:
                  FUNC_ENABLE:   ����CAN���ݽ����뷢��
                  FUNC_DISABLE:  ��ֹCAN���ݽ����뷢��
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
**    ����һ��֡��CAN����
**    ����:   pTxMsg:  ��Ҫ���͵�֡
            timeout:   �ȴ���ʱʱ��, Ϊ0ʱֱ�ӷ��뻺��, ������
**     ����:  TRUE: ֡�Ѿ�������, ��֡�Ѵ��뵽���ͻ�����
             FALSE: ֡���ͳ���, ��֡�޷����뵽���ͻ�����
* ע��     : �ڵȴ����͵Ĺ�����, ��׼ָʾ�Ƶ�������Ҫ�ں����ڲ��ȴ�ʱ���� ����CO_TimeoutDispatch()ʵ��
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
