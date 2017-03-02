/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : main.c
* Author             : David JIANG
* Date First Issued  : 2007.9.17
* Description        : main
*
********************************************************************************
* History:
* 2007.9.17 : Version 1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <includes.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern TRecvBuf RecvBuf[];

extern char fun_name[16][20];
extern char fun_num;
extern char pstwo_use_flag;

extern void mainX1(void);

#if ROB_0 == 0

/******************************************************************************************
* ��������	: ���������OUT�Ӳ������ƽ���߼�����
* �������	: channel 1,2,3,4,5,6,7,8,9,10
* ������ֵ	: void 
*******************************************************************************************/
/*�ڲ�˵��   :                                                                         							          */
/*�汾ʱ��   : 20080923  */
/*�汾ʱ��   : 20140811  */
int set_digital_out(int channel)
{
  u8 cmd,i2c_w_ret;
  
  switch(channel)
  {
    case 1 : SetPWM(TIM8, TIM_Channel_1, SER_SET_HIGH);
    break;
        
    case 2 : SetPWM(TIM8, TIM_Channel_2, SER_SET_HIGH);
    break;
        
    case 3 : SetPWM(TIM1, TIM_Channel_4, SER_SET_HIGH);
    break; 
        
    case 4 : SetPWM(TIM1, TIM_Channel_3, SER_SET_HIGH);
    break;  
        
    case 5 : SetPWM(TIM1, TIM_Channel_2, SER_SET_HIGH);
    break;
         
    case 6 : SetPWM(TIM1, TIM_Channel_1, SER_SET_HIGH);
    break;
        
    case 7: GPIO_SetBits(GPIOG, GPIOG_FAN_1);
    break;
          
    case 8: GPIO_SetBits(GPIOF, GPIOF_FAN_2);
    break;
    
    case 9:
    case 10:
    case 11:
    case 12:
    cmd = IO_SET,
    i2c_w_ret = i2c_WriteBytes(&cmd, IOCMD_LOC+ channel - 9, 1, 0);          //RxBuf[26] = ����д������� ����1 ����
    if(i2c_w_ret < 1)
    {
      led_flash(LED_ON);
      return COM_ERR;
    }
    else
    {
      led_flash(LED_OFF);
      return COM_OK;
    }
    break;
    
    
    default : 
    break;
   }
}

/******************************************************************************************
* ��������	: �������(OUT ����Ӳ������ƽ���߼�����
* �������	: channel 1��2��3��4
* ������ֵ	: void   
*******************************************************************************************/
/*�ڲ�˵��   :                                                                         							           */
/*�汾ʱ��   : 20080923	                                                          							    */
int clear_digital_out(int channel)
{
  u8 cmd , i2c_w_ret;
  switch(channel)
  {
        case 1 : SetPWM(TIM8, TIM_Channel_1, SER_SET_LOW);
        break;
        
        case 2 : SetPWM(TIM8, TIM_Channel_2, SER_SET_LOW);
        break;
        
 	case 3 : SetPWM(TIM1, TIM_Channel_4, SER_SET_LOW);
	break; 
        
        case 4 : SetPWM(TIM1, TIM_Channel_3, SER_SET_LOW);
	break;  
        
        case 5 : SetPWM(TIM1, TIM_Channel_2, SER_SET_LOW);
	break;
        
        case 6 : SetPWM(TIM1, TIM_Channel_1, SER_SET_LOW);
	break;
        
        case 7: GPIO_ResetBits(GPIOG, GPIOG_FAN_1);
        break;
          
        case 8: GPIO_ResetBits(GPIOF, GPIOF_FAN_2);
        break;
        
        
        case 9:
        case 10:
        case 11:
        case 12:
        cmd = IO_RESET,
    i2c_w_ret = i2c_WriteBytes(&cmd, IOCMD_LOC+ channel - 9, 1, 0);          //RxBuf[26] = ����д������� ����1 ����
    if(i2c_w_ret < 1)
    {
      led_flash(LED_ON);
      return COM_ERR;
    }
    else
    {
      led_flash(LED_OFF);
      return COM_OK;
    }
    break;
         
        default : 
          break;
    }
}


/******************************************************************************************
* ��������	: ����ģ��ڵ�ֵ(Ϊ0-255֮�������)
* �������	: p	�����ģ��ͨ�� 1-26
* ������ֵ	: 0-255֮�������
* ���� 120328   : ����Ϊ 4096
*******************************************************************************************/
/*ע��              : p= 0 Ϊ��ص�ѹ�ɼ�ͨ�������ڲ�ʹ��                                    */
/*�汾ʱ�� 20080926                                                                							          */
int analog4096(int p)
{ 
  volatile u16 mid;
 if((p>0)&&(p<28))
 {
  if((p>=1)&&(p<=5))
  {
  mid = ADC_GetConvertValueAVE(ADC3 ,(p+3));
  }else if((p>=6)&&(p<=9))
  {
  mid = ADC_GetConvertValueAVE(ADC1 ,(p+4));
  }else if((p >= 10)&&(p<=17))
  {
  mid = ADC_GetConvertValueAVE(ADC1 ,(p-10));
  }else if(p == 18)
  {
  mid = ADC_GetConvertValueAVE(ADC1 ,14);
  }else if(p == 19)
  {
  mid = ADC_GetConvertValueAVE(ADC1 ,15);
  }else if(p == 20)
  {
  mid = ADC_GetConvertValueAVE(ADC1 ,8);
  }else  if((p>20)&&(p<28))
  {
  SA20S(p-21);
  mid = ADC_GetConvertValueAVE(ADC1 ,ADC_Channel_9);
  }
     
  return mid;
 }else{
  return 0 ;
 }
}


