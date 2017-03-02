#ifndef __PORTSET_H
#define	__PORTSET_H


#include "stm32f10x.h"


#define    macNOFCHANEL             12

//////////////////////////////////////////////////RJ1---RJ11相关IO
#define    rj1out	             			GPIOA
#define    rj1outpin           			GPIO_Pin_12
#define    rj1tim	             			GPIOA
#define    rj1timpin           			GPIO_Pin_8
#define    rj1adc	             			GPIOC
#define    rj1adcpin           			GPIO_Pin_3
#define    rj1adcch           			ADC_Channel_13

#define    rj2out	             			GPIOA
#define    rj2outpin           			GPIO_Pin_13
#define    rj2tim	             			GPIOA
#define    rj2timpin           			GPIO_Pin_9
#define    rj2adc	             			GPIOC
#define    rj2adcpin           			GPIO_Pin_2
#define    rj2adcch           			ADC_Channel_12

#define    rj3out	             			GPIOB
#define    rj3outpin           			GPIO_Pin_3
#define    rj3tim	             			GPIOA
#define    rj3timpin           			GPIO_Pin_10
#define    rj3adc	             			GPIOC
#define    rj3adcpin           			GPIO_Pin_1
#define    rj3adcch           			ADC_Channel_11

#define    rj4out	             			GPIOB
#define    rj4outpin           			GPIO_Pin_4
#define    rj4tim	             			GPIOA
#define    rj4timpin           			GPIO_Pin_11
#define    rj4adc	             			GPIOC
#define    rj4adcpin           			GPIO_Pin_0
#define    rj4adcch           			ADC_Channel_10

#define    rj5out	             			GPIOB
#define    rj5outpin           			GPIO_Pin_5
#define    rj5tim	             			GPIOB
#define    rj5timpin           			GPIO_Pin_6
#define    rj5adc	             			GPIOA
#define    rj5adcpin           			GPIO_Pin_0
#define    rj5adcch           			ADC_Channel_0

#define    rj6out	             			GPIOB
#define    rj6outpin           			GPIO_Pin_9
#define    rj6tim	             			GPIOB
#define    rj6timpin           			GPIO_Pin_7
#define    rj6adc	             			GPIOA
#define    rj6adcpin           			GPIO_Pin_1
#define    rj6adcch           			ADC_Channel_1

#define    rj7out	             			GPIOA
#define    rj7outpin           			GPIO_Pin_6
#define    rj7tim	             			GPIOB
#define    rj7timpin           			GPIO_Pin_8
#define    rj7adc	             			GPIOA
#define    rj7adcpin           			GPIO_Pin_2
#define    rj7adcch           			ADC_Channel_2

#define    rj8out	             			GPIOA
#define    rj8outpin           			GPIO_Pin_7
#define    rj8tim	             			GPIOC
#define    rj8timpin           			GPIO_Pin_6
#define    rj8adc	             			GPIOA
#define    rj8adcpin           			GPIO_Pin_3
#define    rj8adcch           			ADC_Channel_3

#define    rj9out	             			GPIOB
#define    rj9outpin           			GPIO_Pin_0
#define    rj9tim	             			GPIOC
#define    rj9timpin           			GPIO_Pin_7
#define    rj9adc	             			GPIOA
#define    rj9adcpin           			GPIO_Pin_5
#define    rj9adcch           			ADC_Channel_5

#define    rj10out	             		GPIOB
#define    rj10outpin           		GPIO_Pin_1
#define    rj10tim	             		GPIOC
#define    rj10timpin           		GPIO_Pin_8
#define    rj10adc	             		GPIOC
#define    rj10adcpin           		GPIO_Pin_4
#define    rj10adcch           			ADC_Channel_14

#define    rj11out	             		GPIOB
#define    rj11outpin           		GPIO_Pin_12
#define    rj11tim	             		GPIOC
#define    rj11timpin           		GPIO_Pin_9
#define    rj11adc	             		GPIOC
#define    rj11adcpin           		GPIO_Pin_5
#define    rj11adcch           			ADC_Channel_15

////////////////////////////////////////////////////74HC4051相关IO
#define    atchadc	             			GPIOA
#define    atchadcpin           			GPIO_Pin_4
#define    atchadcch           				ADC_Channel_4

#define    atchs0	             				GPIOC
#define    atchs0pin           				GPIO_Pin_13

#define    atchs1	             				GPIOC
#define    atchs1pin           				GPIO_Pin_14

#define    atchs2	             				GPIOC
#define    atchs2pin           				GPIO_Pin_15

#define    atchs3	             				GPIOA
#define    atchs3pin           				GPIO_Pin_14

#define    atchs4	             				GPIOB
#define    atchs4pin           				GPIO_Pin_2

void release_jtagswd(void);
void delay(__IO uint32_t nCount);
void pwmxset(TIM_TypeDef* TIMx, u16 TIM_Channel_x, u16 fre , u16 pulse);
void scanport(void);
void setport(void);

#endif

