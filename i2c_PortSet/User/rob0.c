
#include "rob0.h"
extern TGPIO_STATE GPIO_STATE[16];

/******************************************************************************************
* º¯Êý¹¦ÄÜ	: ÉèÖÃÊä³ö£¨OUT½Ó²å¼þ£©µçÆ½£¨Âß¼­£©¸ß
* ÊäÈë²ÎÊý	:   cha 1,2,3--11				
* ·µ»ØÊýÖµ	: void 
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
* º¯Êý¹¦ÄÜ	: ·µ»ØÄ£Äâ¿ÚµÄÖµ
* ÊäÈë²ÎÊý	:  cha 1,2,3--11		
* ·µ»ØÊýÖµ	: 0-4096Ö®¼äµÄÕûÊý		×óÒÆ2Î»
*******************************************************************************************/
int analog(int cha)/////////////////////////////1024·¶Î§£¿£¿£¿£¿£¿£¿
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
* º¯Êý¹¦ÄÜ	: ¶ÁÈ¡ÏàÓ¦Í¨µÀµÄÊý×ÖÁ¿Öµ
* ÊäÈë²ÎÊý	:  cha 1,2,3--11		
* ·µ»ØÊýÖµ	: 0   ¸ÃÍ¨µÀµÄÂß¼­ÖµÎª0
*                 		  1   ¸ÃÍ¨µÀµÄÂß¼­ÖµÎª1
*                           -1  ÊäÈë²ÎÊýµÄÍ¨µÀÖµ´íÎó 
*******************************************************************************************/
int digital(int cha)///////////////////////////////////////////tim_port,,ad_portÐèÒª×÷ÎªÊäÈëÅÐ¶Ï£¿£¿£¿£¿£¿£¿£¿
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
* º¯Êý¹¦ÄÜ	: Í£Ö¹Ò»¸öµç»ú
* ÊäÈë²ÎÊý	: cha 1,2,3--11	ÐèÒª±»¹Ø±ÕµÄµç»ú 	
* ·µ»ØÊýÖµ	: ÎÞ 
*******************************************************************************************/
void off(int cha)		//Í£Ö¹Ä³¸öÍ¨µÀµÄµç»ú
{
	GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
	GPIO_ResetBits(GPIO_STATE[cha-1].ad_port, GPIO_STATE[cha-1].ad_pin);
}


/******************************************************************************************
* º¯Êý¹¦ÄÜ	: ÒÔ¹¦ÂÊ¼¶±ðwÆô¶¯µç»úcha
* ÊäÈë²ÎÊý	: cha   ËùÒªÆô¶¯µÄµç»ú1,2,3-11
*				  	: w    ËùÒªÉèÖÃµÄ¹¦ÂÊ¼¶±ð100µ½·´ÏòÈ«¹¦ÂÊ¼¶±ð-100
* ·µ»ØÊýÖµ	: ÎÞ
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
		eff = (-w)*20 - 2;			//////////////////////////////////////////////////Ô¤Áô1%ËÀÇø????
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
		eff = (-p)*20 - 2;			//////////////////////////////////////////////////?¤á?1%?à??????
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
* º¯Êý¹¦ÄÜ	: ¹Ø±ÕËùÓÐÓÐË¢µç»ú
* ÊäÈë²ÎÊý	: void
* ·µ»ØÊýÖµ	: void
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
* º¯Êý¹¦ÄÜ	: ËÅ·þµç»úÉèÖÃ
* ÊäÈë²ÎÊý	:  cha 1,2,3--11		
*	ÊäÈë²ÎÊý	£ºs			0-1000	ÉèÖÃËÅ·þµç»úÊäÈëµÄ×ª¶¯Á¿
* ·µ»ØÊýÖµ	: void
* ËµÃ÷          :¶æ»ú²ÎÊý ÖÜÆÚ 18ms<=F<20ms£¬Âö³å¿í¶È1ms ÓÒ×ª¼«ÏÞÎ»ÖÃ£¬1.5ms ¾²Ö¹£¬2ms ×ó×ª¼«ÏÞÎ»ÖÃ
*                0.5ms - 2.5ms Ö®¼ä
*               : 250 ->1ms  375 ->1.5ms  500 ->2ms
*1000 - 20ms  
*1000 - 20ms     200 - 0.4ms 250 - 0.5ms  500 - 1ms 750 - 1.5ms  1000 - 2ms 1250 - 2.5ms 1350 -2.6ms
*200 -1200 --    Futaba S3003
*ÄÇÓ¢ÌØ 0 - 4800 --> 0 - 1000
*******************************************************************************************/
void servo(int cha, int s)
{
	int i;
  i = s+250;
	//pin_cfg(GPIO_STATE[cha-1].tim_port , GPIO_STATE[cha-1].tim_pin ,GPIO_Mode_AF_PP, GPIO_Speed_10MHz);
  set_pwm(GPIO_STATE[cha-1].TIM, GPIO_STATE[cha-1].tim_ch, i);
	
}


