
/* Includes ------------------------------------------------------------------*/
#include <includes.h>

/* Private typedef -----------------------------------------------------------*/
//typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#if 1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/  
//u32 EraseCounter = 0x0, Address = 0x0;
//u32 Data;
//vu32 NbrOfPage = 0;

//volatile TestStatus MemoryProgramStatus;


/* Private function prototypes -----------------------------------------------*/
void SetParms(void);
void DispAllParms(void);
int  GetRemData(int Ax);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
* 注意 屏蔽了jtag 在GPIO中
*******************************************************************************/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))
/* Private variables ---------------------------------------------------------*/
u8 Tx1_Buffer[] = {1,2,3};
u8 Tx2_Buffer[] = "Library Example */";

#define BufferSize1             (countof(Tx1_Buffer)-1)
#define BufferSize2             (countof(Tx2_Buffer)-1)

extern  unsigned char gImage_logo[];

u8 Rx1_Buffer[BufferSize1+1], Rx2_Buffer[BufferSize2];
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

int rec[1000];


void mainX1(void)
{
  
}

void mainX2(void)
{
  
}

void mainX3(void)
{
  
}

void mainX4(void)
{
  
}

void mainX5(void)
{
  
}

void mainX6(void)
{
  
}


void mainX7(void)
{
  
}


