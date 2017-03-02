/*
*********************************************************************************************************
* Filename    : os_can_buf.c
* Version     : V1.00
* Programmer(s) : shijiangxiao    2008-02-20
                  shijiangxiao    2008-04-01  �޸ĳ���, ��ǿģ�������
*********************************************************************************************************
*/

#include <includes.h>
#include "os_can_flag.h"

static OS_FLAG_GRP *s_FlagCan;

/*******************************************************************************
             CAN������ǳ�ʼ��
*******************************************************************************/
BOOLEAN    CAN_FlagInit(void)
{
  INT8U err;
  s_FlagCan = OSFlagCreate(CAN_FLAG_TME0 | CAN_FLAG_TME1 | CAN_FLAG_TME2, &err);
  return (s_FlagCan != NULL) ? TRUE : FALSE;
}


/*******************************************************************************
             CAN������ǵȴ�, ���嶨�嶨���OSFlagPend
*******************************************************************************/
OS_FLAGS     CAN_FlagPend     (OS_FLAGS       flags,
                INT8U     wait_type,
                INT16U    timeout,
                INT8U    *perr)
{
  return OSFlagPend(s_FlagCan, flags, wait_type, timeout, perr);
}

/*******************************************************************************
             CAN�����������, ���嶨�嶨���OSFlagPost
*******************************************************************************/
OS_FLAGS     CAN_FlagPost     (OS_FLAGS       flags,
                INT8U     opt,
                INT8U    *perr)
{
  return OSFlagPost(s_FlagCan, flags, opt, perr);
}

/*******************************************************************************
             CAN������ǲ�ѯ, ���嶨�嶨���OSFlagQuery
*******************************************************************************/
OS_FLAGS     CAN_FlagQuery    (INT8U *perr)
{
  return OSFlagQuery(s_FlagCan, perr);
}
