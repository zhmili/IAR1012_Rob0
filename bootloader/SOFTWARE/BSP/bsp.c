/*
********************************************************************************
*   
*                              ARM Cortex-M3 Port
*
* File          : BSP.C
* Version       : V1.0
* By            : Õı∫Í«ø
*
* For           : Stm32f10x
* Mode          : Thumb2
* Toolchain     : 
*                   RealView Microcontroller Development Kit (MDK)
*                   Keil uVision
* Description   : STM32F10x ƒ⁄≤ø œµÕ≥µƒ≈‰÷√
*
*                   1£¨œµÕ≥÷–∂œ”≈œ»º∂ƒ£ Ω…Ë÷√
*                   2£¨œµÕ≥≥Ã–Ú∆Ù∂Ø÷∏∂®
*                   3£¨œµÕ≥ ±÷”º∆ ±∆˜≈‰÷√
*                   4£¨–æ∆¨“˝Ω≈≥ı ºªØ
*
* Date          : 2012.05.22
*******************************************************************************/

#include <includes.h>

GPIO_InitTypeDef GPIO_InitStructure;
ErrorStatus HSEStartUpStatus;

void DelayNms(u16 ms)
{
    u16 i=0;  
   while(ms--)
   {
      i=15;  //
      while(i--) ;    
   }

}

#if ROB0
/*******************************************************************************
* Function Name  : pin_cfg 				
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void pin_cfg(GPIO_TypeDef* GPIOx, u16 GPIO_Pinx ,GPIOMode_TypeDef GPIO_Modex, GPIOSpeed_TypeDef GPIO_Speed)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pinx;
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed;
  GPIO_InitStructure.GPIO_Mode = GPIO_Modex;
  GPIO_Init(GPIOx, &GPIO_InitStructure);		
}

/*******************************************************************************
* Function Name  : pin_com_cfg
* Description    : ????
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void pin_com_cfg(void)
{
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);				//SWJ????( JTAG+SW-DP)	
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);	      
  
  pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);			//PA4,4051?ADC??
  
  pin_cfg(EX_AS0PORT,EX_AS0PIN,GPIO_Mode_Out_PP,GPIO_Speed_2MHz);
  pin_cfg(EX_AS1PORT,EX_AS1PIN,GPIO_Mode_Out_PP,GPIO_Speed_2MHz);
  pin_cfg(EX_AS2PORT,EX_AS2PIN,GPIO_Mode_Out_PP,GPIO_Speed_2MHz);
  pin_cfg(EX_AS3PORT,EX_AS3PIN,GPIO_Mode_Out_PP,GPIO_Speed_2MHz);
  pin_cfg(EX_AS4PORT,EX_AS4PIN,GPIO_Mode_Out_PP,GPIO_Speed_2MHz);
}

u16 get_adc_value(ADC_TypeDef*  ADCx ,u8 adc_channel)
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
  ADC_RegularChannelConfig(ADCx, adc_channel, 1, ADC_SampleTime_7Cycles5);
  
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
* Function Name  : s4s0_set  				74HC4051  S4-S0 
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void s4_0_set(char num)
{
  int count ;
  switch(num)
  {
   case 10:
      S4_0(1,0,0,1,0);
      break;
      
   case 9:
      S4_0(1,0,1,1,0);
      break;
      
   case 8:
      S4_0(1,0,1,0,0);
      break;
      
   case 7:
      S4_0(1,0,1,1,1);
      break;   
      
   case 6:
      S4_0(1,0,1,0,1);
      break;  
	
   case 5:
      S4_0(0,1,0,1,1);
      break; 
  
   case 4:
      S4_0(0,1,0,0,0);
      break; 
      
   case 3:
      S4_0(0,1,0,0,1);
      break; 
      
   case 2:
      S4_0(0,1,0,1,0);
      break;     

    case 1:             //RJ2
      S4_0(0,1,1,0,0);
      break; 

    case 0:            //RJ1
      S4_0(0,1,1,1,0);
      break; 
      
    case  11:         //RJ12
      S4_0(0,1,1,1,1);
      break;
      
    case 12:
      S4_0(0,1,1,0,1);
      break;

    case 13:
      S4_0(1,0,0,0,1);
      break;
      
    case 14:
      S4_0(1,0,0,1,1);
      break;
      
    case 15:
      S4_0(1,0,0,0,0);
      break;
    
    default : 
      break;
    }
}


int   left_button_read()
{
	 volatile u16 mid;
   s4_0_set(14);			//4051?D??
   pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);			//PA4,4051µ?ADC???˙

   mid = get_adc_value(ADC1, EX_APCH);	

   if(mid < 904)		//1.1V??ÛD∞'?¸∞'??Í±µÌµ???
    return 0;			//∑µ??1±ÌÍ?ÛD∞'?¸∞'??
    else
    return 1;
}

int  middle_button_read()
{
	volatile u16 mid;
   s4_0_set(15);			//4051?D??
   pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);			//PA4,4051µ?ADC???˙
	
   mid = get_adc_value(ADC1, EX_APCH);	

   if(mid < 904)		//1.1V??ÛD∞'?¸∞'??Í±µÌµ???
	return 0;			//∑µ??1±ÌÍ?ÛD∞'?¸∞'??
   else
	return 1;
}

#else
/******************************************************************************************
* ????	: ?????
* ????	: 
* ????	: void
*******************************************************************************************/
int   left_button_read()
{
return GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11);

}

