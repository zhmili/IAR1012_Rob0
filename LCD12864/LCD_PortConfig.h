
#ifndef __LCD_PORTCONFIG_H
#define __LCD_PORTCONFIG_H

#define S6D0144           0
#define ST7735S           1
#define ST7735R           0
#define ILI9163C          0

#if  S6D0144
#define LCD_RS(x)   x?GPIO_SetBits(GPIOE, GPIO_Pin_12):GPIO_ResetBits(GPIOE, GPIO_Pin_12); 
#define LCD_CS(x)   x?GPIO_SetBits(GPIOD, GPIO_Pin_5):GPIO_ResetBits(GPIOD,GPIO_Pin_5);
#define LCD_WR(x)   x?GPIO_SetBits(GPIOF, GPIO_Pin_14):GPIO_ResetBits(GPIOF,GPIO_Pin_14);
#define LCD_RD(x)   
#define LCD_RES(x)  x?GPIO_SetBits(GPIOD, GPIO_Pin_4):GPIO_ResetBits(GPIOD,GPIO_Pin_4);

#define LCD_D0(x)   x?GPIO_SetBits(GPIOF, GPIO_Pin_11):GPIO_ResetBits(GPIOF,GPIO_Pin_11);
#define LCD_D1(x)   x?GPIO_SetBits(GPIOF, GPIO_Pin_12):GPIO_ResetBits(GPIOF,GPIO_Pin_12);
#define LCD_D2(x)   x?GPIO_SetBits(GPIOE, GPIO_Pin_7):GPIO_ResetBits(GPIOE, GPIO_Pin_7);
#define LCD_D3(x)   x?GPIO_SetBits(GPIOF, GPIO_Pin_15):GPIO_ResetBits(GPIOF, GPIO_Pin_15);
#define LCD_D4(x)   x?GPIO_SetBits(GPIOE, GPIO_Pin_15):GPIO_ResetBits(GPIOE, GPIO_Pin_15);
#define LCD_D5(x)   x?GPIO_SetBits(GPIOG, GPIO_Pin_5):GPIO_ResetBits(GPIOG, GPIO_Pin_5);
#define LCD_D6(x)   x?GPIO_SetBits(GPIOG, GPIO_Pin_9):GPIO_ResetBits(GPIOG, GPIO_Pin_9);
#define LCD_D7(x)   x?GPIO_SetBits(GPIOD, GPIO_Pin_1):GPIO_ResetBits(GPIOD, GPIO_Pin_1);
#define LCD_Data_BUS_Set(n)      LCD_D0(n&0x01);LCD_D1((n>>1)&0x01);LCD_D2((n>>2)&0x01);\
        LCD_D3((n>>3)&0x01);LCD_D4((n>>4)&0x01);LCD_D5((n>>5)&0x01);LCD_D6((n>>6)&0x01);\
        LCD_D7((n>>7)&0x01);
#else

#define LCD_CS(x)    x?(LCD_CS_PORT->BSRR = LCD_CS_PIN):(LCD_CS_PORT->BRR = LCD_CS_PIN);
#define LCD_RES(x)   x?(LCD_RE_PORT->BSRR = LCD_RE_PIN):(LCD_RE_PORT->BRR = LCD_RE_PIN);
#define LCD_A0(x)    x?(LCD_A_PORT->BSRR = LCD_A_PIN):(LCD_A_PORT->BRR = LCD_A_PIN);
#define LCD_SDI(x)   x?(LCD_MO_PORT->BSRR = LCD_MO_PIN):(LCD_MO_PORT->BRR = LCD_MO_PIN);
#define LCD_CLK(x)   x?(LCD_CK_PORT->BSRR = LCD_CK_PIN):(LCD_CK_PORT->BRR = LCD_CK_PIN);
#define LCD_LED(x)   x?(GPIOD->BSRR = GPIO_Pin_7):(GPIOD->BRR = GPIO_Pin_7);
#endif

#endif



