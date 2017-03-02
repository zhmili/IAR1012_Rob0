/******************** (C) COPYRIGHT       ********************
* File Name          : 
* Author             : 
* Version            : 
* Description        : 
********************************************************************************/
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifndef __BSP_ROB0_H
#define __BSP_ROB0_H

#include "stm32f10x.h"
#include <stdio.h>

#define ROB_0				   1 

#define COM_CH_NUM    11
#define RJ_NUM        13

#define ADC_AVE_TIMES  4
#define GET_AVE	       1

#define ADC_LEVH   1808					//2730  				//2.2v
#define ADC_LEVM   904					//1365					//1.1v

#define TIM_PERIOD_18KHZ  3999 
#define TIM_PRESC_18KHZ   1

#define TIM_PERIOD_50HZ   9999
#define TIM_PRESC_50HZ    143

#define I2C_NODE_LOC      0
#define I2C_NODE_ADDR			0

typedef enum emode{
  NO_MODE_0 = 0,
  DIG_OUT_PUT_1 = 1,
  RGB_LED_2,
  BRUSH_MOT_3,
  REC_PLY_SOUND_4,
  SERVO_MOT_5,
  RES_6,
  BEEP_7,
  SPI_8,
  RES_9,
  AD_IN_10,
  INF_DIS_11,
  KEY_12,
  RES_13,
  HUM_IND_14,
  SOUND_CTRL_15,
  RES_16,
  LIGHT_17,
  I2C_18,
  RS232_19,
  RS485_20
}EMODE;



typedef struct gpio_state{
  
  GPIO_TypeDef* out_port;
  GPIO_TypeDef* tim_port;
  GPIO_TypeDef* ad_port;	
  u16 out_pin;
  u16 tim_pin;
  u16 ad_pin;
  TIM_TypeDef* TIM;
  ADC_TypeDef* ADC;
  u16 tim_ch;
  u16 ad_ch;
  
  u8 out_pu;
  u8 out_pd;
  u8 tim_pu;
  u8 tim_pd;
  
  u16  ad_value;
  u16  ex_ad_value;
  
  u8 ad_lev;
  u8 ex_ad_lev;
  
  u8 h_num;               //0 ???????
  u8 l_num;
  
}TGPIO_STATE;


//////////////////////////////////////////////////RJ1---RJ11???IO

#define    RJ1_OPORT	             		GPIOB
#define    RJ1_OPIN           		        GPIO_Pin_12
#define    RJ1_TPORT	             		GPIOC
#define    RJ1_TPIN           		        GPIO_Pin_9
#define    RJ1_APORT	             		GPIOC
#define    RJ1_APIN           		        GPIO_Pin_5
#define    RJ1_ADC                              ADC1
#define    RJ1_ACH           			ADC_Channel_15
#define    RJ1_TIM                              TIM8
#define    RJ1_TCH                              TIM_Channel_4

#define    RJ2_OPORT	             		GPIOB
#define    RJ2_OPIN           		        GPIO_Pin_1
#define    RJ2_TPORT	             		GPIOC
#define    RJ2_TPIN           		        GPIO_Pin_8
#define    RJ2_APORT	             		GPIOC
#define    RJ2_APIN           		        GPIO_Pin_4
#define    RJ2_ADC                              ADC1
#define    RJ2_ACH           			ADC_Channel_14
#define    RJ2_TIM                              TIM8
#define    RJ2_TCH                              TIM_Channel_3

#define    RJ3_OPORT	             	        GPIOB
#define    RJ3_OPIN           			GPIO_Pin_0
#define    RJ3_TPORT	             		GPIOC
#define    RJ3_TPIN           			GPIO_Pin_7
#define    RJ3_APORT	             		GPIOA
#define    RJ3_APIN           			GPIO_Pin_5
#define    RJ3_ADC                              ADC1
#define    RJ3_ACH           			ADC_Channel_5
#define    RJ3_TIM                              TIM8
#define    RJ3_TCH                              TIM_Channel_2

#define    RJ4_OPORT	             		GPIOA
#define    RJ4_OPIN           			GPIO_Pin_7
#define    RJ4_TPORT	             		GPIOC
#define    RJ4_TPIN           			GPIO_Pin_6
#define    RJ4_APORT	             		GPIOA
#define    RJ4_APIN           			GPIO_Pin_3
#define    RJ4_ADC                              ADC1
#define    RJ4_ACH           			ADC_Channel_3
#define    RJ4_TIM                              TIM8
#define    RJ4_TCH                              TIM_Channel_1

#define    RJ5_OPORT	             		GPIOA
#define    RJ5_OPIN           			GPIO_Pin_6
#define    RJ5_TPORT	             		GPIOB
#define    RJ5_TPIN           			GPIO_Pin_8
#define    RJ5_APORT	             		GPIOA
#define    RJ5_APIN           			GPIO_Pin_2
#define    RJ5_ADC                              ADC1
#define    RJ5_ACH           			ADC_Channel_2
#define    RJ5_TIM                              TIM4
#define    RJ5_TCH                              TIM_Channel_3

