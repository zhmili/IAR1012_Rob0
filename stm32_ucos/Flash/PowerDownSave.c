/******************************* Copyright (c)**********************************
**                     �㽭��ѧ������缼�����̹�˾
**                              ����������
**
**                       http://www.jingyitech.com
**
**--------------�ļ���Ϣ--------------------------------------------------------
**��   ��   ��: PowerDownSave.c
**��   ��   ��: ʩ��Ф
**��        ��: V1.0
**����޸�����: 2006��3��20��
**��        ��: ���籣�湦���ļ�
**��Ҫ�����б�:
**--------------��ʷ�汾��Ϣ----------------------------------------------------
** ������: ʩ��Ф
** ��  ��: V1.0
** �ա���: 2006��7��8��
** �衡��: ԭʼ�汾

** �޸���: ʩ��Ф
** ��  ��: V1.1
** �ա���: 2007��7��8��
** �衡��: ����������������Ϊ���籣�����������һ��Ҫ��ַ����
** �޸���: ʩ��Ф
** ��  ��: V1.2
** �ա���: 2007��9��20��
** �衡��: �����籣�����������ͨ����ͬ�������������
*******************************************************************************/

#define  POWER_DOWN_GLOBAL
#include "includes.h"
#include "PowerDownSave.h"
#include "stm32_flash.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

static STC_STORE_PARA  s_stcStoreHandle[MAX_STORE_HANDLE];
static INT8U s_HandleIndex;


/******************************************************************************
** ��������: CaculateTableLRC
** ��������: 
** �䡡��:   msg   ������֡������ʼ��ַ
             msgLength  ֡���ݳ���
** �䡡��:   ��
** ����ֵ:   ����õ���У��ֵ
** ����ģ��: ��
*******************************************************************************/
static INT32U  CaculateTableLRC(const INT8U * msg, unsigned msgLength)
{
  INT32U dwAcc = 0;
  while (msgLength --)
  {
    dwAcc += *msg++;
  }
  return dwAcc;
}

static INT32U  FlashDataLRC(INT32U dwFlashAddr,  INT16S size)
{
  return  CaculateTableLRC((const INT8U *)dwFlashAddr, size);
}



static BOOLEAN CopyBufToFlash(INT32U dwFlashAddr, INT8U *buf, INT32U size)
{
    return CopyDataToFlash(dwFlashAddr, buf, size);
}

static BOOLEAN CopyFlashToBuf(INT32U dwFlashAddr, INT8U *buf, INT32U size)
{
	memcpy(buf, (void *)dwFlashAddr, size);
	return TRUE;
}


static BOOLEAN  CompareFlashData(INT32U dwFlashAddr, INT8U *buf, INT32U size) 
{
  INT8U  data; 
  INT32S i = size;
  INT8U  byOrigin;

  /* Read data from SPI FLASH memory */
  do
  {
	data = *(INT8U *)dwFlashAddr;
	dwFlashAddr += 1;

    if (buf == NULL)
    {
      byOrigin = 0xFF;
    }
    else
    {
      byOrigin = *buf++;
    }

	if (byOrigin != data)
	{
	  return FALSE;
	}
  } while (--i > 0);

  return TRUE;
}


static BOOLEAN IsFlashAddrEmpty(INT32U  dwFlashAddr , INT16U size)
{
  return CompareFlashData(dwFlashAddr,  NULL, size);
}



ST_HANDLE AllocStoreHandle(INT16U size, INT8U mainSct, INT8U subSct)
{
  INT16U wAlignSize;
  if (s_HandleIndex >= MAX_STORE_HANDLE)
  {
    return NULL;
  } 
 
  wAlignSize = sizeof(STC_STORE_PARA) + size + BYTE_PER_SECTOR - 1;
  wAlignSize = (wAlignSize / BYTE_PER_SECTOR) * BYTE_PER_SECTOR;
  
  memset(&s_stcStoreHandle[s_HandleIndex], 0, sizeof(STC_STORE_PARA));

  s_stcStoreHandle[s_HandleIndex].bySctArr[0]   = mainSct;
  s_stcStoreHandle[s_HandleIndex].bySctArr[1]   = subSct;
  s_stcStoreHandle[s_HandleIndex].size          = size;
  s_stcStoreHandle[s_HandleIndex].AlignSize     = wAlignSize; 
  s_HandleIndex++;
 
  return &s_stcStoreHandle[s_HandleIndex - 1];
}

