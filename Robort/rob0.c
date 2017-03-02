
#include <includes.h>

#if ROB_0

extern TGPIO_STATE GPIO_STATE[16];
extern u32 mot_tim_ch[4];


/******************************************************************************************
* ��������	: ����ģ��ڵ�ֵ(Ϊ0-255֮�������)
* �������	: p	�����ģ��ͨ�� 1-32
* ������ֵ	: 0-255֮�������
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
* ��������	: ��ȡ��Ӧͨ����������ֵ
* �������	: p   1-12
* ������ֵ	: 0   ��ͨ�����߼�ֵΪ0
*                 		  1   ��ͨ�����߼�ֵΪ1
*                           -1  ���������ͨ��ֵ���� 
* ���� 120328  :  p  1- 9
*******************************************************************************************/
/*ע��              :                                                                         								   */
/*�汾ʱ��   : 20080926                                                                							   */
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
* ��������	: ���ȡ��
* �������	: p   1-12
* ������ֵ	: 0   ��ͨ�����߼�ֵΪ0
*                 1   ��ͨ�����߼�ֵΪ1
*                 -1  ���������ͨ��ֵ���� 
* ���� 120328  :  p  1- 9
*******************************************************************************************/
/*ע��              :                                                                         								   */
/*�汾ʱ��   : 20080926                                                                							   */
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
* ��������	: ���ȡ��
* �������	: p   1-12
* ������ֵ	: 0   ��ͨ�����߼�ֵΪ0
*                 1   ��ͨ�����߼�ֵΪ1
*                 -1  ���������ͨ��ֵ���� 
* ���� 120328  :  p  1- 9
*******************************************************************************************/
/*ע��              :                                                                         								   */
/*�汾ʱ��   : 20080926                                                                							   */
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
* ��������	: ���Ұ�����ȡ
* �������	: 
* ������ֵ	: void
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
* ��������	: ���������OUT�Ӳ������ƽ���߼�����
* �������	: channel 1,2,3,4,..11
* ������ֵ	: void 
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
* ��������	: ���÷��������ʱ��
//* �������	: msecond ����
* ������ֵ	: void   
*******************************************************************************************/
/*�ڲ�˵��   :                                                             */
/*�汾ʱ��   : 20080923	 */
void beep(int cha, int msec)
{
  set_digital(cha, 1);
  delay( 0,  msec);
  set_digital(cha, 0);
}


/******************************************************************************************
* ��������	: ����
* �������	: void
* ������ֵ	: void
*******************************************************************************************/	
#if 0
void clear_lcd(void)
{
  ClrScreen(0);
}

#endif

/******************************************************************************************
* ��������	: ��ʾ0-1000�ڵ������� value -Ҫ��ʾ��ֵ
* �������	:  x - 0-84 ��y - 0,2,4
* ������ֵ	: void 
*******************************************************************************************/
#if 0
void display_int(unsigned char x,unsigned char y,unsigned short value)
{
  PutIntValue(x,y,value);
}
#endif
/******************************************************************************************
* ��������	: ��ʾ0-1000�ڵ������� value -Ҫ��ʾ��ֵ
* �������	:  x - 0-84 ��y - 0,2,4
* ������ֵ	: void 
*******************************************************************************************/
#if 0
void display_str(unsigned char x,unsigned char y,char *p)
{
  PutString(x,y,p);
}
#endif

/******************************************************************************************
* ��������	: �Թ��ʼ���p�������m
* �������	: m   ��Ҫ�����ĵ��1-4
*		  p    ��Ҫ���õĹ��ʼ���100������ȫ���ʼ���-100
* ������ֵ	: ��
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
                  eff = (-p)*40 - 2;			//////////////////////////////////////////////////?�訢?1%?��??????
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
		eff = (-p)*40 - 2;			//////////////////////////////////////////////////?�訢?1%?��??????
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
* ��������	: �ر����е����������stop()һ��
* �������	: void
* ������ֵ	: void
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
* ��������	: ֹͣһ�����
* �������	: id  ��Ҫ���رյĵ����1��2��3��4 
* ������ֵ	: �� 
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
* ��������	: �ŷ��������
* �������	: s	1-4 �ŷ�������
*				: p  	0-1000	�����ŷ���������ת����
* ������ֵ	: void
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
* ��������	: ���ڳ�ʼ��
* �������	: port �˿� 1 2�� baud_rate ������ 4800 9600 19200 38400 115200
* ������ֵ	: void
*******************************************************************************************/
void uart_init(int baud_rate)
{
   UXART_Init(UART5,baud_rate,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,0,0);
}