#define    RJ6_OPORT	             		GPIOB
#define    RJ6_OPIN           			GPIO_Pin_9
#define    RJ6_TPORT	             		GPIOB
#define    RJ6_TPIN           			GPIO_Pin_7
#define    RJ6_APORT	             		GPIOA
#define    RJ6_APIN           			GPIO_Pin_1
#define    RJ6_ADC                              ADC1
#define    RJ6_ACH           			ADC_Channel_1
#define    RJ6_TIM                              TIM4
#define    RJ6_TCH                              TIM_Channel_2

#define    RJ7_OPORT	             		GPIOB
#define    RJ7_OPIN           			GPIO_Pin_5
#define    RJ7_TPORT	             		GPIOB
#define    RJ7_TPIN           			GPIO_Pin_6
#define    RJ7_APORT	             		GPIOA
#define    RJ7_APIN           			GPIO_Pin_0
#define    RJ7_ADC                              ADC1
#define    RJ7_ACH           			ADC_Channel_0
#define    RJ7_TIM                              TIM4
#define    RJ7_TCH                              TIM_Channel_1


#define    RJ8_OPORT	             		GPIOB
#define    RJ8_OPIN           			GPIO_Pin_4
#define    RJ8_TPORT	             		GPIOA
#define    RJ8_TPIN           			GPIO_Pin_11
#define    RJ8_APORT	             		GPIOC
#define    RJ8_APIN           			GPIO_Pin_0
#define    RJ8_ADC                              ADC1
#define    RJ8_ACH           			ADC_Channel_10
#define    RJ8_TIM                              TIM1
#define    RJ8_TCH                              TIM_Channel_4


#define    RJ9_OPORT	             		GPIOB
#define    RJ9_OPIN           			GPIO_Pin_3
#define    RJ9_TPORT	             		GPIOA
#define    RJ9_TPIN           			GPIO_Pin_10
#define    RJ9_APORT	             		GPIOC
#define    RJ9_APIN           			GPIO_Pin_1
#define    RJ9_ADC                              ADC1
#define    RJ9_ACH           			ADC_Channel_11
#define    RJ9_TIM                              TIM1
#define    RJ9_TCH                              TIM_Channel_3

#define    RJ10_OPORT	             		GPIOA
#define    RJ10_OPIN           			GPIO_Pin_13
#define    RJ10_TPORT	             		GPIOA
#define    RJ10_TPIN           			GPIO_Pin_9
#define    RJ10_APORT	             		GPIOC
#define    RJ10_APIN           			GPIO_Pin_2
#define    RJ10_ADC                             ADC1
#define    RJ10_ACH           			ADC_Channel_12
#define    RJ10_TIM                             TIM1
#define    RJ10_TCH                             TIM_Channel_2

#define    RJ11_OPORT	             	        GPIOA
#define    RJ11_OPIN          			GPIO_Pin_12
#define    RJ11_TPORT	             	        GPIOA
#define    RJ11_TPIN           			GPIO_Pin_8
#define    RJ11_APORT	             		GPIOC
#define    RJ11_APIN           			GPIO_Pin_3
#define    RJ11_ADC                             ADC1
#define    RJ11_ACH           			ADC_Channel_13
#define    RJ11_TIM                             TIM1
#define    RJ11_TCH                             TIM_Channel_1

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

//------------------------------------------------------------------------------
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

#if ROB_0 
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
#define    LCD_A_PORT                           LCD_MI_PORT            
#define    LCD_A_PIN                            LCD_MI_PIN

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
#define    LCD_A_PORT                           LCD_MI_PORT            
#define    LCD_A_PIN                            LCD_MI_PIN

#endif

#define S0(x)     (EX_AS0PORT->ODR = (EX_AS0PORT->ODR & ~EX_AS0PIN) | (x ? EX_AS0PIN :0))
#define S1(x)     (EX_AS1PORT->ODR = (EX_AS1PORT->ODR & ~EX_AS1PIN) | (x ? EX_AS1PIN :0))
#define S2(x)     (EX_AS2PORT->ODR = (EX_AS2PORT->ODR & ~EX_AS2PIN) | (x ? EX_AS2PIN :0))
#define S3(x)     (EX_AS3PORT->ODR = (EX_AS3PORT->ODR & ~EX_AS3PIN) | (x ? EX_AS3PIN :0))
#define S4(x)     (EX_AS4PORT->ODR = (EX_AS4PORT->ODR & ~EX_AS4PIN) | (x ? EX_AS4PIN :0))

#define S4_0(x4,x3,x2,x1,x0)  S0(x0);S1(x1);S2(x2);S3(x3);S4(x4);

/**************************????********************************/

void s4_0_set(char num);
void scan_port(void);							//????
void judge(void);									//?????????
void release_jtagswd(void);
void enable_jtagswd(void);
void pin_final_cfg(void);
void bsp_rob0_init(void);
void pin_cfg(GPIO_TypeDef* GPIOx, u16 GPIO_Pinx ,GPIOMode_TypeDef GPIO_Modex, GPIOSpeed_TypeDef GPIO_Speed);
void scan_sim(int h_num, int l_num);
void test_sim_forward(void);
int get_i2c_id(int chan);
//int get_i2c_id(int node_addr , int size,int chan);
void i2c_cfg(int id);

#endif
