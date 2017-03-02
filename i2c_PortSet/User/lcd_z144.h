#ifndef __LCD_Z114_H
#define	__LCD_Z114_H


#include "stm32f10x.h"


#define uchar unsigned char 
#define uint  unsigned int
#define LCD_X_MAX 128
#define LCD_Y_MAX  128


//������ɫ
#define LCD_INITIAL_COLOR	0xFFFF			//����LCD����ʼ��ʱ�ı���ɫ
#define WHITE        	 		0xFFFF
#define BLACK         	 	0x0000	  
#define BLUE         	 		0x001F  
#define BRED             	0XF81F
#define GRED 	         		0XFFE0
#define GBLUE	         		0X07FF
#define RED           	 	0xF800
#define MAGENTA       	 	0xF81F
#define GREEN         	 	0x07E0
#define CYAN          	 	0x7FFF
#define YELLOW        	 	0xFFE0
#define BROWN 		 				0XBC40 //��ɫ
#define BRRED 		 				0XFC07 //�غ�ɫ
#define GRAY  		 				0X8430 //��ɫ
#define BLACK  						0x0000
#define PURPLE 						0xf81f


#define LCD_CS(a)	if (a)	\
          GPIO_SetBits(GPIOB,GPIO_Pin_10);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_10)

#define LCD_RES(a)	if (a)	\
          GPIO_SetBits(GPIOB,GPIO_Pin_11);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_11)
					
#define LCD_A0(a)	if (a)	\
          GPIO_SetBits(GPIOB,GPIO_Pin_14);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_14)
					
#define LCD_SDI(a)	if (a)	\
          GPIO_SetBits(GPIOB,GPIO_Pin_15);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_15)
					
#define LCD_CLK(a)	if (a)	\
          GPIO_SetBits(GPIOB,GPIO_Pin_13);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_13)

					
//��������
void delay_ms(int count);
void lcd_config(void);					
void LCD_Init(void);
void Write_Dot_LCD(unsigned char x,unsigned int y,unsigned int i);
void LCD_Fill(unsigned int Data);		
void LCD_Image(void);				
#endif
