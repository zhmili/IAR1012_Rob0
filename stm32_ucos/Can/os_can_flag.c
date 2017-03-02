/*
*********************************************************************************************************
* Filename    : os_can_buf.c
* Version     : V1.00
* Programmer(s) : shijiangxiao    2008-02-20
                  shijiangxiao    2008-04-01  修改程序, 加强模块独立性
*********************************************************************************************************
*/

#include <includes.h>
#include "os_can_flag.h"

static OS_FLAG_GRP *s_FlagCan;

/*******************************************************************************
             CAN操作标记初始化
*******************************************************************************/
BOOLEAN    CAN_FlagInit(void)
{
  INT8U err;
  s_FlagCan = OSFlagCreate(CAN_FLAG_TME0 | CAN_FLAG_TME1 | CAN_FLAG_TME2, &err);
  return (s_FlagCan != NULL) ? TRUE : FALSE;
}


/*******************************************************************************
             CAN操作标记等待, 具体定义定义见OSFlagPend
*******************************************************************************/
OS_FLAGS     CAN_FlagPend     (OS_FLAGS       flags,
                INT8U     wait_type,
                INT16U    timeout,
                INT8U    *perr)
{
  return OSFlagPend(s_FlagCan, flags, wait_type, timeout, perr);
}

/*******************************************************************************
             CAN操作标记设置, 具体定义定义见OSFlagPost
*******************************************************************************/
OS_FLAGS     CAN_FlagPost     (OS_FLAGS       flags,
                INT8U     opt,
                INT8U    *perr)
{
  return OSFlagPost(s_FlagCan, flags, opt, perr);
}

/*******************************************************************************
             CAN操作标记查询, 具体定义定义见OSFlagQuery
*******************************************************************************/
OS_FLAGS     CAN_FlagQuery    (INT8U *perr)
{
  return OSFlagQuery(s_FlagCan, perr);
}