/******************************************************************************************
* ��������	: ����ģ��ڵ�ֵ(Ϊ0-255֮�������)
* �������	: p	�����ģ��ͨ�� 1-32
* ������ֵ	: 0-255֮�������
* ���� 120328   : ����Ϊ 1024
*******************************************************************************************/
/*ע��              : p= 0 Ϊ��ص�ѹ�ɼ�ͨ�������ڲ�ʹ��                                    */
/*�汾ʱ�� 20080926          */
int analog(int p)
{ 
  return analog4096(p)>> 2;
}


/******************************************************************************************
* ��������	: ��չ��ģ�����Ķ�ȡ
* �������	: rs485port : 1,2,3; addr:0-127; cha
* ������ֵ	: 0-255֮�������
* ���� 120328   : ����Ϊ 1024
*******************************************************************************************/
/*ע��              : p= 0 Ϊ��ص�ѹ�ɼ�ͨ�������ڲ�ʹ��                                    */
/*�汾ʱ�� 20080926          */
//���ϵ�485 �ӿں�
//addr �װ�ĵ�ַ��
//�װ��ϵ�ͨ����
int e_analog(int rs485port,int addr,int channel)
{
  unsigned short value;
  unsigned short i;
  unsigned int recv_len;
  i = 0;
  
  do{
   i++;
  recv_len = MBSerialReadHoldingRegisters(rs485port, addr, channel, 1, &value);
  if(i>=5)
    return -1;
  } while(recv_len <= 0);
    
  return (int)value;
}

/******************************************************************************************
* ��������	: ��ȡ��Ӧͨ����������ֵ
* �������	: p   1-12
* ������ֵ	: 0   ��ͨ�����߼�ֵΪ0
*                 		  1   ��ͨ�����߼�ֵΪ1
*                           -1  ���������ͨ��ֵ���� 
* ���� 120328  :  p  1- 4
*******************************************************************************************/
/*ע��              :                                                                         								   */
/*�汾ʱ��   : 20080926                                                                							   */
int digital(int p)
{
  GPIO_TypeDef* GPIOx;
  u16 GPIO_Pin ;
  if(p > 0 && p < 10)
  {
  switch(p)
  {
  case 1:
    GPIOx = GPIOB;
    GPIO_Pin = GPIOB_SPI2_SCK_IN1;
    break;
  case 2:
    GPIOx = GPIOB;
    GPIO_Pin = GPIOB_SPI2_MISO_IN2;
    break;
  case 3:
    GPIOx = GPIOB;
    GPIO_Pin = GPIOB_SPI2_MOSI_IN3;
    break;
  case 4:
    GPIOx = GPIOC;
    GPIO_Pin = GPIOC_TIM8_CH4_IN4;
    break;
  
  default :
    break;
  }
 return  GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
  }else {
 return 0;
}
}


/******************************************************************************************
* ��������	: ֹͣһ�����
* �������	: id   ��Ҫֹͣ��������1-3��i2c ��չ��� 4 , 5, 6 , 7 ,8 ,9
* ������ֵ	: 0����ͨ�ųɹ���-1 ����ͨ��ʧ��(���i2c ��չ�����Ч,�˷���ֵ���Բ���)
*******************************************************************************************/
int off(int id)
{
  u8 cmd;
  u8 i2c_w_ret;
  cmd = 2;

  switch(id)
  {
  
  case 1:
          CMOTOR1(0);
          CMOTOR1_2(0);
          break;
  case 2:
          CMOTOR2(0);
          CMOTOR2_2(0);
          break;
  case 3:
          CMOTOR3(0);
          CMOTOR3_2(0);
          break;
  
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
        i2c_w_ret = i2c_WriteBytes(&cmd, id-2, 1, 0);          ////RxBuf[2] = ����д������� ���1 �������������ֹͣ, ��ʱ����
    if(i2c_w_ret < 1)
    {
      led_flash(LED_ON);
      return COM_ERR;
    }
    else
    {
      led_flash(LED_OFF);
      return COM_OK;
    }
        break;
         
          
  default: 
          break;      
  
  }
}


/******************************************************************************************
* ��������	: �Թ��ʼ���p�������m
* �������	: m   ��Ҫ������������1-3��i2c ��չ��� 4 , 5, 6 , 7 ,8 ,9
*		  p    ��Ҫ���õĹ��ʼ���100������ȫ���ʼ���-100
* ������ֵ	: 0����ͨ�ųɹ���-1 ����ͨ��ʧ��(���i2c ��չ�����Ч,�˷���ֵ���Բ���)
*******************************************************************************************/
int motor(int m, int p)
{	
  u8 x;
  u8 y;
  u8 i2c_w_ret;
  u8 sp,cmd;
  u16 absp;
  int op ;     
  op = p;
  sp = p;
  cmd = 1;         // MT_RUN = 1,
  if(op == 0)
  {
		x = 0;
		y = 0;
        }else if(op<0){
		x = 0;
		y = 1;	
	}else{
		x = 1;
		y = 0;
   }
	absp = abs(op)*20 - 2;     // �򵥴��� 2000 - 2 = 1999 20080921
        
   switch(m)                      //��·���ϵߵ������ı��20080920
   	{
       case 3:
	CMOTOR3(x);
	CMOTOR3_2(y);
	SetPWM(MOT3, CMOT3, absp);    // TIM_OCInitStructure.TIM_Channel = TIM_Channel_x;	TIM_Channel_x (x= 0 ,1 ,2,3)
	break;
	
	case 2:
	CMOTOR2(x);
	CMOTOR2_2(y);
	SetPWM(MOT2, CMOT2, absp);
	break;
 
	case 1:
	CMOTOR1(x);
	CMOTOR1_2(y);
	SetPWM(MOT1, CMOT1, absp);    // TIM_OCInitStructure.TIM_Channel = TIM_Channel_x;	TIM_Channel_x (x= 0 ,1 ,2,3)
	break;
	
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
          
    i2c_w_ret = i2c_WriteBytes(&sp, m+4, 1, 0);      ////RxBuf[8] = ����д������� ���1 pwmֵ �����ٷְ�, ƫ��4
    if(i2c_w_ret < 1)
    {
      led_flash(LED_ON);
      return COM_ERR;
    }
    
          
     i2c_w_ret = i2c_WriteBytes(&cmd, m-2, 1, 0);          ////RxBuf[2] = ����д������� ���1 �������������ֹͣ, ��ʱ����
      if(i2c_w_ret < 1)
      {
        led_flash(LED_ON);
        return COM_ERR;
      }
      else
      {
        led_flash(LED_OFF);
        return COM_OK;
      }
          break;
          
	
	default: 
          break;
   }

}

