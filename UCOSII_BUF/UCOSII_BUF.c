#include <includes.h>
#include "ucosii_buf.h"


/********************************************************************************************************
**    UCOS_BufOut, 数据出缓冲队列
**    参数:   pUcosiiBuf:  缓冲区头地址
              pMessage:  消息队列存放区, 具有Nums个数据项
			  pItems:    数据存放区, 具有Nums个数据项
			  pItem:   用于存储出队列的数据
			  bWait:   TRUE, 缓冲中没有数据时, 阻止线程执行, 直到接收到有效的数据后才返回
			          FALSE, 缓冲中没有数据时直接返回错误代码
**     返回:    FALSE 缓冲队列创建失败
                TRUE  缓冲队列创建成功
********************************************************************************************************/
BOOLEAN  UCOS_BufCreate(TUcosiiBuf *pUcosiiBuf, void **pMessage, void *pItems, int  Nums, int Size)
{
  INT8U err;
  pUcosiiBuf->QueueMessage = pMessage;
  pUcosiiBuf->Items        = pItems;
  pUcosiiBuf->ItemNums     = Nums;
  pUcosiiBuf->ItemSize     = Size;
  pUcosiiBuf->UcosBufQueue = OSQCreate(pMessage, Nums);
  pUcosiiBuf->UcosBufMem   = OSMemCreate(pItems, Nums, Size, &err);

  if (pUcosiiBuf->UcosBufQueue == NULL || pUcosiiBuf->UcosBufMem == NULL)
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

/********************************************************************************************************
**    UCOS_BufIn, 数据入缓冲队列
**    参数:   pUcosiiBuf:  通过调用UCOS_BufCreate创建的缓冲区
              pItem:   入缓冲队列的数据
**     返回:    BUF_OPERATTION_OK 操作正常完成
                BUF_MEMORY_FULL   缓冲区为满
********************************************************************************************************/
EBufErrCode	UCOS_BufIn(TUcosiiBuf *pUcosiiBuf, void *pItem)
{
  INT8U err;
  void *pBufItem; 

  pBufItem = (void *)OSMemGet(pUcosiiBuf->UcosBufMem, &err);
  if (pBufItem == NULL)
  {
    return  BUF_MEMORY_FULL;
  }
  memcpy(pBufItem, pItem, pUcosiiBuf->ItemSize);

  OSQPost(pUcosiiBuf->UcosBufQueue, pBufItem);

  return BUF_OPERATTION_OK;
}


/********************************************************************************************************
**    UCOS_BufOut, 数据出缓冲队列
**    参数:   pUcosiiBuf:  通过调用UCOS_BufCreate创建的缓冲区
              pItem:   用于存储出队列的数据
			  bWait:   TRUE, 缓冲中没有数据时, 阻止线程执行, 直到接收到有效的数据后才返回
			          FALSE, 缓冲中没有数据时直接返回错误代码
			  timeout: UCOS延时单位, 当bWait为TRUE时有效
**     返回:    BUF_OPERATTION_OK 操作正常完成
                BUF_MEMORY_EMPTY  缓冲区为空
********************************************************************************************************/
EBufErrCode	UCOS_BufOut(TUcosiiBuf *pUcosiiBuf, void *pItem, BOOLEAN bWait, INT32U timeout)
{
  INT8U err;
  void *pBufItem;
  
  if (bWait)
  {
    pBufItem = (void *)OSQPend(pUcosiiBuf->UcosBufQueue, timeout, &err);
  }
  else
  {
    pBufItem = (void *)OSQAccept(pUcosiiBuf->UcosBufQueue, &err);
  }

  if (pBufItem != NULL)
  {	
    memcpy(pItem, pBufItem, pUcosiiBuf->ItemSize);
    OSMemPut(pUcosiiBuf->UcosBufMem, pBufItem);
	return BUF_OPERATTION_OK;
  }
  else if (err == OS_TIMEOUT)
  {
    return BUF_OPER_TIMEOUT;
  }
  else
  {
    return  BUF_MEMORY_EMPTY;
  }
}

/********************************************************************************************************
**    UCOS_BufItems, 缓冲数据个数
**    参数:   pUcosiiBuf:  通过调用UCOS_BufCreate创建的缓冲区
**    返回:   缓冲个数  
********************************************************************************************************/
INT32U	UCOS_BufItems(TUcosiiBuf *pUcosiiBuf)
{
    INT32U items;
    OS_Q      *pq;
#if OS_CRITICAL_METHOD == 3                            /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
    pq = (OS_Q *)(pUcosiiBuf->UcosBufQueue)->OSEventPtr;
    items = pq->OSQEntries;
    OS_EXIT_CRITICAL();
    return (items);
}
