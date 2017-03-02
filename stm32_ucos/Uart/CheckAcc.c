/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : stm32_uart.c
* Version       : V1.00
* Programmer(s) : shijiangxiao	 2008-02-20
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include "CheckAcc.h"

/******************************************************************************
** 函数名称: CheckAccLRC
** 输　入:   pdata 数据地址
             size  数据长度
** 输　出:   无
** 返回值:   和校验
** 全局变量:
** 调用模块:
*******************************************************************************/
INT8U CheckAccLRC(INT8U *pdata, INT16U size)
{
  INT32U LRC = 0;
  while (size-- > 0)
  {
    LRC += *pdata++;
  }
  return LRC;
}

