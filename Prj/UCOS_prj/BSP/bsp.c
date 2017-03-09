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
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#define  BSP_GLOBALS
#include <includes.h>
#include <ucos_ii.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

OS_STK          AppTaskStartStk[APP_TASK_START_STK_SIZE];
OS_STK          ROBTask1Stk[ROB_ALL_TASK_SIZE];
OS_STK          ROBTask2Stk[ROB_ALL_TASK_SIZE];
OS_STK          ROBTask3Stk[ROB_ALL_TASK_SIZE];
OS_STK          ROBTask4Stk[ROB_ALL_TASK_SIZE];
OS_STK          ROBTask5Stk[ROB_ALL_TASK_SIZE];
OS_STK          ROBTask6Stk[ROB_ALL_TASK_SIZE];

extern TGPIO_STATE GPIO_STATE[16];

TRecvBuf RecvBuf[MAX_MODBUS_PORT_NUM];

unsigned char fun_name[16][80];  // 每一组编号 对应的名字

char fun_num;

char pstwo_use_flag = 0;

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  Tmr_TickInit  (void);


/*
*********************************************************************************************************
*                                         BSP INITIALIZATION FOR ROBOT
*
* 
*               functions found in this module.
*
* Arguments   : none 粗延时函数，微秒
*********************************************************************************************************
*/
void DelayNus(u16 us)
{
   u16 i=0;  
   while(us--)
   {
      i=15;  //
      while(i--) ;    
   }
}



void DelayNms(u16 ms)
{
   u16 i=0;  
   while(ms--)
   {
      i = 12000;  //
      while(i--) ;    
   }

}

#if 0

/*
*********************************************************************************************************
*                                         BSP INITIALIZATION FOR ROBOT
*
* Description : This function should be called by your application code before you make use of any of the
*               functions found in this module.
*
* Arguments   : none
*********************************************************************************************************
*/
void  Tick_Init(void)
{
    Tmr_TickInit();                                             /* Initialize the uC/OS-II tick interrupt                   */
}


/*
*********************************************************************************************************
*                                     DISABLE ALL INTERRUPTS
*
* Description : This function disables all interrupts from the interrupt controller.
*
* Arguments   : None.
*
* Returns     : None.
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{
    CPU_IntDis();
}


/*
*********************************************************************************************************
*                                      GET THE CPU CLOCK FREQUENCY
*
* Description: This function reads CPU registers to determine the CPU clock frequency of the chip in KHz.
*
* Argument(s): None.
*
* Returns    : The CPU clock frequency, in Hz.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    static  RCC_ClocksTypeDef  rcc_clocks;

    RCC_GetClocksFreq(&rcc_clocks);

    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}

#endif


/*
*********************************************************************************************************
*                              INITIALIZE TIMER FOR uC/Probe Plug-In for uC/OS-II
*
* Description : This function is called to by uC/Probe Plug-In for uC/OS-II to initialize the
*               free running timer that is used to make time measurements.
*
* Arguments   : none
*
* Returns     : none
*
* Note(s)     : This function is EMPTY because the timer is initialized elsewhere.
*********************************************************************************************************
*/

#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN == 1)
void  OSProbe_TmrInit (void)
{
    TIM_TimeBaseInitTypeDef  tim_init;


#if (OS_PROBE_TIMER_SEL == 2)

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    tim_init.TIM_Period        = 0xFFFF;
    tim_init.TIM_Prescaler     = 0x00;
    tim_init.TIM_ClockDivision = 0x0;
    tim_init.TIM_CounterMode   = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &tim_init);
    TIM_SetCounter(TIM2, 0);
    TIM_PrescalerConfig(TIM2, 256, TIM_PSCReloadMode_Immediate);
    TIM_Cmd(TIM2, ENABLE);

#elif (OS_PROBE_TIMER_SEL == 3)

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    tim_init.TIM_Period        = 0xFFFF;
    tim_init.TIM_Prescaler     = 0x00;
    tim_init.TIM_ClockDivision = 0x0;
    tim_init.TIM_CounterMode   = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &tim_init);
    TIM_SetCounter(TIM3, 0);
    TIM_PrescalerConfig(TIM3, 256, TIM_PSCReloadMode_Immediate);
    TIM_Cmd(TIM3, ENABLE);

#elif (OS_PROBE_TIMER_SEL == 4)

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    tim_init.TIM_Period        = 0xFFFF;
    tim_init.TIM_Prescaler     = 0x00;
    tim_init.TIM_ClockDivision = 0x0;
    tim_init.TIM_CounterMode   = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM4, &tim_init);
    TIM_SetCounter(TIM4, 0);
    TIM_PrescalerConfig(TIM4, 256, TIM_PSCReloadMode_Immediate);
    TIM_Cmd(TIM4, ENABLE);

#endif
}
#endif


/*
*********************************************************************************************************
*                              READ TIMER FOR uC/Probe Plug-In for uC/OS-II
*
* Description : This function is called to read the current counts of a 16 bit free running timer.
*
* Arguments   : none
*
* Returns     : The 16 or 32 bit count of the timer assuming the timer is an UP counter.
*********************************************************************************************************
*/

#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN == 1)
CPU_INT32U  OSProbe_TmrRd (void)
{
#if (OS_PROBE_TIMER_SEL == 2)

    return ((CPU_INT32U)TIM_GetCounter(TIM2));

#elif (OS_PROBE_TIMER_SEL == 3)

    return ((CPU_INT32U)TIM_GetCounter(TIM3));

#elif (OS_PROBE_TIMER_SEL == 4)

    return ((CPU_INT32U)TIM_GetCounter(TIM4));

#endif
}
#endif


/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                         uC/OS-II Timer Functions
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       TICKER INITIALIZATION
*
* Description : This function is called to initialize uC/OS-II's tick source (typically a timer generating
*               interrupts every 1 to 100 mS).
*
* Arguments   : none
*
* Note(s)     : 1) The timer is setup for output compare mode BUT 'MUST' also 'freerun' so that the timer
*                  count goes from 0x00000000 to 0xFFFFFFFF to ALSO be able to read the free running count.
*                  The reason this is needed is because we use the free-running count in uC/OS-View.
*********************************************************************************************************
*/
static INT32U s_dwMaxCnts;
static  void  Tmr_TickInit (void)     // 1ms 产生一次时钟片
{

    RCC_ClocksTypeDef  rcc_clocks;
    CPU_INT32U         cnts;

    RCC_GetClocksFreq(&rcc_clocks);

    cnts = (CPU_INT32U)rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC;
    s_dwMaxCnts = cnts;
    SysTick_SetReload(cnts);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick_CounterCmd(SysTick_Counter_Enable);
    SysTick_ITConfig(ENABLE);
}