/******************************************************************************************
* ��������	: ���ڽ���
* �������	: port �˿� 1 2
* ������ֵ	: һ�ν���8λ����
*******************************************************************************************/
unsigned char uart_read()
{
  if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
   return USART_ReceiveData(UART5);
}


/******************************************************************************************
* ��������	: ���ڳ�ʼ��
* �������	: port �˿� 1 2�� data : һ�η���8bit ����
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
* ��������	: ���ڳ�ʼ��
* �������	: port �˿� 1 2�� data : һ�η���8bit ����
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
* ��������	:rs485��ʼ��
* �������	: baud_rate ������ 4800 9600 19200 38400 115200
* ������ֵ	: void
*******************************************************************************************/
void rs485_init(int baud_rate)
{
  UXART_Init(UART4,baud_rate,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,0,0);
  GPIO_ResetBits( GPIOA, GPIO_Pin_15);		//PA15????485��??����?3?��??������???a?����?���䨬?
}

/******************************************************************************************
* ��������	: rs485��������
* �������	: data ��Ҫ���͵����� һ�η��� 8bit
* ������ֵ	: void
*******************************************************************************************/
void rs485_write(int data)
{
  GPIO_SetBits( GPIOA, GPIO_Pin_15);		//PA15????485��??����?����???a����?�����䨬?
  USART_SendData(UART4, data);
  while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
  {
  }
}


/******************************************************************************************
* ��������	: rs485��������
* �������	: data ��Ҫ���͵����� һ�η��� 8bit
* ������ֵ	: void
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
* ��������	: rs485��������
* �������	: void
* ������ֵ	: int 
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
* ��������	: ��λϵͳʱ��
* �������	: void
* ������ֵ	: void
*******************************************************************************************/
#if 0
void reset_system_clock(void)
{
  OSTimeSet(0L);
}
#endif
/******************************************************************************************
* ��������	: �Ժ�����ʽ����ϵͳ���е�ʱ�����
* �������	: void
* ������ֵ	: ���صĺ���ֵ
*******************************************************************************************/
#if 0
unsigned int msecond(void)
{
  return (unsigned int)OSTimeGet();
}
#endif
/******************************************************************************************
* ��������	: ������ʽ����ϵͳ���е�ʱ�����
* �������	: void
* ������ֵ	: ���ص���ֵ
*******************************************************************************************/	
#if 0
unsigned int second(void)
{
  return (unsigned int)OSTimeGet()/1000;
}
#endif
/******************************************************************************************
* ��������	: ��ʱ����
* �������	: sec  ��Ҫ��ʱ������ 
* ������ֵ	: void
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
* ��������	: ��ͨ�����ݶ�ȡ
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
/*ע��		:
/*�汾ʱ��	: 20080926 		*/														  
void test(void)
{

}




/******************************************************************************************
* ��������	: ���󰴼���ȡ
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
int  left_button_read(void)
{
   volatile u16 mid;
   s4_0_set(15);			//4051?D??
   pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);			//PA4,4051��?ADC???��
	
   #if GET_AVE
   mid = get_ave_adc_value(ADC1, EX_APCH, ADC_AVE_TIMES);								/////////////////////////////////////////
   #else
   mid = get_adc_value(ADC1, EX_APCH);	
   #endif
   if(mid < 904)		//1.1V��?��D���?�����??�����̨���???
	return 1;			//����??1������?��D���?�����??
   else
	return 0;
}

/******************************************************************************************
* ��������	: ���Ұ�����ȡ
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
int  middle_button_read(void)
{
   volatile u16 mid;
   s4_0_set(14);			//4051?D??
   pin_cfg(EX_APORT,EX_APIN,GPIO_Mode_AIN, GPIO_Speed_2MHz);			//PA4,4051��?ADC???��
   #if GET_AVE
   mid = get_ave_adc_value(ADC1, EX_APCH, ADC_AVE_TIMES);								/////////////////////////////////////////
   #else
   mid = get_adc_value(ADC1, EX_APCH);	
   #endif
   if(mid < 904)		//1.1V��?��D���?�����??�����̨���???
    return 1;			//����??1������?��D���?�����??
    else
    return 0;
}

#endif