/******************************************************************************************
* ��������	: �ر����е����������stop()һ��
* �������	: void
* ������ֵ	: void
*******************************************************************************************/
/*ע��		:																		   */
/*�汾ʱ��	: 20080926 								*/
void stop()
{
  CMOTOR1(0);
  CMOTOR1_2(0);
  CMOTOR2(0);
  CMOTOR2_2(0);
  CMOTOR3(0);
  CMOTOR3_2(0);
}

/******************************************************************************************
* ��������	: ���÷��������ʱ��
* �������	: msecond ����
* ������ֵ	: void   
*******************************************************************************************/
/*�ڲ�˵��   :                                                                         							           */
/*�汾ʱ��   : 20080923	 */
void beep(int msecond)
{
 int sec;
 int msec;
 
 if(msecond > 60000)
 {
   sec = 59;
   msec = 0;
 }else {
   sec  = msecond/1000;
   msec = msecond%1000;
 }
 GPIO_SetBits(GPIOC, GPIOC_TIM8_CH3_BEEP );
 OSTimeDlyHMSM(0,0,sec,msec);
 GPIO_ResetBits(GPIOC, GPIOC_TIM8_CH3_BEEP );

}

/******************************************************************************************
* ��������	: ָ����״̬��ֵ����ȡ
* �������	: void
* ������ֵ	: 0-359 ֱ�ӵķ�λֵ
*******************************************************************************************/
/*ע��		:																		   */
/*�汾ʱ��	: 20080926 																   */
int compass()
{
  return (int)UART_Receive(USART1)*2;
//return (UART3_ReceiveB()*2);
}


/******************************************************************************************
* ��������	: �ŷ��������
* �������	: s	1-6 �ŷ�������
*		: p  	0-1000	�����ŷ���������ת����
* ������ֵ	: void
* ˵��          :������� ���� 18ms<=F<20ms��������1ms ��ת����λ�ã�1.5ms ��ֹ��2ms ��ת����λ��
*                0.5ms - 2.5ms ֮��
*               : 250 ->1ms  375 ->1.5ms  500 ->2ms
*1000 - 20ms  
*1000 - 20ms     200 - 0.4ms 250 - 0.5ms  500 - 1ms 750 - 1.5ms  1000 - 2ms 1250 - 2.5ms 1350 -2.6ms
*200 -1200 --    Futaba S3003
*��Ӣ�� 0 - 4800 --> 0 - 1000
*******************************************************************************************/
/*ע��		:																		   */
/*�汾ʱ��	: 20080926 																   */
void servo(int s, int p)
{
  int i;
  i = p+250;
  switch(s)
	{
          
        case 1 : SetPWM(TIM8, TIM_Channel_1, i);
        break;
        
        case 2 : SetPWM(TIM8, TIM_Channel_2, i);
        break;
        
 	case 3 : SetPWM(TIM1, TIM_Channel_4, i);
	break; 
        
        case 4 : SetPWM(TIM1, TIM_Channel_3, i);
	break;  
        
        case 5 : SetPWM(TIM1, TIM_Channel_2, i);
	break;
        
        case 6 : SetPWM(TIM1, TIM_Channel_1, i);
	break;
        
        default : 
          break;
          
	}
}


/******************************************************************************************
* ��������	: ģ������ȡ���Ժ���
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
/*ע��		:
/*�汾ʱ��	: 20080926 		*/

