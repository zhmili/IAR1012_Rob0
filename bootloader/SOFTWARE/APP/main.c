/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : whq
* Version            : V2.0.1
* Date               : 20/02/2013
* Description        : Main program body
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/** @addtogroup IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/


#include <includes.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

int main(void)
{
 static  u8 flg = 0;
	

	
#ifdef DEBUG
  debug();
#endif
    
  BspInit();
  CommonInit();
	
#if 0

 // LCD_LED(1);
  LCD_CS(1);
  LCD_RES(1);
  LCD_A0(1);
  LCD_SDI(1);
  LCD_CLK(1);
  
 // LCD_LED(0);
  LCD_CS(0);
  LCD_RES(0);
  LCD_A0(0);
  LCD_SDI(0);
  LCD_CLK(0);
  
#endif
  
	LCD_LED(1);
	LCD_Init();
	//LCD_Image();
  FontSet(0,RED);
	
	ClrScreen(0); 
	
#if U_DBG
	Print("u_dbg start \r\n\n");
#endif
	
    while (1)
    {
      CommonExec();	
			
			#if 0
        if (IS_TIMEOUT_1MS(eTim1, 1000))
        {
            flg? LedOn(LED_5) : LedOff(LED_5);
            flg = !flg;
        }
			#endif
    }
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif





/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
