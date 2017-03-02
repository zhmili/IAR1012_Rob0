#ifndef __I2C_EE_H
#define	__I2C_EE_H


#include <includes.h>


/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* AT24C01/02每页有8个字节 
 * AT24C04/08A/16A每页有16个字节 
 */



#define macEE_DEV_ADDR	          0xA0		/* 24xx02的设备地址 */
#define macEE_PAGE_SIZE		  8			  /* 24xx02的页面大小 */
#define macEE_SIZE		  256			  /* 24xx02总容量 */

//电机部分参数
#define MTCMD_LOC 2
#define SPEED_LOC 8
#define DLY_LOC  14
#define IOCMD_LOC 26
#define MOTRES_LOC 30


typedef enum{
  COM_OK = 0,
  COM_ERR = -1
}EI2C_ERR;


typedef enum{
  MT_NOCMD = 0,
  MT_RUN = 1,
  MT_STOP = 2,
  MT_RUN_DLY = 3,
  MT_RESET = 4
}EMOT_CMD;

typedef enum{
  MT_NRES =0,
  MT_RES = 1
}EMT_RES;

typedef enum{
  IO_RESET = 0,
  IO_SET = 1,
}EIO_CMD;

u8 ee_CheckOk(int port);
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize, int port);
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize, int port);
uint8_t i2c_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize, int port);
void ee_Erase(int port);
void ee_Test(int port);


#endif /* __I2C_EE_H */
