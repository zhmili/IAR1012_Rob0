#ifndef STM32_FLASH_H
#define STM32_FLASH_H

BOOLEAN   CopyDataToFlash(INT32U dwFlashAddr, INT8U *buf, INT32U size);


INT32U GetFlashAddrFromSct(INT8U bySct);

INT32U GetFlashSctSize(INT8U bySct);

INT32U GetSectorFromAddr(INT32U adr);

void  SelSector(int start, int end);

void  EraseSector(int start, int end);



#endif
