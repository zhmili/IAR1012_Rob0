#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H


#include <inttypes.h>
#include "bsp_rob0.h"

extern TGPIO_STATE GPIO_STATE[16];

#define macI2C_WR	0		/* д����bit */
#define macI2C_RD	1		/* ������bit */

/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define macGPIO_PORT_I2C	GPIOB			/* GPIO�˿� */
#define macRCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define macI2C_SCL_PIN		GPIO_Pin_6			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define macI2C_SDA_PIN		GPIO_Pin_7			/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 0	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define macI2C_SCL_1()  GPIO_SetBits(macGPIO_PORT_I2C, macI2C_SCL_PIN)		/* SCL = 1 */
	#define macI2C_SCL_0()  GPIO_ResetBits(macGPIO_PORT_I2C, macI2C_SCL_PIN)		/* SCL = 0 */
	
	#define macI2C_SDA_1()  GPIO_SetBits(macGPIO_PORT_I2C, macI2C_SDA_PIN)		/* SDA = 1 */
	#define macI2C_SDA_0()  GPIO_ResetBits(macGPIO_PORT_I2C, macI2C_SDA_PIN)		/* SDA = 0 */
	
	#define macI2C_SDA_READ()  GPIO_ReadInputDataBit(macGPIO_PORT_I2C, macI2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define macI2C_SCL_1(i)  GPIO_STATE[i].out_port->BSRR = GPIO_STATE[i].out_pin				/* SCL = 1 */
	#define macI2C_SCL_0(i)  GPIO_STATE[i].out_port->BRR = GPIO_STATE[i].out_pin					/* SCL = 0 */
	
	#define macI2C_SDA_1(i)  GPIO_STATE[i].tim_port->BSRR = GPIO_STATE[i].tim_pin				/* SCL = 1 */
	#define macI2C_SDA_0(i)  GPIO_STATE[i].tim_port->BRR = GPIO_STATE[i].tim_pin					/* SCL = 0 */
	
//	#define macI2C_SDA_READ(i)   GPIO_ReadInputDataBit(GPIO_STATE[i].tim_port, GPIO_STATE[i].tim_pin) /* ��SDA����״̬ */
//	#define macI2C_SCL_READ(i)   GPIO_ReadInputDataBit(GPIO_STATE[i].out_port, GPIO_STATE[i].out_pin) /* ��SCL����״̬ */
	#define macI2C_SDA_READ(i)  ((GPIO_STATE[i].tim_port->IDR & GPIO_STATE[i].tim_pin) != 0)	/* ��SDA����״̬ */
	#define macI2C_SCL_READ(i)  ((GPIO_STATE[i].out_port->IDR & GPIO_STATE[i].out_pin) != 0)	/* ��SCL����״̬ */
	
#endif


//pin_cfg(GPIO_STATE[0].out_port , GPIO_STATE[0].out_pin ,GPIO_Mode_Out_OD, GPIO_Speed_50MHz);
//pin_cfg(GPIO_STATE[0].tim_port , GPIO_STATE[0].tim_pin ,GPIO_Mode_Out_OD, GPIO_Speed_50MHz);

void i2c_Start(int port);
void i2c_Stop(int port);
void i2c_SendByte(uint8_t _ucByte, int port);
uint8_t i2c_ReadByte(int port);
uint8_t i2c_WaitAck(int port);
void i2c_Ack(int port);
void i2c_NAck(int port);
uint8_t i2c_CheckDevice(uint8_t _Address, int port);


#endif