void test()
{
  u8 i,j;
  u8 key ;
  u8 key_no;
  u32 u;
  
  	beep(100);
        ClrScreen(0);
        FontSet(0,BLACK);
        PutString(0,0,"ANALOG:1-14");
        PutString(8,16,"1:");
        PutString(72,16,"2:");
        PutString(8,32,"3:");
        PutString(72,32,"4");
        PutString(8,48,"5:");
        PutString(72,48,"6:");
        PutString(8,64,"7:");
        PutString(72,64,"8:");
        PutString(8,80,"9:");
        PutString(72,80,"10:");
        PutString(8,96,"11:");
        PutString(72,96,"12:");
        PutString(8,112,"13:");
        PutString(72,112,"14:");
        
   
        
  for(i=1; i<7; i++)
  {
    servo(i, 500);
  }
    key = 0 ;
  
  while(left_button_read() != 0x00 )  
  { 
    if(middle_button_read()==0x00)
    { 
      {
    //while(push_button_right_read ()==0x00);
      key++ ;
      ClrScreen(0);
      if(key >=4 || key < 0)
      key = 0 ;
      
      switch(key)
      {
       case 0:
        FontSet(0,BLACK);
        PutString(0,0,"ANALOG:1-14");
        PutString(8,16,"1:");
        PutString(72,16,"2:");
        PutString(8,32,"3:");
        PutString(72,32,"4:");
        PutString(8,48,"5:");
        PutString(72,48,"6:");
        PutString(8,64,"7:");
        PutString(72,64,"8:");
        PutString(8,80,"9:");
        PutString(72,80,"10:");
        PutString(8,96,"11:");
        PutString(72,96,"12:");
        PutString(8,112,"13:");
        PutString(72,112,"14:");
	break;
		
       case 1:
	PutString(0,0,"ANALOG:15-27");
        PutString(8,16,"15:");
        PutString(72,16,"16:");
        PutString(8,32,"17:");
        PutString(72,32,"18");
        PutString(8,48,"19:");
        PutString(72,48,"20:");
        PutString(8,64,"21:");
        PutString(72,64,"22:");
        PutString(8,80,"23:");
        PutString(72,80,"24:");
        PutString(8,96,"25:");
        PutString(72,96,"26:");
        PutString(8,112,"27:");
        PutString(72,112,"28:");
	break;
		
      case 2:
        FontSet(0,BLUE);
        PutString(0,0,"Digital:1-4");
        PutString(8,16,"1:");
        PutString(40,16,"2:");
        PutString(72,16,"3:");
        PutString(104,16,"4:");
        FontSet(0,BROWN);
        PutString(0,32,"i2c:");
        FontSet(0,RED);
        PutString(0,48,"uart1:");
        //FontSet(0,RED);
        //PutString(0,64,"uart2:");
        FontSet(0,GREEN);
        PutString(0,80,"RS485:");        
        break;
        
      case 3:
        FontSet(0,RED);
        PutString(0,64,"uart2:");      
        break;
		
      default:
        break;
       }
      }
    }
    
      
     switch(key)
      {
       case 0:
        FontSet(0,BLACK);
	PutIntValue(32,16,analog(1));
        PutIntValue(96,16,analog(2));
        PutIntValue(32,32,analog(3));
        PutIntValue(96,32,analog(4));
        PutIntValue(32,48,analog(5));
        PutIntValue(96,48,analog(6));
        PutIntValue(32,64,analog(7));
        PutIntValue(96,64,analog(8));
        PutIntValue(32,80,analog(9));
        PutIntValue(96,80,analog(10));
        PutIntValue(32,96,analog(11));
        PutIntValue(96,96,analog(12));
        PutIntValue(32,112,analog(13));
        PutIntValue(96,112,analog(14));
	break;
		
       case 1:
	PutIntValue(32,16,analog(15));
        PutIntValue(96,16,analog(16));
        PutIntValue(32,32,analog(17));
        PutIntValue(96,32,analog(18));
        PutIntValue(32,48,analog(19));
        PutIntValue(96,48,analog(20));
        PutIntValue(32,64,analog(21));
        PutIntValue(96,64,analog(22));
        PutIntValue(32,80,analog(23));
        PutIntValue(96,80,analog(24));
        PutIntValue(32,96,analog(25));
        PutIntValue(96,96,analog(26));
        PutIntValue(32,112,analog(27));
        PutIntValue(96,112,analog(28));
	break;
		

      case 2:
        FontSet(0,BLUE);
        Put10Value(24,16,digital(1));
        Put10Value(56,16,digital(2)); 
        Put10Value(88,16,digital(3));
        Put10Value(120,16,digital(4));
        FontSet(0,BROWN);
        PutIntValue(32,32,122);
        FontSet(0,RED);
        PutIntValue(48,48,compass());
        //FontSet(0,RED);
        //PutIntValue(72,64,compass()); 
        //PutIntValue(48,64,digit_recognize());
        
        break;
        
      case 3:
        FontSet(0,RED);
        PutIntValue(48,64,digit_recognize());
        
        /*
        if((USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET))
        {
          j++;
          OSTimeDlyHMSM(0,0,0,1);
          if(j > 6000)
          {
          return 9999;
          } else {
          break;   
          }
         }
        j = 0;
        PutIntValue(48,64,USART_ReceiveData(USART3));
        */
        break;     
		
      default:
        break;
     }
    }
    //ClrScreen(0);
    LCD_Image();
    //PutString(40,40,"Running ... ...");
    beep(100);
}



/******************************************************************************************
* ��������	: ��ѹ����
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
/*ע��		:
/*�汾ʱ��	: 20080926 		*/
//��ص�ѹ 7.2V , ��ѹ���� 683 ��153
//��ѹ������ѹ�����ȡֵ683 153
//(15/83)*7.2 =  1.3V 
//3.3/255 = 1.3/X = 100.4
//X = 95ʱ��Ӧ�ĵ�ѹ�ǣ�6.8V
//x = 90ʱ��Ӧ�ĵ�ѹ�ǣ�6.4V
int valarm(int vleve)
{
  
  if(analog(0) <  95)
  {
    while(1)
    {
      PutString(0,0,"Voltage Low !");
      ClrScreen(0);
      GPIO_SetBits(GPIOD, GPIO_Pin_7);
      msleep(100);
      GPIO_ResetBits(GPIOD, GPIO_Pin_7);
    }
  }else
  
  return -1;

}

/******************************************************************************************
* ��������	: void set_light(int value) ���õƵ���ʾ
* �������	: value 1 ��Ӧ�ĵ���
* ������ֵ	: void
*******************************************************************************************/
void set_light(int value)
{
  if(value)
  GPIO_ResetBits(GPIOD, GPIO_Pin_7);
  else
  GPIO_SetBits(GPIOD, GPIO_Pin_7);
}


