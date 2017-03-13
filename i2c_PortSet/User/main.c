
// ADC 单通道采集

#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_rob0.h"
#include "lcd_z144.h"
#include "rob0.h"
#include "rob0_test.h"

extern TGPIO_STATE GPIO_STATE[16];

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
	unsigned char temp;
	int i;
	//release_jtagswd();
	//delay(0xffffff);
	bsp_rob0_init();
	
	//scan_port();
	//judge();
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);	
	//
	s4_0_set(0);
	pin_final_cfg();
	uart_init(115200);
	UXART_Init(UART5,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,1,0);//460800
	
	#if 0
		lcd_config();
		LCD_Init();
		LCD_Fill(GREEN);	//
		LCD_Image();
		delay(0xffff);
	#endif
	  //while (1)
	{
		//test_all();
	}
	
 	//scan_sim(BRUSH_MOT_3, 0);
	//test_sim_forward();
	
	//pin_com_cfg();
	//uart_init(115200);
	//UXART_Init(UART5,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,1,0);//460800
	#if 1
		s4_0_set(0);
	  GPIO_ResetBits(EX_APORT,EX_APIN);
		pin_cfg(GPIO_STATE[0].out_port , GPIO_STATE[0].out_pin ,GPIO_Mode_Out_OD, GPIO_Speed_50MHz);
		pin_cfg(GPIO_STATE[0].tim_port , GPIO_STATE[0].tim_pin ,GPIO_Mode_Out_OD, GPIO_Speed_50MHz);
  #endif
	
		//ee_CheckOk(0);
		//ee_Test(0);
	
    code_test();
	
 while (1)
 {
	for(i = 1; i < (COM_CH_NUM + 1); i++)
	{
		//motor(i, 95);
		//delay_ms(300);
		//motor(i, -95);
		//delay_ms(300);
		//servo(i,500);
	}
 }

}
/*********************************************END OF FILE**********************/