/****************************************************************************
* 名称：SysGet100UsTick()
* 功能：得到系统时钟值，以0.1Ms为单位
* 入口参数：无
* 出口参数：无
****************************************************************************/
#if 0
INT32U  SysGet100UsTick(void)
{

#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
  OS_CPU_SR  cpu_sr = 0;
#endif
  INT32U dwOldCTC;
  INT32U dwOSTime;
  INT32U dwTick100UsMain;
  INT32U dwTick100UsSub;
  
  OS_ENTER_CRITICAL();
  dwOldCTC = SysTick_GetCounter();
  dwOSTime = OSTime;
  if (SysTick_GetFlagStatus(SysTick_FLAG_COUNT))
  {
    dwOSTime += 1;
    dwOldCTC =  0;
  }
  OS_EXIT_CRITICAL();

  dwOldCTC        = s_dwMaxCnts - dwOldCTC;
  dwTick100UsMain =  dwOSTime * (10000 / OS_TICKS_PER_SEC);
  dwTick100UsSub  = (dwOldCTC * (10000 / OS_TICKS_PER_SEC)) / (s_dwMaxCnts);

  return dwTick100UsMain + dwTick100UsSub;

}


/*
*********************************************************************************************************
*                                     BSP_OS_SemWait()
*
* Description : Wait on a semaphore to become available
*
* Argument(s) : sem          sempahore handler
*  
*               dly_ms       delay in miliseconds to wait on the semaphore
*
* Return(s)   : DEF_OK       if the semaphore was acquire
*               DEF_FAIL     if the sempahore could not be acquire
*     
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemWait (BSP_OS_SEM  sem, CPU_INT32U  dly_ms)
{
    CPU_INT08U  err;
    CPU_INT32U  dly_ticks;
    
    dly_ticks  = (dly_ms * OS_TICKS_PER_SEC / 1000);
    
    OSSemPend((OS_EVENT   *)sem,
              (CPU_INT32U  )dly_ticks,
              (CPU_INT08U *)&err);

    if (err != OS_ERR_NONE) {
       return (DEF_FAIL);
    }

    return (DEF_OK);
}

/*
*********************************************************************************************************
*                                      BSP_OS_SemPost()
*
* Description : Post a semaphore
*
* Argument(s) : sem          Semaphore handler
*  
* Return(s)   : DEF_OK     if the semaphore was posted.
*               DEF_FAIL      if the sempahore could not be posted.
*     
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemPost (BSP_OS_SEM  sem)
{
    CPU_INT08U  err;
        
    err = OSSemPost((OS_EVENT *)sem);
    
    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}

#endif

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    
      /* Wait till HSE is ready */
    RCC_WaitForHSEStartUp();
 
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    
    FLASH_SetLatency(FLASH_Latency_2);
    
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    
    RCC_PCLK2Config(RCC_HCLK_Div1);

    RCC_PCLK1Config(RCC_HCLK_Div4);        
    
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
#ifdef ROBOT
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);//配置ADC时钟= PCLK2/4
#endif  
    RCC_PLLCmd(ENABLE);

    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
        ;
    }
    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    while (RCC_GetSYSCLKSource() != 0x08) {
        ;
    }
  
  PWR_BackupAccessCmd(ENABLE);//允许修改RTC 和后备寄存器
  RCC_LSICmd(DISABLE);//关闭外部低速外部时钟信号功能 后，PC14 PC15 才可以当普通IO用
  BKP_TamperPinCmd(DISABLE);//关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
  
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM4|RCC_APB1Periph_SPI2|RCC_APB1Periph_SPI3|RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_UART4|RCC_APB1Periph_UART5|RCC_APB1Periph_I2C1|RCC_APB1Periph_I2C2|RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG|RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2|RCC_APB2Periph_TIM1|RCC_APB2Periph_TIM8|RCC_APB2Periph_USART1|RCC_APB2Periph_ADC3, ENABLE);
}



/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
// REMAP
  GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
  GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE); 
  GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); 
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
// PA
  GPIO_InitStructure.GPIO_Pin       =  GPIOA_ADC_IN0 | GPIOA_ADC_IN1 | GPIOA_ADC_IN2 | GPIOA_ADC_IN3 | GPIOA_ADC_IN4| GPIOA_ADC_IN5 | GPIOA_ADC_IN6| GPIOA_ADC_IN7;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin       =  GPIOA_485_EN1 |  GPIOA_485_EN2 ;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
  GPIO_InitStructure.GPIO_Pin 		= GPIOA_UART1_TX ; 
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin 		= GPIOA_UART1_RX; 
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
#if 0
  GPIO_InitStructure.GPIO_Pin           =  GPIOB_ADC_IN8 | GPIOB_ADC_IN9;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  
  GPIO_InitStructure.GPIO_Pin		=  GPIOB_SPI3_CLK|GPIOB_SPI3_MOSI|GPIOB_CD4510_S0|GPIOB_DB6;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode		=  GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin		=  GPIOB_I2C1_SCL|GPIOB_I2C2_SCL|GPIOB_I2C1_SDA|GPIOB_I2C2_SDA;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin		=  GPIOB_SPI3_MISO;
  GPIO_InitStructure.GPIO_Mode          =  GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  
  
  GPIO_InitStructure.GPIO_Pin		=  GPIOB_SPI2_SCK_IN1|GPIOB_SPI2_MISO_IN2|GPIOB_SPI2_MOSI_IN3|GPIOB_KEY4;
  GPIO_InitStructure.GPIO_Mode          =  GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  
  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

#else
  GPIO_InitStructure.GPIO_Pin		=  GPIOB_I2C2_SCL|GPIOB_I2C2_SDA;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin		=  LCD_CS_PIN|LCD_RE_PIN|LCD_CK_PIN|LCD_MO_PIN|LCD_A_PIN;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode		=  GPIO_Mode_Out_PP;
  GPIO_Init(LCD_CS_PORT, &GPIO_InitStructure);
#endif 
  
// PC
  GPIO_InitStructure.GPIO_Pin       =  GPIOC_ADC_IN10 | GPIOC_ADC_IN11| GPIOC_ADC_IN12 | GPIOC_ADC_IN13| GPIOC_ADC_IN14 | GPIOC_ADC_IN15;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
  
  GPIO_InitStructure.GPIO_Pin       =  GPIOC_TIM8_CH1_DS1 |GPIOC_TIM8_CH2_DS2|GPIOC_UART4_TX|GPIOC_UART5_TX;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin       =  GPIOC_TIM8_CH3_BEEP ;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
  
  
 
  GPIO_InitStructure.GPIO_Pin       = GPIOC_TIM8_CH4_IN4|GPIOC_PKT;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  
  /*
  // 实验用usb 插入检测
  GPIO_InitStructure.GPIO_Pin       =  GPIOC_TIM8_CH3;
  GPIO_InitStructure.GPIO_Speed         =  GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_IPD;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  */
  
  GPIO_InitStructure.GPIO_Pin       =  GPIOC_SPI3_SS|GPIOC_CMOTOR6_1;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  
  GPIO_InitStructure.GPIO_Pin =  GPIOC_UART4_RX;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
// PD
  GPIO_InitStructure.GPIO_Pin       =  GPIOD_UART5_RX|GPIOD_UART3_RX;
  GPIO_InitStructure.GPIO_Speed     =  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 	    =  GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin       =  GPIOD_E6|GPIOD_E5|GPIOD_E4|GPIOD_CD4510_S1|GPIOD_CD4510_S2|GPIOD_W_KEY|GPIOD_T1_LCD_LED_EN|GPIOD_UART2_TX_E3;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin       =  GPIOD_TIM4_CH1 | GPIOD_TIM4_CH2_M4| GPIOD_TIM4_CH3_M5 |GPIOD_TIM4_CH4_M6|GPIOD_UART3_TX;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin       =  GPIOD_UART2_RX_TP_IRQ;
  GPIO_InitStructure.GPIO_Speed     =  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 	    =  GPIO_Mode_IPD;
  GPIO_Init(GPIOD, &GPIO_InitStructure);  
  
  