/******************************************************************************************
* ��������	: �ϰ�����ȡ
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
int   push_button_up_read()
{
return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_15);

}

/******************************************************************************************
* ��������	: �Ұ�����ȡ
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
int   left_button_read()
{
return GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11);

}

/******************************************************************************************
* ��������	: �м䰴����ȡ
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
int  middle_button_read()
{
return GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15);

}


/******************************************************************************************
* ��������	: ����ʶ��  5s ��û�з�Ӧ�ͷ��� 9999
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
int  digit_recognize()
{
    int i,j,get;
    i = 0;
    get = 9999;
    
    USART_ReceiveData(USART3);  //�������
    
    while((USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET))
    {
      i++;
      OSTimeDlyHMSM(0,0,0,1);
      if(i > 2000)
      {
        return 9999;
      }
    }
    get = USART_ReceiveData(USART3);
    return get ;
}


/******************************************************************************************
* ��������	: ���ڳ�ʼ��
* �������	: 
* ������ֵ	: void
* 20150427      : uart_init(USART3,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,0,0);
*******************************************************************************************/
void uart_init(int port, int baud_rate)
{
  if(port == 1)
  {
  UXART_Init(USART1,baud_rate,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,0,0);
  } else if(port == 2){
  UXART_Init(USART3,baud_rate,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,0,0);
  }
}

/******************************************************************************************
* ��������	: �������ݻ�ȡ
* �������	: 
* ������ֵ	: int
* 20150427      : UXART_Init(USART3,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,0,0);
*******************************************************************************************/

unsigned char uart_read(int port)
{
  if(port == 1)
  {
    return USART_ReceiveData(USART1);
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    {
    }
    
  } else if(port == 2)  {
    return USART_ReceiveData(USART3);
    while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET)
    {
    }
  }
}


/******************************************************************************************
* ��������	: ���ڷ�������
* �������	: 
* ������ֵ	: void 
* 20150427      : 
*******************************************************************************************/
void uart_write(int port ,unsigned char data)
{
  USART_TypeDef* USARTx;
  
  if(port == 1)
  {
    USART_SendData(USART1, data);
     while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
   {
   }
   
  } else if(port == 2) {
    USART_SendData(USART3, data);
     while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
   {
   }
  }
}


/******************************************************************************************
* ��������	: ���ڷ����ַ���
* �������	: 
* ������ֵ	: void 
* 20150427      : 
*******************************************************************************************/
void uart_write_str(int port, char* pdata)
{
	int i;
	for( i = 0; i < strlen(pdata); i++)
	{
		uart_write(port,pdata[i]);
	}
}


/******************************************************************************************
* ��������	: ���ڳ�ʼ��
* �������	: 
* ������ֵ	: void
* 20150427      : UXART_Init(USART3,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,0,0);
*******************************************************************************************/
void rs485_init(int baud_rate)
{
  UXART_Init(UART4,baud_rate,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,0,0);
  GPIO_ResetBits( GPIOA, GPIOA_485_EN2);
}


/******************************************************************************************
* ��������	: ���ڷ���
* �������	: 
* ������ֵ	: void
* 20150427      : 
*******************************************************************************************/
int rs485_read( )
{
  return RS485_Receive(UART4);
}


/******************************************************************************************
* ��������	: ���ڷ���
* �������	: 
* ������ֵ	: void
* 20150427      : 
*******************************************************************************************/
void rs485_write(int data)
{
  RS485_Send(UART4,data);
}

/******************************************************************************************
* ��������	: I2C�������
* �������	: 
* ������ֵ	: void
* 20150427      : 
*******************************************************************************************/
void iic_write(int addr, int reg,int val)
{
  IIC_Start();                                  
  IIC_WriteByte(addr);                      
  IIC_WriteByte(reg);                         
  IIC_WriteByte(val);                         
  IIC_Stop();                               
}


/******************************************************************************************
* ��������	: I2C���ݶ�ȡ
* �������	: addr ��ַ  , reg ��ȡ��ַ�µļĴ���
* ������ֵ	: void
* 20150427      : 
*******************************************************************************************/
int iic_read(int addr, int reg)
{
  u8 b;
  IIC_Start();                                  
  IIC_WriteByte(addr);                 
  IIC_WriteByte(reg);                        
  IIC_Start();                           
  IIC_WriteByte(addr+1);                    
  b = IIC_ReadByte(NO_ACK);                                                
  IIC_Stop();                                 
  return b;
}


/******************************************************************************************
* ��������	: �����õĵ����������
* �������	: 
*	          
* ������ֵ	: ��
*******************************************************************************************/
int e_motor_pars(int addr,int mot_loc, int mot_gear_rato, int cyc_pluse, int wheel_radius)
{
    int recv_len,dev_loc;
    u16 locv[5];
    locv[0] = mot_gear_rato;
    locv[1] = cyc_pluse;
    locv[2] = wheel_radius;
    
    dev_loc = (mot_loc - 1)*100 + EMOT0_GEAR_RATO_ADD;
    
    RecvBuf[SYS_485PORT].act_lenth = 0;
    
    recv_len = MBSerialWriteMultipleRegisters(SYS_485PORT, addr, dev_loc, 3, 6, locv);
    return recv_len;
}