/******************************************************************************************
* ????	: ??????
* ????	: 
* ????	: void
*******************************************************************************************/
int  middle_button_read()
{
return GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15);

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
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();
    
    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);
    
    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    
    if(HSEStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        
        /* Flash 2 wait state */
        FLASH_SetLatency(FLASH_Latency_2);
        
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1); 
        
        /* PCLK2 = HCLK */
        RCC_PCLK2Config(RCC_HCLK_Div1); 
        
        /* PCLK1 = HCLK */
        RCC_PCLK1Config(RCC_HCLK_Div1);
        
        /* PLLCLK = 8MHz * 9 = 72 MHz */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        
        /* Enable PLL */ 
        RCC_PLLCmd(ENABLE);
        
        /* Wait till PLL is ready */
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }
        
        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        
        /* Wait till PLL is used as system clock source */
        while(RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
	
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
#ifdef USE_STM3210B_EVAL
    /* Enable the USART2 Pins Software Remapping */
    GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
#endif



#if ROB0
	pin_com_cfg();
#else
  
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
#endif 
  
// PB
#if ROB0
	GPIO_InitStructure.GPIO_Pin		=  LCD_CS_PIN|LCD_RE_PIN|LCD_CK_PIN|LCD_MO_PIN|LCD_A_PIN;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode		=  GPIO_Mode_Out_PP;
  GPIO_Init(LCD_CS_PORT, &GPIO_InitStructure);

#else

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
  
#if ROB0 

#else
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
  
 // ‘ˆº”µƒusb ºÏ≤‚π¶ƒ‹ 
  GPIO_InitStructure.GPIO_Mode = GPIOG_USB_TEST;
  GPIO_InitStructure.GPIO_Speed     =  GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode 	    =  GPIO_Mode_IPU;
  GPIO_Init(GPIOG, &GPIO_InitStructure); 
 #endif
}


/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(vu32 nCount)
{
    for(; nCount != 0; nCount--);
}

/*******************************************************************************
∫Ø  ˝ √˚£∫void IWDG_Init(void)
π¶ƒ‹√Ë ˆ£∫ø¥√≈π∑≥ı ºªØ						
»Îø⁄≤Œ ˝£∫							
∑µªÿ≤Œ ˝£∫
¥¥Ω® ±º‰: 2011.6.24
********************************************************************************/
void IWDG_Init(void)
{
    IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );
    IWDG_SetPrescaler( IWDG_Prescaler_64);	//◊Ó–°
    IWDG_SetReload( 0x138);		//40KHzƒ⁄≤ø ±÷” (1/40000 * 64 * 0x138 = 0.5s)
    IWDG_WriteAccessCmd( IWDG_WriteAccess_Disable );
    IWDG_Enable();
    IWDG_ReloadCounter();
}

