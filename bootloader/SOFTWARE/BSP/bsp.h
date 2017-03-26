/****************************************Copyright (c)**************************************************
** Modified by:        ����ǿ
** Modified date:      2012-05-20 
** Version:            v3.0
** Descriptions:       �޸�����STM32F10x   
**
**------------------------------------------------------------------------------------------------------
** Modified by: 		
** Modified date:	   
** Version: 		   
** Descriptions:		
**
********************************************************************************************************/

#ifndef _bsp_h_
#define _bsp_h_
#include "stm32f10x_type.h"


#include "BspTime2.h"
#include "BspTime3.h"
#include "BspUsart1.h"

#include    <LCD_PortConfig.h>
#include    <LCD_Config.h>

#include    <LCD_Driver_User.h>
#include    <LCD_Dis.h>

#define ROB0   	1        //�����rob0 ҲͳһҪ��ѡ
#define ROB_NEW 1					//��ǰ100Ƭ�ײ����� ��100Ƭ�ײ����һ�£��°汾ͳһ��ROB_NEW�汾
#define U_DBG  	0

#ifndef NULL
#define NULL        ((void *)0)
#endif

/********************************/
/*       ϵͳ���ú���           */
/********************************/
#define LED_0 0
#define LED_1 1
#define LED_2 2
#define LED_3 3
#define LED_4 4
#define LED_5 5


//------------------------------------------------------------------------------
#if ROB0
	
	#define    U4_RX_PORT                           GPIOC                                          
	#define    U4_RX_PIN                            GPIO_Pin_11
	#define    U4_TX_PORT                           GPIOC
	#define    U4_TX_PIN                            GPIO_Pin_10

	#define    U5_RX_PORT                           GPIOD
	#define    U5_RX_PIN                            GPIO_Pin_2
	#define    U5_TX_PORT                           GPIOC                           
	#define    U5_TX_PIN                            GPIO_Pin_12
	
	#define    RS485_EN_PORT                        GPIOA
	#define    RS485_EN_PIN                         GPIO_Pin_15

	#define    LCD_CS_PORT                          GPIOB         
	#define    LCD_CS_PIN                           GPIO_Pin_10
	#define    LCD_RE_PORT                          GPIOB
	#define    LCD_RE_PIN                           GPIO_Pin_11
	#define    LCD_CK_PORT                          GPIOB
	#define    LCD_CK_PIN                           GPIO_Pin_13
	#define    LCD_MO_PORT                          GPIOB
	#define    LCD_MO_PIN                           GPIO_Pin_15
	#define    LCD_MI_PORT                          GPIOB
	#define    LCD_MI_PIN                           GPIO_Pin_14


////////////////////////////////////////////////////74HC4051 IO
#define    EX_APORT	             		GPIOA
#define    EX_APIN           			GPIO_Pin_4
#define    EX_APCH           			ADC_Channel_4

#define    EX_AS0PORT	             		GPIOC
#define    EX_AS0PIN           			GPIO_Pin_13

#define    EX_AS1PORT	             		GPIOC
#define    EX_AS1PIN           			GPIO_Pin_14

#define    EX_AS2PORT	             		GPIOC
#define    EX_AS2PIN           			GPIO_Pin_15

#define    EX_AS3PORT	             		GPIOA
#define    EX_AS3PIN           			GPIO_Pin_14

#define    EX_AS4PORT	             		GPIOB
#define    EX_AS4PIN           			GPIO_Pin_2



#define S0(x)     (EX_AS0PORT->ODR = (EX_AS0PORT->ODR & ~EX_AS0PIN) | (x ? EX_AS0PIN :0))
#define S1(x)     (EX_AS1PORT->ODR = (EX_AS1PORT->ODR & ~EX_AS1PIN) | (x ? EX_AS1PIN :0))
#define S2(x)     (EX_AS2PORT->ODR = (EX_AS2PORT->ODR & ~EX_AS2PIN) | (x ? EX_AS2PIN :0))
#define S3(x)     (EX_AS3PORT->ODR = (EX_AS3PORT->ODR & ~EX_AS3PIN) | (x ? EX_AS3PIN :0))
#define S4(x)     (EX_AS4PORT->ODR = (EX_AS4PORT->ODR & ~EX_AS4PIN) | (x ? EX_AS4PIN :0))

#define S4_0(x4,x3,x2,x1,x0)  S0(x0);S1(x1);S2(x2);S3(x3);S4(x4);

#else

	#define    LCD_CS_PORT                          GPIOD         
	#define    LCD_CS_PIN                           GPIO_Pin_4
	#define    LCD_RE_PORT                          GPIOG
	#define    LCD_RE_PIN                           GPIO_Pin_9
	#define    LCD_CK_PORT                          GPIOD
	#define    LCD_CK_PIN                           GPIO_Pin_5
	#define    LCD_MO_PORT                          GPIOF
	#define    LCD_MO_PIN                           GPIO_Pin_15
	#define    LCD_MI_PORT                          GPIOE
	#define    LCD_MI_PIN                           GPIO_Pin_12

#endif

	#define    LCD_A_PORT                           LCD_MI_PORT            
	#define    LCD_A_PIN                            LCD_MI_PIN


void DelayNms(u16 ms);
void Delay(vu32 nCount);
void IWDG_Init(void);
void SysTickInit(void);
void BspInit(void);
void LedInit(void);
void LedOn(u32 n);
void LedOff(u32 n);
void BspClose(void);
int  left_button_read(void);
int  middle_button_read(void);

#endif

/*********************************** END **************************************/



