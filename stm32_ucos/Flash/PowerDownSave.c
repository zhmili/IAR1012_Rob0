/******************************* Copyright (c)**********************************
**                     浙江大学精益机电技术工程公司
**                              喷标机开发组
**
**                       http://www.jingyitech.com
**
**--------------文件信息--------------------------------------------------------
**文   件   名: PowerDownSave.c
**创   建   人: 施江肖
**版        本: V1.0
**最后修改日期: 2006年3月20日
**描        述: 掉电保存功能文件
**主要函数列表:
**--------------历史版本信息----------------------------------------------------
** 创建人: 施江肖
** 版  本: V1.0
** 日　期: 2006年7月8日
** 描　述: 原始版本

** 修改人: 施江肖
** 版  本: V1.1
** 日　期: 2007年7月8日
** 描　述: 将任意扇区都可做为掉电保存的扇区而不一定要地址连续
** 修改人: 施江肖
** 版  本: V1.2
** 日　期: 2007年9月20日
** 描　述: 将掉电保存与参数保存通过不同的扇区隔离出来
*******************************************************************************/

#define  POWER_DOWN_GLOBAL
#include "includes.h"
#include "PowerDownSave.h"
#include "stm32_flash.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

static STC_STORE_PARA  s_stcStoreHandle[MAX_STORE_HANDLE];
static INT8U s_HandleIndex;


/******************************************************************************
** 函数名称: CaculateTableLRC
** 功能描述: 
** 输　入:   msg   需计算的帧数据起始地址
             msgLength  帧数据长度
** 输　出:   无
** 返回值:   计算得到的校验值
** 调用模块: 无
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
  函数名:   CheckPowerDownData
  描  述:   检查掉电保存数据是否有效
  调  用:   
  输  入:   pstcTable
              pstcTable->byTableAcc结构体以前的所有数据
              
  返回值:   TRUE：  掉电数据有效
            FALSE:  掉电数据无效
  其  他:   
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
  函数名:   FindFreeAddr
  描  述:   寻找一个空间, 用于保存下一次的掉电保存数据
  调  用:   
  输  入:   pstcTable
  输  出:   pstcTable

  返回值:   TRUE：  找到保存空间
            FALSE:  无法找到保存空间
  其  他:   
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
  函数名:   FindFreeSctWithErase
  描  述:   寻找一个空间, 用于保存下一次的掉电保存数据, 如果扇区已满, 则自动进行擦除操作
  调  用:   
  输  入:   handle
  输  出:   handle
              handle->dwAddrNext   下一次掉电存储区起始地址

  返回值:   TRUE：  已经准备好下一次保存的地址
            FALSE:  下一次准备保存的地址未准备好
  其  他:   
******************************************************************************/
static BOOLEAN FindFreeAddrWithErase(ST_HANDLE handle)
{
  INT16U wEraseSct;
  INT16U wActiveSct;
  
  if (FindFreeAddr(handle))   //已经找到多余空扇区, 则返回
  {
    return TRUE;
  }
  
  //以下则擦除后重新申请, 擦除选择的是不包含当前掉电扇区的另一扇区
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
  INT32U dwFlashDataAddr;     //掉电保持数据实际存放地址
  INT32U dwCurPriority;       //当前优先级
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
  
  if (!bFind)   //没有找到有效的掉电保存数据
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
  handle->dwPriority++;    //优先级加1 
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
  
  handle->dwAddrCur = handle->dwAddrNext;    //重新定义当前掉电数据地址
  return FindFreeAddrWithErase(handle);      //寻找新的空扇区
}
