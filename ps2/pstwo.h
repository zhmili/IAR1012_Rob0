#ifndef __PSTWO_H
#define __PSTWO_H
#include <includes.h>
/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File��PS2��������
Author��pinggai    Version:1.1     Data:2015/10/20
Description: PS2��������
             ���ӹ��ܣ�
			 1��������á����ģʽ�������̵�ģʽ�������������á����桱��ͨ���ֱ���ģʽ�������޷��ı�
			 2�������ֱ��𶯣�ͨ����ֵ�����ã��ı������𶯵����Ƶ�ʡ�
			                  ͨ����ֵ�����ã������Ҳ�С�𶯵����
History:  
V1.0: 	2015/05/16
1���ֱ����룬ʶ�𰴼�ֵ����ȡģ��ֵ��       
**********************************************************/
#define PS_SCK_PIN  DEF_BIT_13    //PB13
#define PS_DO_PIN   DEF_BIT_14    //PB14
#define PS_DI_PIN   DEF_BIT_15    //PB15
#define PS_CS_PIN   DEF_BIT_09     //PC9

#define DI   PBin(15)           //PB12  ����
#define DO_H PBout(14)=1        //����λ��
#define DO_L PBout(14)=0        //����λ��


#define CS_H PCout(9) = 1      //CS����
#define CS_L PCout(9) = 0   

#define CLK_H PBout(13)=1      //ʱ������
#define CLK_L PBout(13)=0      //ʱ������


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

#define PSS_RX 5                //��ҡ��X������
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
u8 PS2_RedLight(void);   //�ж��Ƿ�Ϊ���ģʽ
void PS2_ReadData(void); //���ֱ�����
void PS2_Cmd(u8 CMD);		  //���ֱ���������
u8 PS2_DataKey(void);		  //����ֵ��ȡ
u8 PS2_AnologData(u8 button); //�õ�һ��ҡ�˵�ģ����
void PS2_ClearData(void);	  //������ݻ�����
void PS2_Vibration(u8 motor1, u8 motor2);//������motor1  0xFF���������أ�motor2  0x40~0xFF



void PS2_EnterConfing(void);	 //��������
void PS2_TurnOnAnalogMode(void); //����ģ����
void PS2_VibrationMode(void);    //������
void PS2_ExitConfing(void);	     //�������
void PS2_SetInit(void);		     //���ó�ʼ��

#endif