/*******************************************************************************
  ������:   CheckPowerDownData
  ��  ��:   �����籣�������Ƿ���Ч
  ��  ��:   
  ��  ��:   pstcTable
              pstcTable->byTableAcc�ṹ����ǰ����������
              
  ����ֵ:   TRUE��  ����������Ч
            FALSE:  ����������Ч
  ��  ��:   
******************************************************************************/
static BOOLEAN CheckParaData(ST_HANDLE handle, STC_STORE_PARA *pstcTable, INT32U priority)
{ 
  if (pstcTable->bySctArr[0] != handle->bySctArr[0]
     || pstcTable->bySctArr[1] != handle->bySctArr[1]
     || pstcTable->size   != handle->size)
  {
    return FALSE;
  }
  if (pstcTable->dwPriority < priority)
  {
    return FALSE;
  }
  return TRUE;
}

static INT32U GetAlignSize(ST_HANDLE handle)
{
  return handle->AlignSize;
}

/*******************************************************************************
  ������:   FindFreeAddr
  ��  ��:   Ѱ��һ���ռ�, ���ڱ�����һ�εĵ��籣������
  ��  ��:   
  ��  ��:   pstcTable
  ��  ��:   pstcTable

  ����ֵ:   TRUE��  �ҵ�����ռ�
            FALSE:  �޷��ҵ�����ռ�
  ��  ��:   
******************************************************************************/
static BOOLEAN FindFreeAddr(ST_HANDLE handle)
{
  INT32U dwFlashDataAddr;
  INT32U dwFlashSize;
  INT32U dwCurAddr;
  
  INT32U i, j, items, wAlignSize;
  handle->dwAddrNext = 0;
  wAlignSize         = GetAlignSize(handle);

  for (i = 0; i < 2; i++)
  { 
    dwFlashSize     = GetFlashSctSize(handle->bySctArr[i]);
    dwFlashDataAddr = GetFlashAddrFromSct(handle->bySctArr[i]);
    items = dwFlashSize / wAlignSize;
    
    for (j = 0; j < items; j++)
    {
       dwCurAddr = dwFlashDataAddr + j * wAlignSize;
       if (IsFlashAddrEmpty(dwCurAddr, wAlignSize))
       {
          handle->byAddrNextValid = TRUE;
		  handle->dwAddrNext = (INT32U)dwCurAddr;
          return TRUE;
       }
    } 
  }
  return FALSE;
}


/*******************************************************************************
  ������:   FindFreeSctWithErase
  ��  ��:   Ѱ��һ���ռ�, ���ڱ�����һ�εĵ��籣������, �����������, ���Զ����в�������
  ��  ��:   
  ��  ��:   handle
  ��  ��:   handle
              handle->dwAddrNext   ��һ�ε���洢����ʼ��ַ

  ����ֵ:   TRUE��  �Ѿ�׼������һ�α���ĵ�ַ
            FALSE:  ��һ��׼������ĵ�ַδ׼����
  ��  ��:   
******************************************************************************/
static BOOLEAN FindFreeAddrWithErase(ST_HANDLE handle)
{
  INT16U wEraseSct;
  INT16U wActiveSct;
  
  if (FindFreeAddr(handle))   //�Ѿ��ҵ����������, �򷵻�
  {
    return TRUE;
  }
  
  //�������������������, ����ѡ����ǲ�������ǰ������������һ����
  wActiveSct = GetSectorFromAddr(handle->dwAddrCur);
  if (wActiveSct == handle->bySctArr[0])
  {
    wEraseSct = handle->bySctArr[1];
  }
  else
  {
    wEraseSct = handle->bySctArr[0];
  }

  SelSector(wEraseSct, wEraseSct);
  EraseSector(wEraseSct, wEraseSct);
  return FindFreeAddr(handle);
}



