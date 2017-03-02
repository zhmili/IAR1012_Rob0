/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   重现c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_usart.h"


/*******************************************************************************
* Function Name  : void UXART_Init(USART_TypeDef* UXARTx,u32 USART_BaudRate,u16 USART_WordLength,u16 USART_StopBits,u16 USART_Parity,u8 IT_RX_EN,u8 IT_TX_EN)
* Description    : Word Length = 8 Bits 1 Stop Bit No parity BaudRate = 9600 baud ???
*                  IT_RX_EN :1 - ??RX ??,IT_TX_EN :??TX??
* Input          : UXARTx - USART1 USART2 USART3 UART4 UART5 USART3 
* Return         : None
* Comment        :         
*******************************************************************************/
void UXART_Init(USART_TypeDef* UXARTx,u32 USART_BaudRate,u16 USART_WordLength,u16 USART_StopBits,u16 USART_Parity,u8 IT_RX_EN,u8 IT_TX_EN)
{
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = USART_BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength;
  USART_InitStructure.USART_StopBits = USART_StopBits;
  USART_InitStructure.USART_Parity = USART_Parity;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UXARTx, &USART_InitStructure);
  
  if(IT_RX_EN)
  {
   USART_ITConfig(UXARTx, USART_IT_RXNE, ENABLE);
  }else{
  USART_ITConfig(UXARTx, USART_IT_RXNE, DISABLE);
  }
  
  if(IT_TX_EN)
  {
   USART_ITConfig(UXARTx, USART_IT_TXE, ENABLE);
  }else{
   USART_ITConfig(UXARTx, USART_IT_TXE, DISABLE);
  }
  
  USART_ClearFlag(UXARTx, USART_FLAG_TC);//
  USART_ClearFlag(UXARTx, USART_FLAG_LBD);//
  USART_ClearFlag(UXARTx,USART_FLAG_CTS);
  USART_ClearFlag(UXARTx,USART_FLAG_RXNE);
  
  USART_Cmd(UXARTx, ENABLE);
}


/*******************************************************************************
* Function Name  : UART485_Config  				485通信初始化
* Description    : TX: PC10 	RX: PC11
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void UART485_Config(int baud_rate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
	
	//方向控制IO配置	PA15
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure UART4 Tx (PC.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);    
	/* Configure UART4 Rx (PC.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* USART4 mode config */
	USART_InitStructure.USART_BaudRate = baud_rate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);
	
	USART_Cmd(UART4, ENABLE);
}



 /**
  * @brief  USART1 GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void USARTx_Config(int baud_rate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	macUSART_APBxClock_FUN(macUSART_CLK, ENABLE);
	macUSART_GPIO_APBxClock_FUN(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx () as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  macUSART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(macUSART_TX_PORT, &GPIO_InitStructure);    
	/* Configure USART1 Rx () as input floating */
	GPIO_InitStructure.GPIO_Pin = macUSART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(macUSART_RX_PORT, &GPIO_InitStructure);
	
	/* USART5 mode config */
	USART_InitStructure.USART_BaudRate = baud_rate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(macUSARTx, &USART_InitStructure);
	
	USART_Cmd(macUSARTx, ENABLE);
}

/// 重定向c库函数printf到macUSARTx
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到macUSARTx */
		USART_SendData(macUSARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(macUSARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/// 重定向c库函数scanf到macUSARTx
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(macUSARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(macUSARTx);
}
/*********************************************END OF FILE**********************/