//ErrorStatus HSEStartUpStatus;
// 内要有delay
void mainX(void *p_arg)
{ 
int gvar2; int gvar3; int gvar4; 
  u16 data;
  u16 value[2], get_value;
  u16 i,j;
  u32 x,y;
  int get , recv_len;
  int rs485port = 2, addr = 10,  channel = 1000;
  void *target;
 
  x = 0;
  y = 0;
 
#if ROB_0
    analog(1);
    digital(1);
    key(1);
    beep(1,1);
    set_digital(1,1);
    mot_off(1);
    servo(1,1);
    uart_read();
    uart_write(1);
    clear_lcd( );
    display_int(1,1 ,1);
    display_str(1, 1,"1");
    stop( );
    mot_off(1 );
    motor(1  ,1 );
    servo(1 ,1);
    beep( 1, 23);
    uart_init(9600);
    uart_read( );
    uart_write( 1);
    reset_system_clock( );
    mseconds( );
    seconds( );
    delay(1 ,1);
#endif
  
  set_name(MAINX1,"");
  set_name(MAINX2,"");
  set_name(MAINX3,"");
  set_name(MAINX4,"");
  set_name(MAINX5,"");
  set_name(MAINX6,"");
  set_name(MAINX7,"");
  
  select_main();
  
  
  //....
  
  //sel_run();
  
   clear_lcd( );
    while(60)
    {
        if(key(1))
        {
            display_str(1,2,"1");
        }
        else
        {
            display_str(1,2,"2");
        }
    }
  
  
//  test();
//  uart_write(2, 1);
  
//  uart_write(2, 2);
#if 0
  get = -1;
  
  play_mp3(1);
  
  play_mp3(2); 

  GPIO_SetBits( GPIOA, GPIOA_485_EN2);
  
  rs485_write(1);
  
  rs485_write(2);
   
  while(1)
  {

#if 0
    recv_len = e_motor_pars(addr,1, 123, 456, 78);
    
    if(recv_len >= 0)
    {
      uart_write(2, 1);
    }
    
  
    recv_len = e_motor_pid(addr,1, 234, 567,89);
    
    if(recv_len >= 0)
    {
      uart_write(2, 2);
    }  
    
    
    recv_len =  e_motor(addr,1, 2000);
    
     if(recv_len >= 0)
    {
      uart_write(2, 3);
    }    

    
    
    recv_len =  e_motor_mm(addr, 1);
    
    
    if(recv_len >= 0)
    {
      uart_write(2, 4);
    }

#endif    

    get = GetVoiceIndex();
    
    switch(get)
    {
    case 2:
      e_off(addr, 1);
      break;
      
    case 3:
      e_motor(addr,1, 1000);
      break;
      
    case 4:
    case 5:
    case 6:
      e_off(addr, 1);
      e_motor(addr,1, -1000);
      OSTimeDlyHMSM(0,0,5,0);
      e_off(addr, 1);
      break;
   
    case 7:
    case 8:
    case 9:
    case 10:
      e_off(addr, 1);
      e_motor(addr,1, 1000);
      OSTimeDlyHMSM(0,0,3,0);
      e_off(addr, 1);
      break;
   

   case 11:
      e_off(addr, 1);
      e_motor(addr,1, 3000);
      break;
      
   case 12:
      e_off(addr, 1);
      e_motor(addr,1, 200);
      break;  
    
    default:
      break;
    }
  OSTimeDlyHMSM(0,0,0,100);
    
  }
  

  servo(1,100);
  servo(2,800);
  servo(3,100); 
  servo(4,200);
  servo(5,400);
  servo(6,900);
 
  motor( 1, 99);
  motor( 2, 10);
  //Show_Image(gImage_logo);
#if 1
  while(1)
  {
    motor( 1, 100);

    msleep( 900);
    
    motor( 1, -100);

    msleep( 900);
  }
#endif  
  

  
  //Gui_DrawFont_GBK16(4, 4, RED, GBLUE, "Tel:15989313508");
    for(i = 0; i < 999; i++)
    {
    rec[i] = 111;
    } 
  
  while(1)
  {
 
   //get = digit_recognize();
   //USART_SendData(USART3,get);  //清空数据

    OSTimeDlyHMSM(0,0,0,100);
  }
  /*  10ms 测试
   while(1)
   {
   GPIO_ResetBits(GPIOG, GPIO_Pin_8);
   OSTimeDlyHMSM(0,0,0,10);
   GPIO_SetBits(GPIOG, GPIO_Pin_8);
   OSTimeDlyHMSM(0,0,0,10);
   
   }
   */
   
  /* 100ms test 
  while(1)
  {
  
  x =  OSTimeGet();
 
  if(x>=100)
  {
    
    GPIO_ResetBits(GPIOG, GPIO_Pin_8);
    if(x == 200)
    {
    OSTimeSet(0L);
     x = 0;
    }
  }
  else
  {
   
    GPIO_SetBits(GPIOG, GPIO_Pin_8);
    
  }
    
  }
   */
 /* 
  for(data = 0;data < 100;data++)
  {
//  RS485Send(1, data);
//  RS485Send(2, data);
   RS485Send(3, data);
//  RS485Send(1, data);
   if(data > 90)
    data = 0;
  OSTimeDlyHMSM(0,0,0,5);
  }*/
  i=0;
  j=0;
  while(1)
  {
    int recv_len;
    GPIO_ResetBits(GPIOG, GPIO_Pin_8); 
      
    data = value[0];
   // recv_len = MBSerialReadHoldingRegisters(1, 0x08, 1, 1, value);
    recv_len = e_analog1024(1,0x08,1);
    if(recv_len < 0)
    {
    j++;
      PutIntValue(0,2, j);
    if(j>65530)
      j=0;
    }
    
    if(recv_len > 0)
    {
    i++;
    PutIntValue(0,4, i);
    if(i> 65530)
      i=0;
    }
    data = value[0];
    data = data;
    PutIntValue(0,0, recv_len);
    GPIO_SetBits(GPIOG, GPIO_Pin_8); 
  }

//  Test();
//  RS485Send(1, recv);
/*
  while(1)
  {
  
  recv = RS485Receive(1);
//  PutIntValue(0,0,recv); 
  recv = 2;
  RS485Send(1, recv);
  RS485Send(1, recv);
  RS485Send(1, recv);
  msleep(5);
    
  
  PutIntValue(0,0,RS485_Receive(UART4));
  PutIntValue(0,0,RS485_Receive(UART5));
  
  }
   
  int dat;
  
   {
  ADC_GetConvertValue(ADC1 ,ADC_Channel_10 );
  SetPWM(TIM1, TIM_Channel_1, 1);
  SetPWM(TIM1, TIM_Channel_2, 100);
  SetPWM(TIM1, TIM_Channel_3, 5000);
  SetPWM(TIM1, TIM_Channel_4, 9999);
  
  SetPWM(TIM4, TIM_Channel_1, 1000);
  SetPWM(TIM4, TIM_Channel_2, 1000);
  SetPWM(TIM4, TIM_Channel_3, 1000);
  SetPWM(TIM4, TIM_Channel_4, 1000);
   }
 //while(1)
 {
   motor(1,80);
   motor(1,-80);
   motor(2,80);
   motor(2,-80);
   motor(3,80);
   motor(3,-80);
   motor(4,80);
   motor(4,-80);
   motor(5,80);
   motor(5,-80);
   motor(6,80);
   motor(6,-80);
   
   set_digital_out(1);
   set_digital_out(2);
   set_digital_out(3);
   set_digital_out(4);
   set_digital_out(5);
   
   set_digital_out(6);
   set_digital_out(7);
   set_digital_out(8);
   set_digital_out(9);
   set_digital_out(10);
   
   clear_digital_out(1);
   clear_digital_out(2);
   clear_digital_out(3);
   clear_digital_out(4);
   clear_digital_out(5);
 
   clear_digital_out(6);
   clear_digital_out(7);
   clear_digital_out(8);
   clear_digital_out(9);
   clear_digital_out(10);
   
   dat = analog(1);
   dat = analog(2);
   dat = analog(3);
   dat = analog(4);
   dat = analog(5);
   
   dat = analog(6);
   dat = analog(7);
   dat = analog(8);
   dat = analog(9);
   dat = analog(10);
   
   dat = analog(11);
   dat = analog(12);
   dat = analog(13);
   dat = analog(14);
   dat = analog(15);
   
   dat = analog(16);
   dat = analog(17);
   dat = analog(18);
   dat = analog(19);
   dat = analog(20);
   
   dat = analog(21);
   dat = analog(22);
   dat = analog(23);
   dat = analog(24);
   
   dat = analog(25);
   dat = analog(26);
   dat = analog(27);
   dat = analog(28);
   
  dat = digital(1);
  dat = digital(2);
  dat = digital(3);
  dat = digital(4);
  dat = digital(5);
  dat = digital(6);
  dat = digital(7);
  
 // beep(20);
  
 }  
 
   beep(1);
   beep(2);
   
 while(1){
  
        Put100Value(0,0,UARTReceive());
 }
 // Test();
*/
#endif
}

#endif


