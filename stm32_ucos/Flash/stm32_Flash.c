#include "includes.h"
#include "stm32_flash.h"

#define PageSize   ((u16)0x400)


INT32U GetFlashAddrFromSct(INT8U bySct)
{ //设置每个扇区为8K字节
  return  0x8000000 + bySct * 0x2000; 
}

INT32U GetFlashSctSize(INT8U bySct)
{ //所有扇区均为8K字节, 以方便处理
  return 0x2000;
}

INT32U GetSectorFromAddr(INT32U adr)
{
  return (adr  - 0x8000000) / 0x2000;
}

void  SelSector(int start, int end)
{
  return;
}

void  EraseSector(int start, int end)
{
  INT32U EndAddr, StartAddr;
  INT32U NbrOfPage;
  INT32U EraseCounter;
  FLASH_Status FLASHStatus;

  FLASH_Unlock();

  StartAddr  = GetFlashAddrFromSct(start);
  EndAddr    = GetFlashAddrFromSct(end);
  EndAddr   += GetFlashSctSize(end);

  /* Define the number of page to be erased */
  NbrOfPage = (EndAddr - StartAddr) >> 10;

  /* Clear All pending flags */
  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

  FLASHStatus = FLASH_COMPLETE;
  /* Erase the FLASH pages */
  for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
  {
    FLASHStatus = FLASH_ErasePage(StartAddr + (PageSize * EraseCounter));
  }
}


BOOLEAN   CopyDataToFlash(INT32U dwFlashAddr, INT8U *buf, INT32U size)
{
  INT32U StartAddr, EndAddr;
  INT32U Address;
  INT32U *pData;
  FLASH_Status FLASHStatus;

  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();

  StartAddr = dwFlashAddr;
  EndAddr   = dwFlashAddr + size;
 
  Address = StartAddr;
  FLASHStatus = FLASH_COMPLETE;
  pData = (INT32U *)buf;

  while((Address < EndAddr) && (FLASHStatus == FLASH_COMPLETE))
  {
    FLASHStatus = FLASH_ProgramWord(Address, *pData++);
    Address = Address + 4;
  }
  
  /* Check the corectness of written data */
  Address = StartAddr;
  pData = (INT32U *)buf;

  while(Address < EndAddr)
  {
    if((*(vu32*) Address) != *pData++)
    {
      return FALSE;
    }
    Address += 4;
  }

  return TRUE;
}
