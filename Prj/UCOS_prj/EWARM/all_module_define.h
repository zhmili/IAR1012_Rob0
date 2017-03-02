


#ifndef __ALL_MODULE_DEFINE_H
#define __ALL_MODULE_DEFINE_H
//-------------VUOper 
//--
// I2C_Channel
//            0 - I2C1
//            1 - I2C2:0 2-I2C2:1 3-I2C2:2 4 -I2C2:3 5 - I2C2:4

#define GPIOA_SPI3_CS             DEF_BIT_01
#define GPIOA_SPI3_SCK            DEF_BIT_02
#define GPIOC_SPI3_MOSI           DEF_BIT_04

#define GPIOA_SPI4_CS             DEF_BIT_03
#define GPIOC_SPI4_SCK            DEF_BIT_05
#define GPIOB_SPI4_MOSI           DEF_BIT_00

#define GPIOA_ET2                 DEF_BIT_00
#define GPIOA_MAX4444EN1          DEF_BIT_06
#define GPIOB_MAX4444EN0          DEF_BIT_01

#define GPIOA_USART1_TX           DEF_BIT_09
#define GPIOA_USART1_RX           DEF_BIT_10

#define GPIOA_SPI_CS              DEF_BIT_04
#define GPIOA_SPI_SCK             DEF_BIT_05
#define GPIOA_SPI_MOSI            DEF_BIT_07

#define GPIOB_SPI2_CS             DEF_BIT_12
#define GPIOB_SPI2_SCK            DEF_BIT_13
#define GPIOB_SPI2_MOSI           DEF_BIT_15

#define GPIOB_LM5720_SD_0         DEF_BIT_14
#define GPIOC_LM5720_SD_1         DEF_BIT_09


#define GPIOC_PCA9516_I2C_EN2     DEF_BIT_00
#define GPIOC_PCA9516_I2C_EN1     DEF_BIT_01
#define GPIOC_PCA9516_I2C_EN3     DEF_BIT_02
#define GPIOC_PCA9516_I2C_EN4     DEF_BIT_03

#define GPIOC_PLUSE               DEF_BIT_06

#define GPIOC_2TO1                DEF_BIT_07
#define GPIOC_ET1                 DEF_BIT_15

//#define GPIOB_DAT2_1              DEF_BIT_00
//#define GPIOC_CLK2_1              DEF_BIT_05
//#define GPIOA_LTC2_1              DEF_BIT_03
//#define GPIOC_DAT1_1              DEF_BIT_04
//#define GPIOA_CLK1_1              DEF_BIT_02
//#define GPIOA_LTC1_1              DEF_BIT_01

#define GPIOB_SCL1                DEF_BIT_06
#define GPIOB_SDA1                DEF_BIT_07

#define GPIOB_SCL2                DEF_BIT_10
#define GPIOB_SDA2                DEF_BIT_11


#define GPIOB_MAX4444EN0          DEF_BIT_01
#define GPIOA_USART1_TX           DEF_BIT_09
#define GPIOA_USART1_RX           DEF_BIT_10
#define GPIOA_SPI_CS              DEF_BIT_04
#define GPIOA_SPI_SCK             DEF_BIT_05
#define GPIOA_SPI_MISO            DEF_BIT_06
#define GPIOA_SPI_MOSI            DEF_BIT_07

#define GPIOB_SPI2_CS             DEF_BIT_12
#define GPIOB_SPI2_SCK            DEF_BIT_13
#define GPIOB_SPI2_MOSI           DEF_BIT_15

#define GPIOB_LM5720_SD_0         DEF_BIT_14
#define GPIOC_T_SD                DEF_BIT_09

#define GPIOC_PLUSE               DEF_BIT_06

#define GPIOB_AGD_S4              DEF_BIT_00
#define GPIOC_AGD_S3              DEF_BIT_05
#define GPIOA_AGD_S2              DEF_BIT_03
#define GPIOC_AGD_S1              DEF_BIT_04
#define GPIOA_AD8099_01           DEF_BIT_02
#define GPIOA_AD8099_00           DEF_BIT_01
#define GPIOA_IN1                 DEF_BIT_08
#define GPIOA_IN2                 DEF_BIT_11
#define GPIOC_FILTER_D1           DEF_BIT_14
#define GPIOC_FILTER_D2           DEF_BIT_13
#define GPIOB_FILTER_D3           DEF_BIT_09


