
#include "rob0.h"
extern TGPIO_STATE GPIO_STATE[16];

/******************************************************************************************
* 函数功能	: 设置输出（OUT接插件）电平（逻辑）高
* 输入参数	:   cha 1,2,3--11				
* 返回数值	: void 
*******************************************************************************************/
void set_digital(int cha , int h_o_l)
{
   if((cha > 0)&&(cha < 12))
   {
     if(h_o_l == 0)
     {
       GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
     }
     else
     {
       GPIO_SetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
     }
   }
}



/******************************************************************************************
* 函数功能	: 返回模拟口的值
* 输入参数	:  cha 1,2,3--11		
* 返回数值	: 0-4096之间的整数		左移2位
*******************************************************************************************/
int analog(int cha)/////////////////////////////1024范围？？？？？？
{ 
  volatile u16 mid;
 
	if((cha>0)&&(cha<12))
	{
		//pin_cfg(GPIO_STATE[cha-1].ad_port , GPIO_STATE[cha-1].ad_pin ,GPIO_Mode_AIN, GPIO_Speed_2MHz);
												//////////////
		#if GET_AVE
    mid = get_ave_adc_value(GPIO_STATE[cha-1].ADC , GPIO_STATE[cha-1].ad_ch, ADC_AVE_TIMES);/////////////ADC_GetConvertValueAVE
		#else
    mid = get_adc_value(GPIO_STATE[cha-1].ADC , GPIO_STATE[cha-1].ad_ch);
    #endif	
    
		mid=mid>>2;
		return mid;
	}
 else
	 {return 0 ;}
}


/******************************************************************************************
* 函数功能	: 读取相应通道的数字量值
* 输入参数	:  cha 1,2,3--11		
* 返回数值	: 0   该通道的逻辑值为0
*                 		  1   该通道的逻辑值为1
*                           -1  输入参数的通道值错误 
*******************************************************************************************/
int digital(int cha)///////////////////////////////////////////tim_port,,ad_port需要作为输入判断？？？？？？？
{
	u16 GPIO_Pin ;
	if(cha >0 && cha < 12)
	{
		//pin_cfg(GPIO_STATE[cha-1].out_port , GPIO_STATE[cha-1].out_pin ,GPIO_Mode_IN_FLOATING, GPIO_Speed_2MHz);	
	
		return  GPIO_ReadInputDataBit(GPIO_STATE[cha-1].ad_port , GPIO_STATE[cha-1].ad_pin);	
	}
 
	else { return 0;}
}


/******************************************************************************************
* ????	: ????
* ????	: p   1-12
* ????	: 0   ????????0
*                 1   ????????1
*                 -1  ?????????? 
* ?? 120328  :  p  1- 9
*******************************************************************************************/
/*??              :                                                                         								   */
/*????   : 20080926                                                                							   */
int op_digital(int cha)
{
  int ret;
  if((cha > 0)&&(cha < 12))
  {
    ret = GPIO_ReadInputDataBit(GPIO_STATE[cha-1].ad_port , GPIO_STATE[cha-1].ad_pin);
    if(ret)
    {
      ret = 0;
    }
    else
    {
      ret = 1;
    }
    return ret;
  }
  else
  {
    return SET_CHA_ERR;
  }
}


/******************************************************************************************
* 函数功能	: 停止一个电机
* 输入参数	: cha 1,2,3--11	需要被关闭的电机 	
* 返回数值	: 无 
*******************************************************************************************/
void off(int cha)		//停止某个通道的电机
{
	GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
	GPIO_ResetBits(GPIO_STATE[cha-1].ad_port, GPIO_STATE[cha-1].ad_pin);
}


/******************************************************************************************
* 函数功能	: 以功率级别w启动电机cha
* 输入参数	: cha   所要启动的电机1,2,3-11
*				  	: w    所要设置的功率级别100到反向全功率级别-100
* 返回数值	: 无
*******************************************************************************************/
#if 1
void motor(int cha, int w)
{
	int eff;
	if(w==0)
	{
		GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
		GPIO_ResetBits(GPIO_STATE[cha-1].ad_port, GPIO_STATE[cha-1].ad_pin);
	}
	
	if(w<0)
	{
		eff = (-w)*20 - 2;			//////////////////////////////////////////////////预留1%死区????
		GPIO_SetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
		GPIO_ResetBits(GPIO_STATE[cha-1].ad_port, GPIO_STATE[cha-1].ad_pin);
		set_pwm(GPIO_STATE[cha-1].TIM , GPIO_STATE[cha-1].tim_ch , eff); 
	}
	
	if(w>0)
	{
		eff = w*20 - 2;		
		GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
		GPIO_SetBits(GPIO_STATE[cha-1].ad_port, GPIO_STATE[cha-1].ad_pin);
		set_pwm(GPIO_STATE[cha-1].TIM , GPIO_STATE[cha-1].tim_ch , eff);
	}
	
}
#else

void motor(int cha, int p)
{
  int eff;
  if(p == 0)
  {
  GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
  GPIO_ResetBits(GPIO_STATE[cha-1].ad_port, GPIO_STATE[cha-1].ad_pin);
  }
	
  if(p < 0)
  {
		eff = (-p)*20 - 2;			//////////////////////////////////////////////////?め?1%?�??????
		GPIO_SetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
		GPIO_ResetBits(GPIO_STATE[cha-1].ad_port, GPIO_STATE[cha-1].ad_pin);
		set_pwm(GPIO_STATE[cha-1].TIM , GPIO_STATE[cha-1].tim_ch , eff); 
	}
	
	if(p > 0)
	{
		eff = p*20 - 2;		
		GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
		GPIO_SetBits(GPIO_STATE[cha-1].ad_port, GPIO_STATE[cha-1].ad_pin);
		set_pwm(GPIO_STATE[cha-1].TIM , GPIO_STATE[cha-1].tim_ch , eff);
	}
}
#endif