/******************************************************************************************
* ��������	: ���PID����
* �������	: ��֧�� 16λģʽ
*	          
* ������ֵ	: ��
*******************************************************************************************/
int e_motor_pid(int addr,int mot_loc, int pid_p, int pid_i, int pid_d)
{
    int recv_len, dev_loc;
    u16 locv[8];
    locv[0] = pid_p&0x0ffff;
    locv[1] = ((pid_p&0xffff0000) >> 16);
    
    locv[2] = pid_i&0x0ffff;
    locv[3] = ((pid_i&0xffff0000) >> 16);
    
    locv[4] = pid_d&0x0ffff;
    locv[5] = ((pid_d&0xffff0000) >> 16);
    
    dev_loc = (mot_loc - 1)*100 + EMOT0_PID_P;
    
    RecvBuf[SYS_485PORT].act_lenth = 0;
    
    recv_len = MBSerialWriteMultipleRegisters(SYS_485PORT, addr, dev_loc, 6, 12, locv);
    return recv_len;
}


/******************************************************************************************
* ��������	: ������е��ٶ� mm per second  ������ʾ����
* �������	: 
*	          
* ������ֵ	: ��
*******************************************************************************************/
int e_motor(int addr,int mot_loc, int sp_mmps)
{
  int recv_len,dev_loc;
  
  dev_loc = (mot_loc - 1)*100 + EMOT0_SET_SPEED;
  
  RecvBuf[SYS_485PORT].act_lenth = 0;
  
  recv_len = MBSerialWriteSingleRegister(SYS_485PORT, addr, dev_loc, sp_mmps);
    
  dev_loc = (mot_loc - 1)*100 + EMOT0_CMD;
  
  RecvBuf[SYS_485PORT].act_lenth = 0;
  
  recv_len = MBSerialWriteSingleRegister(SYS_485PORT, addr, dev_loc, EMOT0_CMD_RUN);
  
  return recv_len;
    
}

/******************************************************************************************
* ��������	: ��ȡ��������˶��پ���
* �������	: 
*	          
* ������ֵ	: ��
*******************************************************************************************/
int e_motor_mm(int addr,int mot_loc)
{
  u16 recv_mm,dev_loc;
  int rec_len;
  
  dev_loc = (mot_loc - 1)*100 + EMOT0_ALREADY_RUM_MM;
  
  RecvBuf[SYS_485PORT].act_lenth = 0;
  
  rec_len = MBSerialReadHoldingRegisters(SYS_485PORT, addr, dev_loc, 1, &recv_mm);
  
  if(rec_len < 0)
  {
    return rec_len;
  } else {
    return recv_mm;
  }
}


/******************************************************************************************
* ��������	: 
* �������	: 
* ������ֵ	: �� 
*******************************************************************************************/															   
int e_off(int addr,int mot_loc)
{

  u16 recv_len,dev_loc;
  
  dev_loc = (mot_loc - 1)*100 + EMOT0_CMD;

  RecvBuf[SYS_485PORT].act_lenth = 0;
  
  recv_len = MBSerialWriteSingleRegister(SYS_485PORT, addr, dev_loc, EMOT0_CMD_STOP);
  
  return recv_len;

}




/******************************************************************************************
* ��������	: ��ȡʶ���� ������16 ���Ʒ�ʽ�洢���涨 ��0x10 0x11��ͷ  �м�Ϊ 0x00 0x00 ���һλΪ ��������
* �������	: void 
* ������ֵ	: int ����ʶ��ı�ʶ���� �༭��һһ��Ӧ -1��ʾ��û������ -2��ʾͨ�Ŵ���
*******************************************************************************************/
int voice_read(void)
{
  int rec_len;
  
  rec_len = RecvBuf[1].act_lenth;
  
  if(rec_len > 5)
  {
    if(RecvBuf[1].buf[0] == 0x10)
    {
      RecvBuf[1].act_lenth = 0;
      return RecvBuf[1].buf[4];
    }else if(RecvBuf[1].buf[1] == 0x10){    //�����һ�������������
      RecvBuf[1].act_lenth = 0; 
      return RecvBuf[1].buf[5];
    }else {
      return -2;
    }
  }else {
   return -1; 
  }
}


/******************************************************************************************
* ��������	: ����mp3
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
void play_mp3(int loc)
{
  UART_Send(USART1,0xFA);
  UART_Send(USART1,0x5F);
  UART_Send(USART1,0xA5);
  UART_Send(USART1,loc);
}

/******************************************************************************************
* ��������	: ����mp3
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
void speak_sound(unsigned char *speak_buf)
{
  
  
}

#endif

//--- ���ò��� --- ---

/******************************************************************************************
* ��������	: ��������
* �������	: pid 1-5 		�������ĵڼ��Ž���Ҫ������
* ������ֵ	: 0  �����ɹ�
*		: 1 ���̲����ڣ���������
*******************************************************************************************/
/*ע��		:																		   */
/*�汾ʱ��	: 20080926 																   */
/*�汾�޸�      : 20081105  pid��1-4 ��Ϊ1-5 */
/*bug �޸�      : ���ȼ���������ı� */
int kill_process(int pid)
{
  u8 result;
  result = OSTaskDel(pid+2);
  if(result == OS_TASK_DEL_ERR)
  return 1;
  else
  return 0;
}

/******************************************************************************************
* ��������	: ��������
* �������	: pid 1-5 		�������ĵڼ��Ž���Ҫ������
* ������ֵ	: 0  �����ɹ�
*				: 1 ���̲����ڣ���������
*******************************************************************************************/
/*ע��		:																		   */
/*�汾ʱ��	: 20081105															   */
/*�汾�޸�      : 20081105  pid��1-4 ��Ϊ1-5 */
void start_process(int pid)
{
  CreatTask(pid);
}