BOOLEAN   LoadSystemPara(ST_HANDLE handle, INT8U *pdata)
{
  INT32U dwFlashDataAddr;     //���籣������ʵ�ʴ�ŵ�ַ
  INT32U dwCurPriority;       //��ǰ���ȼ�
  INT32U dwFlashSize;
  INT16U wAlignSize;
  INT16U datasize;
  INT16U items;
  INT32U dwDataOff;
  INT32U dwFlashAddr;
  INT32U dwAccChk;
  int i, j;
  INT32U  dwValidFlashAddr;
  STC_STORE_PARA  stcTable;
  BOOLEAN bFind;

  bFind = FALSE;
  dwValidFlashAddr = 0;

  if (handle == NULL)
  {
    return FALSE;
  }

  datasize   = handle->size;
  wAlignSize = handle->AlignSize;
  dwCurPriority      = 0;  
  
  for (i = 0; i < 2; i++)
  {
    dwFlashSize     = GetFlashSctSize(handle->bySctArr[i]);
    dwFlashDataAddr = GetFlashAddrFromSct(handle->bySctArr[i]);
    items = dwFlashSize / wAlignSize;
    
    for (j = items-1; j >= 0; j--)
    {
	   dwFlashAddr = (dwFlashDataAddr + j * wAlignSize);
       CopyFlashToBuf(dwFlashAddr, (INT8U *)&stcTable, sizeof(STC_STORE_PARA));

       if (CheckParaData(handle, &stcTable, dwCurPriority))
       {
         dwDataOff   = (INT32U)(((ST_HANDLE)0)->data);
		 dwAccChk = FlashDataLRC(dwFlashAddr + dwDataOff, stcTable.size);
		 if (dwAccChk != stcTable.dwTableAcc)
		 {
		    continue;
		 }

         bFind = TRUE;
         dwValidFlashAddr  = dwFlashAddr + dwDataOff;
         dwCurPriority     = stcTable.dwPriority;
         handle->dwAddrCur = dwFlashAddr;
       }       
    }
  }
  
  if (!bFind)   //û���ҵ���Ч�ĵ��籣������
  {
    FindFreeAddrWithErase(handle);
    return FALSE;
  }

  CopyFlashToBuf(dwValidFlashAddr, pdata, datasize);

  handle->dwPriority  = dwCurPriority;
  handle->byDataValid = TRUE;

  return FindFreeAddrWithErase(handle);
}


BOOLEAN   StoreSystemPara(ST_HANDLE handle, INT8U *pdata)
{
  INT8U buf[BYTE_PER_SECTOR];
  INT32U dwDataOff, dwFlashDataAddr;
  int leftSize, curSize;
  
  
  if (handle == NULL || !handle->byAddrNextValid)
  {
    return FALSE;
  }
  dwFlashDataAddr = handle->dwAddrNext;
    
  memset(buf, 0xFF, sizeof(buf));
  handle->dwPriority++;    //���ȼ���1 
  handle->dwTableAcc = CaculateTableLRC((INT8U *)pdata, handle->size);
  dwDataOff = (INT32U)(((ST_HANDLE)0)->data);
  memcpy(buf, handle, dwDataOff);

  leftSize =  handle->size;
  curSize  =  min(leftSize, BYTE_PER_SECTOR - dwDataOff);
  memcpy(&buf[dwDataOff], pdata, curSize);
  pdata    = pdata + curSize;
  leftSize = leftSize - curSize;

  while (leftSize >= 0)
  {
    if (!CopyBufToFlash(dwFlashDataAddr, buf, BYTE_PER_SECTOR))
    {
      return FALSE;
    }
    dwFlashDataAddr += BYTE_PER_SECTOR;
 
    curSize = min(leftSize, BYTE_PER_SECTOR);
    if (curSize == 0)
    {
      break;
    }
    
    memcpy(buf, pdata, curSize);
    pdata += curSize;
    leftSize  -= curSize;
  }
  
  handle->dwAddrCur = handle->dwAddrNext;    //���¶��嵱ǰ�������ݵ�ַ
  return FindFreeAddrWithErase(handle);      //Ѱ���µĿ�����
}
