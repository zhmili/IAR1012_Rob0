/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
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
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : Brian Nagel
*********************************************************************************************************
*/
//#include"includes.h"

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__




//#define  ROB_II    0


/*
*********************************************************************************************************
*                                       ADDITIONAL uC/MODULE ENABLES
*********************************************************************************************************
*/

#define  uC_PROBE_OS_PLUGIN              DEF_DISABLED            /* DEF_ENABLED = Present, DEF_DISABLED = Not Present        */
#define  uC_PROBE_COM_MODULE             DEF_DISABLED

/*
*********************************************************************************************************
*                                            TASK ID
*********************************************************************************************************
*/
#define  ROB_TASK1_ID                         1
#define  ROB_TASK2_ID                         2
#define  ROB_TASK3_ID                         3
#define  ROB_TASK4_ID                         4
#define  ROB_TASK5_ID                         5

#define  ROB_TASK6_ID                         6
/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_TASK_START_PRIO                   3
//#define  APP_TASK_KBD_PRIO                     4
//#define  APP_TASK_USER_IF_PRIO                 5
//#define  APP_TASK_PROBE_STR_PRIO               6

//#define  APP_TASK_CAN_ERROR_PRIO               7
//#define  APP_TASK_CAN_TRANS_PRIO               8
//#define  APP_TASK_CAN_EXAMPLE_PRIO             2

#define  OS_TASK_TMR_PRIO              (OS_LOWEST_PRIO - 2)


#define ROB_TASK1              4
#define ROB_TASK2              5
#define ROB_TASK3              6
#define ROB_TASK4              7
#define ROB_TASK5              8

#define ROB_TASK6              2        //task6 优先级高于其他优先级 用于串口状态扫描

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/
#define  APP_TASK_START_STK_SIZE             1024		  
#define  APP_TASK_CAN_EXAMPLE_STK_SIZE       257
#define  ROB_ALL_TASK_SIZE                   256

/*
*********************************************************************************************************
*                               uC/Probe plug-in for uC/OS-II CONFIGURATION
*********************************************************************************************************
*/

#define  OS_PROBE_TASK                         0                /* Task will be created for uC/Probe OS Plug-In             */
#define  OS_PROBE_TMR_32_BITS                  0                /* uC/Probe OS Plug-In timer is a 16-bit timer              */
#define  OS_PROBE_TIMER_SEL                    2
#define  OS_PROBE_HOOKS_EN                     1


/*
*********************************************************************************************************
*                              CanDriver CONFIGURATION
*********************************************************************************************************
*/
#define  Fcanclk               36000000
#define  CO_MEM_DYNAMIC_ALLOC  0
#define  CO_AUTO_BPS_SUPPORT   1
#define  CO_MAX_FILTER_NUMS    20


/*
*********************************************************************************************************
*                                      运用环境设置 系统功能块配置
* ROBOT - ROBOT 环境
* VUSOUND - VUSOUND环境
* VUSOUNDAIT - VUSOUNDAIR
*********************************************************************************************************
*/
//#define VUSOUND
#define ROBOT


/*
*********************************************************************************************************
*                              串口缓冲配置
*********************************************************************************************************
*/

#define UART_DMA_BUF_SIZE     32
#define UART_DMA_ITEMS        8
#define MAX_TX_FRAME_NUMS     16
#define UART_DMA_TX_BUF_SIZE  32

#define DMA_USE_UART1
//#define DMA_USE_UART2
//#define DMA_USE_UART3
//void AppTaskStart(void *p_arg);
void mainX(void *p_arg);


void ROB_Task1(void *pdata);
void ROB_Task2(void *pdata);
void ROB_Task3(void *pdata);
void ROB_Task4(void *pdata);
void ROB_Task5(void *pdata);
void process_1(void *pdata);
void process_2(void *pdata);
void process_3(void *pdata);
void process_4(void *pdata);
void process_5(void *pdata);
void process_6(void *pdata);
#endif
