//---------------------------------------------------------------------------

#ifndef CommH
#define CommH
//---------------------------------------------------------------------------

BOOLEAN InitUserProgramArea(void);

INT8U CheckAccLRC(INT8U *pdata, INT16U size);

INT8U RecvFxMsg(char *buf, INT8U maxlen);

INT16U PlcFrameExeucte(INT8U *pszDst, INT8U *pszSrc, INT16U srcLen);

#endif
				/******************************* Copyright (c)**********************************
**                     �㽭��ѧ������缼�����̹�˾
**                              ����������
**
**                       http://www.jingyitech.com
**
**--------------�ļ���Ϣ--------------------------------------------------------
**��   ��   ��: PowerDownSave.h
**��   ��   ��: ʩ��Ф
**��        ��: V1.0
**����޸�����: 2006��3��20��
**��        ��: ���籣�湦��ͷ�ļ�
**��Ҫ�����б�:
**--------------��ʷ�汾��Ϣ----------------------------------------------------
** ������: ʩ��Ф
** ��  ��: V1.0
** �ա���: 2006��7��8��
** �衡��: ԭʼ�汾
** �޸���: ʩ��Ф
** ��  ��: V1.0
** �ա���: 2007��9��20��
** �衡��: ���˵��籣�湦����, ͬʱҲ�ɱ���ϵͳ����. ֻ��Ҫ�����������������ż���
*******************************************************************************/

#ifndef POWERDOWN_SAVE_H
#define POWERDOWN_SAVE_H

#ifdef POWER_DOWN_GLOBAL
#define POWER_DOWN_EXTERN
#else
#define POWER_DOWN_EXTERN extern
#endif


#ifndef MAX_STORE_HANDLE
#define MAX_STORE_HANDLE   3
#endif

#ifndef BYTE_PER_SECTOR
#define BYTE_PER_SECTOR    256
#endif

typedef struct STC_STORE_PARA
{
  INT8U  bySctArr[2]; //[0]: �������� [1] ��������
  INT8U  byDataValid; //������Ч���
  INT8U  byAddrNextValid;  //��һ������ַ��Ч���
  INT16U AlignSize;   //���������������ݴ�С
  INT16U size;        //ʵ�����ݴ�С
  INT32U dwPriority;  //���籣����ЧȨ, ֵԽ��, ���ȼ�Խ��. ������ȼ���Ϊʵ�ʲ���
  INT32U dwTableAcc;  //У���, �û�����У���ۼӺ� 
  INT32U dwAddrCur;   //�����������ڵ�ǰ�����е�ƫ����
  INT32U dwAddrNext;  //��һ���������ڵ�ǰ�����е�ƫ����
  INT8U  data[1];     //�û�����
} STC_STORE_PARA;

typedef STC_STORE_PARA*  ST_HANDLE;

POWER_DOWN_EXTERN ST_HANDLE hSystemPara;

ST_HANDLE AllocStoreHandle(INT16U size, INT8U mainSct, INT8U subSct);

BOOLEAN   LoadSystemPara(ST_HANDLE handle, INT8U *pdata);

BOOLEAN   StoreSystemPara(ST_HANDLE handle, INT8U *pdata);

BOOLEAN   CopyDataToFlash(INT32U dwFlashAddr, INT8U *buf, INT32U size);


INT32U GetFlashAddrFromSct(INT8U bySct);

INT32U GetFlashSctSize(INT8U bySct);

INT32U GetSectorFromAddr(INT32U adr);


#endif