/*******************************************************************************
* Function Name :void SysTickInit(void)
* Description   :œµÕ≥∂® ±∆˜ ±º‰≈‰÷√
* Input         :
* Output        :
* Other         : ±ª˘Œ™1ms
* Date          :2011.11.03  12:59:13
*******************************************************************************/
void SysTickInit(void)
{
//    SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK_Div8 );     //œµÕ≥º∆ ˝∆˜ ±÷”‘¥Œ™AHB/8
//    SysTick_SetReload( 0x002328 );                              //SysTick÷ÿ◊∞‘ÿ÷µ   ±ª˘1ms
//    SysTick_ITConfig( ENABLE );                                 // πƒ‹÷–∂œ
//    SysTick_CounterCmd( SysTick_Counter_Enable );               //ø™∆ÙœµÕ≥º∆ ˝∆˜
}

/*******************************************************************************
* Function Name :void InterruptOrder(void)
* Description   :÷–∂œœÚ¡ø£¨”≈œ»º∂
* Input         :
* Output        :
* Other         :
* Date          :2011.10.27  11:50:05
*******************************************************************************/
void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x20000000 */
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
#endif
    
    /* 1 bit for pre-emption priority, 3 bits for subpriority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//”≈œ»º∂…Ë÷√  »´Œ™«¿’º Ω”≈œ»º∂
}




/*******************************************************************************
* Function Name :void SystemConfig(void)
* Description   :œµÕ≥≥ı ºªØ
* Input         :
* Output        :
* Other         :
* Date          :2011.10.27  13:14:59
*******************************************************************************/
void BspInit(void)
{
    RCC_Configuration();
    NVIC_Configuration();	//÷–∂œ”≈œ»º∂…Ë÷√
    GPIO_Configuration();	//∂Àø⁄≥ı ºªØ£¨À˘”–∂Àø⁄πÿ

//  LedInit();
    BspTim2Init();
    BspTim3Init();

#ifdef USE_USART1
    BspUsart1Init();
#endif

#ifdef USE_UART5
    BspUart5Init();
#endif
	
}

void BspClose(void)
{
    BspTim2Close();
    BspTim3Close();
#ifdef USE_USART1
    BspUsart1Close();
#endif 

#ifdef USE_UART5
    BspUart5Close();
#endif
}

void LedInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}



void LedOn(u32 n)
{
    switch (n)
    {
    case LED_0:
        GPIO_SetBits(GPIOA, GPIO_Pin_11);
        break;
    case LED_1:
        GPIO_SetBits(GPIOA, GPIO_Pin_11);
        break;
    case LED_2:
        GPIO_SetBits(GPIOF, GPIO_Pin_6);
        break;
    case LED_3:
        GPIO_SetBits(GPIOF, GPIO_Pin_7);
        break;
    case LED_4:
        GPIO_SetBits(GPIOF, GPIO_Pin_8);
        break;
    case LED_5:
        GPIO_SetBits(GPIOF, GPIO_Pin_9);
        break;
    default:
        break;
    }
}


void LedOff(u32 n)
{
    switch (n)
    {
    case LED_0:
        GPIO_ResetBits(GPIOA, GPIO_Pin_11);
        break;
    case LED_1:
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
        break;
    case LED_2:
        GPIO_ResetBits(GPIOF, GPIO_Pin_6);
        break;
    case LED_3:
        GPIO_ResetBits(GPIOF, GPIO_Pin_7);
        break;
    case LED_4:
        GPIO_ResetBits(GPIOF, GPIO_Pin_8);
        break;
    case LED_5:
        GPIO_ResetBits(GPIOF, GPIO_Pin_9);
        break;
    default:
        break;
    }
}


/*******************************************************************************
* Function Name  : SysTickHandler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{

}


/******************************************************************************************
* ∫Ø ˝π¶ƒ‹	: …Ë÷√œ¬‘ÿ±Íº«  µº  «÷±Ω”≤¡≥˝ ApplicationAddress Œª÷√ø™ º¥¶µƒƒ⁄»›
*  ‰»Î≤Œ ˝	: 
* ∑µªÿ ˝÷µ	: 
*               : 
*******************************************************************************************/
int SetDownLoadRecorder(char parm1, char parm2)
{
  FLASH_Status FLASHStatus;
  
  FLASH_Unlock();
  
  FLASHStatus  = FLASH_ErasePage(AppDownloadRecAddr);
  FLASHStatus = FLASH_ProgramHalfWord(AppDownloadRecAddr, parm1);
  FLASHStatus = FLASH_ProgramHalfWord(AppDownloadRecAddr+4, parm2);
  
  FLASH_Lock();

}




/*********************************** END **************************************/