#define RELAY_DELAY_TIMES_MS  20

#define I2C1_0       0
#define I2C2_1      GPIOC_PCA9516_I2C_EN1
#define I2C2_2      GPIOC_PCA9516_I2C_EN2
#define I2C2_3      GPIOC_PCA9516_I2C_EN3
#define I2C2_4      GPIOC_PCA9516_I2C_EN4

#define MAX4444_1  GPIOB_MAX4444EN0
#define MAX4444_2  GPIOA_MAX4444EN1

#define LM5720_1   GPIOB_LM5720_SD_0
#define LM5720_2   GPIOB_LM5720_SD_1

#define DNYAMP_1    1
#define DNYAMP_2    2
#define DNYAMP_3    3
#define DNYAMP_4    4




#define AD8099_0_IN  GPIOB_SPI2_CS
#define AD8099_1_IN  GPIOA_SPI_MOSI
#define DIR_IN       MAX4444_1

#define VA1          GPIOB_SPI2_SCK
#define VA2          GPIOB_SPI2_MOSI

#define PLUSE_EN(x)    x?GPIO_SetBits(GPIOC, GPIOC_PLUSE):GPIO_ResetBits(GPIOC, GPIOC_PLUSE)
#define SPI1_CS(x)     x?GPIO_SetBits(GPIOA,GPIOA_SPI_CS):GPIO_ResetBits(GPIOA,GPIOA_SPI_CS)
#define SPI2_CS(x)     x?GPIO_SetBits(GPIOB,GPIOB_SPI2_CS):GPIO_ResetBits(GPIOB,GPIOB_SPI2_CS)
#define SPI3_CS(x)     x?GPIO_SetBits(GPIOA,GPIOA_SPI3_CS):GPIO_ResetBits(GPIOA,GPIOA_SPI3_CS)
#define SPI4_CS(x)     x?GPIO_SetBits(GPIOA,GPIOA_SPI4_CS):GPIO_ResetBits(GPIOA,GPIOA_SPI4_CS)


#define SPI1_SCK(x)    x?GPIO_SetBits(GPIOA,GPIOA_SPI_SCK):GPIO_ResetBits(GPIOA,GPIOA_SPI_SCK)
#define SPI1_MOSI(x)   x?GPIO_SetBits(GPIOA,GPIOA_SPI_MOSI):GPIO_ResetBits(GPIOA,GPIOA_SPI_MOSI)
#define SPI2_SCK(x)    x?GPIO_SetBits(GPIOB,GPIOB_SPI2_SCK):GPIO_ResetBits(GPIOB,GPIOB_SPI2_SCK)
#define SPI2_MOSI(x)   x?GPIO_SetBits(GPIOB,GPIOB_SPI2_MOSI):GPIO_ResetBits(GPIOB,GPIOB_SPI2_MOSI)

#define SPI3_SCK(x)    x?GPIO_SetBits(GPIOA,GPIOA_SPI3_SCK):GPIO_ResetBits(GPIOA,GPIOA_SPI3_SCK)
#define SPI3_MOSI(x)   x?GPIO_SetBits(GPIOC,GPIOC_SPI3_MOSI):GPIO_ResetBits(GPIOC,GPIOC_SPI3_MOSI)
#define SPI4_SCK(x)    x?GPIO_SetBits(GPIOC,GPIOC_SPI4_SCK):GPIO_ResetBits(GPIOC,GPIOC_SPI4_SCK)
#define SPI4_MOSI(x)   x?GPIO_SetBits(GPIOB,GPIOB_SPI4_MOSI):GPIO_ResetBits(GPIOB,GPIOB_SPI4_MOSI)

