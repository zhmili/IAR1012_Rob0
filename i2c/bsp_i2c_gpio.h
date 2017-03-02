#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H


#include <inttypes.h>
#include <includes.h>

//extern TGPIO_STATE GPIO_STATE[16];

#define macI2C_WR	0		/* д����bit */
#define macI2C_RD	1		/* ������bit */

/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define macGPIO_PORT_I2C	GPIOB			/* GPIO�˿� */
#define macRCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define macI2C_SCL_PIN		GPIO_Pin_10			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define macI2C_SDA_PIN		GPIO_Pin_11			/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */

    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
#if ROB_0
	#define macI2C_SCL_1(i)  GPIO_STATE[i].out_port->BSRR = GPIO_STATE[i].out_pin				/* SCL = 1 */
	#define macI2C_SCL_0(i)  GPIO_STATE[i].out_port->BRR = GPIO_STATE[i].out_pin					/* SCL = 0 */
	
	#define macI2C_SDA_1(i)  GPIO_STATE[i].tim_port->BSRR = GPIO_STATE[i].tim_pin				/* SCL = 1 */
	#define macI2C_SDA_0(i)  GPIO_STATE[i].tim_port->BRR = GPIO_STATE[i].tim_pin					/* SCL = 0 */
	
	#define macI2C_SDA_READ(i)  ((GPIO_STATE[i].tim_port->IDR & GPIO_STATE[i].tim_pin) != 0)	/* ��SDA����״̬ */
	#define macI2C_SCL_READ(i)  ((GPIO_STATE[i].out_port->IDR & GPIO_STATE[i].out_pin) != 0)	/* ��SCL����״̬ */
#else
	#define macI2C_SCL_1(i)  macGPIO_PORT_I2C->BSRR = macI2C_SCL_PIN				/* SCL = 1 */
	#define macI2C_SCL_0(i)  macGPIO_PORT_I2C->BRR = macI2C_SCL_PIN					/* SCL = 0 */
	
	#define macI2C_SDA_1(i)  macGPIO_PORT_I2C->BSRR = macI2C_SDA_PIN				/* SCL = 1 */
	#define macI2C_SDA_0(i)  macGPIO_PORT_I2C->BRR = macI2C_SDA_PIN					/* SCL = 0 */
	
	#define macI2C_SDA_READ(i)  ((macGPIO_PORT_I2C->IDR & macI2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
	#define macI2C_SCL_READ(i)  ((macGPIO_PORT_I2C->IDR & macI2C_SCL_PIN) != 0)	/* ��SCL����״̬ */
#endif

void i2c_Start(int port);
void i2c_Stop(int port);
void i2c_SendByte(uint8_t _ucByte, int port);
uint8_t i2c_ReadByte(int port);
uint8_t i2c_WaitAck(int port);
void i2c_Ack(int port);
void i2c_NAck(int port);
uint8_t i2c_CheckDevice(uint8_t _Address, int port);


#endif

