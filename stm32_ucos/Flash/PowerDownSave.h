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
**                     浙江大学精益机电技术工程公司
**                              喷标机开发组
**
**                       http://www.jingyitech.com
**
**--------------文件信息--------------------------------------------------------
**文   件   名: PowerDownSave.h
**创   建   人: 施江肖
**版        本: V1.0
**最后修改日期: 2006年3月20日
**描        述: 掉电保存功能头文件
**主要函数列表:
**--------------历史版本信息----------------------------------------------------
** 创建人: 施江肖
** 版  本: V1.0
** 日　期: 2006年7月8日
** 描　述: 原始版本
** 修改人: 施江肖
** 版  本: V1.0
** 日　期: 2007年9月20日
** 描　述: 除了掉电保存功能外, 同时也可保存系统参数. 只需要传入主从两个扇区号即可
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
  INT8U  bySctArr[2]; //[0]: 主扇区号 [1] 从扇区号
  INT8U  byDataValid; //数据有效标记
  INT8U  byAddrNextValid;  //下一参数地址有效标记
  INT16U AlignSize;   //与扇区对齐后的数据大小
  INT16U size;        //实际数据大小
  INT32U dwPriority;  //掉电保持有效权, 值越大, 优先级越高. 最高优先级的为实际参数
  INT32U dwTableAcc;  //校验和, 用户数据校验累加和 
  INT32U dwAddrCur;   //本掉电数据在当前扇区中的偏移量
  INT32U dwAddrNext;  //下一掉电数据在当前扇区中的偏移量
  INT8U  data[1];     //用户参数
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
