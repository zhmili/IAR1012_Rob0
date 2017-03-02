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
#include    <rob0_1_def.h>
#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <stdarg.h>

#include    <app_cfg.h>
#include    <stm32f10x_conf.h>


#include    <LCD_PortConfig.h>
#include    <LCD_Config.h>

#include    <LCD_Driver_User.h>
#include    <LCD_Dis.h>

#include    <stm32f10x_lib.h>
#include    <stm32f10x_it.h>

#include    <ucos_ii.h>
#include    <ucosii_buf.h>

#include    <pstwo.h>
#include    <step_28byj-48.h>
#include    <bsp.h>
//#include    <lcd.h>
#include    <Robot.h>
#include    <math.h>



//#include    <i2c_comm_isr.h>
//#include   <i2c_comm_dma.h>

//#include    <MB_CFG.H>
//#include    <mb_os.h>
//#include    <MB_DEF.H>
//#include    <mb.h>

//#include    <uv_oper.h>

//#include    <ad9850.h>
#include    <flash_loc.h>

#include  <modbus_config.h>
#include  <mbserial.h>
#include  <modbus_utils.h>
#include  <bsp_rob0.h>
#include  <rob0.h>

#include    <bsp_i2c_ee.h>
#include    <bsp_i2c_gpio.h>
#endif