/******************************************************************************************
* ��������	: ���ú�����Ӧ������
* �������	: loc ӳ���� , pname ��Ӧ������ַ������ַ����ȿ�����14���ַ�
* ������ֵ	: 
*******************************************************************************************/
void set_name(char loc, char *pname)
{
  int i;
  
  fun_name[loc][0]='1'+loc;   //�ӱ��1����ʼ
  fun_name[loc][1]=':';
  
  fun_num = loc + 1;              //��¼�ܵ�fun no ����
  
  for(i = 0; (pname[i]!='\0')&&(i < 14);i++)
  {
    fun_name[loc][i+2] = pname[i];
  }
}

/******************************************************************************************
* ��������	: ѡ��������غ���
* �������	: 
* ������ֵ	: 
*******************************************************************************************/
void select_main(void)
{
  u8 i,j;
  u8 left_key;  
  
#if 1
  
  if(fun_num == 0)    //�����������ʱΪ0 ˵���޶�ѡ����
    return;
  
  if(fun_num == 1)    //���ֻ��һ�� ˵����ֻ��
  { 
     mainX1(); 
  }
   
  ClrScreen(0);
  FontSet(0,RED);
  PutString(0,0,"MainSelect:1-7");
  FontSet(0,BLACK);
  
  for(i = 0; i< fun_num; i++)
  {
    PutString(8,16*(i+1),&fun_name[i][0]);
  }
  left_key = 0;
  
  FontSet(0,RED);
  PutChar(0,16,'S');
  
loop:
  
  while(left_button_read() != 0x00)
  {
    if(middle_button_read() == 0x00)
    {
      OSTimeDlyHMSM(0,0,0,30);
      if(middle_button_read() == 0x00)
      { 
        OSTimeDlyHMSM(0,0,0,20);
        if(middle_button_read() == 0x00)
        {
        while(middle_button_read() == 0x00);
        
        FontSet(0, BLACK); 
        PutChar(0, 16*(left_key +1), ' ');
        
        left_key++;                
        
        if(left_key >= fun_num) // 7
        {
          left_key = 0;
        }
        FontSet(0,RED);
        PutChar(0, 16*(left_key +1), 'S');
       }
      }
    }
    OSTimeDlyHMSM(0,0,0,20);
  }
   
  if(left_button_read() == 0x00)
  {
    OSTimeDlyHMSM(0,0,0,30);
    
    if(left_button_read() == 0x00)
    {
      ClrScreen(0);
      PutString(8,16,"Run Prom:");
      PutString(8,64,&fun_name[left_key][0]);
      
      switch(left_key)
      {
        case 0:
           mainX1();
          break;
       
        case 1:
          mainX2();
          break;
        
        case 2:
          mainX3();
          break;
          
        case 3:
          mainX4();
          break;
        
        case 4:
          mainX5();
          break;
          
        case 5:
          mainX6();
          break;
       
        case 6:
          mainX7();
          break;
        
        default:
          break;
      }
 //     beep(50);
    } else {
      OSTimeDlyHMSM(0,0,0,30);
      goto loop;
    }
    
  }
#endif
}

/******************************************************************************************
* ��������	: ���Ұ�����ȡ
* �������	: 
* ������ֵ	: void
*******************************************************************************************/
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


/******************************************************************************************
* ��������	: ���������OUT�Ӳ������ƽ���߼�����
* �������	: cha 1,2,3,4,5,6,7,8, i2c�����չ���Ӧ 9,10,11,12 h_o_l ����Ǹ߻��ǵ�
* ������ֵ	: ����0����ͨ�ųɹ�������-1����ͨ��ʧ�ܣ�ֻ�����i2c�����չ����Ч�����Բ��ã�
*******************************************************************************************/
int set_digital(int cha , int h_o_l)
{
  if(h_o_l)
  {
   return set_digital_out(cha);
  }
  else
  {
   return clear_digital_out(cha);
  }
}



/******************************************************************************************
* ��������	: �������� ���ΰ�װĬ������Ϊ0
* �������	: 
* ������ֵ	: void 
*******************************************************************************************/
void clr_screen()
{
  ClrScreen(0);
}


/******************************************************************************************
* ��������	: ��ʾ0-1000�ڵ������� value -Ҫ��ʾ��ֵ
* �������	:  x - 0-84 ��y - 0,2,4
* ������ֵ	: void 
*******************************************************************************************/
void put_int_value(unsigned char x,unsigned char y,unsigned short value)
{
  PutIntValue(x,y,value);
}

/******************************************************************************************
* ��������	: ��ʾ�ַ�����p- ��Ӧ���ַ���ָ��
* �������	: x - 0-84 ��y - 0,2,4
* ������ֵ	: void 
*******************************************************************************************/
void put_string(unsigned char x,unsigned char y,char *p)
{

  PutString(x,y,p);
  
}


/******************************************************************************************
* ��������	: ��λϵͳʱ��
* �������	: void
* ������ֵ	: void
*******************************************************************************************/
/*ע��		:																		   */
/*�汾ʱ��	: 20080926 																   */
void reset_system_time(void)
{

  OSTimeSet(0L);

}

/******************************************************************************************
* ��������	: �Ժ�����ʽ����ϵͳ���е�ʱ�����
* �������	: void
* ������ֵ	: ���صĺ���ֵ
*******************************************************************************************/
/*ע��		:																		   */
/*�汾ʱ��	: 20080926 																   */
unsigned int mseconds(void)
{
  return (unsigned int)OSTimeGet();
}



