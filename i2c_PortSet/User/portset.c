#include "portset.h"
#include "bsp_usart.h"

__IO uint16_t ADC_ConvertedValue[macNOFCHANEL];

char outIOIPU_state[11];		//设置为上拉输入时，OUT脚的状态RJ1---RJ11
char outIOIPD_state[11];		//设置为下拉输入时，OUT脚的状态RJ1---RJ11
char timIOIPU_state[11];		//设置为上拉输入时，TIM脚的状态RJ1---RJ11
char timIOIPD_state[11];		//设置为下拉输入时，TIM脚的状态RJ1---RJ11
char atch_adtemp[16];				//4051处16路ATCH的AD level，RJ1--RJ13----电池 按键1 2
char RJ_adtemp[11];					//RJ1--RJ11 AD脚的AD level
char main_idnum[13];
char son_idnum[13];
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



/*
*********************************************************************************************************
*	函 数 名: release_jtagswd
*	功能说明: 释放JTAG/SWD 
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
void release_jtagswd(void)
{
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
 //GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);					//都使能
 //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		//JTAG-DP失能+ SW-DP使能
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);				//SWJ完全失能（ JTAG+SW-DP）	
}
/*******************************************************************************
* Function Name  : ioxset  				单个IO模式设置
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void ioxset(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx ,GPIOMode_TypeDef GPIO_Modex)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 IO端口时钟
	if(GPIOx==GPIOA)
		RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA, ENABLE );
	if(GPIOx==GPIOB)
		RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB, ENABLE );
	if(GPIOx==GPIOC)
		RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOC, ENABLE );
		
	// 配置 IO 引脚模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pinx;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;				//加这句时S4-S0切换异常
	GPIO_InitStructure.GPIO_Mode = GPIO_Modex;
	// 初始化  IO
	GPIO_Init(GPIOx, &GPIO_InitStructure);		
}



/*******************************************************************************
* Function Name  : adcconfig  				所有AD口初始化配置
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void adcconfig()
{
	
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	ioxset(rj1adc,rj1adcpin,GPIO_Mode_AIN);	//AD口IO设置   RJ1--RJ11--AD4
	ioxset(rj2adc,rj2adcpin,GPIO_Mode_AIN);	
	ioxset(rj3adc,rj3adcpin,GPIO_Mode_AIN);	
	ioxset(rj4adc,rj4adcpin,GPIO_Mode_AIN);	
	ioxset(rj5adc,rj5adcpin,GPIO_Mode_AIN);	
	ioxset(rj6adc,rj6adcpin,GPIO_Mode_AIN);	
	ioxset(rj7adc,rj7adcpin,GPIO_Mode_AIN);	
	ioxset(rj8adc,rj8adcpin,GPIO_Mode_AIN);	
	ioxset(rj9adc,rj9adcpin,GPIO_Mode_AIN);	
	ioxset(rj10adc,rj10adcpin,GPIO_Mode_AIN);	
	ioxset(rj11adc,rj11adcpin,GPIO_Mode_AIN);
	ioxset(atchadc,atchadcpin,GPIO_Mode_AIN);
	
	// 打开DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	// 打开ADC时钟
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_ADC1, ENABLE );
	// 复位DMA控制器
	DMA_DeInit(DMA1_Channel1);
	
	// 配置 DMA 初始化结构体
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC1->DR ) );
	// 存储器地址，实际上就是一个内部SRAM的变量
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;	
	// 数据源来自外设
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	
	// 缓冲区大小，应该等于数据目的地的大小
	DMA_InitStructure.DMA_BufferSize = macNOFCHANEL;	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// 存储器地址递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	// 外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	// 内存数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	// 禁止存储器到存储器模式，因为是从外设到存储器
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	
	// 初始化DMA
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	// 使能 DMA 通道
	DMA_Cmd(DMA1_Channel1 , ENABLE);
	
	// ADC 模式配置
	// 只使用一个ADC，属于单模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	// 扫描模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 
	// 连续转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	// 转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	// 转换通道个数
	ADC_InitStructure.ADC_NbrOfChannel = macNOFCHANEL;			
	// 初始化ADC
	ADC_Init(ADC1, &ADC_InitStructure);	
	// 配置ADC时钟Ｎ狿CLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 	
	// 配置ADC 通道的转换顺序和采样时间
	ADC_RegularChannelConfig(ADC1, rj1adcch,  1, ADC_SampleTime_55Cycles5);	//RJ1
	ADC_RegularChannelConfig(ADC1, rj2adcch,  2, ADC_SampleTime_55Cycles5);	//RJ2
	ADC_RegularChannelConfig(ADC1, rj3adcch,  3, ADC_SampleTime_55Cycles5);	//RJ3
	ADC_RegularChannelConfig(ADC1, rj4adcch,  4, ADC_SampleTime_55Cycles5);	//RJ4
	ADC_RegularChannelConfig(ADC1, rj5adcch,  5, ADC_SampleTime_55Cycles5);	//RJ5
	ADC_RegularChannelConfig(ADC1, rj6adcch,  6, ADC_SampleTime_55Cycles5);	//RJ6
	ADC_RegularChannelConfig(ADC1, rj7adcch,  7, ADC_SampleTime_55Cycles5);	//RJ7
	ADC_RegularChannelConfig(ADC1, rj8adcch,  8, ADC_SampleTime_55Cycles5);	//RJ8
	ADC_RegularChannelConfig(ADC1, rj9adcch,  9, ADC_SampleTime_55Cycles5);	//RJ9
	ADC_RegularChannelConfig(ADC1, rj10adcch, 10,ADC_SampleTime_55Cycles5);	//RJ10
	ADC_RegularChannelConfig(ADC1, rj11adcch, 11,ADC_SampleTime_55Cycles5);	//RJ11
	ADC_RegularChannelConfig(ADC1, atchadcch, 12,ADC_SampleTime_55Cycles5);	//ATCH AD4
	// 使能ADC DMA 请求
	ADC_DMACmd(ADC1, ENABLE);	
	// 开启ADC ，并开始转换
	ADC_Cmd(ADC1, ENABLE);
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADC1);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADC1));	
	// ADC开始校准
	ADC_StartCalibration(ADC1);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC1));	
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
}



/*******************************************************************************
* Function Name  : s4s0_set  				74HC4051  S4-S0 IO切换
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void s4s0_set(char num)
{
	/////////////////////////////////////S4----S0  IO输出
	if(num==0)
	{
	 GPIO_SetBits(atchs4,atchs4pin);			//////////10010  RJ1
	 GPIO_ResetBits(atchs3,atchs3pin);
	 GPIO_ResetBits(atchs2,atchs2pin);
	 GPIO_SetBits(atchs1,atchs1pin);
	 GPIO_ResetBits(atchs0,atchs0pin);
	}
	else if(num==1)
	{
	 GPIO_SetBits(atchs4,atchs4pin);			//////////10110  RJ2
	 GPIO_ResetBits(atchs3,atchs3pin);
	 GPIO_SetBits(atchs2,atchs2pin);
	 GPIO_SetBits(atchs1,atchs1pin);
	 GPIO_ResetBits(atchs0,atchs0pin);
	}
	else if(num==2)
	{
	 GPIO_SetBits(atchs4,atchs4pin);			//////////10100  RJ3
	 GPIO_ResetBits(atchs3,atchs3pin);
	 GPIO_SetBits(atchs2,atchs2pin);
	 GPIO_ResetBits(atchs1,atchs1pin);
	 GPIO_ResetBits(atchs0,atchs0pin);
	}
	else if(num==3)
	{
	 GPIO_SetBits(atchs4,atchs4pin);			//////////10111  RJ4
	 GPIO_ResetBits(atchs3,atchs3pin);
	 GPIO_SetBits(atchs2,atchs2pin);
	 GPIO_SetBits(atchs1,atchs1pin);
	 GPIO_SetBits(atchs0,atchs0pin);
	}
	else if(num==4)
	{
	 GPIO_SetBits(atchs4,atchs4pin);			//////////10101  RJ5
	 GPIO_ResetBits(atchs3,atchs3pin);
	 GPIO_SetBits(atchs2,atchs2pin);
	 GPIO_ResetBits(atchs1,atchs1pin);
	 GPIO_SetBits(atchs0,atchs0pin);
	}
	else if(num==5)
	{
	 GPIO_ResetBits(atchs4,atchs4pin);			//////////01011  RJ6
	 GPIO_SetBits(atchs3,atchs3pin);
	 GPIO_ResetBits(atchs2,atchs2pin);
	 GPIO_SetBits(atchs1,atchs1pin);
	 GPIO_SetBits(atchs0,atchs0pin);
	}
	else if(num==6)
	{
	 GPIO_ResetBits(atchs4,atchs4pin);			//////////01000  RJ7
	 GPIO_SetBits(atchs3,atchs3pin);
	 GPIO_ResetBits(atchs2,atchs2pin);
	 GPIO_ResetBits(atchs1,atchs1pin);
	 GPIO_ResetBits(atchs0,atchs0pin);
	}
	else if(num==7)
	{
	 GPIO_ResetBits(atchs4,atchs4pin);			//////////01001  RJ8
	 GPIO_SetBits(atchs3,atchs3pin);
	 GPIO_ResetBits(atchs2,atchs2pin);
	 GPIO_ResetBits(atchs1,atchs1pin);
	 GPIO_SetBits(atchs0,atchs0pin);
	}
	else if(num==8)
	{
	 GPIO_ResetBits(atchs4,atchs4pin);			//////////01010  RJ9
	 GPIO_SetBits(atchs3,atchs3pin);
	 GPIO_ResetBits(atchs2,atchs2pin);
	 GPIO_SetBits(atchs1,atchs1pin);
	 GPIO_ResetBits(atchs0,atchs0pin);
	}
	else if(num==9)
	{
	 GPIO_ResetBits(atchs4,atchs4pin);			//////////01100  RJ10
	 GPIO_SetBits(atchs3,atchs3pin);
	 GPIO_SetBits(atchs2,atchs2pin);
	 GPIO_ResetBits(atchs1,atchs1pin);
	 GPIO_ResetBits(atchs0,atchs0pin);
	}
	else if(num==10)
	{
	 GPIO_ResetBits(atchs4,atchs4pin);			//////////01110  RJ11
	 GPIO_SetBits(atchs3,atchs3pin);
	 GPIO_SetBits(atchs2,atchs2pin);
	 GPIO_SetBits(atchs1,atchs1pin);
	 GPIO_ResetBits(atchs0,atchs0pin);
	}
	else if(num==11)
	{
	 GPIO_ResetBits(atchs4,atchs4pin);			//////////01111  RJ12
	 GPIO_SetBits(atchs3,atchs3pin);
	 GPIO_SetBits(atchs2,atchs2pin);
	 GPIO_SetBits(atchs1,atchs1pin);
	 GPIO_SetBits(atchs0,atchs0pin);
	}
	else if(num==12)
	{
	 GPIO_ResetBits(atchs4,atchs4pin);			//////////01101  RJ13
	 GPIO_SetBits(atchs3,atchs3pin);
	 GPIO_SetBits(atchs2,atchs2pin);
	 GPIO_ResetBits(atchs1,atchs1pin);
	 GPIO_SetBits(atchs0,atchs0pin);
	}
	else if(num==13)
	{
	 GPIO_SetBits(atchs4,atchs4pin);			//////////10001  14电压采集
	 GPIO_ResetBits(atchs3,atchs3pin);
	 GPIO_ResetBits(atchs2,atchs2pin);
	 GPIO_ResetBits(atchs1,atchs1pin);
	 GPIO_SetBits(atchs0,atchs0pin);
	}
	else if(num==14)
	{
	 GPIO_SetBits(atchs4,atchs4pin);			//////////10011  15按键1
	 GPIO_ResetBits(atchs3,atchs3pin);
	 GPIO_ResetBits(atchs2,atchs2pin);
	 GPIO_SetBits(atchs1,atchs1pin);
	 GPIO_SetBits(atchs0,atchs0pin);
	}
	else if(num==15)
	{
	 GPIO_SetBits(atchs4,atchs4pin);			//////////10000  16按键2
	 GPIO_ResetBits(atchs3,atchs3pin);
	 GPIO_ResetBits(atchs2,atchs2pin);
	 GPIO_ResetBits(atchs1,atchs1pin);
	 GPIO_ResetBits(atchs0,atchs0pin);
	}
	else
	{
		GPIO_SetBits(atchs4,atchs4pin);
		GPIO_SetBits(atchs3,atchs3pin);
	}
	delay(0xfff);				//稍稍延时，避免IO切换时AD采集不稳
	
}



/*******************************************************************************
* Function Name  : adc_level  				初始化获取所以AD口的AD_level
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void adc_level()	
{
	float temp,data;
	char i;
	////////////////////////////////////////////////ADC初始化
	adcconfig();
	
	{
		GPIO_InitTypeDef GPIO_InitStructure;	
		// 打开 IO端口时钟
		RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );		
		// 配置 IO 引脚模式
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;				//加这句时S4-S0切换异常
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);		
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;				//加这句时S4-S0切换异常
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);			
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;				//加这句时S4-S0切换异常
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
	}
	
	for(i=0;i<16;i++)
	{
	 s4s0_set(i);															//////////////////////////////////////////////////////////////////////////////////
	 temp=(float) ADC_ConvertedValue[11]/4096*3.3/100*151;				//在ad采集序列数组中排第12个
	 if(temp>3.5)	atch_adtemp[i]=3;
	 if(temp>2.2&temp<3.3)	atch_adtemp[i]=2;
	 if(temp>1.1&temp<2.0)	atch_adtemp[i]=1;
	 if(temp<1.0)	atch_adtemp[i]=0;
	}
	
	for(i=0;i<11;i++)
	{
	 data=(float) ADC_ConvertedValue[i]/4096*3.3;
	 if(data>2.2)	RJ_adtemp[i]=2;
	 if(data>1.1&data<2.0)	RJ_adtemp[i]=1;
	 if(data<1.0)	RJ_adtemp[i]=0;
	}

}


/*******************************************************************************
* Function Name  : adcx_ioset  			PWM IO设置
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void adcx_ioset(ADC_TypeDef* ADCx, u16 ADC_Channel_x)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_TypeDef* GPIOx;
	uint16_t pinx;
	/* 设置TIM3CLK 为 72MHZ */
	if(ADC_Channel_x==rj1adcch)
		{
			GPIOx=rj1adc;
			pinx=rj1adcpin;
		}
	if(ADC_Channel_x==rj2adcch)
		{
			GPIOx=rj2adc;
			pinx=rj2adcpin;
		}
	if(ADC_Channel_x==rj3adcch)
		{
			GPIOx=rj3adc;
			pinx=rj3adcpin;
		}
	if(ADC_Channel_x==rj4adcch)
		{
			GPIOx=rj4adc;
			pinx=rj4adcpin;
		}
	if(ADC_Channel_x==rj5adcch)
		{
			GPIOx=rj5adc;
			pinx=rj5adcpin;
		}
	if(ADC_Channel_x==rj6adcch)
		{
			GPIOx=rj6adc;
			pinx=rj6adcpin;
		}
	if(ADC_Channel_x==rj7adcch)
		{
			GPIOx=rj7adc;
			pinx=rj7adcpin;
		}
	if(ADC_Channel_x==rj8adcch)
		{
			GPIOx=rj8adc;
			pinx=rj8adcpin;
		}
	if(ADC_Channel_x==rj9adcch)
		{
			GPIOx=rj9adc;
			pinx=rj9adcpin;
		}
	if(ADC_Channel_x==rj10adcch)
		{
			GPIOx=rj10adc;
			pinx=rj10adcpin;
		}
	if(ADC_Channel_x==rj11adcch)
		{
			GPIOx=rj11adc;
			pinx=rj11adcpin;
		}
		
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = pinx;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    

	GPIO_Init(GPIOx, &GPIO_InitStructure);
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
  
	adcx_ioset(ADCx, adc_channel);
	
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
  ADC_RegularChannelConfig(ADCx, adc_channel, 1, ADC_SampleTime_28Cycles5);
  
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


