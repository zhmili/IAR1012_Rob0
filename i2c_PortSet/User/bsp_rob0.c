
#include "bsp_rob0.h"
#include "bsp_usart.h"
#include "stm32f10x_rtc.h"
//0-10 : 对应实际 模式 RJ0 - RJ10
//11:    对应 485      RJ11
//12:    对应 232      RJ12
//13:    对应          电池电压
//14:    对应          KEY1
//15:    对应          KEY2
TGPIO_STATE GPIO_STATE[16];

/*******************************************************************************
* Function Name  :   				
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void static port_init(void)
{
  memset(GPIO_STATE,0,16*sizeof(TGPIO_STATE));///////////////////////////////////////////////////////////////////?????
  
  GPIO_STATE[0].out_port = RJ1_OPORT;
  GPIO_STATE[0].out_pin = RJ1_OPIN;
  GPIO_STATE[0].tim_port = RJ1_TPORT;
  GPIO_STATE[0].tim_pin = RJ1_TPIN;
  GPIO_STATE[0].ad_port = RJ1_APORT;
  GPIO_STATE[0].ad_pin = RJ1_APIN;
  GPIO_STATE[0].ADC =  RJ1_ADC;
  GPIO_STATE[0].ad_ch = RJ1_ACH;
  GPIO_STATE[0].TIM = RJ1_TIM;
  GPIO_STATE[0].tim_ch = RJ1_TCH;  
  
  GPIO_STATE[1].out_port = RJ2_OPORT;
  GPIO_STATE[1].out_pin = RJ2_OPIN;
  GPIO_STATE[1].tim_port = RJ2_TPORT;
  GPIO_STATE[1].tim_pin = RJ2_TPIN;
  GPIO_STATE[1].ad_port = RJ2_APORT;
  GPIO_STATE[1].ad_pin = RJ2_APIN;
  GPIO_STATE[1].ADC =  RJ2_ADC;
  GPIO_STATE[1].ad_ch = RJ2_ACH;
  GPIO_STATE[1].TIM = RJ2_TIM;
  GPIO_STATE[1].tim_ch = RJ2_TCH;  
  
  GPIO_STATE[2].out_port = RJ3_OPORT;
  GPIO_STATE[2].out_pin =  RJ3_OPIN;
  GPIO_STATE[2].tim_port = RJ3_TPORT;
  GPIO_STATE[2].tim_pin =  RJ3_TPIN;
  GPIO_STATE[2].ad_port =  RJ3_APORT;
  GPIO_STATE[2].ad_pin =   RJ3_APIN;
  GPIO_STATE[2].ADC =      RJ3_ADC;
  GPIO_STATE[2].ad_ch =    RJ3_ACH;
  GPIO_STATE[2].TIM =      RJ3_TIM;
  GPIO_STATE[2].tim_ch =   RJ3_TCH;
  
  GPIO_STATE[3].out_port = RJ4_OPORT;
  GPIO_STATE[3].out_pin =  RJ4_OPIN;
  GPIO_STATE[3].tim_port = RJ4_TPORT;
  GPIO_STATE[3].tim_pin =  RJ4_TPIN;
  GPIO_STATE[3].ad_port =  RJ4_APORT;
  GPIO_STATE[3].ad_pin =   RJ4_APIN;
  GPIO_STATE[3].ADC =      RJ4_ADC;
  GPIO_STATE[3].ad_ch =    RJ4_ACH;
  GPIO_STATE[3].TIM =      RJ4_TIM;
  GPIO_STATE[3].tim_ch =   RJ4_TCH;
  
  GPIO_STATE[4].out_port = RJ5_OPORT;
  GPIO_STATE[4].out_pin =  RJ5_OPIN;
  GPIO_STATE[4].tim_port = RJ5_TPORT;
  GPIO_STATE[4].tim_pin =  RJ5_TPIN;
  GPIO_STATE[4].ad_port =  RJ5_APORT;
  GPIO_STATE[4].ad_pin =   RJ5_APIN;
  GPIO_STATE[4].ADC =      RJ5_ADC;
  GPIO_STATE[4].ad_ch =    RJ5_ACH;
  GPIO_STATE[4].TIM =      RJ5_TIM;
  GPIO_STATE[4].tim_ch =   RJ5_TCH;
  
  GPIO_STATE[5].out_port = RJ6_OPORT;
  GPIO_STATE[5].out_pin =  RJ6_OPIN;
  GPIO_STATE[5].tim_port = RJ6_TPORT;
  GPIO_STATE[5].tim_pin =  RJ6_TPIN;
  GPIO_STATE[5].ad_port =  RJ6_APORT;
  GPIO_STATE[5].ad_pin =   RJ6_APIN;
  GPIO_STATE[5].ADC =      RJ6_ADC;
  GPIO_STATE[5].ad_ch =    RJ6_ACH;
  GPIO_STATE[5].TIM =      RJ6_TIM;
  GPIO_STATE[5].tim_ch =   RJ6_TCH; 
  
  GPIO_STATE[6].out_port = RJ7_OPORT;
  GPIO_STATE[6].out_pin =  RJ7_OPIN;
  GPIO_STATE[6].tim_port = RJ7_TPORT;
  GPIO_STATE[6].tim_pin =  RJ7_TPIN;
  GPIO_STATE[6].ad_port =  RJ7_APORT;
  GPIO_STATE[6].ad_pin =   RJ7_APIN;
  GPIO_STATE[6].ADC =      RJ7_ADC;
  GPIO_STATE[6].ad_ch =    RJ7_ACH;
  GPIO_STATE[6].TIM =      RJ7_TIM;
  GPIO_STATE[6].tim_ch =   RJ7_TCH; 

  GPIO_STATE[7].out_port = RJ8_OPORT;
  GPIO_STATE[7].out_pin =  RJ8_OPIN;
  GPIO_STATE[7].tim_port = RJ8_TPORT;
  GPIO_STATE[7].tim_pin =  RJ8_TPIN;
  GPIO_STATE[7].ad_port =  RJ8_APORT;
  GPIO_STATE[7].ad_pin =   RJ8_APIN;
  GPIO_STATE[7].ADC =      RJ8_ADC;
  GPIO_STATE[7].ad_ch =    RJ8_ACH;
  GPIO_STATE[7].TIM =      RJ8_TIM;
  GPIO_STATE[7].tim_ch =   RJ8_TCH;
  
  GPIO_STATE[8].out_port = RJ9_OPORT;
  GPIO_STATE[8].out_pin =  RJ9_OPIN;
  GPIO_STATE[8].tim_port = RJ9_TPORT;
  GPIO_STATE[8].tim_pin =  RJ9_TPIN;
  GPIO_STATE[8].ad_port =  RJ9_APORT;
  GPIO_STATE[8].ad_pin =   RJ9_APIN;
  GPIO_STATE[8].ADC =      RJ9_ADC;
  GPIO_STATE[8].ad_ch =    RJ9_ACH;
  GPIO_STATE[8].TIM =      RJ9_TIM;
  GPIO_STATE[8].tim_ch =   RJ9_TCH;
  
  GPIO_STATE[9].out_port = RJ10_OPORT;
  GPIO_STATE[9].out_pin =  RJ10_OPIN;
  GPIO_STATE[9].tim_port = RJ10_TPORT;
  GPIO_STATE[9].tim_pin =  RJ10_TPIN;
  GPIO_STATE[9].ad_port =  RJ10_APORT;
  GPIO_STATE[9].ad_pin =   RJ10_APIN;
  GPIO_STATE[9].ADC =      RJ10_ADC;
  GPIO_STATE[9].ad_ch =    RJ10_ACH;
  GPIO_STATE[9].TIM =      RJ10_TIM;
  GPIO_STATE[9].tim_ch =   RJ10_TCH;
 
  GPIO_STATE[10].out_port = RJ11_OPORT;
  GPIO_STATE[10].out_pin =  RJ11_OPIN;
  GPIO_STATE[10].tim_port = RJ11_TPORT;
  GPIO_STATE[10].tim_pin =  RJ11_TPIN;
  GPIO_STATE[10].ad_port =  RJ11_APORT;
  GPIO_STATE[10].ad_pin =   RJ11_APIN;
  GPIO_STATE[10].ADC =      RJ11_ADC;
  GPIO_STATE[10].ad_ch =    RJ11_ACH;
  GPIO_STATE[10].TIM =      RJ11_TIM;
  GPIO_STATE[10].tim_ch =   RJ11_TCH;
}


/*******************************************************************************
* Function Name  : 	
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void release_jtagswd(void)
{
 //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
 //GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);					//都使能
 //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		//JTAG-DP失能+ SW-DP使能
 GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);				//SWJ完全失能（ JTAG+SW-DP）	
}
/*******************************************************************************
* Function Name  : 				
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void static rcc_cfg(void)
{
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    
    RCC_WaitForHSEStartUp();
    
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    
    FLASH_SetLatency(FLASH_Latency_2);
    
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    
    RCC_PCLK2Config(RCC_HCLK_Div1);

    RCC_PCLK1Config(RCC_HCLK_Div2);        
    
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);//??ADC??= PCLK2/4
    
    RCC_PLLCmd(ENABLE);
    
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
        ;
    }
    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    while (RCC_GetSYSCLKSource() != 0x08) {
        ;
    }
  
    PWR_BackupAccessCmd(ENABLE);//????RTC ??????
    RCC_LSICmd(DISABLE);//?????????????? ?,PC14 PC15 ??????IO?
    BKP_TamperPinCmd(DISABLE);//????????,??? PC13,??????IO ??
    
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM4|RCC_APB1Periph_SPI2|RCC_APB1Periph_UART4|RCC_APB1Periph_UART5|RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_ADC1|RCC_APB2Periph_TIM1|RCC_APB2Periph_TIM8, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1|RCC_APB1Periph_I2C2|RCC_APB1Periph_USART2|RCC_APB1Periph_USART3, DISABLE); 
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void static nvic_cfg(void)
{
 /* Set the Vector Table base location at 0x08008000 */ 
  NVIC_InitTypeDef NVIC_InitStructure;
