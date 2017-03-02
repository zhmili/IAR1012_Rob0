#ifndef __PSTWO_H
#define __PSTWO_H
#include <includes.h>
/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File：PS2驱动程序
Author：pinggai    Version:1.1     Data:2015/10/20
Description: PS2驱动程序
             增加功能：
			 1、软件设置“红灯模式”、“绿灯模式”，并可以设置“保存”，通过手柄“模式按键”无法改变
			 2、设置手柄震动：通过数值的设置，改变左侧大震动电机震动频率。
			                  通过数值的设置，开关右侧小震动电机。
History:  
V1.0: 	2015/05/16
1、手柄解码，识别按键值，读取模拟值。       
**********************************************************/
#define PS_SCK_PIN  DEF_BIT_13    //PB13
#define PS_DO_PIN   DEF_BIT_14    //PB14
#define PS_DI_PIN   DEF_BIT_15    //PB15
#define PS_CS_PIN   DEF_BIT_09     //PC9

#define DI   PBin(15)           //PB12  输入
#define DO_H PBout(14)=1        //命令位高
#define DO_L PBout(14)=0        //命令位低


#define CS_H PCout(9) = 1      //CS拉高
#define CS_L PCout(9) = 0   

#define CLK_H PBout(13)=1      //时钟拉高
#define CLK_L PBout(13)=0      //时钟拉低


#define ANALOG					1
#define DIGITAL					0
#define MODE_FREE				0x0EE
#define MODE_LOCK			  0x03

//These are our button constants
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16
#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      16

#define PSS_RX 5                //右摇杆X轴数据
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

//#define WHAMMY_BAR		8
//These are stick values
#define RD_MD      1
#define RD_16KEY   2
#define RD_FIR_KEY 3
#define RD_PSS_RX  4
#define RD_PSS_RY  5
#define RD_PSS_LX  6
#define RD_PSS_LY  7

#define MD_LED_GREEN 1
#define MD_LED_RED   2


extern u8 Data[9];
extern u16 MASK[16];
extern u16 Handkey;

void PS2_Init(void);
u8 PS2_RedLight(void);   //判断是否为红灯模式
void PS2_ReadData(void); //读手柄数据
void PS2_Cmd(u8 CMD);		  //向手柄发送命令
u8 PS2_DataKey(void);		  //按键值读取
u8 PS2_AnologData(u8 button); //得到一个摇杆的模拟量
void PS2_ClearData(void);	  //清除数据缓冲区
void PS2_Vibration(u8 motor1, u8 motor2);//振动设置motor1  0xFF开，其他关，motor2  0x40~0xFF



void PS2_EnterConfing(void);	 //进入配置
void PS2_TurnOnAnalogMode(void); //发送模拟量
void PS2_VibrationMode(void);    //振动设置
void PS2_ExitConfing(void);	     //完成配置
void PS2_SetInit(void);		     //配置初始化

#endif