void timxset()
{
	
	
}


/*******************************************************************************
* Function Name  : pwmx_ioset  			PWM IO设置
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void pwmx_ioset(TIM_TypeDef* TIMx, u16 TIM_Channel_x)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_TypeDef* GPIOx;
	uint16_t pinx;
	/* 设置TIM3CLK 为 72MHZ */
	if(TIMx==TIM1 & TIM_Channel_x==TIM_Channel_1)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
			GPIOx=rj1tim;
			pinx=rj1timpin;
		}
	if(TIMx==TIM1 & TIM_Channel_x==TIM_Channel_2)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
			GPIOx=rj2tim;
			pinx=rj2timpin;
		}
	if(TIMx==TIM1 & TIM_Channel_x==TIM_Channel_3)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
			GPIOx=rj3tim;
			pinx=rj3timpin;
		}
	if(TIMx==TIM1 & TIM_Channel_x==TIM_Channel_4)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
			GPIOx=rj4tim;
			pinx=rj4timpin;
		}
	if(TIMx==TIM4 & TIM_Channel_x==TIM_Channel_1)
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
			GPIOx=rj5tim;
			pinx=rj5timpin;
		}
	if(TIMx==TIM4 & TIM_Channel_x==TIM_Channel_2)
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
			GPIOx=rj6tim;
			pinx=rj6timpin;
		}
	if(TIMx==TIM4 & TIM_Channel_x==TIM_Channel_3)
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
			GPIOx=rj7tim;
			pinx=rj7timpin;
		}
	if(TIMx==TIM8 & TIM_Channel_x==TIM_Channel_1)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 
			GPIOx=rj8tim;
			pinx=rj8timpin;
		}
	if(TIMx==TIM8 & TIM_Channel_x==TIM_Channel_2)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 
			GPIOx=rj9tim;
			pinx=rj9timpin;
		}
	if(TIMx==TIM8 & TIM_Channel_x==TIM_Channel_3)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 
			GPIOx=rj10tim;
			pinx=rj10timpin;
		}
	if(TIMx==TIM8 & TIM_Channel_x==TIM_Channel_4)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 
			GPIOx=rj11tim;
			pinx=rj11timpin;
		}
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = pinx;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
	GPIO_Init(GPIOx, &GPIO_InitStructure);
	
}
/*******************************************************************************
* Function Name  : pwmxset  			PWM输出
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void pwmxset(TIM_TypeDef* TIMx, u16 TIM_Channel_x, u16 fre , u16 pulse)
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	pwmx_ioset(TIMx,TIM_Channel_x);
	
  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period =(1000000/fre) - 1;		//当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 71;	    //设置预分频：不预分频，即为72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频(这里用不到)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = pulse*(10000/fre);	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
  
	if(TIM_Channel_x==TIM_Channel_1)
	{
		TIM_OC1Init(TIMx, &TIM_OCInitStructure);	 //使能通道1
		TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	if(TIM_Channel_x==TIM_Channel_2)
	{
		TIM_OC2Init(TIMx, &TIM_OCInitStructure);	 //使能通道2
		TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	if(TIM_Channel_x==TIM_Channel_3)
	{
		TIM_OC3Init(TIMx, &TIM_OCInitStructure);	 //使能通道3
		TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	if(TIM_Channel_x==TIM_Channel_4)
	{
		TIM_OC4Init(TIMx, &TIM_OCInitStructure);	 //使能通道4
		TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}

  /* TIMx enable counter */
  TIM_Cmd(TIMx, ENABLE);                   //使能定时器
	
	if(TIMx==TIM8)
	TIM_CtrlPWMOutputs(TIMx,ENABLE);					//高级定时器必须加这句才能有PWM
	if(TIMx==TIM1)
	TIM_CtrlPWMOutputs(TIMx,ENABLE);
}