//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0xFFFF);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;															////////////////////////
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;														/////////////////////////////////
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
  //NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 0, 9);
}	



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
* Description    : 固定脚位
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void pin_com_cfg(void)
{
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);				//SWJ完全失能（ JTAG+SW-DP）	
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);	      

  pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);			//PA4,4051的ADC端口
  
  pin_cfg(EX_AS0PORT,EX_AS0PIN,GPIO_Mode_Out_PP,GPIO_Speed_2MHz);
  pin_cfg(EX_AS1PORT,EX_AS1PIN,GPIO_Mode_Out_PP,GPIO_Speed_2MHz);
  pin_cfg(EX_AS2PORT,EX_AS2PIN,GPIO_Mode_Out_PP,GPIO_Speed_2MHz);
  pin_cfg(EX_AS3PORT,EX_AS3PIN,GPIO_Mode_Out_PP,GPIO_Speed_2MHz);
  pin_cfg(EX_AS4PORT,EX_AS4PIN,GPIO_Mode_Out_PP,GPIO_Speed_2MHz);
    
  pin_cfg(U4_TX_PORT,U4_TX_PIN,GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
  pin_cfg(U4_RX_PORT,U4_RX_PIN,GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);
  
  pin_cfg(U5_TX_PORT,U5_TX_PIN,GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
  pin_cfg(U5_RX_PORT,U5_RX_PIN,GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);  
  
  pin_cfg(RS485_EN_PORT,RS485_EN_PIN,GPIO_Mode_Out_PP,GPIO_Speed_50MHz); 
  
  pin_cfg(LCD_CS_PORT,LCD_CS_PIN,GPIO_Mode_Out_PP,GPIO_Speed_50MHz); 
  pin_cfg(LCD_RE_PORT,LCD_RE_PIN,GPIO_Mode_Out_PP,GPIO_Speed_50MHz); 
  pin_cfg(LCD_CK_PORT,LCD_CK_PIN,GPIO_Mode_Out_PP,GPIO_Speed_50MHz); 
  pin_cfg(LCD_MO_PORT,LCD_MO_PIN,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
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
//	delay(0xffff);
}


/*******************************************************************************
* Function Name  : pin_cfg 				
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
#if 1
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
#endif

/*******************************************************************************
* Function Name  : pin_cfg 				
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void set_pwm(TIM_TypeDef* TIMx, u16 TIM_Channel_x, s16 eff)
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


/*******************************************************************************
* Function Name  : tim_base_cfg
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void tim_com_cfg(void)		
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

  TIM_OC1Init(TIM4,&TIM_OCInitStructure); 
  TIM_OC2Init(TIM4,&TIM_OCInitStructure);
  TIM_OC3Init(TIM4,&TIM_OCInitStructure);
  TIM_OC4Init(TIM4,&TIM_OCInitStructure);

  TIM_OC1Init(TIM8,&TIM_OCInitStructure); 
  TIM_OC2Init(TIM8,&TIM_OCInitStructure);
  TIM_OC3Init(TIM8,&TIM_OCInitStructure); 
  TIM_OC4Init(TIM8,&TIM_OCInitStructure);
	
  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
  TIM_Cmd(TIM4, ENABLE);
  TIM_CtrlPWMOutputs(TIM4, ENABLE);
  
  TIM_Cmd(TIM8, ENABLE);
  TIM_CtrlPWMOutputs(TIM8, ENABLE);
	  
}


/*******************************************************************************
* Function Name  : adc_mode
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void static  adc_cfg(ADC_TypeDef* ADCx)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2|RCC_APB2Periph_ADC3, ENABLE );	//需要使用ADC2，3时打开
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	ADC_DeInit(ADCx);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
  
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADCx, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
  
	ADC_Cmd(ADCx, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADCx);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADCx));	//等待复位校准结束
	
	ADC_StartCalibration(ADCx);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADCx));	 //等待校准结束
 
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);		//使能指定的ADC1的软件转换启动功能

}	

/*******************************************************************************
* Function Name  : adc_valueave  ADC值求平均
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 get_ave_adc_value(ADC_TypeDef* ADCx, u8 ch, u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val += get_adc_value(ADCx , ch);
	}
	return temp_val/times;
} 	


/*******************************************************************************
* Function Name  : gpio_scan_cfg  
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void scan_port(void)
{
  int i;
  for(i = 0; i < COM_CH_NUM; i++)
  {
    pin_cfg(GPIO_STATE[i].out_port , GPIO_STATE[i].out_pin ,GPIO_Mode_IPU, GPIO_Speed_2MHz);	
    GPIO_STATE[i].out_pu = GPIO_ReadInputDataBit( GPIO_STATE[i].out_port , GPIO_STATE[i].out_pin);
    pin_cfg(GPIO_STATE[i].out_port , GPIO_STATE[i].out_pin ,GPIO_Mode_IPD, GPIO_Speed_2MHz);
    GPIO_STATE[i].out_pd = GPIO_ReadInputDataBit( GPIO_STATE[i].out_port , GPIO_STATE[i].out_pin);
    
    pin_cfg(GPIO_STATE[i].tim_port , GPIO_STATE[i].tim_pin ,GPIO_Mode_IPU, GPIO_Speed_10MHz);
    GPIO_STATE[i].tim_pu = GPIO_ReadInputDataBit(GPIO_STATE[i].tim_port , GPIO_STATE[i].tim_pin);
    pin_cfg(GPIO_STATE[i].tim_port , GPIO_STATE[i].tim_pin ,GPIO_Mode_IPD, GPIO_Speed_10MHz);
    GPIO_STATE[i].tim_pd = GPIO_ReadInputDataBit(GPIO_STATE[i].tim_port , GPIO_STATE[i].tim_pin);
		
    pin_cfg(GPIO_STATE[i].ad_port , GPIO_STATE[i].ad_pin ,GPIO_Mode_AIN, GPIO_Speed_2MHz);
												//////////////
		#if GET_AVE
    GPIO_STATE[i].ad_value = get_ave_adc_value(GPIO_STATE[i].ADC , GPIO_STATE[i].ad_ch, ADC_AVE_TIMES);/////////////ADC_GetConvertValueAVE
		#else
    GPIO_STATE[i].ad_value = get_adc_value(GPIO_STATE[i].ADC , GPIO_STATE[i].ad_ch);
    #endif		
		
		
    if(GPIO_STATE[i].ad_value >= 2730)	//2.2V
    {
      GPIO_STATE[i].ad_lev = 2;
    }
    else if(GPIO_STATE[i].ad_value >= 1365)		//1.1V
    {
      GPIO_STATE[i].ad_lev = 1;
    }
    else
    {
      GPIO_STATE[i].ad_lev = 0;
    }  
  }
  
	//IO配置在pin_com_cfg()
	//adc_mode(ADC1,EX_APCH);
  for(i = 0; i < 16; i++)
  {
    s4_0_set(i);
		
		#if GET_AVE
    GPIO_STATE[i].ex_ad_value = get_ave_adc_value(ADC1, EX_APCH, ADC_AVE_TIMES);								/////////////////////////////////////////
    #else
		GPIO_STATE[i].ex_ad_value = get_adc_value(ADC1, EX_APCH);	
		#endif
		
    if( GPIO_STATE[i].ex_ad_value > 1808)		//2.2V
    {
      GPIO_STATE[i].ex_ad_lev = 2;
    }
    else if(GPIO_STATE[i].ex_ad_value > 904)			//1.1V
    {
      GPIO_STATE[i].ex_ad_lev = 1;
    }
    else
    {
      GPIO_STATE[i].ex_ad_lev = 0;
    }
  }  
}

/*******************************************************************************
* Function Name  : judge_  
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void judge(void)
{
  int i;
  
  for(i = 0 ; i < COM_CH_NUM; i++)
  {
    //if((GPIO_STATE[i].out_pu == GPIO_STATE[i].out_pd)&&(GPIO_STATE[i].tim_pu == GPIO_STATE[i].tim_pd))
    {
      if((GPIO_STATE[i].out_pu == 0)&&(GPIO_STATE[i].tim_pu == 0))
      {
          if((GPIO_STATE[i].ad_lev == 0)&&(GPIO_STATE[i].ex_ad_lev == 0))
          {
            GPIO_STATE[i].h_num = DIG_OUT_PUT_1;//DIG_OUT_PUT_1;
            GPIO_STATE[i].l_num = 0;
          }
          else if((GPIO_STATE[i].ad_lev == 0)&&(GPIO_STATE[i].ex_ad_lev == 1))
          {
            //spi
            GPIO_STATE[i].h_num = RGB_LED_2;//SPI_2;
            GPIO_STATE[i].l_num = 0;
          }
          else if((GPIO_STATE[i].ad_lev == 0)&&(GPIO_STATE[i].ex_ad_lev == 2))
          {
            //res
            GPIO_STATE[i].h_num = BRUSH_MOT_3;//BRUSH_MOT_3;
            GPIO_STATE[i].l_num = 0;
          }
          else if((GPIO_STATE[i].ad_lev == 1)&&(GPIO_STATE[i].ex_ad_lev == 0))
          {
            GPIO_STATE[i].h_num = REC_PLY_SOUND_4;//BRUSH_MOT_4;
            GPIO_STATE[i].l_num = 0;
          }
          else if((GPIO_STATE[i].ad_lev == 1)&&(GPIO_STATE[i].ex_ad_lev == 1))
          {
            GPIO_STATE[i].h_num = SERVO_MOT_5;//SERVO_MOT_5;
            GPIO_STATE[i].l_num = 0;
          }
          else if((GPIO_STATE[i].ad_lev == 1)&&(GPIO_STATE[i].ex_ad_lev == 2))
          {
            GPIO_STATE[i].h_num = RES_6;//STEP_MOT_6;
            GPIO_STATE[i].l_num = 0;           
          }
          else if((GPIO_STATE[i].ad_lev == 2)&&(GPIO_STATE[i].ex_ad_lev == 0))
          {
            GPIO_STATE[i].h_num = BEEP_7;//RGB_LED_7;
            GPIO_STATE[i].l_num = 0; 
          }
          else if((GPIO_STATE[i].ad_lev == 2)&&(GPIO_STATE[i].ex_ad_lev == 1))
          {
            GPIO_STATE[i].h_num = SPI_8;//REC_PLY_SOUND_8;
            GPIO_STATE[i].l_num = 0;          
          }
          else if((GPIO_STATE[i].ad_lev == 2)&&(GPIO_STATE[i].ex_ad_lev == 2))
          {
            GPIO_STATE[i].h_num = RES_9;//BEEP_9;
            GPIO_STATE[i].l_num = 0;              
          }
      }
      else if((GPIO_STATE[i].out_pu == 1)&&(GPIO_STATE[i].tim_pu == 0))
      {
         if(GPIO_STATE[i].ex_ad_lev == 0)
          {
            GPIO_STATE[i].h_num = AD_IN_10;
            GPIO_STATE[i].l_num = 0; 
          }
          else if(GPIO_STATE[i].ex_ad_lev == 1)
          {
            GPIO_STATE[i].h_num = INF_DIS_11;
            GPIO_STATE[i].l_num = 0;         
          }
          else if(GPIO_STATE[i].ex_ad_lev == 2)
          {
            GPIO_STATE[i].h_num = KEY_12;
            GPIO_STATE[i].l_num = 0;            
          }
      }
      else if((GPIO_STATE[i].out_pu == 0)&&(GPIO_STATE[i].tim_pu == 1))
      {
         if(GPIO_STATE[i].ex_ad_lev == 0)
          {
            GPIO_STATE[i].h_num = RES_13;
            GPIO_STATE[i].l_num = 0;  
          }
          else if(GPIO_STATE[i].ex_ad_lev == 1)
          {
            GPIO_STATE[i].h_num = HUM_IND_14;
            GPIO_STATE[i].l_num = 0;            
          }
          else if(GPIO_STATE[i].ex_ad_lev == 2)
          {
            GPIO_STATE[i].h_num = SOUND_CTRL_15;
            GPIO_STATE[i].l_num = 0;             
          }
      }
      else if((GPIO_STATE[i].out_pu == 1)&&(GPIO_STATE[i].tim_pu == 1))
      {
          if((GPIO_STATE[i].ex_ad_lev == 0))
          {
            GPIO_STATE[i].h_num = RES_16;//RES_10;
            GPIO_STATE[i].l_num = 0; 
          }
          else if(GPIO_STATE[i].ex_ad_lev == 1)
          {
            GPIO_STATE[i].h_num = LIGHT_17;
            GPIO_STATE[i].l_num = 0; 
          }
          else if(GPIO_STATE[i].ex_ad_lev == 2)
          {
            GPIO_STATE[i].h_num = I2C_18;
            GPIO_STATE[i].l_num = 0;            
          }
      } 
      else
     {
      GPIO_STATE[i].h_num = 0;
      GPIO_STATE[i].l_num = 0; 
     }	
    }
 //   else
 //   {
 //     GPIO_STATE[i].h_num = 0;
 //     GPIO_STATE[i].l_num = 0; 
 //   }			
  }
  
  if(GPIO_STATE[11].ex_ad_lev == 2)
  {
      GPIO_STATE[11].h_num = RS232_19;
  }
  else
  {
  GPIO_STATE[i].h_num = 0;
  }
		
  if(GPIO_STATE[12].ex_ad_lev == 2)
  {
       GPIO_STATE[12].h_num = RS485_20;
  }
  else
  {
       GPIO_STATE[i].h_num = 0;
  }
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void pin_final_cfg(void)
{
  int i;
  
  for(i = 0; i < COM_CH_NUM; i++)
  {
    if(GPIO_STATE[i].h_num > 0)
    {
      switch(GPIO_STATE[i].h_num)
      {
      case DIG_OUT_PUT_1:
         pin_cfg(GPIO_STATE[i].out_port, GPIO_STATE[i].out_pin ,GPIO_Mode_Out_PP,GPIO_Speed_10MHz);
        break;
      
      case RGB_LED_2:
        pin_cfg(GPIO_STATE[i].out_port, GPIO_STATE[i].out_pin ,GPIO_Mode_Out_PP,GPIO_Speed_10MHz);
        pin_cfg(GPIO_STATE[i].ad_port, GPIO_STATE[i].ad_pin ,GPIO_Mode_Out_PP,GPIO_Speed_10MHz);
        pin_cfg(GPIO_STATE[i].tim_port, GPIO_STATE[i].tim_pin ,GPIO_Mode_Out_PP,GPIO_Speed_10MHz);
        break;
          
      case BRUSH_MOT_3:
        pin_cfg(GPIO_STATE[i].out_port, GPIO_STATE[i].out_pin ,GPIO_Mode_Out_PP,GPIO_Speed_10MHz);
        pin_cfg(GPIO_STATE[i].ad_port, GPIO_STATE[i].ad_pin ,GPIO_Mode_Out_PP,GPIO_Speed_10MHz);
        pin_cfg(GPIO_STATE[i].tim_port, GPIO_STATE[i].tim_pin ,GPIO_Mode_AF_PP,GPIO_Speed_10MHz);
        GPIO_STATE[i].TIM->ARR = TIM_PERIOD_18KHZ;
        GPIO_STATE[i].TIM->PSC = TIM_PRESC_18KHZ;
        set_pwm(GPIO_STATE[i].TIM, GPIO_STATE[i].tim_ch,0);
        break;
			
      case REC_PLY_SOUND_4:
      case BEEP_7:
        pin_cfg(GPIO_STATE[i].tim_port, GPIO_STATE[i].tim_pin ,GPIO_Mode_Out_PP,GPIO_Speed_10MHz);
        pin_cfg(GPIO_STATE[i].out_port, GPIO_STATE[i].out_pin ,GPIO_Mode_Out_PP,GPIO_Speed_10MHz);
        break;  

      case SERVO_MOT_5:
        pin_cfg(GPIO_STATE[i].tim_port, GPIO_STATE[i].tim_pin ,GPIO_Mode_AF_PP,GPIO_Speed_10MHz);
        GPIO_STATE[i].TIM->ARR = TIM_PERIOD_50HZ;
        GPIO_STATE[i].TIM->PSC = TIM_PRESC_50HZ;
        set_pwm(GPIO_STATE[i].TIM, GPIO_STATE[i].tim_ch, 0);
        break; 		
      
      case RES_6:
      case RES_13:
      case RES_16:
        break;
        
#if 0      
      case BEEP_7:
        pin_cfg(GPIO_STATE[i].tim_port, GPIO_STATE[i].tim_pin ,GPIO_Mode_AF_PP,GPIO_Speed_10MHz);
        GPIO_STATE[i].TIM->ARR = TIM_PERIOD_18KHZ;
        GPIO_STATE[i].TIM->PSC = TIM_PRESC_18KHZ;
        set_pwm(GPIO_STATE[i].TIM, GPIO_STATE[i].tim_ch, 0);
        break;    
#endif    
        
      case SPI_8:
        break;

      case RES_9:
        break;       

      case AD_IN_10:
      case LIGHT_17:
        pin_cfg(GPIO_STATE[i].ad_port , GPIO_STATE[i].ad_pin ,GPIO_Mode_AIN, GPIO_Speed_2MHz);	
        break;  
      
      case INF_DIS_11:
      case KEY_12:
      case HUM_IND_14:
      case SOUND_CTRL_15:
        pin_cfg(GPIO_STATE[i].ad_port , GPIO_STATE[i].ad_pin ,GPIO_Mode_IN_FLOATING, GPIO_Speed_2MHz);	
        break;

      case I2C_18:
				//pin_cfg(GPIO_STATE[i].ad_port , GPIO_STATE[i].ad_pin ,GPIO_Mode_IN_FLOATING, GPIO_Speed_2MHz);	 
			  GPIO_STATE[i].l_num = get_i2c_id(i);
				i2c_cfg(GPIO_STATE[i].l_num);
        break;
        
      default:
				break;
      }
    } 
  }
}
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void bsp_rob0_init(void)
{ 
	port_init();
  rcc_cfg();
	nvic_cfg();
	pin_com_cfg();
	tim_com_cfg();
	adc_cfg(ADC1);
}

#if 0
int get_i2c_id(int size,int chan)
{
	int ret;
	uint8_t ReadBuf[1];
	s4_0_set(chan);
	pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	GPIO_ResetBits(EX_APORT,EX_APIN);
	pin_cfg(GPIO_STATE[chan].out_port , GPIO_STATE[chan].out_pin ,GPIO_Mode_Out_OD, GPIO_Speed_50MHz);
	pin_cfg(GPIO_STATE[chan].tim_port , GPIO_STATE[chan].tim_pin ,GPIO_Mode_Out_OD, GPIO_Speed_50MHz);		
	ret = ee_ReadBytes(ReadBuf, 0, size, chan);
	
	GPIO_SetBits(EX_APORT,EX_APIN);
	s4_0_set(chan+1);
	pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);
	
	if(ret == 0)
		return -1;
	else
		return ReadBuf[0];
}
#endif

int get_i2c_id(int chan)
{
	int ret;
	uint8_t ReadBuf[2];
	s4_0_set(chan);
	pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	GPIO_ResetBits(EX_APORT,EX_APIN);
	pin_cfg(GPIO_STATE[chan].out_port , GPIO_STATE[chan].out_pin ,GPIO_Mode_Out_OD, GPIO_Speed_50MHz);
	pin_cfg(GPIO_STATE[chan].tim_port , GPIO_STATE[chan].tim_pin ,GPIO_Mode_Out_OD, GPIO_Speed_50MHz);		
	ret = ee_ReadBytes(ReadBuf, 0, 2, chan);
	GPIO_SetBits(EX_APORT,EX_APIN);
	s4_0_set(chan+1);
	pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);
	
	if(ret == 0)
	{
		return -1;
	}
	else
	{
		ret = (ReadBuf[0] + (ReadBuf[1]<<8));
		return ret;
	}
		
}



void i2c_cfg(int id)
{
	switch(id)
	{
		case 1:
			break;
		case 2:
			break;
	}
}





//---------------------------------------------

/**
  * @brief  配置TIM3输出的PWM信号的模式，如周期、极性、占空比
  * @param  无
  * @retval 无
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期
 * 占空比=TIMx_CCR/(TIMx_ARR +1)
 */
static void TIMx_CAPTURE_Mode_Config(TIM_TypeDef* TIMx, u16 TIM_Channel_x)
{
	TIM_ICInitTypeDef        TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_TIM8, ENABLE);//开头rcc_cfg（）有使能
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	

	//初始化定时器5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period      = 0xFFFF;                         //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler   = 71; 	                       //预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;            //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;        //TIM向上计数模式
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);                  //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//初始化TIMx输入捕获参数
	TIM_ICInitStructure.TIM_Channel     = TIM_Channel_x;              //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	            //配置输入分频,不分频 
	TIM_ICInitStructure.TIM_ICFilter    = 0x00;                         //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIMx, &TIM_ICInitStructure);

	//中断分组初始化
//	TIMx_NVIC_Configuration();
	if(TIM_Channel_x==TIM_Channel_1)
	TIM_ITConfig ( TIMx, TIM_IT_Update | TIM_IT_CC1, ENABLE );//允许更新中断 ,允许CC1IE捕获中断	
	if(TIM_Channel_x==TIM_Channel_2)
	TIM_ITConfig ( TIMx, TIM_IT_Update | TIM_IT_CC2, ENABLE );//允许更新中断 ,允许CC2IE捕获中断	
	if(TIM_Channel_x==TIM_Channel_3)
	TIM_ITConfig ( TIMx, TIM_IT_Update | TIM_IT_CC3, ENABLE );//允许更新中断 ,允许CC3IE捕获中断	
	if(TIM_Channel_x==TIM_Channel_4)
	TIM_ITConfig ( TIMx, TIM_IT_Update | TIM_IT_CC4, ENABLE );//允许更新中断 ,允许CC4IE捕获中断	
	
	TIM_Cmd ( TIMx, ENABLE ); 	//使能定时器5
	
}


static void TIMx_NVIC_Configuration(int TIMx_IRQn)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
	NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}



/*
*********************************************************************************************************
*	函 数 名: Delay
*	功能说明: 延时 
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
void delay(__IO uint32_t nCount)
{

  for(; nCount != 0; nCount--);
	
}



