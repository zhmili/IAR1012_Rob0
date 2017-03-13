#ifndef __ROB0_H
#define __ROB0_H

//



#if ROB_0

#define SET_CHA_ERR   -1

#define SERVO_I2C_BEG   2



int analog(int cha);
int digital(int cha);
int op_digital(int cha); 
void pluse(int cha);
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
//void rs485_write_str(char* pdata);
//int iic_read(int addr, int reg);
//void iic_write(int addr, int reg,int val);

#if 0
int key(int l_or_r);
void set_digital(int cha, int h_or_l);
void reset_system_clock(void);
unsigned int msecond(void);													   
unsigned int second(void);													  
void delay(int sec, int msec);
void clear_lcd(void);
void display_int(unsigned char x,unsigned char y,unsigned short value);
void display_str(unsigned char x,unsigned char y,char *p);		
#endif

void test(void);

int  middle_button_read(void);
int  left_button_read(void);



#define sig_led      set_digital
#define recoder      pluse
#define inf_switch   op_digital
#define mag_switch   op_digital
#define gray_lever   analog
#define hum_inf_switch      op_digital
#define sound_switch op_digital
#define out_key      op_digital


#endif

#endif
