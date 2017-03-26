/******************** (C) COPYRIGHT 威典科技 WDR ********************
* File Name          : Robot.h
* Author             : zhmili
********************************************************************************/
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifndef __ROBOT_H
#define __ROBOT_H

#define MAINX1   0
#define MAINX2   1
#define MAINX3   2
#define MAINX4   3
#define MAINX5   4
#define MAINX6   5
#define MAINX7   6
#define MAINX8   7
#define MAINX9   8
#define MAINX10  9
#define MAINX11  10
#define MAINX12  11
#define MAINX13  12
#define MAINX14  13
#define MAINX15  14
#define MAINX16  15

extern void mainX1(void);
extern void mainX2(void);
extern void mainX3(void);
extern void mainX4(void);
extern void mainX5(void);
extern void mainX6(void);
extern void mainX7(void);

#if ROB_0 == 0
  
  #define SER_SET_LOW    0
  #define SER_SET_HIGH   10000
  #define MOT_SET_LOW    0
  #define MOT_SET_HIGH   2000
  
  #define MOT1          TIM4
  #define MOT2          TIM4
  #define MOT3          TIM4
  
  #define CMOT1          TIM_Channel_2
  #define CMOT2          TIM_Channel_3
  #define CMOT3          TIM_Channel_4
  
  #define BAD1_SCH      ADC1
  #define BAD2_SCH      ADC1
  #define BAD3_SCH      ADC1
  #define BAD4_SCH      ADC1
  #define BAD5_SCH      ADC1
  #define BAD6_SCH      ADC1
  #define BAD7_SCH      ADC1
  #define BAD8_SCH      ADC1
  
  #define BAD9_SCH      ADC2
  #define BAD10_SCH      ADC2
  #define BAD11_SCH      ADC2
  #define BAD12_SCH      ADC2
  #define BAD13_SCH      ADC2
  #define BAD14_SCH      ADC2
  #define BAD15_SCH      ADC2
  #define BAD16_SCH      ADC2
  
  #define BAD17_SCH      ADC3
  #define BAD18_SCH      ADC3
  #define BAD19_SCH      ADC3
  #define BAD20_SCH      ADC3
  
  #define CADC_1_SCH     ADC_Channel_0
  #define CADC_2_SCH     ADC_Channel_1
  #define CADC_3_SCH     ADC_Channel_2
  #define CADC_4_SCH     ADC_Channel_3
  #define CADC_5_SCH     ADC_Channel_4
  
  #define CADC_6_SCH     ADC_Channel_5
  #define CADC_7_SCH     ADC_Channel_6
  #define CADC_8_SCH     ADC_Channel_7
  #define CADC_9_SCH     ADC_Channel_8
  #define CADC_10_SCH     ADC_Channel_9
  
  #define CADC_11_SCH     ADC_Channel_10
  #define CADC_12_SCH     ADC_Channel_11
  #define CADC_13_SCH     ADC_Channel_12
  #define CADC_14_SCH     ADC_Channel_13
  #define CADC_15_SCH     ADC_Channel_14
  
  #define CADC_16_SCH     ADC_Channel_15
  
  #define SYS_485PORT          2
  #define EMOT0_CMD            100
  #define EMOT0_CMD_RUN        2
  #define EMOT0_CMD_STOP       1
  
  #define EMOT0_GEAR_RATO_ADD  105
  #define EMOT0_CYC_PLUSE      106
  #define EMOT0_WHEEL_RADIUS   107
  
  #define EMOT0_SET_SPEED      110
  
  #define EMOT0_ALREADY_RUM_MM 113
  
  #define EMOT0_PID_P          114
  #define EMOT0_PID_I          116
  #define EMOT0_PID_D          117
  
  int digital(int p);
                                                                                                                            
  int analog(int p);
  
  int motor(int m, int p);
                                                                                                             
  int off(int id);
                  
  void servo(int s, int p);
  
  void stop(void);
  
  int compass(void);
  
  void test(void);
  
  void beep(int msecond);
  
  int valarm(int vleve);
  
  void set_light(int value);
  
  void get_act_data(void);
  
  int  digit_recognize(void);
  
  int  QR_recognize(void);
  
  void uart_init(int port, int baud_rate);
  
  unsigned char uart_read(int port);
  
  void uart_write(int port, unsigned char data);
  
  void uart_write_str(int port, char* pdata);
  
  void rs485_init(int baud_rate);
  
  void rs485_write(int data);
  
  int  rs485_read(void);
  
  int iic_read(int addr, int reg);
  void iic_write(int addr, int reg,int val);
  
  //为旧版本兼容而保留的版本
  int set_digital_out(int channel);                                        							    
  int clear_digital_out(int channel);
  int   push_button_up_read(void);
  int   left_button_read(void);
  int  middle_button_read(void);
  //暂不用接口
  int  e_analog(int rs485port,int addr,int channel);
  int e_motor_pars(int addr,int mot_loc, int mot_gear_rato, int cyc_pluse, int wheel_radius);
  int e_motor_pid(int addr,int mot_loc, int pid_p, int pid_i, int pid_d);
  int e_motor(int addr,int mot_loc, int sp_mmps);
  int e_motor_mm(int addr,int mot_loc);
  int e_off(int addr,int mot_loc);
  int analog4096(int p);
  int voice_read(void);
  void play_mp3(int loc);
  void speak_sound(unsigned char *speak_buf);
  
#endif

#define clear_lcd   clr_screen
#define display_int put_int_value
#define display_str put_string
#define reset_system_clock  reset_system_time
#define clear_lcd   clr_screen
#define display_int put_int_value
#define display_str put_string
#define mot_off     off
#define mot_stop    stop

//公用函数
void reset_system_time(void);
unsigned int mseconds(void);														   
unsigned int seconds(void);													  
void sleep(int sec);													  
void msleep(int msec);	
void clr_screen(void);
void put_int_value(unsigned char x,unsigned char y,unsigned short value);
void put_string(unsigned char x,unsigned char y,char *p);
int kill_process(int pid);
void start_process(int pid);
void set_name(char loc, char *pname);
void select_main(void);
int key(int loc);
void press_go_on(void);
int set_digital(int cha , int h_o_l);
void delay(int sec, int msec);
void ps2_open(void);
void ps2_scan(int delay_ms);
int ps2_read(int kind);
void ps2_write(int vib,int delay_ms);
void ps2_close(void);

#endif