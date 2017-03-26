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
#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__



//Éý¼¶×ª»»
#define DMAChannel1_IRQChannel DMA1_Channel1_IRQChannel
#define DMAChannel2_IRQChannel DMA1_Channel2_IRQChannel
#define DMAChannel3_IRQChannel DMA1_Channel3_IRQChannel
#define DMAChannel4_IRQChannel DMA1_Channel4_IRQChannel
#define DMAChannel5_IRQChannel DMA1_Channel5_IRQChannel
#define DMAChannel6_IRQChannel DMA1_Channel6_IRQChannel
#define DMAChannel7_IRQChannel DMA1_Channel7_IRQChannel

#define _DMA_Channel1 _DMA1_Channel1
#define _DMA_Channel2 _DMA1_Channel2
#define _DMA_Channel3 _DMA1_Channel3
#define _DMA_Channel4 _DMA1_Channel4
#define _DMA_Channel5 _DMA1_Channel5
#define _DMA_Channel6 _DMA1_Channel6
#define _DMA_Channel7 _DMA1_Channel7



#define ADCCHS 9           //ADC channels 
#define TMS  8          //use for the ADC DMA trans  times of all ADC channels
#define HLFTMS      TMS/2
#define MEMS        ADCCHS*TMS
#define HLFMEMS     MEMS/2


#define SADCCHS      12    //At the signal adc1 conditions how many channels 
//#define DMAREP       4     //DMA repeat times
#define DMABUFSIZE  SADCCHS*DMAREP

#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <stdarg.h>
#include    <math.h>
#include "stm32f10x_type.h"
#include "cortexm3_macro.h"
#include "stm32f10x_nvic.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_systick.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"



#include    <LCD_PortConfig.h>
#include    <LCD_Config.h>

#include    <LCD_Driver_User.h>
#include    <LCD_Dis.h>


#include    <bsp.h>
#include    <RobBsp.h>


#include "common.h"
#include "BspTime2.h"
#include "BspTime3.h"
#include "BspUsart1.h"
#include "Download.h"
#include "YModem.h"







#endif
