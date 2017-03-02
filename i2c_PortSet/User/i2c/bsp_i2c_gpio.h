#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H


#include <inttypes.h>
#include "bsp_rob0.h"

extern TGPIO_STATE GPIO_STATE[16];

#define macI2C_WR	0		/* 写控制bit */
#define macI2C_RD	1		/* 读控制bit */

/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define macGPIO_PORT_I2C	GPIOB			/* GPIO端口 */
#define macRCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define macI2C_SCL_PIN		GPIO_Pin_6			/* 连接到SCL时钟线的GPIO */
#define macI2C_SDA_PIN		GPIO_Pin_7			/* 连接到SDA数据线的GPIO */


/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 0	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
	#define macI2C_SCL_1()  GPIO_SetBits(macGPIO_PORT_I2C, macI2C_SCL_PIN)		/* SCL = 1 */
	#define macI2C_SCL_0()  GPIO_ResetBits(macGPIO_PORT_I2C, macI2C_SCL_PIN)		/* SCL = 0 */
	
	#define macI2C_SDA_1()  GPIO_SetBits(macGPIO_PORT_I2C, macI2C_SDA_PIN)		/* SDA = 1 */
	#define macI2C_SDA_0()  GPIO_ResetBits(macGPIO_PORT_I2C, macI2C_SDA_PIN)		/* SDA = 0 */
	
	#define macI2C_SDA_READ()  GPIO_ReadInputDataBit(macGPIO_PORT_I2C, macI2C_SDA_PIN)	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define macI2C_SCL_1(i)  GPIO_STATE[i].out_port->BSRR = GPIO_STATE[i].out_pin				/* SCL = 1 */
	#define macI2C_SCL_0(i)  GPIO_STATE[i].out_port->BRR = GPIO_STATE[i].out_pin					/* SCL = 0 */
	
	#define macI2C_SDA_1(i)  GPIO_STATE[i].tim_port->BSRR = GPIO_STATE[i].tim_pin				/* SCL = 1 */
	#define macI2C_SDA_0(i)  GPIO_STATE[i].tim_port->BRR = GPIO_STATE[i].tim_pin					/* SCL = 0 */
	
//	#define macI2C_SDA_READ(i)   GPIO_ReadInputDataBit(GPIO_STATE[i].tim_port, GPIO_STATE[i].tim_pin) /* 读SDA口线状态 */
//	#define macI2C_SCL_READ(i)   GPIO_ReadInputDataBit(GPIO_STATE[i].out_port, GPIO_STATE[i].out_pin) /* 读SCL口线状态 */
	#define macI2C_SDA_READ(i)  ((GPIO_STATE[i].tim_port->IDR & GPIO_STATE[i].tim_pin) != 0)	/* 读SDA口线状态 */
	#define macI2C_SCL_READ(i)  ((GPIO_STATE[i].out_port->IDR & GPIO_STATE[i].out_pin) != 0)	/* 读SCL口线状态 */
	
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

