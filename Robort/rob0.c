
#include <includes.h>

#if ROB_0

extern TGPIO_STATE GPIO_STATE[16];
extern u32 mot_tim_ch[4];


/******************************************************************************************
* 函数功能	: 返回模拟口的值(为0-255之间的整数)
* 输入参数	: p	输入的模拟通道 1-32
* 返回数值	: 0-255之间的整数
*******************************************************************************************/                                                               							          
int analog(int cha)
{
  int mid;
  if((cha > 0)&&(cha < 12))
  {
    #if GET_AVE
    mid = get_ave_adc_value(GPIO_STATE[cha-1].ADC , GPIO_STATE[cha-1].ad_ch, ADC_AVE_TIMES);
    #else
    mid = get_adc_value(GPIO_STATE[cha-1].ADC , GPIO_STATE[cha-1].ad_ch);
    #endif	
    
    mid = mid>>2;
    return mid;
  }
  else
  {
    return SET_CHA_ERR; 
  }
}

      
/******************************************************************************************
* 函数功能	: 读取相应通道的数字量值
* 输入参数	: p   1-12
* 返回数值	: 0   该通道的逻辑值为0
*                 		  1   该通道的逻辑值为1
*                           -1  输入参数的通道值错误 
* 修正 120328  :  p  1- 9
*******************************************************************************************/
/*注意              :                                                                         								   */
/*版本时间   : 20080926                                                                							   */
int digital(int cha)
{
  if((cha > 0)&&(cha < 12))
  {
    return  GPIO_ReadInputDataBit(GPIO_STATE[cha-1].ad_port , GPIO_STATE[cha-1].ad_pin);
  }
  else
  {
    return SET_CHA_ERR;
  }
}

/******************************************************************************************
* 函数功能	: 输出取反
* 输入参数	: p   1-12
* 返回数值	: 0   该通道的逻辑值为0
*                 1   该通道的逻辑值为1
*                 -1  输入参数的通道值错误 
* 修正 120328  :  p  1- 9
*******************************************************************************************/
/*注意              :                                                                         								   */
/*版本时间   : 20080926                                                                							   */
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
* 函数功能	: 输出取反
* 输入参数	: p   1-12
* 返回数值	: 0   该通道的逻辑值为0
*                 1   该通道的逻辑值为1
*                 -1  输入参数的通道值错误 
* 修正 120328  :  p  1- 9
*******************************************************************************************/
/*注意              :                                                                         								   */
/*版本时间   : 20080926                                                                							   */
void pluse(int cha)
{
  int ret;
  if((cha > 0)&&(cha < 12))
  {
    GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
    GPIO_SetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
    GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
  }
}



/******************************************************************************************
* 函数功能	: 向右按键读取
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
#if 0
int key(int loc)
{
  if(loc == 1)
  {
    left_button_read();
  }
  else if(loc == 2)
  {
    middle_button_read();
  }
}   
#endif



void set_digital_out(int cha)
{
  if((cha > 0)&&(cha < 12))
   {
     GPIO_SetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
   }
}


void clear_digital_out(int cha)
{
  if((cha > 0)&&(cha < 12))
   {
     GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
   }
}


//
/******************************************************************************************
* 函数功能	: 设置输出（OUT接插件）电平（逻辑）高
* 输入参数	: channel 1,2,3,4,..11
* 返回数值	: void 
*******************************************************************************************/  
#if 0
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
#endif
/******************************************************************************************
* 函数功能	: 设置蜂鸣器响的时间
//* 输入参数	: msecond 毫秒
* 返回数值	: void   
*******************************************************************************************/
/*内部说明   :                                                             */
/*版本时间   : 20080923	 */
void beep(int cha, int msec)
{
  set_digital(cha, 1);
  delay( 0,  msec);
  set_digital(cha, 0);
}


/******************************************************************************************
* 函数功能	: 清屏
* 输入参数	: void
* 返回数值	: void
*******************************************************************************************/	
#if 0
void clear_lcd(void)
{
  ClrScreen(0);
}

#endif

/******************************************************************************************
* 函数功能	: 显示0-1000内的正整数 value -要显示的值
* 输入参数	:  x - 0-84 ，y - 0,2,4
* 返回数值	: void 
*******************************************************************************************/
#if 0
void display_int(unsigned char x,unsigned char y,unsigned short value)
{
  PutIntValue(x,y,value);
}
#endif
/******************************************************************************************
* 函数功能	: 显示0-1000内的正整数 value -要显示的值
* 输入参数	:  x - 0-84 ，y - 0,2,4
* 返回数值	: void 
*******************************************************************************************/
#if 0
void display_str(unsigned char x,unsigned char y,char *p)
{
  PutString(x,y,p);
}
#endif