//PE 
  GPIO_InitStructure.GPIO_Pin       =  GPIOE_DB7|GPIOE_T2|GPIOE_USB_EN | GPIOE_CMOTOR3_2| GPIOE_CMOTOR2_2 |GPIOE_CMOTOR1_2 |GPIOE_RF_RESET|GPIOE_E1|GPIOE_LCD_RS|GPIOE_LCD_DB4;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin       =  GPIOE_TIM1_CH1_DS6|GPIOE_TIM1_CH3_DS4|GPIOE_TIM1_CH4_DS3|GPIOE_TIM1_CH2N|GPIOE_TIM1_CH2_DS5;
  GPIO_InitStructure.GPIO_Speed 	=   GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		=   GPIO_Mode_AF_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin       =  GPIOE_TIM1_CH1N;
  GPIO_InitStructure.GPIO_Speed 	=   GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		=   GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  

//PF
  GPIO_InitStructure.GPIO_Pin       =  GPIOF_FAN_2|GPIOF_COMTRO6_2 | GPIOF_COMTRO5_2| GPIOF_COMTRO4_2 |GPIOF_245EN_2 |GPIOF_COMTRO5_1|GPIOF_COMTRO4_1|GPIOF_LCD_DB0|GPIOF_LCD_DB1|GPIOF_DC_RW|GPIOF_LCD_EN;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_Out_PP;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin       =  GPIOF_ADC_IN16 | GPIOF_ADC_IN17| GPIOF_ADC_IN18 | GPIOF_ADC_IN19| GPIOF_ADC_IN20;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_AIN;
  GPIO_Init(GPIOF, &GPIO_InitStructure); 
  
//PG
  GPIO_InitStructure.GPIO_Pin       =  GPIOG_FAN_1|GPIOG_LCD_DB2 | GPIOG_LCD_DB3| GPIOG_LCD_DB5|GPIOG_485_EN3|GPIOG_245EN_1|GPIOG_E2;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_Out_PP;
  GPIO_Init(GPIOG, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin       =  GPIOG_INT1|GPIOG_KEY_1|GPIOG_KEY_2|GPIOG_KEY_3|GPIOG_REM|GPIOG_START_STOP;
  GPIO_InitStructure.GPIO_Speed     =  GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode 	    =  GPIO_Mode_IPU;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIOG_LED;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_10MHz;
  GPIO_Init(GPIOG, &GPIO_InitStructure); 
  
//增加的usb 检测功能 
  GPIO_InitStructure.GPIO_Mode = GPIOG_USB_TEST;
  GPIO_InitStructure.GPIO_Speed     =  GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode 	    =  GPIO_Mode_IPU;
  GPIO_Init(GPIOG, &GPIO_InitStructure); 
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{ 
  /* Set the Vector Table base location at 0x08008000 */ 
  NVIC_InitTypeDef NVIC_InitStructure;
//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0xFFFF);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
#ifdef 0
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  
  
  NVIC_InitStructure.NVIC_IRQChannel = UART3_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  
  
  NVIC_InitStructure.NVIC_IRQChannel =  EXTI15_10_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
#endif
 
#ifdef 0
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel =  EXTI15_10_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
#endif
  
  NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 0, 9);
}


/*******************************************************************************
* Function Name  : TIMEx_Base_Configruation
* Description    : Configures the pw
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//TIM1 counter clock = TIMxCLK / (Prescaler +1) 
//                   = 72M/144 = 500K
//TIMx ARR Register = 10000 => TIMx Frequency = TIMxcounter clock/(9999 + 1)= 500K/10000 = 50HZ 
//
void TIMEx_Base_Configruation(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  TIM_TimeBaseStructure.TIM_Period = 9999;  
  TIM_TimeBaseStructure.TIM_Prescaler = 143;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);               //50HZ
  
  TIM_TimeBaseStructure.TIM_Period = 1999;		   
  TIM_TimeBaseStructure.TIM_Prescaler = 0;		 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);               //18khz
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//exp ----
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
 // TIM_OCInitStructure.TIM_Pulse = eff;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OC1Init(TIM1,&TIM_OCInitStructure); 
  TIM_OC2Init(TIM1,&TIM_OCInitStructure);
  TIM_OC3Init(TIM1,&TIM_OCInitStructure);
  TIM_OC4Init(TIM1,&TIM_OCInitStructure);
  TIM_OC1Init(TIM8,&TIM_OCInitStructure); 
  TIM_OC2Init(TIM8,&TIM_OCInitStructure);
  
  TIM_OC1Init(TIM4,&TIM_OCInitStructure); 
  TIM_OC2Init(TIM4,&TIM_OCInitStructure);
  TIM_OC3Init(TIM4,&TIM_OCInitStructure);
  TIM_OC4Init(TIM4,&TIM_OCInitStructure);
  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  
  TIM_Cmd(TIM4, ENABLE);
  TIM_CtrlPWMOutputs(TIM4, ENABLE);
  
  TIM_Cmd(TIM8, ENABLE);
  TIM_CtrlPWMOutputs(TIM8, ENABLE);
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input           : TIM: 3,4
			: TIM_Channel_x : 2,3,(4)
			:
* Output         : None
* Return         : None
 -----------------------------------------------------------------------
	  TIMx Configuration: generate 4 PWM signals with 4 different duty cycles:
	  TIMxCLK = 36 MHz, Prescaler = 0x0, TIMx counter clock = 36 MHz
	  TIMx ARR Register = 999 => TIMx Frequency = TIMxcounter clock/(ARR + 1) 
	  TIMx Frequency = 36 KHz.

	  u16 CCR1_Val = 500;
	  u16 CCR2_Val = 375;
         u16 CCR3_Val = 250;
         u16 CCR4_Val = 125;
	  TIMx Channel1 duty cycle = (TIMx_CCR1/ TIMx_ARR)* 100 = 50% 
	  TIMx Channel2 duty cycle = (TIMx_CCR2/ TIMx_ARR)* 100 = 37.5% 
	  TIMx Channel3 duty cycle = (TIMx_CCR3/ TIMx_ARR)* 100 = 25%
	  TIMx Channel4 duty cycle = (TIMx_CCR4/ TIMx_ARR)* 100 = 12.5% 
	----------------------------------------------------------------------- 
说明：从999 减少至 800 后维持在8V 左右
*******************************************************************************/


/******************************************************************************************
* 函数名称	: 
* 函数功能	: 
*				  
* 输入参数	: TIMx:         TIM1      TIM4      TIM8 
*		  TIM_Channel_x: 1/2/3/4  1/2/3/4   1/2
                  eff:        0-10000          0-2000
* 返回数值	: 无
*		 
*补充说明	: judg 用于判断是time1 还是 time2-time4，当是time1时 ，TIMx 该值无效
*                 judg 0 对应的时TIM1
*修正说明       : 版本V2.0.3 TIM1与TIMx融和 去除judg 20120324
*******************************************************************************************/

