#ifndef UCOSII_BUF_H
#define UCOSII_BUF_H

typedef enum EBufErrCode
{
  BUF_OPERATTION_OK    = 0,
  BUF_MEMORY_FULL      = -1,
  BUF_MEMORY_EMPTY     = -2,
  BUF_OPER_TIMEOUT     = -3,  //缓冲操作超时
} EBufErrCode;

typedef struct TUcosiiBuf
{
  OS_EVENT  *UcosBufQueue;		 //队列
  OS_MEM    *UcosBufMem;		 //动态分配的项内存
  INT32U    ItemNums;	         //总共存储项
  INT32U    ItemSize;            //单项大小
  void    **QueueMessage;       //存放队列指针
  void     *Items;              //存放各类存储项
} TUcosiiBuf;

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
BOOLEAN  UCOS_BufCreate(TUcosiiBuf *pUcosiiBuf, void **pMessage, void *pItems, int  Nums, int Size);

/********************************************************************************************************
**    UCOS_BufIn, 数据入缓冲队列
**    参数:   pUcosiiBuf:  通过调用UCOS_BufCreate创建的缓冲区
              pItem:   入缓冲队列的数据
**     返回:    BUF_OPERATTION_OK 操作正常完成
                BUF_MEMORY_FULL   缓冲区为满
********************************************************************************************************/
EBufErrCode	UCOS_BufIn(TUcosiiBuf *pUcosiiBuf, void *pItem);

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
EBufErrCode	UCOS_BufOut(TUcosiiBuf *pUcosiiBuf, void *pItem, BOOLEAN bWait, INT32U timeout);

/********************************************************************************************************
**    UCOS_BufItems, 缓冲数据个数
**    参数:   pUcosiiBuf:  通过调用UCOS_BufCreate创建的缓冲区
**    返回:   缓冲个数  
********************************************************************************************************/
INT32U	UCOS_BufItems(TUcosiiBuf *pUcosiiBuf);

#endif