/******************************************************************************************
* 函数功能	: 以功率级别p启动电机m
* 输入参数	: m   所要启动的电机1-4
*		  p    所要设置的功率级别100到反向全功率级别-100
* 返回数值	: 无
*******************************************************************************************/
void motor(int cha, int p)
{
#if CONST_MOT
  int eff;
  
  if(p == 0)
  {
  GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
  GPIO_ResetBits(GPIO_STATE[cha-1].tim_port, GPIO_STATE[cha-1].tim_pin);
  }
  
  if((cha > 3)&&(cha < 8))   // 4 5 6 7
  {
    if(p < 0)
    {
                  eff = (-p)*40 - 2;			//////////////////////////////////////////////////?¤á?1%?à??????
                  GPIO_SetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
                  GPIO_ResetBits(GPIO_STATE[cha-1].tim_port, GPIO_STATE[cha-1].tim_pin);
                  set_pwm(MOT_TIM , mot_tim_ch[cha - 4] , eff); 
    }
          
    if(p > 0)
    {
                  eff = p*40 - 2;		
                  GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
                  GPIO_SetBits(GPIO_STATE[cha-1].ad_port, GPIO_STATE[cha-1].ad_pin);
                  set_pwm(MOT_TIM , mot_tim_ch[cha - 4] , eff);
    }
  }
  
#else
  int eff;
  if(p == 0)
  {
  GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
  GPIO_ResetBits(GPIO_STATE[cha-1].ad_port, GPIO_STATE[cha-1].ad_pin);
  }
	
  if(p < 0)
  {
		eff = (-p)*40 - 2;			//////////////////////////////////////////////////?¤á?1%?à??????
		GPIO_SetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
		GPIO_ResetBits(GPIO_STATE[cha-1].ad_port, GPIO_STATE[cha-1].ad_pin);
		set_pwm(GPIO_STATE[cha-1].TIM , GPIO_STATE[cha-1].tim_ch , eff); 
  }
	
  if(p > 0)
  {
		eff = p*40 - 2;		
		GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
		GPIO_SetBits(GPIO_STATE[cha-1].ad_port, GPIO_STATE[cha-1].ad_pin);
		set_pwm(GPIO_STATE[cha-1].TIM , GPIO_STATE[cha-1].tim_ch , eff);
  }
#endif
}

/******************************************************************************************
* 函数功能	: 关闭所有电机，功能与stop()一致
* 输入参数	: void
* 返回数值	: void
*******************************************************************************************/														   
void stop(void)
{
#if CONST_MOT
 int i;
  for(i = 3; i<7; i++)  //3 4 5 6
  {
   if(GPIO_STATE[i].h_num == BRUSH_MOT_3)
   {
    GPIO_ResetBits(GPIO_STATE[i].out_port, GPIO_STATE[i].out_pin);
    GPIO_ResetBits(GPIO_STATE[i].tim_port, GPIO_STATE[i].tim_pin);
   }
  }
#else
  int i;
  for(i=0;i<11;i++)
  {
   if(GPIO_STATE[i].h_num == BRUSH_MOT_3)
   {
    GPIO_ResetBits(GPIO_STATE[i].out_port, GPIO_STATE[i].out_pin);
    GPIO_ResetBits(GPIO_STATE[i].ad_port, GPIO_STATE[i].ad_pin);
   }
  } 
#endif
}    
/******************************************************************************************
* 函数功能	: 停止一个电机
* 输入参数	: id  需要被关闭的电机，1，2，3，4 
* 返回数值	: 无 
*******************************************************************************************/															   
void off(int cha)
{
#if CONST_MOT
  GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
  GPIO_ResetBits(GPIO_STATE[cha-1].tim_port, GPIO_STATE[cha-1].tim_pin);
#else
  GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
  GPIO_ResetBits(GPIO_STATE[cha-1].ad_port, GPIO_STATE[cha-1].ad_pin);
#endif  
}

/******************************************************************************************
* 函数功能	: 伺服电机设置
* 输入参数	: s	1-4 伺服电机编号
*				: p  	0-1000	设置伺服电机输入的转动量
* 返回数值	: void
*******************************************************************************************/
void servo(int cha, int p)
{
  int i;
  if((cha > 0)&&(cha < 12))
  {
    i = p+250;
    set_pwm(GPIO_STATE[cha-1].TIM, GPIO_STATE[cha-1].tim_ch, i);
  }
}

/******************************************************************************************
* 函数功能	: 串口初始化
* 输入参数	: port 端口 1 2， baud_rate 波特率 4800 9600 19200 38400 115200
* 返回数值	: void
*******************************************************************************************/
void uart_init(int baud_rate)
{
   UXART_Init(UART5,baud_rate,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,0,0);
}

/******************************************************************************************
* 函数功能	: 串口接收
* 输入参数	: port 端口 1 2
* 返回数值	: 一次接收8位数据
*******************************************************************************************/
unsigned char uart_read()
{
  if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
   return USART_ReceiveData(UART5);
}