void SetPWM(TIM_TypeDef* TIMx, u16 TIM_Channel_x, s16 eff)
{
   switch(TIM_Channel_x)
    {
  case TIM_Channel_1:
  //TIM_OC1Init(TIMx,&TIM_OCInitStructure); 
   TIMx->CCR1 = eff;
  break;
  
  case TIM_Channel_2:
  //TIM_OC2Init(TIMx,&TIM_OCInitStructure);
    TIMx->CCR2 = eff;
  break;

  case TIM_Channel_3:
  //TIM_OC3Init(TIMx,&TIM_OCInitStructure);
    TIMx->CCR3 = eff;
  break;
  
  case TIM_Channel_4:
  //TIM_OC4Init(TIMx,&TIM_OCInitStructure);
    TIMx->CCR4 = eff;
  break;
  
  default:
    break;
    }
}

/******************************************************************************************
* 函数名称	: ADC_Configruation(void)
* 函数功能	: 
*				  
* 输入参数	: 
*		  
                  
* 返回数值	: 无
*		 
*修正说明       : 版本V2.0.3 TIM1与TIMx融和 去除judg 20120324
*******************************************************************************************/
void ADC_Configruation(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_Init(ADC2, &ADC_InitStructure);
  ADC_Init(ADC3, &ADC_InitStructure);
}

/******************************************************************************************
* 函数名称	: u16 ADC_GetConvertValueAVE(ADC_TypeDef*  ADCx ,u8 adc_channel)
* 函数功能	: 
*				  
* 输入参数	: 
*		             
* 返回数值	: 无
*		 
*修正说明       : 调用ADC_GetConvertValue 函数
*******************************************************************************************/
u16 ADC_GetConvertValueAVE(ADC_TypeDef*  ADCx ,u8 adc_channel)
{
 volatile u16 adc;
 u16 value_buf[VCFILTERTMS];
 u8 i;
 for(i = 0; i<VCFILTERTMS;i++)
 value_buf[i] = ADC_GetConvertValue(ADCx ,adc_channel);

 adc = midavrfilter( value_buf, VCFILTERTMS);
  return adc;
}

/******************************************************************************************
* 函数名称	: u16 ADC_GetConvertValue(ADC_TypeDef*  ADCx ,u8 adc_channel)
* 函数功能	: 获取某个通道的值
*				  
* 输入参数	: ADCx：      ADC1 ADC2 ADC3
*		  adc_channel 对于的通道号，注意 ADC3通道号5个
                  
* 返回数值	: 获取的ADC 值 0-4069
*		 
*修正说明       :
*******************************************************************************************/
u16 ADC_GetConvertValue(ADC_TypeDef*  ADCx ,u8 adc_channel)
{
  volatile u16 data;
  ADC_InitTypeDef ADC_InitStructure;
  
  ADC_DeInit(ADCx);
  //  channel = SetAddr(ADC_GROUP);
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADCx, &ADC_InitStructure);
  
   /* ADC1 regular channel10 configuration */
  ADC_RegularChannelConfig(ADCx, adc_channel, 1, ADC_SampleTime_13Cycles5);
  
    /* Enable ADC1 */
  ADC_Cmd(ADCx, ENABLE);
  
  /* Enable ADC1 reset calibaration register */
  ADC_ResetCalibration(ADCx);
  
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADCx));
  
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADCx);
  
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADCx));
   
  ADC_ClearFlag(ADCx, ADC_FLAG_EOC);
	
  /* Start ADC1 Software Conversion */
  ADC_SoftwareStartConvCmd(ADCx, ENABLE);
  while(ADC_GetSoftwareStartConvStatus(ADCx));
  
  while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC));
  
  data = ADC_GetConversionValue(ADCx);
  return data;
}

/*******************************************************************************
* Function Name  : void UXART_Init(USART_TypeDef* UXARTx,u32 USART_BaudRate,u16 USART_WordLength,u16 USART_StopBits,u16 USART_Parity,u8 IT_RX_EN,u8 IT_TX_EN)
* Description    : Word Length = 8 Bits 1 Stop Bit No parity BaudRate = 9600 baud 。。。
*                  IT_RX_EN ：1 - 使能RX 中断，IT_TX_EN ：使能TX中断
* Input          : UXARTx - USART1 USART2 USART3 UART4 UART5 USART3 
* Return         : None
* Comment        :         
*******************************************************************************/
void UXART_Init(USART_TypeDef* UXARTx,u32 USART_BaudRate,u16 USART_WordLength,u16 USART_StopBits,u16 USART_Parity,u8 IT_RX_EN,u8 IT_TX_EN)
{
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = USART_BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength;
  USART_InitStructure.USART_StopBits = USART_StopBits;
  USART_InitStructure.USART_Parity = USART_Parity;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UXARTx, &USART_InitStructure);
  
  if(IT_RX_EN)
  {
   USART_ITConfig(UXARTx, USART_IT_RXNE, ENABLE);
  }else{
  USART_ITConfig(UXARTx, USART_IT_RXNE, DISABLE);
  }
  
  if(IT_TX_EN)
  {
   USART_ITConfig(UXARTx, USART_IT_TXE, ENABLE);
  }else{
   USART_ITConfig(UXARTx, USART_IT_TXE, DISABLE);
  }
  
  USART_ClearFlag(UXARTx, USART_FLAG_TC);//
  USART_ClearFlag(UXARTx, USART_FLAG_LBD);//
  USART_ClearFlag(UXARTx,USART_FLAG_CTS);
  USART_ClearFlag(UXARTx,USART_FLAG_RXNE);
  
  USART_Cmd(UXARTx, ENABLE);
}

/******************************************************************************************
* 函数名称	: u16 RS485_Receive(USART_TypeDef* USARTx)
* 函数功能	: 
*				  
* 输入参数	:USARTX:USART1,UART4，UART5
*		          
* 返回数值	: 接受的数据
*		 
*修正说明       :
*******************************************************************************************/
u16 RS485_Receive(USART_TypeDef* USARTx)
{
    if(USARTx == USART1)
    {
      GPIO_ResetBits( GPIOA, GPIOA_485_EN1);
    }else if(USARTx == UART4){
      GPIO_ResetBits( GPIOA, GPIOA_485_EN2);
    }else if(USARTx == UART5){
      GPIO_ResetBits( GPIOG, GPIOG_485_EN3);
    }
    while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET)
    {
    }
    return USART_ReceiveData(USARTx);
}


/******************************************************************************************
* 函数名称	: void RS485_Send(USART_TypeDef* USARTx,u16 data)
* 函数功能	: 
*				  
* 输入参数	: USARTX:USART1,UART4，UART5
*		          
* 返回数值	: 发送数据
*		 
*******************************************************************************************/
void RS485_Send(USART_TypeDef* USARTx,u16 data)
{
   int i;
   
   if(USARTx == USART1)
    {
      GPIO_SetBits( GPIOA, GPIOA_485_EN1);
    }else if(USARTx == UART4){
      GPIO_SetBits( GPIOA, GPIOA_485_EN2);
    
    }else if(USARTx == UART5){
      GPIO_SetBits( GPIOG, GPIOG_485_EN3);
    }
   
   USART_SendData(USARTx, data);

   while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
   {
   }
   
   // 发送完毕后马上 切回接受状态
   for(i = 1000; i > 0 ;i--);
   
   if(USARTx == USART1)
    {
      GPIO_ResetBits( GPIOA, GPIOA_485_EN1);
    }else if(USARTx == UART4){
      GPIO_ResetBits( GPIOA, GPIOA_485_EN2);
    
    }else if(USARTx == UART5){
      GPIO_ResetBits( GPIOG, GPIOG_485_EN3);
    }  
}