/******************************************************************************************
* ��������	: ������ʽ����ϵͳ���е�ʱ�����
* �������	: void
* ������ֵ	: ���ص���ֵ
*******************************************************************************************/
/*ע��		:																		   */
/*�汾ʱ��	: 20080926 																   */
unsigned int seconds(void)
{
  return (unsigned int)OSTimeGet()/1000;
}

/******************************************************************************************
* ��������	: ��ʱ����
* �������	: sec  ��Ҫ��ʱ������
* ������ֵ	: void
*******************************************************************************************/
/*ע��		:																		   */
/*�汾ʱ��	: 20080926 																   */
void sleep(int sec)
{
 if((sec < 60)&&(sec > 0))
 {
 OSTimeDlyHMSM(0,0,sec,0);
 }else if(sec > 59){
 OSTimeDlyHMSM(0,0,59,0);
 }
}

/******************************************************************************************
* ��������	: ��ʱ������
* �������	: msec ��Ҫ��ʱ�ĺ�����
* ������ֵ	: void
*******************************************************************************************/
/*ע��		:
/*�汾ʱ��	: 20080926 		*/
void msleep(int msec)
{
  if((msec < 1000)&&(msec > 0))
  {
  OSTimeDlyHMSM(0,0,0,msec);
  }else if(msec > 999){
  OSTimeDlyHMSM(0,0,0,999);
  }
}

/******************************************************************************************
* ��������	: ��ʱ����
* �������	: sec  ��Ҫ��ʱ������ 
* ������ֵ	: void
*******************************************************************************************/															  
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

/******************************************************************************************
* ��������	: ʹ��PS2_ǰ����
* �������	: void
* ������ֵ	: void
*******************************************************************************************/	
void ps2_open()
{
#if ROB_0 == 0
    ps2_gpio_cfg();
#endif
    PS2_ShortPoll();
    PS2_ShortPoll();
    PS2_ShortPoll();
    PS2_EnterConfing();		//��������ģʽ
    ps2_cfg(ANALOG, MODE_LOCK);
    PS2_VibrationMode();	//������ģʽ
    PS2_ExitConfing();		//��ɲ���������
    pstwo_use_flag = 1;
}


/******************************************************************************************
* ��������	: ����ɨ�裬��ȡ������Ϣ
* �������	: delay_ms �� ɨ����ʱ��   0 - 999����ֵ̫С�ᵼ�� ��Ӧ̫��������ֵ̫��
                  �ᵼ�£���Ӧ�ٶۣ�����ο���Χ 50 - 500�� ���������ʵ���������
* ������ֵ	: void
*******************************************************************************************/	
void ps2_scan(int delay_ms)
{
	PS2_ClearData();
        msleep(delay_ms+2);
	PS2_ReadData();
}

/******************************************************************************************
* ��������	: ��ȡPS2��Ϣ
* �������	: kind ��2 - 16��������λ��Ϣ,3 - 16���������״ΰ��µİ���,4 - ���ҡ�����ҷ�������
                  5 - ���ҡ�����·�������� 6 - �Ҳ�ҡ�����ҷ�������� 7 - �Ҳ�ҡ�����·��������
* ������ֵ	: ��kind Ϊ2 ʱ������16��������ȫ����Ϣ��1Ϊ����0 Ϊλ��
                  ��kind Ϊ3 ʱ�������״ΰ��µİ�����Ϣ��������Ŷ�Ӧ�μ� PS2������Ӧ����ʹ��
                  ��kind Ϊ4 -- 7 ʱ��������ط�λ�������� 0-255��4 - ���ҡ�����ҷ�������
                  5 - ���ҡ�����·�������ݣ�6 - �Ҳ�ҡ�����ҷ�������� 7 - �Ҳ�ҡ�����·��������
*******************************************************************************************/	
int ps2_read(int kind)
{
	 int key = 0;
	 int ret = 0;
	 int i;
	 switch(kind)
	 {
		 case 1:  //1 - ģʽ���	1-�̵�ģʽ 2-���ģʽ
			 if(Data[1] == 0x41)
			 {
				 ret = 1;
			 }
			 else if(Data[1] == 0x73)
			 {
				 ret = 2;
			 }
			 break;
		 
		 case 2:
			 ret =(Data[4]<<8)|Data[3]; 
			 break;
		 
		 case 3:
		   key =(Data[4]<<8)|Data[3];     //����16������  ����Ϊ0�� δ����Ϊ1
			 for(i = 0;i < 16;i++)
			 {	    
				if((key&(1<<(MASK[i]-1)))==0)
				ret = i + 1;
			 }
			 
		   break;
		 
		 case 4:
			 ret = Data[PSS_LX];
			 break;
		 
		 case 5:
			 ret = Data[PSS_LY];
		   break;
		 
		 case 6:
			 ret = Data[PSS_RX];
		   break;
		 
		 case 7:
			 ret = Data[PSS_RY];
		   break;
		 
		 default:
			 break;
	 }
	 return ret;
}



/******************************************************************************************
* ��������	: �ֱ������
* �������	: vibl:�𶯵�� 0 - 191 �������ֵԽ�� ��Խ��
                  delay_ms �� �ֱ������ʱ��  0 - 999��
* ������ֵ	: void
*******************************************************************************************/	
void ps2_write(int vibration,int delay_ms)
{
   PS2_Vibration(0,vibration+0x40);
   msleep(delay_ms+2);
}


/******************************************************************************************
* ��������	: PS2ʹ����Ϻ�����˿��л�������������״̬
* �������	: void
* ������ֵ	: void
*******************************************************************************************/	
void ps2_close()
{
#if ROB_0 == 0
   GPIO_Configuration();
#endif
  pstwo_use_flag = 0;
}