/******************************************************************************************
* 函数功能	: 串口初始化
* 输入参数	: port 端口 1 2， data : 一次发送8bit 数据
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
* 函数功能	: 串口初始化
* 输入参数	: port 端口 1 2， data : 一次发送8bit 数据
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
* 函数功能	:rs485初始化
* 输入参数	: baud_rate 波特率 4800 9600 19200 38400 115200
* 返回数值	: void
*******************************************************************************************/
void rs485_init(int baud_rate)
{
  UXART_Init(UART4,baud_rate,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,0,0);
  GPIO_ResetBits( GPIOA, GPIO_Pin_15);		//PA15????485·??ò￡?3?ê??ˉéè???a?óê?×′ì?
}

/******************************************************************************************
* 函数功能	: rs485发送数据
* 输入参数	: data 需要发送的数据 一次发送 8bit
* 返回数值	: void
*******************************************************************************************/
void rs485_write(int data)
{
  GPIO_SetBits( GPIOA, GPIO_Pin_15);		//PA15????485·??ò￡?éè???a·￠?í×′ì?
  USART_SendData(UART4, data);
  while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
  {
  }
}


/******************************************************************************************
* 函数功能	: rs485发送数据
* 输入参数	: data 需要发送的数据 一次发送 8bit
* 返回数值	: void
*******************************************************************************************/
#if 0
void rs485_write_str(char* pdata)
{
  int i;
  for( i = 0; i < strlen(pdata); i++)
  {
    rs485_write_char(pdata[i]);
  }
}
#endif
/******************************************************************************************
* 函数功能	: rs485接收数据
* 输入参数	: void
* 返回数值	: int 
*******************************************************************************************/
int  rs485_read(void)
{
  if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
   return USART_ReceiveData(UART4);
}

#if 0
int iic_read(int addr, int reg)
{

}
      
void iic_write(int addr, int reg,int val)
{

}
#endif    
      
/******************************************************************************************
* 函数功能	: 复位系统时钟
* 输入参数	: void
* 返回数值	: void
*******************************************************************************************/
#if 0
void reset_system_clock(void)
{
  OSTimeSet(0L);
}
#endif
/******************************************************************************************
* 函数功能	: 以毫秒形式返回系统运行的时间计数
* 输入参数	: void
* 返回数值	: 返回的毫秒值
*******************************************************************************************/
#if 0
unsigned int msecond(void)
{
  return (unsigned int)OSTimeGet();
}
#endif
/******************************************************************************************
* 函数功能	: 以秒形式返回系统运行的时间计数
* 输入参数	: void
* 返回数值	: 返回的秒值
*******************************************************************************************/	
#if 0
unsigned int second(void)
{
  return (unsigned int)OSTimeGet()/1000;
}
#endif
/******************************************************************************************
* 函数功能	: 延时秒数
* 输入参数	: sec  所要延时的秒数 
* 返回数值	: void
*******************************************************************************************/	
#if 0
void delay(int sec, int msec)
{
   int get_msec;
   int get_min;
   int get_sec;
   
   get_msec = msec%1000;
   get_sec = msec/1000;
   get_sec = sec + get_sec;
   
   get_sec = get_sec%60;
   get_min = get_sec/60;
   
   if(get_min >59)
   {
     get_min = 59;
   }
   OSTimeDlyHMSM(0,get_min,get_sec,get_msec); 
}
#endif

/******************************************************************************************
* 函数功能	: 各通道数据读取
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
/*注意		:
/*版本时间	: 20080926 		*/														  
void test(void)
{

}




/******************************************************************************************
* 函数功能	: 向左按键读取
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
int  left_button_read(void)
{
   volatile u16 mid;
   s4_0_set(15);			//4051?D??
   pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);			//PA4,4051μ?ADC???ú
	
   #if GET_AVE
   mid = get_ave_adc_value(ADC1, EX_APCH, ADC_AVE_TIMES);								/////////////////////////////////////////
   #else
   mid = get_adc_value(ADC1, EX_APCH);	
   #endif
   if(mid < 904)		//1.1V￡?óD°′?ü°′??ê±μíμ???
	return 1;			//·μ??1±íê?óD°′?ü°′??
   else
	return 0;
}

/******************************************************************************************
* 函数功能	: 向右按键读取
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
int  middle_button_read(void)
{
   volatile u16 mid;
   s4_0_set(14);			//4051?D??
   pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);			//PA4,4051μ?ADC???ú
   #if GET_AVE
   mid = get_ave_adc_value(ADC1, EX_APCH, ADC_AVE_TIMES);								/////////////////////////////////////////
   #else
   mid = get_adc_value(ADC1, EX_APCH);	
   #endif
   if(mid < 904)		//1.1V￡?óD°′?ü°′??ê±μíμ???
    return 1;			//·μ??1±íê?óD°′?ü°′??
    else
    return 0;
}

#endif