/******************************************************************************************
* 函数名称	: u16 UART_Receive(USART_TypeDef* USARTx)
* 函数功能	: 
*				  
* 输入参数	: USARTX:USART3
*		          
* 返回数值	: 接收数据
*		 
*******************************************************************************************/
u16 UART_Receive(USART_TypeDef* USARTx)
{
    int i;
    while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET)
    {
      i++;
      OSTimeDlyHMSM(0,0,0,1);
      if(i>1000)
        return -1;
        break;
    }
    return USART_ReceiveData(USARTx);
}


/******************************************************************************************
* 函数名称	: void UART_Send(USART_TypeDef* USARTx,u16 data)
* 函数功能	: 
*				  
* 输入参数	: USARTX:USART3，data : 要发送的数据
*		          
* 返回数值	: 发送数据
*		 
*******************************************************************************************/
void UART_Send(USART_TypeDef* USARTx,u16 data)
{ 
   USART_SendData(USARTx, data);
   
     /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
  {
  }
}




/******************************************************************************************
* 函数名称	: u16 midavrfilter(u16 value_buf[], u8 size)
* 函数功能	: 
*				  
* 输入参数	: 
*		          
* 返回数值	: 中位值平均滤波法
*		 
*******************************************************************************************/
//中位值平均滤波法（又称防脉冲干扰平均滤波法）
//081023修改 为直接平均值法
//将编译格式改变后 出现问题 改for (j=0;j<N-1;j++) 为for(j = 0;j<N; j++) 之后正常
u16 midavrfilter(u16 value_buf[], u8 size)
{
  volatile u16 madc;
  u8 count,N;
  u32 sum;
  N = size;
 
  sum=0; 
  for(count=0;count<N;count++)
  sum += value_buf[count];
  madc =(u16)(sum/N);
  return madc;
}