#define SPI1_SCKD       SPI1_SCK(0); SPI1_SCK(1)
#define SPI2_SCKD       SPI2_SCK(0); SPI2_SCK(1)
#define SPI3_SCKD       SPI3_SCK(0); SPI3_SCK(1)
#define SPI4_SCKD       SPI4_SCK(0); SPI4_SCK(1)

#define MAX4444_1_EN(x) x?GPIO_SetBits(GPIOB,GPIOB_MAX4444EN0):GPIO_ResetBits(GPIOB,GPIOB_MAX4444EN0)
#define MAX4444_2_EN(x) x?GPIO_SetBits(GPIOA,GPIOA_MAX4444EN1):GPIO_ResetBits(GPIOA,GPIOA_MAX4444EN1)

//-- low affect
#define LM5720_1_OP_EN(x) x?GPIO_SetBits(GPIOB,GPIOB_LM5720_SD_0):GPIO_ResetBits(GPIOB,GPIOB_LM5720_SD_0)
#define LM5720_2_OP_EN(x) x?GPIO_SetBits(GPIOC,GPIOC_LM5720_SD_1):GPIO_ResetBits(GPIOC,GPIOC_LM5720_SD_1)

#define I2C2_1_EN(x) x?GPIO_SetBits(GPIOC,GPIOC_PCA9516_I2C_EN1):GPIO_ResetBits(GPIOC,GPIOC_PCA9516_I2C_EN1)
#define I2C2_2_EN(x) x?GPIO_SetBits(GPIOC,GPIOC_PCA9516_I2C_EN2):GPIO_ResetBits(GPIOC,GPIOC_PCA9516_I2C_EN2)
#define I2C2_3_EN(x) x?GPIO_SetBits(GPIOC,GPIOC_PCA9516_I2C_EN3):GPIO_ResetBits(GPIOC,GPIOC_PCA9516_I2C_EN3)
#define I2C2_4_EN(x) x?GPIO_SetBits(GPIOC,GPIOC_PCA9516_I2C_EN4):GPIO_ResetBits(GPIOC,GPIOC_PCA9516_I2C_EN4)

#define SEL2TO1(x) x?GPIO_SetBits(GPIOC,GPIOC_2TO1):GPIO_ResetBits(GPIOC,GPIOC_2TO1)

//---MD OPER 
//---Hold Registor Write
#define I2C_SEND_REL_LOC    1
#define I2C_SEND_REL_SET    23
#define I2C_SEND_ADD  2

#define I2C_S1_1       3
#define I2C_S1_2       10
#define I2C_S3         7

#define I2C_S1_1_ADD   4
#define FIR_CHN_AMP_1  5
#define SEC_CHN_AMP_1  6
#define I2C_S3_ADD     8

#define I2C_S1_2_ADD   11
#define I2C_REL        15
#define I2C_REL_ADD    17
#define FIR_CHN_AMP_2  12
#define SEC_CHN_AMP_2  13
#define PLUSE_WIDE_SET 16

#define S_FIR_CHN_AMP_1 18
#define S_FIR_CHN_AMP_2 19
#define S_SEC_CHN_AMP_1 20
#define S_SEC_CHN_AMP_2 21

#define I2C_CIR_REL_LOC 22

#define FREQ_SET        24

#define DAC_1_DATA      25
#define DAC_2_DATA      26


#define I2C1_READ_TEST  9

//---Coin Write 
#define MAX4444_1_MD    1
#define LMH6720EN_1_MD  2

#define AMP_EN_G1      3   
#define AMP_EN_G2      8

#define I2C_6720EN_1   10
#define I2C_6720EN_2   11

#define GPLUSE_MD_VUS_CLR 4  // clear VU_SEND FLAG
#define MAX4444_2_MD      5
#define LMH6720EN_2_MD    6
#define S2TO1_MD          7
#define GPLUSE_MD         9

#define I2C_READ_TEST_C  13

//#define I2C_SEND_MR       13
//#define I2C_SEND_CM       14

#define DEFAULT_SET  12

#define VAL_1        14
#define VAL_2        15

#define AD8099_1     16
#define AD8099_2     17
#define DIR_IN       18

#endif