/******************************************************************************************
* 函数功能	: 关闭所有有刷电机
* 输入参数	: void
* 返回数值	: void
*******************************************************************************************/
void stop()
{
	int i;
	for(i=0;i<11;i++)
		{
			if(GPIO_STATE[i].h_num==4)
			{
				GPIO_ResetBits(GPIO_STATE[i].out_port, GPIO_STATE[i].out_pin);
				GPIO_ResetBits(GPIO_STATE[i].ad_port, GPIO_STATE[i].ad_pin);
			}
		}
}


/******************************************************************************************
* 函数功能	: 伺服电机设置
* 输入参数	:  cha 1,2,3--11		
*	输入参数	：s			0-1000	设置伺服电机输入的转动量
* 返回数值	: void
* 说明          :舵机参数 周期 18ms<=F<20ms，脉冲宽度1ms 右转极限位置，1.5ms 静止，2ms 左转极限位置
*                0.5ms - 2.5ms 之间
*               : 250 ->1ms  375 ->1.5ms  500 ->2ms
*1000 - 20ms  
*1000 - 20ms     200 - 0.4ms 250 - 0.5ms  500 - 1ms 750 - 1.5ms  1000 - 2ms 1250 - 2.5ms 1350 -2.6ms
*200 -1200 --    Futaba S3003
*那英特 0 - 4800 --> 0 - 1000
*******************************************************************************************/
void servo(int cha, int s)
{
	int i;
  i = s+250;
	//pin_cfg(GPIO_STATE[cha-1].tim_port , GPIO_STATE[cha-1].tim_pin ,GPIO_Mode_AF_PP, GPIO_Speed_10MHz);
  set_pwm(GPIO_STATE[cha-1].TIM, GPIO_STATE[cha-1].tim_ch, i);
	
}


/******************************************************************************************
* 函数功能	: 左按键读取
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
int left_button_read(void)
{
	volatile u16 mid;
	s4_0_set(15);			//4051切换
	pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);			//PA4,4051的ADC端口
	
	#if GET_AVE
   mid = get_ave_adc_value(ADC1, EX_APCH, ADC_AVE_TIMES);								/////////////////////////////////////////
		#else
		mid = get_adc_value(ADC1, EX_APCH);	
		#endif
	if(mid < 904)		//1.1V，有按键按下时低电平
		return 1;			//返回1表示有按键按下
	else
		return 0;
}

/******************************************************************************************
* 函数功能	: 中间按键读取
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
int  middle_button_read(void)
{
	volatile u16 mid;
	s4_0_set(14);			//4051切换
	pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);			//PA4,4051的ADC端口

	#if GET_AVE
    mid = get_ave_adc_value(ADC1, EX_APCH, ADC_AVE_TIMES);								/////////////////////////////////////////
    #else
		mid = get_adc_value(ADC1, EX_APCH);	
		#endif
	if(mid < 904)		//1.1V，有按键按下时低电平
		return 1;			//返回1表示有按键按下
	else
		return 0;
}


/******************************************************************************************
* 函数功能	: 串口初始化
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
void uart_init(int baud_rate)
{
	USARTx_Config(baud_rate);
}

/******************************************************************************************
* 函数功能	: 串口数据获取
* 输入参数	: 
* 返回数值	: int
*******************************************************************************************/
unsigned char uart_read(void)
{
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	return USART_ReceiveData(UART5);
  //while(USART_GetFlagStatus(UART5, USART_FLAG_RXNE) == RESET)
  //{
  //}
}

/******************************************************************************************
* 函数功能	: 串口发送数据
* 输入参数	: 
* 返回数值	: void 
*******************************************************************************************/
void uart_write(unsigned char data)
{
	USART_SendData(UART5, data);
  while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET)
   {
   }
}

/******************************************************************************************
* 函数功能	: 串口发送字符串
* 输入参数	: 
* 返回数值	: void 
*******************************************************************************************/
void uart_write_str(char* pdata)
{
	int i;
	for( i = 0; i < strlen(pdata); i++)
	{
		uart_write(pdata[i]);
	}
}

/******************************************************************************************
* 函数功能	: 串口初始化
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
void rs485_init(int baud_rate)
{
	UART485_Config(baud_rate);
  GPIO_ResetBits( GPIOA, GPIO_Pin_15);		//PA15控制485方向，初始化设置为接收状态
}


/******************************************************************************************
* 函数功能	: 串口接收
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
int rs485_read(void)
{
	
	return USART_ReceiveData(UART4);
  //while(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == RESET)
  //{
  //}
	
}
/******************************************************************************************
* 函数功能	: 串口发送
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
void rs485_write(int data)
{
	GPIO_SetBits( GPIOA, GPIO_Pin_15);		//PA15控制485方向，设置为发送状态
	USART_SendData(UART4, data);
  while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
   {
   }
	
	 GPIO_ResetBits( GPIOA, GPIO_Pin_15);		//PA15控制485方向，发送结束立即切换成接收状态
}