/******************************************************************************************
* 函数名称	: int  main (void *pbos)
* 函数功能	: 主函数
*				  
* 输入参数	: 
*		          
* 返回数值	: 
*		 
*******************************************************************************************/
int  main (void *pbos)
{  
//  MODBUS_CH      *ph;
//  MODBUS_CH      PH; 
int i;
#ifdef DEBUG
  debug();
#endif
  
  vector_loc();
  
#if ROB_0
  bsp_rob0_init();
  
  UXART_Init(UART4,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,1,0);    
  UXART_Init(UART5,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,1,0);
  release_jtagswd();
  scan_port();
  judge();
  pin_final_cfg();
  
#else
   
   RCC_Configuration();
   NVIC_Configuration();
   GPIO_Configuration();
   
   for(i = 1 ; i < 7; i++)
   {
     clear_digital_out(i);         //为舵机的需要
   }
   
   SHC245EN1(0);
     
   TIMEx_Base_Configruation(); 
     
   ADC_Configruation();
    
   UXART_Init(USART1,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,1,0); 
    
    //UXART_Init(USART1,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,1,0);
   #ifdef ROB_III
    UXART_Init(USART1,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,1,0);
   #endif
   
    //UXART_Init(USART2,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,0,0);
    UXART_Init(USART3,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,1,0);
    
   #ifdef ROB_III 
    UXART_Init(UART4,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,1,0);
   #endif  
    
    UXART_Init(UART5,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,1,0);//460800
    
    
    GPIO_ResetBits( GPIOA, GPIOA_485_EN2);      //485 接收状态
    
    //UXART_Init(UART5,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,1,0);
    //SHC245EN2(1);
    //SHC245EN3(1);
    GPIO_ResetBits(GPIOE, GPIOC_TIM8_CH3_BEEP);
    //GPIO_ResetBits(GPIOG,  GPIOG_LED); 
    GPIO_SetBits(GPIOG, GPIO_Pin_8);
    
    for(i = 1; i< 6; i++)
    {
      servo(i, 500);
    }
    
    //GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);
    //EXTI_InitStructure.EXTI_Line = EXTI_Line8;
    //GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource10);// 修改
    //EXTI_InitStructure.EXTI_Line = EXTI_Line10; // 修改
    //EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    //EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    //EXTI_Init(&EXTI_InitStructure);
    
    MBSerialInit();
    //MBSerialOpen(1, MB_RTU_PROTOCOL);
    MBSerialOpen(2, MB_RTU_PROTOCOL);
    //MBSerialOpen(3, MB_RTU_PROTOCOL);
    //MBSerialOpen(4, MB_RTU_PROTOCOL);
    
   #if 0
      LCD_LED(1);
      LCD_CS(1);
      LCD_RES(1);
      LCD_A0(1);
      LCD_SDI(1);
      LCD_CLK(1);
      
      LCD_LED(0);
      LCD_CS(0);
      LCD_RES(0);
      LCD_A0(0);
      LCD_SDI(0);
      LCD_CLK(0);
    #endif  
    LCD_LED(1);
    //I2C_Comm_Init(I2C1,400000, 0x0);
    SHC245EN1(1);
    //ClrScreen(0);
    
     
  //LCD_Image();
  // 
  //  while(Push_Button_START_Read());// 未按
  //  while(!Push_Button_START_Read());// 松开                           
  /*  MB_Init (10);
  /   MB_CfgCh(1,
                    MODBUS_MASTER,
                    5,
                    MODBUS_MODE_RTU,
                    1,
                    115200,
                    8,
                    MODBUS_PARITY_NONE,
                    1,
                    MODBUS_WR_EN);
    MB_CfgCh(2,
                    MODBUS_MASTER,
                    5,
                    MODBUS_MODE_RTU,
                    2,
                    115200,
                    8,
                    MODBUS_PARITY_NONE,
                    1,
                    MODBUS_WR_EN);
      
    MB_CfgCh(3,
                    MODBUS_MASTER,
                    5,
                    MODBUS_MODE_RTU,
                    3,
                    115200,
                    8,
                    MODBUS_PARITY_NONE,
                    1,
                    MODBUS_WR_EN);
    */
  #endif
    LCD_Init();
    FontSet(0,BLACK);
    CPU_IntDis();  // ucos-ii tick
    Tmr_TickInit();// ucos-ii ticks 
    
    OSInit();
    
    OSTaskCreateExt( start_mainX,                              
                    (void *)0,
                    (OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1],
                    APP_TASK_START_PRIO,
                    APP_TASK_START_PRIO,
                    (OS_STK *)&AppTaskStartStk[0],
                    APP_TASK_START_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
    
  #if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity                                   */
  #endif
    OSStart();   

}

/******************************************************************************************
* 函数名称	: void CreatTask(int i)
* 函数功能	: 建立子进程
*				  
* 输入参数	: 
*		          
* 返回数值	:
*		 
*******************************************************************************************/
void CreatTask(int i)
{
   
  switch(i){
    
  case 1:
      OSTaskCreateExt(process_1,                              
                    (void *)0,
                     (OS_STK *)&ROBTask1Stk[ROB_ALL_TASK_SIZE-1],
                    ROB_TASK1,
                    ROB_TASK1,
                    (OS_STK *)&ROBTask1Stk[0],
                    ROB_ALL_TASK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
    
        break;
        
  case 2:
      OSTaskCreateExt(process_2,                              
                    (void *)0,
                     (OS_STK *)&ROBTask2Stk[ROB_ALL_TASK_SIZE-1],
                    ROB_TASK2,
                    ROB_TASK2,
                    (OS_STK *)&ROBTask2Stk[0],
                    ROB_ALL_TASK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
       break; 
  
  case 3:
      OSTaskCreateExt(process_3,                              
                    (void *)0,
                     (OS_STK *)&ROBTask3Stk[ROB_ALL_TASK_SIZE-1],
                    ROB_TASK3,
                    ROB_TASK3,
                    (OS_STK *)&ROBTask3Stk[0],
                    ROB_ALL_TASK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
      
  break;
  
  case 4:
      OSTaskCreateExt(process_4,                              
                    (void *)0,
                     (OS_STK *)&ROBTask4Stk[ROB_ALL_TASK_SIZE-1],
                    ROB_TASK4,
                    ROB_TASK4,
                    (OS_STK *)&ROBTask4Stk[0],
                    ROB_ALL_TASK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
      
   break;
   
  case 5:
      OSTaskCreateExt(process_5,                              
                    (void *)0,
                     &ROBTask5Stk[ROB_ALL_TASK_SIZE-1],
                    ROB_TASK5,
                    ROB_TASK5,
                    (OS_STK *)&ROBTask5Stk[0],
                    ROB_ALL_TASK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
      
   break;
   
  case 6:
      OSTaskCreateExt(process_1,                              
                    (void *)0,
                     (OS_STK *)&ROBTask1Stk[ROB_ALL_TASK_SIZE-1],
                    ROB_TASK1,
                    ROB_TASK1,
                    (OS_STK *)&ROBTask1Stk[0],
                    ROB_ALL_TASK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
      
        
      OSTaskCreateExt(process_2,                              
                    (void *)0,
                     (OS_STK *)&ROBTask2Stk[ROB_ALL_TASK_SIZE-1],
                    ROB_TASK2,
                    ROB_TASK2,
                    (OS_STK *)&ROBTask2Stk[0],
                    ROB_ALL_TASK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
      
        
      OSTaskCreateExt(process_3,                              
                    (void *)0,
                     (OS_STK *)&ROBTask3Stk[ROB_ALL_TASK_SIZE-1],
                    ROB_TASK3,
                    ROB_TASK3,
                    (OS_STK *)&ROBTask3Stk[0],
                    ROB_ALL_TASK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
      
        
      OSTaskCreateExt(process_4,                              
                    (void *)0,
                     (OS_STK *)&ROBTask4Stk[ROB_ALL_TASK_SIZE-1],
                    ROB_TASK4,
                    ROB_TASK4,
                    (OS_STK *)&ROBTask4Stk[0],
                    ROB_ALL_TASK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
      
        
      OSTaskCreateExt(process_5,                              
                    (void *)0,
                     &ROBTask5Stk[ROB_ALL_TASK_SIZE-1],
                    ROB_TASK5,
                    ROB_TASK5,
                    (OS_STK *)&ROBTask5Stk[0],
                    ROB_ALL_TASK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
break;

  default:
    break;
  }
}

/******************************************************************************************
* 函数名称	: void start_mainX(void *p_arg)
* 函数功能	: // 防止跳进 硬件死循环
*				  
* 输入参数	: 
*		          
* 返回数值	:
*		 
*******************************************************************************************/

void start_mainX(void *p_arg)
{
     
  OSTaskCreateExt(process_6,                              
                    (void *)0,
                     &ROBTask6Stk[ROB_ALL_TASK_SIZE-1],
                    ROB_TASK6,
                    ROB_TASK6,
                    (OS_STK *)&ROBTask6Stk[0],
                    ROB_ALL_TASK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
  mainX(p_arg);
  
  while(1)
  {
  
  OSTimeDlyHMSM(0,0,0,100);
  }
}

/******************************************************************************************
* 函数名称	: void SetParms()
* 函数功能	: 参数设置
*				  
* 输入参数	: 
*		          
* 返回数值	:
*		 
*******************************************************************************************/
#if 0

void SetParms()
{
#if 0 
  ClrScreen(0);
  PutString(0,2,"-SetDataOper-");
  SetLight(0);
  SetLight(1);
  key = 0;
  
  DataNum = 33 ;// 第一个字节空
  
  Address = AppEndAddr;
  for(i=0 ; i< DataNum; i++)
  {
  SetData[i] = (*(vu32*)Address);
  Address = Address + 4;
  }
  
  //数据保存到寄存器区
 
  FLASH_Unlock();
  
  
  while(1)// 当双键都按下时跳出
  {
  if((left_button_read() == 0x00) && (right_button_read()!=0x00))//判断是否有进入状态
  {
  while(1)
  { 
   if( left_button_read() != 0x00)
   {  
    if(left_button_read() ==0x00)
    for(u = 0; u < 29000; u++);
    if(left_button_read() ==0x00)
     { 
       for(u = 0;u < 1500000; u++);  
       key++ ;
       ClrScreen(0);
       if(key >=33|| key < 0)//44
       key = 1 ;
     }  
        PutString(0,0,"SelectState");
        PutString(0,2,"Set AIN:");
        PutIntValue(65,2,key);
        PutString(0,4,"DnyData:");
        PutIntValue(65,4,analog(key));
        
   }else{
      ClrScreen(0);
      SetLight(0);
      msleep(300);
      if( left_button_read() != 0x00)//每次刷新一次 赋值更改一次
      {
          PutString(0,0,"GetDataState");
          PutString(0,2,"ChalAIN:");
          PutIntValue(65,2,key);
          PutString(0,4,"DnyData:");
          PutIntValue(65,4,analog(key));
          SetData[key] = analog(key);
          PutIntValue(65,6,SetData[key]); 
          PutString(0,6,"TheData:");
          SetLight(1);
          
          while((left_button_read() != 0x00 )&& (right_button_read() !=0x00))// 没有按键动作就 刷新模拟通道值
          {
          PutIntValue(65,4,analog(key));  
          }
          
      } else {
        
           msleep(500);
           if( right_button_read() != 0x00)
           break;
        
           FLASHStatus = FLASH_ErasePage(AppEndAddr);
            if(FLASHStatus != FLASH_COMPLETE)
           {
            ClrScreen(0);
            PutString(0,2,"-FLASH_EARSE-ERROR--");
            break;
            }
        
            Address = StartAddr;
            for(i = 0; i< DataNum; i++)
            {
            FLASHStatus = FLASH_ProgramWord(Address, SetData[i]);
            Address = Address + 4;
            }
            SetLight(1);
            break;
      }
   }
  }
ClrScreen(0); 
PutString(0,0,"FinishState");
PutString(0,2,"-SetFinish-");   


}

if(right_button_read()==0 && left_button_read()==0)
  {
FLASH_Lock();
break;
  }
}
#endif
}

#endif



static void DownLoadAct(int port, USART_TypeDef  *USARTx)
{
  int i;
  if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
  {
    RecvBuf[port].buf[RecvBuf[port].act_lenth]= USART_ReceiveData(USARTx);//RS485_Receive(USART_TypeDef* USARTx)
    //USART_SendData(USARTx, RecvBuf[port].buf[RecvBuf[port].act_lenth]);
    if(RecvBuf[port].act_lenth == 0)
    {
      if(RecvBuf[port].buf[0] == '5')                 // 接收到5，放回a 表明是app端
      {
        UART_Send(USARTx, 'a');
      }
    }
    RecvBuf[port].act_lenth++;
  }
                                     //exp 串口中断内发送数据实验
  if(RecvBuf[port].act_lenth >= 4)  //sizeof("dow")  本身4个 再加 一个 RecvBuf[port].act_lenth++，总共5个
  {
    if((RecvBuf[port].buf[0]== '5')&&(RecvBuf[port].buf[1]== 'd')&&(RecvBuf[port].buf[2]== 'o')&&(RecvBuf[port].buf[3]== 'w'))
    { 
       //GPIO_ResetBits(GPIOG, GPIO_Pin_8);
       //printf("in SetDownLoadRecorder  \n\r");
       SetDownLoadRecorder();
       //printf("SetDownLoadRecorder finish \n\r");
#if 0
       SetDownLoadRecorder();
       if ((((*(volatile vu8*)AppDownloadRecAddr)) == 'r')&&(((*(volatile vu8*)(AppDownloadRecAddr+4))) == 'l'))
       {
         PutString(2,60,"rl");
       }
       else
       {
        PutString(2,60,"nrl");
       }
#endif
       
       NVIC_SETFAULTMASK();
       //printf("begin GenerateSystemReset \n\r");
       NVIC_GenerateSystemReset();
       //printf("end GenerateSystemReset \n\r");
    } 
    else if((RecvBuf[port].buf[0]== 's')&&(RecvBuf[port].buf[1]== 'c')&&(RecvBuf[port].buf[2]== 'a')&&(RecvBuf[port].buf[3]== 'n'))
    {  
       scan_and_config();
       for(i = 0; i < RJ_NUM; i++)
       {
         UART_Send(USARTx, GPIO_STATE[i].l_num);
         UART_Send(USARTx, GPIO_STATE[i].h_num);
       }
      RecvBuf[port].act_lenth = 0;            //处理完命令后清零
    }
    else if((RecvBuf[port].buf[0]== 'v')&&(RecvBuf[port].buf[1]== 'e')&&(RecvBuf[port].buf[2]== 'r')&&(RecvBuf[port].buf[3]== 's'))
    {
#if ROB_0
      UART_Send(USARTx, '0');
#else
      UART_Send(USARTx, '1');
#endif
      RecvBuf[port].act_lenth = 0;
    }
    else 
    {
      RecvBuf[port].act_lenth = 0;
    }
    /*
    else if((RecvBuf[port].buf[0]== 'j')&&(RecvBuf[port].buf[1]== 't')&&(RecvBuf[port].buf[2]== 'o')&&(RecvBuf[port].buf[3]== 'f'))
    {
      release_jtagswd();
    }
    else if((RecvBuf[port].buf[0]== 'j')&&(RecvBuf[port].buf[1]== 't')&&(RecvBuf[port].buf[2]== 'o')&&(RecvBuf[port].buf[3]== 'n'))
    {
      enable_jtagswd();
    }
    */
  }
  
}


/******************************************************************************************
* 函数名称	: void RxTxISR_Handler (u8 port)
* 函数功能	: 串口中断处理函数
*				  
* 输入参数	: 
*		          
* 返回数值	:
*		 
*******************************************************************************************/
void RxTxISR_Handler (u8 port)
{
  int i; 
  USART_TypeDef     *USARTx;
  
  USARTx = GetUSARTx(port);
  
  if(port == DOWNLOAD_PORT)
  {
    DownLoadAct(port,USARTx);
  }
  else if(port == DIGI_REC)
  {
    printf("rx hd\n\r");
    RecvBuf[port].buf[RecvBuf[port].act_lenth]= USART_ReceiveData(USARTx);
    RecvBuf[port].act_lenth++;
  }
  
#if 0
  if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
  {
    RecvBuf[port].buf[RecvBuf[port].act_lenth]= USART_ReceiveData(USARTx);//RS485_Receive(USART_TypeDef* USARTx)
    //USART_SendData(USARTx, RecvBuf[port].buf[RecvBuf[port].act_lenth]);
    if(RecvBuf[port].act_lenth == 0)
    {
      if(RecvBuf[port].buf[0] == '5')                 // 接收到5，放回a 表明是app端
      {
        UART_Send(USARTx, 'a');
      }
    }
    RecvBuf[port].act_lenth++;
  }
                                     //exp 串口中断内发送数据实验
  if(RecvBuf[port].act_lenth >= 4)  //sizeof("dow")  本身4个 再加 一个 RecvBuf[port].act_lenth++，总共5个
  {
    if((RecvBuf[port].buf[0]== '5')&&(RecvBuf[port].buf[1]== 'd')&&(RecvBuf[port].buf[2]== 'o')&&(RecvBuf[port].buf[3]== 'w'))
    { 
       //GPIO_ResetBits(GPIOG, GPIO_Pin_8);
       //printf("in SetDownLoadRecorder  \n\r");
       SetDownLoadRecorder();
       //printf("SetDownLoadRecorder finish \n\r");
#if 0
       SetDownLoadRecorder();
       if ((((*(volatile vu8*)AppDownloadRecAddr)) == 'r')&&(((*(volatile vu8*)(AppDownloadRecAddr+4))) == 'l'))
       {
         PutString(2,60,"rl");
       }
       else
       {
        PutString(2,60,"nrl");
       }
#endif
       
       NVIC_SETFAULTMASK();
       //printf("begin GenerateSystemReset \n\r");
       NVIC_GenerateSystemReset();
       //printf("end GenerateSystemReset \n\r");
    } 
    else if((RecvBuf[port].buf[0]== 's')&&(RecvBuf[port].buf[1]== 'c')&&(RecvBuf[port].buf[2]== 'a')&&(RecvBuf[port].buf[3]== 'n'))
    {  
       scan_and_config();
       for(i = 0; i < RJ_NUM; i++)
       {
         UART_Send(USARTx, GPIO_STATE[i].l_num);
         UART_Send(USARTx, GPIO_STATE[i].h_num);
       }
      RecvBuf[port].act_lenth = 0;            //处理完命令后清零
    }
    else if((RecvBuf[port].buf[0]== 'v')&&(RecvBuf[port].buf[1]== 'e')&&(RecvBuf[port].buf[2]== 'r')&&(RecvBuf[port].buf[3]== 's'))
    {
#if ROB_0
      UART_Send(USARTx, '0');
#else
      UART_Send(USARTx, '1');
#endif
      RecvBuf[port].act_lenth = 0;
    }
    else 
    {
      RecvBuf[port].act_lenth = 0;
    }
    /*
    else if((RecvBuf[port].buf[0]== 'j')&&(RecvBuf[port].buf[1]== 't')&&(RecvBuf[port].buf[2]== 'o')&&(RecvBuf[port].buf[3]== 'f'))
    {
      release_jtagswd();
    }
    else if((RecvBuf[port].buf[0]== 'j')&&(RecvBuf[port].buf[1]== 't')&&(RecvBuf[port].buf[2]== 'o')&&(RecvBuf[port].buf[3]== 'n'))
    {
      enable_jtagswd();
    }
    */
  }
  
#endif
  
  if(USART_GetITStatus(USARTx, USART_IT_TXE) != RESET)
  {     
    
  }
}

/******************************************************************************************
* 函数名称	: int  RS485Receive(int No)
* 函数功能	: 
*				  
* 输入参数	: No 1 2 3 分别对应 USART1 UART4 UART5
*		          
* 返回数值	:
*		 
*******************************************************************************************/
int  RS485Receive(int No)
{
  USART_TypeDef* USARTx;
  
  if(No == 1)
    USARTx = USART1;
  else if(No == 2)
    USARTx = UART4;
  else 
    USARTx = UART5;
  return RS485_Receive(USARTx);
}


/******************************************************************************************
* 函数名称	: void RS485Send(int No, unsigned short data)
* 函数功能	: 
*				  
* 输入参数	: No 1 2 3 分别对应 USART1 UART4 UART5
*		          
* 返回数值	:
*		 
*******************************************************************************************/
void RS485Send(int No, unsigned short data)
{

   USART_TypeDef* USARTx;
  if(No == 1)
    USARTx = USART1;
  else if(No == 2)
    USARTx = UART4;
  else 
    USARTx = UART5;
  RS485_Send(USARTx,data);
}

/******************************************************************************************
* 函数名称	: USART_TypeDef    *GetUSARTx(CPU_INT08U  port_nbr)
* 函数功能	: 
*				  
* 输入参数	: port_nbr : 1 2 3 ，获取对应的UART 
*		          
* 返回数值	:
*		 
*******************************************************************************************/
USART_TypeDef    *GetUSARTx(CPU_INT08U  port_nbr)
{

    if(port_nbr == 1)
      return USART1;
    else if(port_nbr == 2)
      return USART2;
    else if(port_nbr == 3)
      return USART3;
    else if(port_nbr == 4)
      return UART4;
    else if(port_nbr == 5)
      return UART5;
    else
      return 0;
}



/******************************************************************************************
* 函数名称	: 
* 函数功能	: 
*				  
* 输入参数	: 
*		          
* 返回数值	:
*		 
*******************************************************************************************/

void IIC_Start (void)
{
	IIC2_SDA=HIGH;
	IIC2_SCL=HIGH;
	DelayNus(6*2);
	IIC2_SDA=LOW;
	DelayNus(6*2);
	IIC2_SCL=LOW;
	DelayNus(6);
}



void IIC_Stop (void)
{
	IIC2_SDA=LOW;
	IIC2_SCL=LOW;
	DelayNus(6*2);
	IIC2_SCL=HIGH;
	DelayNus(6*2);
	IIC2_SDA=HIGH;
	DelayNus(6*4);
}


u8 IIC_WriteByte (u8 txByte)
{
    u8 mask,erro=0;
    for (mask=0x80; mask>0; mask>>=1) 
    { 
	    if ((mask & txByte) == 0) 
                IIC2_SDA=LOW;
		else IIC2_SDA=HIGH;
		DelayNus(6);
		IIC2_SCL=HIGH; 
		DelayNus(6*3);
		IIC2_SCL=LOW;
		DelayNus(6*3);
    }
    IIC2_SDA=HIGH; //release SDA-line
    IIC2_SCL=HIGH; //clk #9 for ack
    DelayNus(6*3);
	if(READ2_SDA==HIGH) erro=ACK_ERROR;
    IIC2_SCL=LOW;
	DelayNus(6*3);
    return erro; //return error code
}


u8 IIC_ReadByte (u8 ack)
{
  u8 mask,rxByte=0;
  IIC2_SDA=HIGH; 
  DelayNus(6);
  for (mask=0x80; mask>0; mask>>=1) 
  { 
    IIC2_SCL=HIGH; //start clock on SCL-line
    DelayNus(6*3);
    if(READ2_SDA==1) rxByte=(rxByte | mask); 
    IIC2_SCL=LOW;
    DelayNus(6*3);
  }
	IIC2_SDA=ack; 
	DelayNus(6);
	IIC2_SCL=HIGH; //clk #9 for ack
	DelayNus(6*3);
	IIC2_SCL=LOW;
	IIC2_SDA=HIGH;
	DelayNus(6*3);
	return rxByte; 
}



/******************************************************************************************
* 函数功能	: 数据读取
* 输入参数	: 
* 返回数值	: 
* 20150427      : 0x10 0x11 0x00 0x00 0xNo.
*******************************************************************************************/
#if 0
int GetVoiceIndex(void)
{
  int lenth;
  int get_int;
  char port;
  
  port = 1;
  get_int = -1;
  
  lenth = SerialDataInInputQueue(port);
  
  if(lenth >= 5)
  {
     if((RecvBuf[port].buf[0] == 0x10)&&(RecvBuf[port].buf[1] == 0x11))
     {
        get_int =  RecvBuf[port].buf[4];
     }  else {
        get_int = -1;
     }
     RecvBuf[port].act_lenth = 0;
  }
  return get_int;
}

#endif

/******************************************************************************************
* 函数功能	: 设置下载标记 实际是直接擦除 ApplicationAddress 位置开始处的内容
* 输入参数	: 
* 返回数值	: 
*               : 
*******************************************************************************************/
int SetDownLoadRecorder(void)
{
#if ROB_0
  
  (*(volatile vu8*)TOP_RAM_ADDR_P2) = 'c';
  (*(volatile vu8*)TOP_RAM_ADDR_P3) = 'r';
#else
  FLASH_Status FLASHStatus;
  
  (*(volatile vu8*)TOP_RAM_ADDR_P2) = 'c';
  (*(volatile vu8*)TOP_RAM_ADDR_P3) = 'r';
  
  FLASH_Unlock();
  
  FLASHStatus  = FLASH_ErasePage(AppDownloadRecAddr);
  //printf("FLASH_ErasePage:%d\n\r",FLASHStatus);
  
  if(FLASH_COMPLETE != FLASH_COMPLETE)
  {
     FLASHStatus  = FLASH_ErasePage(AppDownloadRecAddr);
     printf("FLASH_ErasePage Err:%d\n\r",FLASHStatus);
  }
  
  FLASHStatus = FLASH_ProgramHalfWord(AppDownloadRecAddr, 'c');
  if ((*(volatile vu8*)AppDownloadRecAddr) != 'c')
  {
    printf("write c Err:%d\n\r",FLASHStatus);
    FLASHStatus = FLASH_ProgramHalfWord(AppDownloadRecAddr, 'c');
  }
  
  FLASHStatus = FLASH_ProgramHalfWord(AppDownloadRecAddr+4, 'r');
  if ((*(volatile vu8*)AppDownloadRecAddr) != 'r')
  {
    printf("write c Err:%d\n\r",FLASHStatus);
    FLASHStatus = FLASH_ProgramHalfWord(AppDownloadRecAddr+4, 'r');
  }
  
  FLASH_Lock();
#endif
  
}


void process_6(void *pdata)
{
  while(1)
  {
  if(pstwo_use_flag)
  {
    PS2_ShortPoll();
  }
  OSTimeDlyHMSM(0,0,0,500);
  }
}

void led_flash(ELED en)
{}











