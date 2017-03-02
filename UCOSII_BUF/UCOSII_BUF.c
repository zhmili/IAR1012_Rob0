#include <includes.h>
#include "ucosii_buf.h"


/********************************************************************************************************
**    UCOS_BufOut, ���ݳ��������
**    ����:   pUcosiiBuf:  ������ͷ��ַ
              pMessage:  ��Ϣ���д����, ����Nums��������
			  pItems:    ���ݴ����, ����Nums��������
			  pItem:   ���ڴ洢�����е�����
			  bWait:   TRUE, ������û������ʱ, ��ֹ�߳�ִ��, ֱ�����յ���Ч�����ݺ�ŷ���
			          FALSE, ������û������ʱֱ�ӷ��ش������
**     ����:    FALSE ������д���ʧ��
                TRUE  ������д����ɹ�
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
**    UCOS_BufIn, �����뻺�����
**    ����:   pUcosiiBuf:  ͨ������UCOS_BufCreate�����Ļ�����
              pItem:   �뻺����е�����
**     ����:    BUF_OPERATTION_OK �����������
                BUF_MEMORY_FULL   ������Ϊ��
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
**    UCOS_BufOut, ���ݳ��������
**    ����:   pUcosiiBuf:  ͨ������UCOS_BufCreate�����Ļ�����
              pItem:   ���ڴ洢�����е�����
			  bWait:   TRUE, ������û������ʱ, ��ֹ�߳�ִ��, ֱ�����յ���Ч�����ݺ�ŷ���
			          FALSE, ������û������ʱֱ�ӷ��ش������
			  timeout: UCOS��ʱ��λ, ��bWaitΪTRUEʱ��Ч
**     ����:    BUF_OPERATTION_OK �����������
                BUF_MEMORY_EMPTY  ������Ϊ��
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
**    UCOS_BufItems, �������ݸ���
**    ����:   pUcosiiBuf:  ͨ������UCOS_BufCreate�����Ļ�����
**    ����:   �������  
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
