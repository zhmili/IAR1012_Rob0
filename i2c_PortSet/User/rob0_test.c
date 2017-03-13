#include "rob0_test.h"
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_rob0.h"
#include "lcd_z144.h"
#include "rob0.h"
#include <stdlib.h>

extern TGPIO_STATE GPIO_STATE[16];
char rx_flag;

unsigned char uart_rec;
unsigned char set_ready;
unsigned char uart_buf[6];
unsigned char get_lenth;
unsigned int get_id;

/******************************************************************************************
* 函数功能	: scanport和judge的测试函数，打印出读取到的AD值、高电平、电平及判断出的传感器类别
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
void judge_test(void)		//test ok 20160816
{
	int i;
		printf("OUT上拉：");
		for(i=0;i<11;i++)
		printf(" %d",GPIO_STATE[i].out_pu);
		printf("\n\n");
		
		printf("OUT下拉：");
		for(i=0;i<11;i++)
			printf(" %d",GPIO_STATE[i].out_pd);		
		printf("\n\n");
		
		printf("TIM上拉：");
		for(i=0;i<11;i++)
			printf(" %d",GPIO_STATE[i].tim_pu);	
		printf("\n\n");
		
		printf("OUT下拉：");
		for(i=0;i<11;i++)
			printf(" %d",GPIO_STATE[i].tim_pd);	
		printf("\n\n");
		
		printf("EX_AD  ：");
		for(i=0;i<16;i++)
			printf(" %d",GPIO_STATE[i].ex_ad_lev);
		printf("\n\n");
		
		printf("RJ_AD  ：");
		for(i=0;i<11;i++)
			printf(" %d", GPIO_STATE[i].ad_lev);
		printf("\n\n");
		
		printf("M_NUM  ：");
		for(i=0;i<13;i++)
			printf(" %d  ",GPIO_STATE[i].h_num);
		printf("\n\n");
			
		delay(0x5fffff);
	
}

/******************************************************************************************
* 函数功能	: 测试OUT脚的数字高低输出函数
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
void digital_out_test(int cha)		//test ok 20160816
{
	set_digital(cha,1);		//OK
	delay(0xfffff);
	
	set_digital(cha,0);	//OK
	delay(0xfffff);
}

/******************************************************************************************
* 函数功能	: 测试模拟量读取
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
void analog_test(int cha)		//test ok 20160816
{
	printf("%d  ",analog(cha));		//OK	
	delay(0xff);
}

/******************************************************************************************
* 函数功能	: 测试数字量读取
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
void digital_test(int cha)		//test ok 20160816
{
	printf("数字量 %d\n",digital(cha));		//OK	
	delay(0xff);
	
}

/******************************************************************************************
* 函数功能	: 测试按键读取函数
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
void button_test(void)		//test ok 20160816
{
	printf("left key %d \n",left_button_read());	//ok
	printf("midd key %d \n",middle_button_read());	//ok
}
			
/******************************************************************************************
* 函数功能	: 测试servo舵缓
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
void servo_test(int cha)		//test ok 20160816
{
	servo(cha, 250);//ok
}


void uart_test()
{		
	int temp;
	
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		temp=uart_read();//ok
		rx_flag=1;
	}
	
	if(rx_flag==1)
	{	
		rx_flag=0;
		uart_write(temp);//ok
	}
	
}

/******************************************************************************************
* 函数功能	: moto1相关函数测试函数
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
void moto1_test(int cha)		//test IO电平及PWM正常，电机不正转，应该是驱动板问题20160818
{
	if(left_button_read()==1)				//
	{
		motor(cha, 100);
		delay(0xffff);
	}
	
	if(middle_button_read()==1)
	{
		stop();
	}
}
/******************************************************************************************
* 函数功能	: moto2相关函数测试函数
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
void moto2_test(int cha)		//test ok 20160818
{
	if(left_button_read()==1)				//
	{
		motor(cha, -100);
		delay(0xffff);	
	}
	
	if(middle_button_read()==1)
	{
		off(cha);
	}
}
  u8 out_pu;
  u8 out_pd;
  u8 tim_pu;
  u8 tim_pd;
  
  u16  ad_value;
  u16  ex_ad_value;
  
  u8 ad_lev;
  u8 ex_ad_lev;
  
  u8 h_num;               //0 表示没有接模块
  u8 l_num;

void port_state()
{
	int i;
	for(i = 0; i < RJ_NUM; i++)
	{
	printf("ch_%d: out_pu_%d, out_pd_%d,tim_pu_%d, tim_pd_%d, ad_value_%d, ex_ad_value_%d,ad_lev_%d,ex_ad_lev_%d,h_num_%d,l_num_%d\n\r", \
	i,GPIO_STATE[i].out_pu, GPIO_STATE[i].out_pd,  GPIO_STATE[i].tim_pu, GPIO_STATE[i].tim_pd, GPIO_STATE[i].ad_value,GPIO_STATE[i].ex_ad_value,GPIO_STATE[i].ad_lev,GPIO_STATE[i].ex_ad_lev,GPIO_STATE[i].h_num,GPIO_STATE[i].l_num);
	}
}


void hard_test()
{
	#if 1
	printf("help           :  ?  \n\r");
	printf("scan test      :  '1'  \n\r");
	printf("judge test      :  '2'  \n\r");
	printf("input test     :  '3'  \n\r");    
	//printf("dig out 1 test :  '4'  \n\r");
	//printf("dig out 0 test :  '5'  \n\r");
	printf("moto test forward :   '6'  \n\r");// 
	printf("moto test reverse :   '7'  \n\r");// 
	//printf("servo test forward :  '8'  \n\r");
	//printf("servo test reverse :  '9'  \n\r");
	printf("get port state :  'a'  \n\r");
	//printf("jtag release :  'b'  \n\r");
	//printf("jtag enable :  'c'  \n\r");
	
	uart_rec = 0;
	while(1)
	{
		switch(uart_rec)
		{
			case 0:
				break;
			
			case '?':
				printf("help           :  ?  \n\r");
				printf("scan test      :  '1'  \n\r");
				printf("judge test      :  '2'  \n\r");
				printf("input test     :  '3'  \n\r");
				printf("dig out 1 test :  '4'  \n\r");
				printf("dig out 0 test :  '5'  \n\r");
				printf("moto test forward :   '6'  \n\r");
				printf("moto test reverse :   '7'  \n\r");
				printf("servo test forward :  '8'  \n\r");
				printf("servo test reverse :  '9'  \n\r");
				printf("get port state :  'a'  \n\r");		
				printf("jtag release :  'b'  \n\r");
				printf("jtag enable :  'c'  \n\r");
				uart_rec = 0; 
				break;
			
		case '1':
			scan_port();
		  uart_rec = 0; 
				break;	
			
			case '2':
			 judge();
			 uart_rec = 0; 
				break;
			
			case '3':
				  printf("input test \n\r");
					scan_sim(AD_IN_10, 0);
					test_sim_reverse();
					uart_rec = 0;
				break;
			
			case '4':
					printf("dig out  1 text \n\r");
					scan_sim(DIG_OUT_PUT_1, 0);
					test_sim_reverse();
			    uart_rec = 0; 
				break;
			
			case '5':
				  printf("dig out  0 text \n\r");
				  scan_sim(DIG_OUT_PUT_1, 0);
				  test_sim_reverse();
			    uart_rec = 0;
				break;
			
		  case '6':
					printf("moto test forward \n\r");
					scan_sim(BRUSH_MOT_3, 0);
					test_sim_forward();
          uart_rec = 0;			
				break;
		
			 case '7':
					printf("moto test reverse \n\r");
					scan_sim(BRUSH_MOT_3, 0);
					test_sim_reverse();	
			 uart_rec = 0;
				break;
		
			 case '8':
					printf("servo test forward \n\r");
					scan_sim(SERVO_MOT_5, 0);
					test_sim_forward();
			 uart_rec = 0;
				break;
			 
			 case '9':
					printf("servo test reverse \n\r");
					scan_sim(SERVO_MOT_5, 0);
					test_sim_reverse();
			 uart_rec = 0;
				break;
			 
			 case 'a':
				 port_state();
			 uart_rec = 0;
				 break;
			 
			 case 'b':
				 release_jtagswd();
			 uart_rec = 0;
				 break;
			 
			 case 'c':
				 GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);	
			 uart_rec = 0;
				 break;
			 
			default:
				uart_rec = 0;
				break;
		}
		
	}	
	#endif
}


void code_test()
{
	#if 1
	char seeprom_addr[4];
	char seeprom_data[4];
	int  ieeprom_addr;
	uint8_t  ReadBuf[10];
	uint8_t  ieeprom_data[4] = {234,115,101,90};
	int ret;
	#if 0
	printf("help           :  ?  \n\r");
	printf("scan test      :  '1'  \n\r");
	printf("judge test      :  '2'  \n\r");
	printf("input test     :  '3'  \n\r");    
	printf("dig out 1 test :  '4'  \n\r");
	printf("dig out 0 test :  '5'  \n\r");
	printf("moto test forward :   '6'  \n\r");// 
	printf("moto test reverse :   '7'  \n\r");// 
	printf("servo test forward :  '8'  \n\r");
	printf("servo test reverse :  '9'  \n\r");
	printf("get port state :  'a'  \n\r");
	printf("jtag release :  'b'  \n\r");
	printf("jtag enable :  'c'  \n\r");
	#endif
	printf("i2c write :  'd', set data rang: 9999E, please with E end \n\r");
	printf("i2c read :  'e'  \n\r");
	uart_rec = 0;
	
	while(1)
	{
		switch(uart_rec)
		{
			case 0:
				break;
			
			case '?':
				#if 0
				printf("help           :  ?  \n\r");
				printf("scan test      :  '1'  \n\r");
				printf("judge test      :  '2'  \n\r");
				printf("input test     :  '3'  \n\r");
				printf("dig out 1 test :  '4'  \n\r");
				printf("dig out 0 test :  '5'  \n\r");
				printf("moto test forward :   '6'  \n\r");
				printf("moto test reverse :   '7'  \n\r");
				printf("servo test forward :  '8'  \n\r");
				printf("servo test reverse :  '9'  \n\r");
				printf("get port state :  'a'  \n\r");		
				printf("jtag release :  'b'  \n\r");
				printf("jtag enable :  'c'  \n\r");
		    #endif
			  printf("i2c write :  'd'  \n\r");
			  printf("i2c read :  'e'  \n\r");
				uart_rec = 0; 
				break;
			
		 case '1':
			printf("scan test      :  '1'  \n\r");
			scan_port();
		  uart_rec = 0; 
				break;	
			
			case '2':
			printf("judge test      :  '2'  \n\r");	
			 judge();
			 uart_rec = 0; 
				break;
			
			case '3':
			printf("input test \n\r");
			scan_sim(AD_IN_10, 0);
			test_sim_reverse();
			uart_rec = 0;
				break;
			
			case '4':
					printf("dig out  1 text \n\r");
					scan_sim(DIG_OUT_PUT_1, 0);
					test_sim_reverse();
			    uart_rec = 0; 
				break;
			
			case '5':
				  printf("dig out  0 text \n\r");
				  scan_sim(DIG_OUT_PUT_1, 0);
				  test_sim_reverse();
			    uart_rec = 0;
				break;
			
		  case '6':
					printf("moto test forward \n\r");
					scan_sim(BRUSH_MOT_3, 0);
					test_sim_forward();
          uart_rec = 0;			
				break;
		   
			 case '7':
					printf("moto test reverse \n\r");
					scan_sim(BRUSH_MOT_3, 0);
					test_sim_reverse();	
			 uart_rec = 0;
				break;
		
			 case '8':
					printf("servo test forward \n\r");
					scan_sim(SERVO_MOT_5, 0);
			    
					test_sim_forward();
			 uart_rec = 0;
				break;
			 
			 case '9':
					printf("servo test reverse \n\r");
					scan_sim(SERVO_MOT_5, 0);
			
					test_sim_reverse();
			 uart_rec = 0;
				break;
			 
			 case 'a':
				 printf("get port state :  'a'  \n\r");	
				 port_state();
			 uart_rec = 0;
				 break;
			 
			 case 'b':
				 printf("jtag release :  'b'  \n\r");
				 release_jtagswd();
			 uart_rec = 0;
				 break;
			 
			 case 'c':
				 printf("jtag enable :  'c'  \n\r");
				 GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);	
				 uart_rec = 0;
				 break;
			 
			 case 'd':
				 uart_rec = 0;
				 printf("i2c write at port1 \n\r");
				 printf("i2c write :  'd'  \n\r");
			   uart_buf[0] = 0;
			   uart_buf[1] = 0;
			   uart_buf[2] = 0;
			   uart_buf[3] = 0;
			   uart_buf[4] = 0;
				 get_lenth = 0;
			   set_ready = 0;
			   get_id = 0;
			   
			   while( set_ready == 0);			   
			 //  get_id = 0x02142;
			//   printf("please put in eeprom_addr:  \n\r");
			   printf("set id:%d \n\r",get_id);
			//   printf("please put write in data  \n\r");
			//   scanf("%s", seeprom_data);
			//   ieeprom_addr = atoi(seeprom_addr);
			//   ieeprom_data = atoi(seeprom_data);
			   
			   s4_0_set(0);
			   GPIO_ResetBits(EX_APORT,EX_APIN);
			   pin_cfg(GPIO_STATE[0].out_port , GPIO_STATE[0].out_pin ,GPIO_Mode_Out_OD, GPIO_Speed_50MHz);
				 pin_cfg(GPIO_STATE[0].tim_port , GPIO_STATE[0].tim_pin ,GPIO_Mode_Out_OD, GPIO_Speed_50MHz);
				 
				 uart_buf[0] = (unsigned char)(get_id&0x0ff);
				 uart_buf[1] = (unsigned char)(get_id>>8);
				 
				 printf("high 8bit:%d, low 8bit:%d \n\r",uart_buf[1],uart_buf[0]);
			   ret = ee_WriteBytes(uart_buf,  0, 2, 0);// addr0 - low8bit , addr1 - high8bit 
				 
			   if(ret == 0)
				 {
					 printf("ee_WriteBytes err  \n\r");
				 }
				 else
				 {
					ee_Delay(0x0FFFFF);
					ee_Delay(0x0FFFFF);
					ret = get_i2c_id(0);
					printf("read id: %d  \n\r",ret);
				 }
				 
				 uart_buf[0] = 0;
			   uart_buf[1] = 0;
			   uart_buf[2] = 0;
			   uart_buf[3] = 0;
				 uart_buf[4] = 0;
				 get_lenth = 0;
				 uart_rec = 0; 
				 get_id = 0;
				 break;
				 
			 case 's':
				 uart_rec = 0;
				 printf("i2c write at port1 \n\r");
				 printf("i2c write :  'd'  \n\r");
			   uart_buf[0] = 0;
			   uart_buf[1] = 0;
			   uart_buf[2] = 0;
			   uart_buf[3] = 0;
			   uart_buf[4] = 0;
				 get_lenth = 0;
			   set_ready = 0;
			   get_id = 0;
			   
			   while( set_ready == 0);			   
			 //  get_id = 0x02142;
			//   printf("please put in eeprom_addr:  \n\r");
			   printf("set id:%d \n\r",get_id);
			//   printf("please put write in data  \n\r");
			//   scanf("%s", seeprom_data);
			//   ieeprom_addr = atoi(seeprom_addr);
			//   ieeprom_data = atoi(seeprom_data);
			   
			   s4_0_set(0);
			   GPIO_ResetBits(EX_APORT,EX_APIN);
			   pin_cfg(GPIO_STATE[0].out_port , GPIO_STATE[0].out_pin ,GPIO_Mode_Out_OD, GPIO_Speed_50MHz);
				 pin_cfg(GPIO_STATE[0].tim_port , GPIO_STATE[0].tim_pin ,GPIO_Mode_Out_OD, GPIO_Speed_50MHz);
				 
				 uart_buf[0] = (unsigned char)(get_id&0x0ff);
				 uart_buf[1] = (unsigned char)(get_id>>8);
				 
				 printf("high 8bit:%d, low 8bit:%d \n\r",uart_buf[1],uart_buf[0]);
			   ret = i2c_WriteBytes(uart_buf,  0, 2, 0);// addr0 - low8bit , addr1 - high8bit 
				 
			   if(ret == 0)
				 {
					 printf("ee_WriteBytes err  \n\r");
				 }
				 else
				 {
					ee_Delay(0x0FFFFF);
					ee_Delay(0x0FFFFF);
					ret = get_i2c_id(0);
					printf("read id: %d  \n\r",ret);
				 }
				 
				 uart_buf[0] = 0;
			   uart_buf[1] = 0;
			   uart_buf[2] = 0;
			   uart_buf[3] = 0;
				 uart_buf[4] = 0;
				 get_lenth = 0;
				 uart_rec = 0; 
				 get_id = 0;
				 break;	 
				 
			 case 'e':
				 printf("i2c read at port0 \n\r");
				 printf("i2c read :  'e'  \n\r");

         GPIO_ResetBits(EX_APORT,EX_APIN);
			   pin_cfg(GPIO_STATE[0].out_port , GPIO_STATE[0].out_pin ,GPIO_Mode_Out_OD, GPIO_Speed_50MHz);
				 pin_cfg(GPIO_STATE[0].tim_port , GPIO_STATE[0].tim_pin ,GPIO_Mode_Out_OD, GPIO_Speed_50MHz);
			   
			   ret = get_i2c_id(0);
			   if(ret < 0)
				 {
					 printf("ee_ReadBytes err  \n\r");
				 }
				 else
				 {
					 printf("ee_ReadBytes ：%d \n\r",ret);
				 }
				 uart_rec = 0; 
				 break;
			
			default:
				 uart_rec = 0;
				break;
		}
		
	}	
	#endif
}


void scan_sim(int h_num, int l_num)
{
	int i;
	for(i = 0; i < COM_CH_NUM ; i++)
	{
	  GPIO_STATE[i].h_num = h_num;
		GPIO_STATE[i].l_num = l_num;
	}
	
	pin_final_cfg();
	
}




void test_sim_reverse()
{
	int i = 0;
	for(i = 0; i < COM_CH_NUM; i++)
	{
		switch(GPIO_STATE[i].h_num)
		{
#if 0
			case NO_MODE_0:
				break;
			
			case DIG_OUT_PUT_1:
				set_digital(i+1,0);
				break;
			
			case SPI_2:
				break;
			
			case RES_4:
				break;
			
			case BRUSH_MOT_3:
				motor(i+1, 80);
				break;
			
			case SERVO_MOT_5:
				servo(i+1, 500);
				break;
			
			case STEP_MOT_6:
				break;
			
			case RGB_LED_7:
				break;
			
			case REC_PLY_SOUND_8:
				break;
			
			case BEEP_9:
				break;
			
			case RES_10:
				break;
		
			case ULT_DIS_11:
				break;
			
			case TEMP_12:
				break;	
			
			case RES_13:
				break;
			
			case I2C_14:
				break;
			
			case RES_15:
				break;			
	
			case RES_16:
				break;	

			case RES_17:
				break;	
			
			case RES_18:
				break;	

			case AD_IN_19:
				
				break;
      
		  case RES_20:
				break;
			
		  case RES_21:
				break;
			
		  case LIGHT_22:
				break;			
			
		  case INF_IND_23:
				break;
			
		  case SOUND_CTRL_24:
				break;


		  case INF_DIS_25:
				break;	

		  case MAG_IND_26:
				break;
			
	    case RS232_28:
				break;	
			
		  case RS485_29:
				break;		
#endif
		}
	}
}

void test_sim_forward()
{
	int i = 0;
	for(i = 0; i < COM_CH_NUM; i++)
	{
#if 0
		switch(GPIO_STATE[i].h_num)
		{
			case NO_MODE_0:
				break;
			
			case DIG_OUT_PUT_1:
				set_digital(i+1,1);
				break;
			
			case SPI_2:
				break;
			
			case RES_4:
				break;
			
			case BRUSH_MOT_3:
				motor(i+1, -80);
				break;
			
			case SERVO_MOT_5:
				servo(i+1, 100);
				break;
			
			case STEP_MOT_6:
				break;
			
			case RGB_LED_7:
				break;
			
			case REC_PLY_SOUND_8:
				break;
			
			case BEEP_9:
				break;
			
			case RES_10:
				break;
		
			case ULT_DIS_11:
				break;
			
			case TEMP_12:
				break;	
			
			case RES_13:
				break;
			
			case I2C_14:
				break;
			
			case RES_15:
				break;			
	
			case RES_16:
				break;	

			case RES_17:
				break;	
			
			case RES_18:
				break;	

			case AD_IN_19:
				
				break;
      
		  case RES_20:
				break;
			
		  case RES_21:
				break;
			
		  case LIGHT_22:
				break;			
			
		  case INF_IND_23:
				break;
			
		  case SOUND_CTRL_24:
				break;


		  case INF_DIS_25:
				break;	

			
		  case MAG_IND_26:
				break;
			
	    case RS232_28:
				break;	
			
		  case RS485_29:
				break;		
		}
#endif
	}
}






/******************************************************************************************
* 函数功能	: test集合
* 输入参数	: 
* 返回数值	: void
*******************************************************************************************/
void test_all(void)
{
	judge_test();
	digital_out_test(11); //模拟NUM=1  输出模块 0 0 0 0 ok
	digital_test(10);			//模拟NUM=12  DS18B20 1 1 0 2	ok
	button_test();				//OK
	servo_test(9);				//模拟NUM=5  舵机模块 0 0 1 1 ok
	moto1_test(8);				//电机模块OK
	moto2_test(3);				//电机模块OK
}
