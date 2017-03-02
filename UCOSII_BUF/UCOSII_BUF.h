#ifndef UCOSII_BUF_H
#define UCOSII_BUF_H

typedef enum EBufErrCode
{
  BUF_OPERATTION_OK    = 0,
  BUF_MEMORY_FULL      = -1,
  BUF_MEMORY_EMPTY     = -2,
  BUF_OPER_TIMEOUT     = -3,  //���������ʱ
} EBufErrCode;

typedef struct TUcosiiBuf
{
  OS_EVENT  *UcosBufQueue;		 //����
  OS_MEM    *UcosBufMem;		 //��̬��������ڴ�
  INT32U    ItemNums;	         //�ܹ��洢��
  INT32U    ItemSize;            //�����С
  void    **QueueMessage;       //��Ŷ���ָ��
  void     *Items;              //��Ÿ���洢��
} TUcosiiBuf;

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
BOOLEAN  UCOS_BufCreate(TUcosiiBuf *pUcosiiBuf, void **pMessage, void *pItems, int  Nums, int Size);

/********************************************************************************************************
**    UCOS_BufIn, �����뻺�����
**    ����:   pUcosiiBuf:  ͨ������UCOS_BufCreate�����Ļ�����
              pItem:   �뻺����е�����
**     ����:    BUF_OPERATTION_OK �����������
                BUF_MEMORY_FULL   ������Ϊ��
********************************************************************************************************/
EBufErrCode	UCOS_BufIn(TUcosiiBuf *pUcosiiBuf, void *pItem);

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
EBufErrCode	UCOS_BufOut(TUcosiiBuf *pUcosiiBuf, void *pItem, BOOLEAN bWait, INT32U timeout);

/********************************************************************************************************
**    UCOS_BufItems, �������ݸ���
**    ����:   pUcosiiBuf:  ͨ������UCOS_BufCreate�����Ļ�����
**    ����:   �������  
********************************************************************************************************/
INT32U	UCOS_BufItems(TUcosiiBuf *pUcosiiBuf);

#endif