/******************************************************************************************
* º¯Êý¹¦ÄÜ	: ×ó°´¼ü¶ÁÈ¡
* ÊäÈë²ÎÊý	: 
* ·µ»ØÊýÖµ	: void
*******************************************************************************************/
int left_button_read(void)
{
	volatile u16 mid;
	s4_0_set(15);			//4051ÇÐ»»
	pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);			//PA4,4051µÄADC¶Ë¿Ú
	
	#if GET_AVE
   mid = get_ave_adc_value(ADC1, EX_APCH, ADC_AVE_TIMES);								/////////////////////////////////////////
		#else
		mid = get_adc_value(ADC1, EX_APCH);	
		#endif
	if(mid < 904)		//1.1V£¬ÓÐ°´¼ü°´ÏÂÊ±µÍµçÆ½
		return 1;			//·µ»Ø1±íÊ¾ÓÐ°´¼ü°´ÏÂ
	else
		return 0;
}

/******************************************************************************************
* º¯Êý¹¦ÄÜ	: ÖÐ¼ä°´¼ü¶ÁÈ¡
* ÊäÈë²ÎÊý	: 
* ·µ»ØÊýÖµ	: void
*******************************************************************************************/
int  middle_button_read(void)
{
	volatile u16 mid;
	s4_0_set(14);			//4051ÇÐ»»
	pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);			//PA4,4051µÄADC¶Ë¿Ú

	#if GET_AVE
    mid = get_ave_adc_value(ADC1, EX_APCH, ADC_AVE_TIMES);								/////////////////////////////////////////
    #else
		mid = get_adc_value(ADC1, EX_APCH);	
		#endif
	if(mid < 904)		//1.1V£¬ÓÐ°´¼ü°´ÏÂÊ±µÍµçÆ½
		return 1;			//·µ»Ø1±íÊ¾ÓÐ°´¼ü°´ÏÂ
	else
		return 0;
}


/******************************************************************************************
* º¯Êý¹¦ÄÜ	: ´®¿Ú³õÊ¼»¯
* ÊäÈë²ÎÊý	: 
* ·µ»ØÊýÖµ	: void
*******************************************************************************************/
void uart_init(int baud_rate)
{
	USARTx_Config(baud_rate);
}

/******************************************************************************************
* º¯Êý¹¦ÄÜ	: ´®¿ÚÊý¾Ý»ñÈ¡
* ÊäÈë²ÎÊý	: 
* ·µ»ØÊýÖµ	: int
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
* º¯Êý¹¦ÄÜ	: ´®¿Ú·¢ËÍÊý¾Ý
* ÊäÈë²ÎÊý	: 
* ·µ»ØÊýÖµ	: void 
*******************************************************************************************/
void uart_write(unsigned char data)
{
	USART_SendData(UART5, data);
  while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET)
   {
   }
}

/******************************************************************************************
* º¯Êý¹¦ÄÜ	: ´®¿Ú·¢ËÍ×Ö·û´®
* ÊäÈë²ÎÊý	: 
* ·µ»ØÊýÖµ	: void 
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
* º¯Êý¹¦ÄÜ	: ´®¿Ú³õÊ¼»¯
* ÊäÈë²ÎÊý	: 
* ·µ»ØÊýÖµ	: void
*******************************************************************************************/
void rs485_init(int baud_rate)
{
	UART485_Config(baud_rate);
  GPIO_ResetBits( GPIOA, GPIO_Pin_15);		//PA15¿ØÖÆ485·½Ïò£¬³õÊ¼»¯ÉèÖÃÎª½ÓÊÕ×´Ì¬
}


/******************************************************************************************
* º¯Êý¹¦ÄÜ	: ´®¿Ú½ÓÊÕ
* ÊäÈë²ÎÊý	: 
* ·µ»ØÊýÖµ	: void
*******************************************************************************************/
int rs485_read(void)
{
	
	return USART_ReceiveData(UART4);
  //while(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == RESET)
  //{
  //}
	
}
/******************************************************************************************
* º¯Êý¹¦ÄÜ	: ´®¿Ú·¢ËÍ
* ÊäÈë²ÎÊý	: 
* ·µ»ØÊýÖµ	: void
*******************************************************************************************/
void rs485_write(int data)
{
	GPIO_SetBits( GPIOA, GPIO_Pin_15);		//PA15¿ØÖÆ485·½Ïò£¬ÉèÖÃÎª·¢ËÍ×´Ì¬
	USART_SendData(UART4, data);
  while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
   {
   }
	
	 GPIO_ResetBits( GPIOA, GPIO_Pin_15);		//PA15¿ØÖÆ485·½Ïò£¬·¢ËÍ½áÊøÁ¢¼´ÇÐ»»³É½ÓÊÕ×´Ì¬
}
