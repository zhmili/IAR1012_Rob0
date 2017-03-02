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
#ifndef __ROB0_H
#define __ROB0_H

#include "stm32f10x.h"
#include "bsp_rob0.h"
#include "bsp_usart.h"
#define SET_CHA_ERR   -1

int analog(int cha);
int digital(int cha);
int op_digital(int cha);
int key(int l_or_r);                                                            							          
                                         							   
void set_digital(int cha, int h_or_l);

void clear_lcd(void);
void display_int(unsigned char x,unsigned char y,unsigned short value);
void display_str(unsigned char x,unsigned char y,char *p);
void beep(int cha, int msecond);

void motor(int cha, int p);
void stop(void);
void off(int cha);
void servo(int cha, int p);

void uart_init(int baud_rate);
unsigned char uart_read(void);
void uart_write(unsigned char data);
void uart_write_str(char* pdata);
void rs485_init(int baud_rate);
int  rs485_read(void);
void rs485_write(int data);
void rs485_write_str(char* pdata);
int iic_read(int addr, int reg);
void iic_write(int addr, int reg,int val);

void reset_system_clock(void);
unsigned int msecond(void);													   
unsigned int second(void);													  
void sleep(int sec, int msec);
													  
void test(void);


#define sig_led      set_digital
#define recoder      set_digital
#define inf_switch   op_digital
#define mag_switch   op_digital
#define gray_lever   analog
#define hum_inf_switch      op_digital
#define sound_switch op_digital
#define out_key      op_digital

#endif