/*******************************************************************************
* Function Name  : scanport  				扫描各个端口
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void scanport(void)
{
	char i;
///////////////////////////////////////////////////////////////OUT TIM  IO判断
////	上拉模式下判断													////////////////
//////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////OUT口IO引脚模式设置——输入上拉////
	ioxset(rj1out , rj1outpin ,GPIO_Mode_IPU);		//PA12	RJ1 	out5
	ioxset(rj2out , rj2outpin ,GPIO_Mode_IPU);		//PA13	RJ2 	out3
	ioxset(rj3out , rj3outpin ,GPIO_Mode_IPU);		//PB3		RJ3 	out8
	ioxset(rj4out , rj4outpin ,GPIO_Mode_IPU);		//PB4		RJ4 	out2
	ioxset(rj5out , rj5outpin ,GPIO_Mode_IPU);		//PB5		RJ5 	out1
	ioxset(rj6out , rj6outpin ,GPIO_Mode_IPU);		//PB9		RJ6 	out4
	ioxset(rj7out , rj7outpin ,GPIO_Mode_IPU);		//PA6		RJ7 	out9
	ioxset(rj8out , rj8outpin ,GPIO_Mode_IPU);		//PA7		RJ8 	out10
	ioxset(rj9out , rj9outpin ,GPIO_Mode_IPU);		//PB0		RJ9 	out11
	ioxset(rj10out, rj10outpin,GPIO_Mode_IPU);		//PB1		RJ10 	out7
	ioxset(rj11out, rj11outpin,GPIO_Mode_IPU);		//PB12	RJ11 	out6
																							//			RJ12
	///////////////////////////////////////////////////////////////////TIM口IO引脚模式设置——输入上拉////
	ioxset(rj1tim ,rj1timpin ,GPIO_Mode_IPU);		//PA8		RJ1 	TIM1_CH1
	ioxset(rj2tim ,rj2timpin ,GPIO_Mode_IPU);		//PA9		RJ2 	TIM1_CH2
	ioxset(rj3tim ,rj3timpin ,GPIO_Mode_IPU);		//PA10	RJ3 	TIM1_CH3
	ioxset(rj4tim ,rj4timpin ,GPIO_Mode_IPU);		//PA11	RJ4 	TIM1_CH4
	ioxset(rj5tim ,rj5timpin ,GPIO_Mode_IPU);		//PB6		RJ5 	TIM4_CH1
	ioxset(rj6tim ,rj6timpin ,GPIO_Mode_IPU);		//PB7		RJ6 	TIM4_CH2
	ioxset(rj7tim ,rj7timpin ,GPIO_Mode_IPU);		//PB8		RJ7 	TIM4_CH3
	ioxset(rj8tim ,rj8timpin ,GPIO_Mode_IPU);		//PC6		RJ8 	TIM8_CH1
	ioxset(rj9tim ,rj9timpin ,GPIO_Mode_IPU);		//PC7		RJ9 	TIM8_CH2
	ioxset(rj10tim,rj10timpin,GPIO_Mode_IPU);		//PC8		RJ10 	TIM8_CH3
	ioxset(rj11tim,rj11timpin,GPIO_Mode_IPU);		//PC9		RJ11 	TIM8_CH4
																							//			RJ12																						
	/////////////////////////////////////////////////////////////读取OUT口状态
	outIOIPU_state[0]=GPIO_ReadInputDataBit( rj1out ,rj1outpin);
	outIOIPU_state[1]=GPIO_ReadInputDataBit( rj2out ,rj2outpin);
	outIOIPU_state[2]=GPIO_ReadInputDataBit( rj3out ,rj3outpin);
	outIOIPU_state[3]=GPIO_ReadInputDataBit( rj4out ,rj4outpin);
	outIOIPU_state[4]=GPIO_ReadInputDataBit( rj5out ,rj5outpin);
	outIOIPU_state[5]=GPIO_ReadInputDataBit( rj6out ,rj6outpin);
	outIOIPU_state[6]=GPIO_ReadInputDataBit( rj7out ,rj7outpin);
	outIOIPU_state[7]=GPIO_ReadInputDataBit( rj8out ,rj8outpin);
	outIOIPU_state[8]=GPIO_ReadInputDataBit( rj9out ,rj9outpin);
	outIOIPU_state[9]=GPIO_ReadInputDataBit( rj10out ,rj10outpin);
	outIOIPU_state[10]=GPIO_ReadInputDataBit( rj11out ,rj11outpin);
	
	/////////////////////////////////////////////////////////////读取TIM口状态
	timIOIPU_state[0]=GPIO_ReadInputDataBit( rj1tim ,rj1timpin);
	timIOIPU_state[1]=GPIO_ReadInputDataBit( rj2tim ,rj2timpin);
	timIOIPU_state[2]=GPIO_ReadInputDataBit( rj3tim ,rj3timpin);
	timIOIPU_state[3]=GPIO_ReadInputDataBit( rj4tim ,rj4timpin);
	timIOIPU_state[4]=GPIO_ReadInputDataBit( rj5tim ,rj5timpin);
	timIOIPU_state[5]=GPIO_ReadInputDataBit( rj6tim ,rj6timpin);
	timIOIPU_state[6]=GPIO_ReadInputDataBit( rj7tim ,rj7timpin);
	timIOIPU_state[7]=GPIO_ReadInputDataBit( rj8tim ,rj8timpin);
	timIOIPU_state[8]=GPIO_ReadInputDataBit( rj9tim ,rj9timpin);
	timIOIPU_state[9]=GPIO_ReadInputDataBit( rj10tim ,rj10timpin);
	timIOIPU_state[10]=GPIO_ReadInputDataBit( rj11tim ,rj11timpin);
	
///////////////////////////////////////////////////////////////
////	下拉模式下判断													////////////////
//////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////OUT口IO引脚模式设置——输入下拉////
	ioxset(rj1out , rj1outpin ,GPIO_Mode_IPD);		//PA12	RJ1 	out5
	ioxset(rj2out , rj2outpin ,GPIO_Mode_IPD);		//PA13	RJ2 	out3
	ioxset(rj3out , rj3outpin ,GPIO_Mode_IPD);		//PB3		RJ3 	out8
	ioxset(rj4out , rj4outpin ,GPIO_Mode_IPD);		//PB4		RJ4 	out2
	ioxset(rj5out , rj5outpin ,GPIO_Mode_IPD);		//PB5		RJ5 	out1
	ioxset(rj6out , rj6outpin ,GPIO_Mode_IPD);		//PB9		RJ6 	out4
	ioxset(rj7out , rj7outpin ,GPIO_Mode_IPD);		//PA6		RJ7 	out9
	ioxset(rj8out , rj8outpin ,GPIO_Mode_IPD);		//PA7		RJ8 	out10
	ioxset(rj9out , rj9outpin ,GPIO_Mode_IPD);		//PB0		RJ9 	out11
	ioxset(rj10out, rj10outpin,GPIO_Mode_IPD);		//PB1		RJ10 	out7
	ioxset(rj11out, rj11outpin,GPIO_Mode_IPD);		//PB12	RJ11 	out6
																							//			RJ12
	////////////////////////////////////////////////////////////////////TIM口IO引脚模式设置——输入上拉////
	ioxset(rj1tim ,rj1timpin ,GPIO_Mode_IPD);		//PA8		RJ1 	TIM1_CH1
	ioxset(rj2tim ,rj2timpin ,GPIO_Mode_IPD);		//PA9		RJ2 	TIM1_CH2
	ioxset(rj3tim ,rj3timpin ,GPIO_Mode_IPD);		//PA10	RJ3 	TIM1_CH3
	ioxset(rj4tim ,rj4timpin ,GPIO_Mode_IPD);		//PA11	RJ4 	TIM1_CH4
	ioxset(rj5tim ,rj5timpin ,GPIO_Mode_IPD);		//PB6		RJ5 	TIM4_CH1
	ioxset(rj6tim ,rj6timpin ,GPIO_Mode_IPD);		//PB7		RJ6 	TIM4_CH2
	ioxset(rj7tim ,rj7timpin ,GPIO_Mode_IPD);		//PB8		RJ7 	TIM4_CH3
	ioxset(rj8tim ,rj8timpin ,GPIO_Mode_IPD);		//PC6		RJ8 	TIM8_CH1
	ioxset(rj9tim ,rj9timpin ,GPIO_Mode_IPD);		//PC7		RJ9 	TIM8_CH2
	ioxset(rj10tim,rj10timpin,GPIO_Mode_IPD);		//PC8		RJ10 	TIM8_CH3
	ioxset(rj11tim,rj11timpin,GPIO_Mode_IPD);		//PC9		RJ11 	TIM8_CH4
																							//			RJ12
	
	/////////////////////////////////////////////////////////////读取OUT口状态
	outIOIPD_state[0]=GPIO_ReadInputDataBit( rj1out ,rj1outpin);
	outIOIPD_state[1]=GPIO_ReadInputDataBit( rj2out ,rj2outpin);
	outIOIPD_state[2]=GPIO_ReadInputDataBit( rj3out ,rj3outpin);
	outIOIPD_state[3]=GPIO_ReadInputDataBit( rj4out ,rj4outpin);
	outIOIPD_state[4]=GPIO_ReadInputDataBit( rj5out ,rj5outpin);
	outIOIPD_state[5]=GPIO_ReadInputDataBit( rj6out ,rj6outpin);
	outIOIPD_state[6]=GPIO_ReadInputDataBit( rj7out ,rj7outpin);
	outIOIPD_state[7]=GPIO_ReadInputDataBit( rj8out ,rj8outpin);
	outIOIPD_state[8]=GPIO_ReadInputDataBit( rj9out ,rj9outpin);
	outIOIPD_state[9]=GPIO_ReadInputDataBit( rj10out ,rj10outpin);
	outIOIPD_state[10]=GPIO_ReadInputDataBit( rj11out ,rj11outpin);
	
	/////////////////////////////////////////////////////////////读取TIM口状态
	timIOIPD_state[0]=GPIO_ReadInputDataBit( rj1tim ,rj1timpin);
	timIOIPD_state[1]=GPIO_ReadInputDataBit( rj2tim ,rj2timpin);
	timIOIPD_state[2]=GPIO_ReadInputDataBit( rj3tim ,rj3timpin);
	timIOIPD_state[3]=GPIO_ReadInputDataBit( rj4tim ,rj4timpin);
	timIOIPD_state[4]=GPIO_ReadInputDataBit( rj5tim ,rj5timpin);
	timIOIPD_state[5]=GPIO_ReadInputDataBit( rj6tim ,rj6timpin);
	timIOIPD_state[6]=GPIO_ReadInputDataBit( rj7tim ,rj7timpin);
	timIOIPD_state[7]=GPIO_ReadInputDataBit( rj8tim ,rj8timpin);
	timIOIPD_state[8]=GPIO_ReadInputDataBit( rj9tim ,rj9timpin);
	timIOIPD_state[9]=GPIO_ReadInputDataBit( rj10tim ,rj10timpin);
	timIOIPD_state[10]=GPIO_ReadInputDataBit( rj11tim ,rj11timpin);

	adc_level();			//////////////////获取所以AD的level值
	
	for(i=0;i<11;i++)
	{
		if(outIOIPU_state[i]==outIOIPD_state[i] & timIOIPU_state[i]==timIOIPD_state[i])		//有接模块 OUTx      	TIM_CHx  			Adx 
			{	
			 if(outIOIPU_state[i]==0 &timIOIPU_state[i]==0 &RJ_adtemp[i]==0 &atch_adtemp[i]==0)			
			  { main_idnum[i]=1;  son_idnum[i]=0;}
				
			 if(outIOIPU_state[i]==0 &timIOIPU_state[i]==0 &RJ_adtemp[i]==0 &atch_adtemp[i]==1)		
			  { main_idnum[i]=2;  son_idnum[i]=0;}
				
			 if(outIOIPU_state[i]==0 &timIOIPU_state[i]==0 &RJ_adtemp[i]==0 &atch_adtemp[i]==2)		
			  { main_idnum[i]=3;  son_idnum[i]=0;}
			 
			 if(outIOIPU_state[i]==0 &timIOIPU_state[i]==0 &RJ_adtemp[i]==1 &atch_adtemp[i]==0)		
			  { main_idnum[i]=4;  son_idnum[i]=0;}
				
			 if(outIOIPU_state[i]==0 &timIOIPU_state[i]==0 &RJ_adtemp[i]==1 &atch_adtemp[i]==1)		
				{ main_idnum[i]=5;  son_idnum[i]=0;}
			 
			 if(outIOIPU_state[i]==0 &timIOIPU_state[i]==0 &RJ_adtemp[i]==1 &atch_adtemp[i]==2)		
			  { main_idnum[i]=6;  son_idnum[i]=0;}
				
			 if(outIOIPU_state[i]==0 &timIOIPU_state[i]==0 &RJ_adtemp[i]==2 &atch_adtemp[i]==0)			
			  { main_idnum[i]=7;  son_idnum[i]=0;}
				
			 if(outIOIPU_state[i]==0 &timIOIPU_state[i]==0 &RJ_adtemp[i]==2 &atch_adtemp[i]==1)		
			  { main_idnum[i]=7;  son_idnum[i]=0;}/////////////////////////////////////////////////////88888888888888//////////////
				
			 if(outIOIPU_state[i]==0 &timIOIPU_state[i]==0 &RJ_adtemp[i]==2 &atch_adtemp[i]==2)		
			  { main_idnum[i]=9;  son_idnum[i]=0;}
																																													/////////////
			 if(outIOIPU_state[i]==1 &timIOIPU_state[i]==1 &RJ_adtemp[i]==0 &atch_adtemp[i]==1)	 
			  { main_idnum[i]=10;  son_idnum[i]=0;}
				
			 if(outIOIPU_state[i]==1 &timIOIPU_state[i]==1 &RJ_adtemp[i]==0 &atch_adtemp[i]==2)		
			  { main_idnum[i]=11;  son_idnum[i]=0;}
				
			 if(outIOIPU_state[i]==1 &timIOIPU_state[i]==1 &RJ_adtemp[i]==1 &atch_adtemp[i]==1)		
			  { main_idnum[i]=12;  son_idnum[i]=0;}
				
			 if(outIOIPU_state[i]==1 &timIOIPU_state[i]==1 &RJ_adtemp[i]==1 &atch_adtemp[i]==2)		
			  { main_idnum[i]=13;  son_idnum[i]=0;}
				
			 if(outIOIPU_state[i]==1 &timIOIPU_state[i]==1 &RJ_adtemp[i]==2 &atch_adtemp[i]==1)		
			  { main_idnum[i]=14;  son_idnum[i]=0;}
				
			 if(outIOIPU_state[i]==1 &timIOIPU_state[i]==1 &RJ_adtemp[i]==2 &atch_adtemp[i]==2)		
			  { main_idnum[i]=15;  son_idnum[i]=0;}
																																													/////////////
			 if(outIOIPU_state[i]==1 &timIOIPU_state[i]==0 &atch_adtemp[i]==0)			
			  { main_idnum[i]=16;  son_idnum[i]=0;}	
				
			 if(outIOIPU_state[i]==0 &timIOIPU_state[i]==1 &atch_adtemp[i]==0)			
			  { main_idnum[i]=17;  son_idnum[i]=0;}
				
			 if(outIOIPU_state[i]==0 &timIOIPU_state[i]==1 &atch_adtemp[i]==1)			
			  { main_idnum[i]=18;  son_idnum[i]=0;}
			
			 if(outIOIPU_state[i]==0 &timIOIPU_state[i]==1 &atch_adtemp[i]==2)			
			  { main_idnum[i]=19;  son_idnum[i]=0;}
				
			 if(outIOIPU_state[i]==1 &timIOIPU_state[i]==0 &atch_adtemp[i]==0)			
			  { main_idnum[i]=20;  son_idnum[i]=0;}
				
			 if(outIOIPU_state[i]==1 &timIOIPU_state[i]==0 &atch_adtemp[i]==1)		
			  { main_idnum[i]=21;  son_idnum[i]=0;}
				
			 if(outIOIPU_state[i]==1 &timIOIPU_state[i]==0 &atch_adtemp[i]==2)			
			  { main_idnum[i]=22;  son_idnum[i]=0;}									
			}
		else {	main_idnum[i]=0;	son_idnum[i]=0;}
		
	}
	
	if(atch_adtemp[11]==3)													//是否有接485
		{ main_idnum[11]=23;  son_idnum[11]=0;}				
	else { main_idnum[11]=0;	son_idnum[11]=0; }	
			
  if(atch_adtemp[12]==3)											
			  { main_idnum[12]=24;  son_idnum[12]=0;}		//是否有接串口
	else { main_idnum[12]=0;	son_idnum[12]=0; }
		
}


/*******************************************************************************
* Function Name  : setport  				扫描各个端口
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void setport(void)
{
	GPIO_TypeDef* xout;
	GPIO_TypeDef* xtim;
	GPIO_TypeDef* xadc;	
	uint16_t xoutpin;
	uint16_t xtimpin;
	uint16_t xadcpin;
	TIM_TypeDef* TIMx;
	ADC_TypeDef* ADCx;
	u16 TIM_Channel_x;
	u16 ADC_Channel_x;
	char i,idnum;
	for(i=0;i<11;i++)
	{
		if(i==0){	xout=rj1out;	xoutpin=rj1outpin;	xtim=rj1tim;	xtimpin=rj1timpin;	xadc=rj1adc;	xadcpin=rj1adcpin;
							ADCx=ADC1;	ADC_Channel_x=rj1adcch;	TIMx=TIM1;		TIM_Channel_x=TIM_Channel_1; }
		
		if(i==1){	xout=rj2out;	xoutpin=rj2outpin;	xtim=rj2tim;	xtimpin=rj2timpin;	xadc=rj2adc;	xadcpin=rj2adcpin;
							ADCx=ADC1;	ADC_Channel_x=rj2adcch;	TIMx=TIM1;		TIM_Channel_x=TIM_Channel_2; }
		
		if(i==2){	xout=rj3out;	xoutpin=rj3outpin;	xtim=rj3tim;	xtimpin=rj3timpin;	xadc=rj3adc;	xadcpin=rj3adcpin;
							ADCx=ADC1;	ADC_Channel_x=rj3adcch;	TIMx=TIM1;		TIM_Channel_x=TIM_Channel_3; }
		
		if(i==3){	xout=rj4out;	xoutpin=rj4outpin;	xtim=rj4tim;	xtimpin=rj4timpin;	xadc=rj4adc;	xadcpin=rj4adcpin;
							ADCx=ADC1;	ADC_Channel_x=rj4adcch;	TIMx=TIM1;		TIM_Channel_x=TIM_Channel_4; }
		
		if(i==4){	xout=rj5out;	xoutpin=rj5outpin;	xtim=rj5tim;	xtimpin=rj5timpin;	xadc=rj5adc;	xadcpin=rj5adcpin;
							ADCx=ADC1;	ADC_Channel_x=rj5adcch;	TIMx=TIM4;		TIM_Channel_x=TIM_Channel_1; }
		
		if(i==5){	xout=rj6out;	xoutpin=rj6outpin;	xtim=rj6tim;	xtimpin=rj6timpin;	xadc=rj6adc;	xadcpin=rj6adcpin;
							ADCx=ADC1;	ADC_Channel_x=rj6adcch;	TIMx=TIM4;		TIM_Channel_x=TIM_Channel_2; }
		
		if(i==6){	xout=rj7out;	xoutpin=rj7outpin;	xtim=rj7tim;	xtimpin=rj7timpin;	xadc=rj7adc;	xadcpin=rj7adcpin;
							ADCx=ADC1;	ADC_Channel_x=rj7adcch;	TIMx=TIM4;		TIM_Channel_x=TIM_Channel_3; }
		
		if(i==7){	xout=rj8out;	xoutpin=rj8outpin;	xtim=rj8tim;	xtimpin=rj8timpin;	xadc=rj8adc;	xadcpin=rj8adcpin;
							ADCx=ADC1;	ADC_Channel_x=rj8adcch;	TIMx=TIM8;		TIM_Channel_x=TIM_Channel_1; }
		
		if(i==8){	xout=rj9out;	xoutpin=rj9outpin;	xtim=rj9tim;	xtimpin=rj9timpin;	xadc=rj9adc;	xadcpin=rj9adcpin;
							ADCx=ADC1;	ADC_Channel_x=rj9adcch;	TIMx=TIM8;		TIM_Channel_x=TIM_Channel_2; }
		
		if(i==9){xout=rj10out;	xoutpin=rj10outpin;	xtim=rj10tim;	xtimpin=rj10timpin;	xadc=rj10adc;	xadcpin=rj10adcpin;
							ADCx=ADC1;	ADC_Channel_x=rj10adcch;	TIMx=TIM8;		TIM_Channel_x=TIM_Channel_3; }
		
		if(i==10){xout=rj11out;	xoutpin=rj11outpin;	xtim=rj11tim;	xtimpin=rj11timpin;	xadc=rj11adc;	xadcpin=rj11adcpin;
							ADCx=ADC1;	ADC_Channel_x=rj11adcch;	TIMx=TIM8;		TIM_Channel_x=TIM_Channel_4; }
		
		
		idnum=main_idnum[i];
		switch (idnum)
		{
			case 0:																						//没接模块
			break;
			
			case 1: 																				   //IO输出   			X						X			输出 1	
				ioxset(xout , xoutpin ,GPIO_Mode_Out_PP);			
			break;

			case 2: 																					//							X						X			SPI		2
				/////////////////////////////////////////// 
			break;

			case 3: 																					//		X					X						X			备用	3	
				////////////////////////////////////////////		 
			break;
			
			case 4: 																					//	IO输出			PWM				IO输出	电机	4	
				{
					ioxset(xout , xoutpin ,GPIO_Mode_Out_PP);
					pwmxset(TIMx, TIM_Channel_x, 50 , 10);		//50HZ 占空比10%
					ioxset(xadc , xadcpin ,GPIO_Mode_Out_PP);			
				}
			break;
			
			case 5: 																					//		X					PWM					X			舵机	5
				pwmxset(TIMx, TIM_Channel_x, 50 , 10);		 //50HZ 占空比10%
			break;
			
			case 6: 																					//		X					X						X			步进	6	
				//////////////////////////////////////////////
			break;
			
			case 7: 																					//	IO输出		 PWM				IO输出	三色灯	7								
				{
					ioxset(xout , xoutpin ,GPIO_Mode_Out_PP);
					pwmxset(TIMx, TIM_Channel_x, 50 , 10);		//50HZ 占空比50%
					ioxset(xadc , xadcpin ,GPIO_Mode_Out_PP);	
				}
			break;
			
			case 8: 																					//	IO输出			X						X			录放	8	
				ioxset(xout , xoutpin ,GPIO_Mode_Out_PP);
			break;
			
			case 9: 																					//		X				PWM						X			蜂鸣器	9
				pwmxset(TIMx, TIM_Channel_x, 50 , 10);	
			break;
			
			case 10: 																					//11xx	IO输出	定时器捕获		X			超声波 10			
				{
					ioxset(xout , xoutpin ,GPIO_Mode_Out_PP);
					///////////////////////////////////////////////////	
				}
			break;
			
			case 11: 																					//	根据18B20			X				X		DS19B20	11
				/////////////////////////////////////////
			break;
			
			case 12: 																					//	I2C_IO			I2C_CLK		X		I2C 12
				////////////////////////////////////////////////////
				ioxset(xout , xoutpin ,GPIO_Mode_Out_PP);
				ioxset(xtim , xtimpin ,GPIO_Mode_Out_PP);
			break;
			
			case 13: 																					//  	X						X				X		
				/////////////////////////////////////////////////
			break;
			
			case 14: 																					//  	X						X				X		
				////////////////////////////////////////////////
			break;
			
			case 15: 																					//  	X						X				X		
				//////////////////////////////////////////////
			break;
			
			case 16: 																					//	X		X		ADCIN		输入模块 16
				ADC_GetConvertValue(ADCx ,ADC_Channel_x);
			break;
			
			case 17: 																					//	X		X		ADCIN		灰度	17
				ADC_GetConvertValue(ADCx ,ADC_Channel_x);
			break;
			
			case 18: 																					//	X		X		ADCIN		红外人体 18
				ADC_GetConvertValue(ADCx ,ADC_Channel_x);
			break;
			
			case 19: 																					//	X		X		ADCIN		声控		19			
				ADC_GetConvertValue(ADCx ,ADC_Channel_x);
			break;
			
			case 20: 																					//	X		X		ADCIN		红外测距20
				ADC_GetConvertValue(ADCx ,ADC_Channel_x);
			break;
			
			case 21: 																					//	X		X		ADCIN		磁感应	21		
				ADC_GetConvertValue(ADCx ,ADC_Channel_x);
			break;
			
			case 22: 																					//	X		X		ADCIN		湿度		22
				ADC_GetConvertValue(ADCx ,ADC_Channel_x);
			break;
			
			default:
			break;
		}
			
	}
	if(main_idnum[11]==3)
	{
		UART485_Config();
	}//485，暂时缺少发送接收函数
	
	if(main_idnum[12]==3)
	{
		USARTx_Config();
	}//232 UART5

}
/*********************************************END OF FILE**********************/
