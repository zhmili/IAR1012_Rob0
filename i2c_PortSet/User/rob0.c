
#include "rob0.h"
extern TGPIO_STATE GPIO_STATE[16];

/******************************************************************************************
* ��������	: ���������OUT�Ӳ������ƽ���߼�����
* �������	:   cha 1,2,3--11				
* ������ֵ	: void 
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
* ��������	: ����ģ��ڵ�ֵ
* �������	:  cha 1,2,3--11		
* ������ֵ	: 0-4096֮�������		����2λ
*******************************************************************************************/
int analog(int cha)/////////////////////////////1024��Χ������������
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
* ��������	: ��ȡ��Ӧͨ����������ֵ
* �������	:  cha 1,2,3--11		
* ������ֵ	: 0   ��ͨ�����߼�ֵΪ0
*                 		  1   ��ͨ�����߼�ֵΪ1
*                           -1  ���������ͨ��ֵ���� 
*******************************************************************************************/
int digital(int cha)///////////////////////////////////////////tim_port,,ad_port��Ҫ��Ϊ�����жϣ�������������
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
* ��������	: ֹͣһ�����
* �������	: cha 1,2,3--11	��Ҫ���رյĵ�� 	
* ������ֵ	: �� 
*******************************************************************************************/
void off(int cha)		//ֹͣĳ��ͨ���ĵ��
{
	GPIO_ResetBits(GPIO_STATE[cha-1].out_port, GPIO_STATE[cha-1].out_pin);
	GPIO_ResetBits(GPIO_STATE[cha-1].ad_port, GPIO_STATE[cha-1].ad_pin);
}


/******************************************************************************************
* ��������	: �Թ��ʼ���w�������cha
* �������	: cha   ��Ҫ�����ĵ��1,2,3-11
*				  	: w    ��Ҫ���õĹ��ʼ���100������ȫ���ʼ���-100
* ������ֵ	: ��
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
		eff = (-w)*20 - 2;			//////////////////////////////////////////////////Ԥ��1%����????
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
		eff = (-p)*20 - 2;			//////////////////////////////////////////////////?��?1%?�??????
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
* ��������	: �ر�������ˢ���
* �������	: void
* ������ֵ	: void
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
* ��������	: �ŷ��������
* �������	:  cha 1,2,3--11		
*	�������	��s			0-1000	�����ŷ���������ת����
* ������ֵ	: void
* ˵��          :������� ���� 18ms<=F<20ms��������1ms ��ת����λ�ã�1.5ms ��ֹ��2ms ��ת����λ��
*                0.5ms - 2.5ms ֮��
*               : 250 ->1ms  375 ->1.5ms  500 ->2ms
*1000 - 20ms  
*1000 - 20ms     200 - 0.4ms 250 - 0.5ms  500 - 1ms 750 - 1.5ms  1000 - 2ms 1250 - 2.5ms 1350 -2.6ms
*200 -1200 --    Futaba S3003
*��Ӣ�� 0 - 4800 --> 0 - 1000
*******************************************************************************************/
void servo(int cha, int s)
{
	int i;
  i = s+250;
	//pin_cfg(GPIO_STATE[cha-1].tim_port , GPIO_STATE[cha-1].tim_pin ,GPIO_Mode_AF_PP, GPIO_Speed_10MHz);
  set_pwm(GPIO_STATE[cha-1].TIM, GPIO_STATE[cha-1].tim_ch, i);
	
}


/******************************************************************************************
* ��������	: �󰴼���ȡ
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
int left_button_read(void)
{
	volatile u16 mid;
	s4_0_set(15);			//4051�л�
	pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);			//PA4,4051��ADC�˿�
	
	#if GET_AVE
   mid = get_ave_adc_value(ADC1, EX_APCH, ADC_AVE_TIMES);								/////////////////////////////////////////
		#else
		mid = get_adc_value(ADC1, EX_APCH);	
		#endif
	if(mid < 904)		//1.1V���а�������ʱ�͵�ƽ
		return 1;			//����1��ʾ�а�������
	else
		return 0;
}

/******************************************************************************************
* ��������	: �м䰴����ȡ
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
int  middle_button_read(void)
{
	volatile u16 mid;
	s4_0_set(14);			//4051�л�
	pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);			//PA4,4051��ADC�˿�

	#if GET_AVE
    mid = get_ave_adc_value(ADC1, EX_APCH, ADC_AVE_TIMES);								/////////////////////////////////////////
    #else
		mid = get_adc_value(ADC1, EX_APCH);	
		#endif
	if(mid < 904)		//1.1V���а�������ʱ�͵�ƽ
		return 1;			//����1��ʾ�а�������
	else
		return 0;
}


/******************************************************************************************
* ��������	: ���ڳ�ʼ��
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
void uart_init(int baud_rate)
{
	USARTx_Config(baud_rate);
}

/******************************************************************************************
* ��������	: �������ݻ�ȡ
* �������	: 
* ������ֵ	: int
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
* ��������	: ���ڷ�������
* �������	: 
* ������ֵ	: void 
*******************************************************************************************/
void uart_write(unsigned char data)
{
	USART_SendData(UART5, data);
  while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET)
   {
   }
}

/******************************************************************************************
* ��������	: ���ڷ����ַ���
* �������	: 
* ������ֵ	: void 
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
* ��������	: ���ڳ�ʼ��
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
void rs485_init(int baud_rate)
{
	UART485_Config(baud_rate);
  GPIO_ResetBits( GPIOA, GPIO_Pin_15);		//PA15����485���򣬳�ʼ������Ϊ����״̬
}


/******************************************************************************************
* ��������	: ���ڽ���
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
int rs485_read(void)
{
	
	return USART_ReceiveData(UART4);
  //while(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == RESET)
  //{
  //}
	
}
/******************************************************************************************
* ��������	: ���ڷ���
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
void rs485_write(int data)
{
	GPIO_SetBits( GPIOA, GPIO_Pin_15);		//PA15����485��������Ϊ����״̬
	USART_SendData(UART4, data);
  while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
   {
   }
	
	 GPIO_ResetBits( GPIOA, GPIO_Pin_15);		//PA15����485���򣬷��ͽ��������л��ɽ���״̬
}
