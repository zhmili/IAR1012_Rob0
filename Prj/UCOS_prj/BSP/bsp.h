/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

#ifndef  __BSP_H__
#define  __BSP_H__

typedef OS_EVENT    *BSP_OS_SEM;
static  BSP_OS_SEM   BSP_SerLockRx;

#define ROB_0                                             0
#define UART_PRINTF                                       1

#define  DEF_FAIL                                         0
#define  DEF_OK                                           1

#define DOWNLOAD_PORT                                     5
#define DIGI_REC                                          3     //数字识别

/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/
#ifdef   BSP_GLOBALS
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif

//IO口宏操作定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址印射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO只对单一的IO
//确保地址小于16
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入




/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/


#define ADC1_DR_Address    ((u32)0x4001244C)
#define USART3_DR_Base     ((u32)0x40004804)
#define VCFILTERTMS 8

//硬件的通道说明
// 对于用户 1-32       1-8    9-16  17-24 25-32
// 对于程序 0-32       0-8    9-16  17-24 25-32  //其中0 通道为 判断VVAL
     
#define  DEF_TRUE    1
#define  DEF_FALSE   0
#define  DEF_BIT_NONE                                   0x00

#define  DEF_BIT_00                                     0x01
#define  DEF_BIT_01                                     0x02
#define  DEF_BIT_02                                     0x04
#define  DEF_BIT_03                                     0x08
#define  DEF_BIT_04                                     0x10
#define  DEF_BIT_05                                     0x20
#define  DEF_BIT_06                                     0x40
#define  DEF_BIT_07                                     0x80

#define  DEF_BIT_08                                   0x0100
#define  DEF_BIT_09                                   0x0200
#define  DEF_BIT_10                                   0x0400
#define  DEF_BIT_11                                   0x0800
#define  DEF_BIT_12                                   0x1000
#define  DEF_BIT_13                                   0x2000
#define  DEF_BIT_14                                   0x4000
#define  DEF_BIT_15                                   0x8000

#define  DEF_BIT_16                               0x00010000
#define  DEF_BIT_17                               0x00020000
#define  DEF_BIT_18                               0x00040000
#define  DEF_BIT_19                               0x00080000
#define  DEF_BIT_20                               0x00100000
#define  DEF_BIT_21                               0x00200000
#define  DEF_BIT_22                               0x00400000
#define  DEF_BIT_23                               0x00800000

#define  DEF_BIT_24                               0x01000000
#define  DEF_BIT_25                               0x02000000
#define  DEF_BIT_26                               0x04000000
#define  DEF_BIT_27                               0x08000000
#define  DEF_BIT_28                               0x10000000
#define  DEF_BIT_29                               0x20000000
#define  DEF_BIT_30                               0x40000000
#define  DEF_BIT_31                               0x80000000


typedef            void      CPU_VOID;
typedef            char      CPU_CHAR;                          /*  8-bit character                                     */
typedef  unsigned  char      CPU_BOOLEAN;                       /*  8-bit boolean or logical                            */
typedef  unsigned  char      CPU_INT08U;                        /*  8-bit unsigned integer                              */
typedef    signed  char      CPU_INT08S;                        /*  8-bit   signed integer                              */
typedef  unsigned  short     CPU_INT16U;                        /* 16-bit unsigned integer                              */
typedef    signed  short     CPU_INT16S;                        /* 16-bit   signed integer                              */
typedef  unsigned  long      CPU_INT32U;                        /* 32-bit unsigned integer                              */
typedef    signed  long      CPU_INT32S;                        /* 32-bit   signed integer                              */
typedef            float     CPU_FP32;                          /* 32-bit floating point                                */
typedef            double    CPU_FP64;                          /* 64-bit floating point                                */


typedef enum led{
    LED_OFF = 0,
    LED_ON = 1,

}ELED;


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
// use for IARBOARD


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

//UseFor robort - V2.0
/* ---------------------- GPIOA Pins ---------------------- */
#define GPIOA_ADC_IN0                    DEF_BIT_00
#define GPIOA_ADC_IN1                    DEF_BIT_01
#define GPIOA_ADC_IN2                    DEF_BIT_02
#define GPIOA_ADC_IN3                    DEF_BIT_03
#define GPIOA_ADC_IN4                    DEF_BIT_04
#define GPIOA_ADC_IN5                    DEF_BIT_05
#define GPIOA_ADC_IN6                    DEF_BIT_06
#define GPIOA_ADC_IN7                    DEF_BIT_07

#define GPIOA_485_EN1                    DEF_BIT_08
#define GPIOA_UART1_TX                   DEF_BIT_09
#define GPIOA_UART1_RX                   DEF_BIT_10
#define GPIOA_USB_DM                     DEF_BIT_11
#define GPIOA_USB_DP                     DEF_BIT_12
#define GPIOA_SW_DIO                     DEF_BIT_13
#define GPIOA_SW_CLK                     DEF_BIT_14
#define GPIOA_485_EN2                    DEF_BIT_15

/* ---------------------- GPIOB Pins ---------------------- */
//-----------ROBOT V2.0
#define GPIOB_ADC_IN8                    DEF_BIT_00
#define GPIOB_ADC_IN9                    DEF_BIT_01

#define GPIOB_SPI3_CLK                   DEF_BIT_03
#define GPIOB_SPI3_MISO                  DEF_BIT_04
#define GPIOB_SPI3_MOSI                  DEF_BIT_05
//#define GPIOB_CMOTOR1_1                 DEF_BIT_06
#define GPIOB_KEY4                       DEF_BIT_06 //

#define GPIOB_DB6                        DEF_BIT_07 //
#define GPIOB_I2C1_SCL                   DEF_BIT_08
#define GPIOB_I2C1_SDA                   DEF_BIT_09
#define GPIOB_I2C2_SCL                   DEF_BIT_10
#define GPIOB_I2C2_SDA                   DEF_BIT_11
#define GPIOB_CD4510_S0                  DEF_BIT_12
#define GPIOB_SPI2_SCK_IN1               DEF_BIT_13
#define GPIOB_SPI2_MISO_IN2                  DEF_BIT_14
#define GPIOB_SPI2_MOSI_IN3                  DEF_BIT_15

/* ---------------------- GPIOC Pins ---------------------- */
//-----------ROBOT V2.0
#define GPIOC_ADC_IN10                    DEF_BIT_00
#define GPIOC_ADC_IN11                    DEF_BIT_01
#define GPIOC_ADC_IN12                    DEF_BIT_02
#define GPIOC_ADC_IN13                    DEF_BIT_03
#define GPIOC_ADC_IN14                    DEF_BIT_04

#define GPIOC_ADC_IN15                    DEF_BIT_05
#define GPIOC_TIM8_CH1_DS1                DEF_BIT_06
#define GPIOC_TIM8_CH2_DS2                DEF_BIT_07
#define GPIOC_TIM8_CH3_BEEP               DEF_BIT_08
#define GPIOC_TIM8_CH4_IN4                DEF_BIT_09
#define GPIOC_UART4_TX                    DEF_BIT_10
#define GPIOC_UART4_RX                    DEF_BIT_11
#define GPIOC_UART5_TX                    DEF_BIT_12

#define GPIOC_PKT                         DEF_BIT_13 //
#define GPIOC_SPI3_SS                     DEF_BIT_14 
#define GPIOC_CMOTOR6_1                   DEF_BIT_15

/* ---------------------- GPIOD Pins ---------------------- */

//#define GPIOD_START_STOP                  DEF_BIT_00
#define GPIOD_W_KEY                       DEF_BIT_00
#define GPIOD_E4                          DEF_BIT_01
#define GPIOD_UART5_RX                    DEF_BIT_02
#define GPIOD_E5                        DEF_BIT_03
#define GPIOD_E6                        DEF_BIT_04
#define GPIOD_UART2_TX_E3                    DEF_BIT_05
#define GPIOD_UART2_RX_TP_IRQ                   DEF_BIT_06
#define GPIOD_T1_LCD_LED_EN                          DEF_BIT_07
#define GPIOD_UART3_TX                    DEF_BIT_08
#define GPIOD_UART3_RX                    DEF_BIT_09
#define GPIOD_CD4510_S1                   DEF_BIT_10
#define GPIOD_CD4510_S2                   DEF_BIT_11
#define GPIOD_TIM4_CH1                    DEF_BIT_12
#define GPIOD_TIM4_CH2_M4                    DEF_BIT_13
#define GPIOD_TIM4_CH3_M5                   DEF_BIT_14
#define GPIOD_TIM4_CH4_M6                    DEF_BIT_15


/* ---------------------- GPIOE Pins ---------------------- */

#define GPIOE_DB7                         DEF_BIT_00
#define GPIOE_T2                          DEF_BIT_01
#define GPIOE_USB_EN                      DEF_BIT_02
#define GPIOE_CMOTOR3_2                   DEF_BIT_03
#define GPIOE_CMOTOR2_2                   DEF_BIT_04
#define GPIOE_CMOTOR1_2                   DEF_BIT_05
#define GPIOE_RF_RESET                   DEF_BIT_06
#define GPIOE_E1                      DEF_BIT_07
#define GPIOE_TIM1_CH1N                   DEF_BIT_08
#define GPIOE_TIM1_CH1_DS6                    DEF_BIT_09
#define GPIOE_TIM1_CH2N                        DEF_BIT_10
#define GPIOE_TIM1_CH2_DS5                    DEF_BIT_11
#define GPIOE_LCD_RS                      DEF_BIT_12
#define GPIOE_TIM1_CH3_DS4                    DEF_BIT_13
#define GPIOE_TIM1_CH4_DS3                    DEF_BIT_14
#define GPIOE_LCD_DB4                     DEF_BIT_15

/* ---------------------- GPIOF Pins ---------------------- */
#define GPIOF_COMTRO6_2                   DEF_BIT_00
#define GPIOF_COMTRO5_2                   DEF_BIT_01
#define GPIOF_COMTRO4_2                   DEF_BIT_02
#define GPIOF_COMTRO5_1                   DEF_BIT_03
#define GPIOF_COMTRO4_1                    DEF_BIT_04
#define GPIOF_245EN_2                  DEF_BIT_05

#define GPIOF_ADC_IN16                    DEF_BIT_06
#define GPIOF_ADC_IN17                    DEF_BIT_07
#define GPIOF_ADC_IN18                    DEF_BIT_08
#define GPIOF_ADC_IN19                    DEF_BIT_09
#define GPIOF_ADC_IN20                    DEF_BIT_10

#define GPIOF_LCD_DB0                     DEF_BIT_11
#define GPIOF_LCD_DB1                     DEF_BIT_12
#define GPIOF_FAN_2                       DEF_BIT_13

#define GPIOF_DC_RW                       DEF_BIT_14
#define GPIOF_LCD_EN                      DEF_BIT_15

/* ---------------------- GPIOG Pins ---------------------- */
#define GPIOG_FAN_1                       DEF_BIT_00
#define GPIOG_245EN_1                     DEF_BIT_01
#define GPIOG_LCD_DB2                     DEF_BIT_02
#define GPIOG_LCD_DB3                     DEF_BIT_03  


#define GPIOG_LCD_DB5                     DEF_BIT_05
#define GPIOG_INT1                        DEF_BIT_06
#define GPIOG_485_EN3                     DEF_BIT_07
#define GPIOG_LED                         DEF_BIT_08

#define GPIOG_E2                         DEF_BIT_09

#define GPIOG_USB_TEST                    DEF_BIT_10

//#define GPIOG_KEY3                        DEF_BIT_09
//#define GPIOG_KEY4                        DEF_BIT_10

#define GPIOG_START_STOP              DEF_BIT_11
#define GPIOG_KEY_3                   DEF_BIT_12
#define GPIOG_KEY_2                   DEF_BIT_13

#define GPIOG_KEY_1                   DEF_BIT_14
#define GPIOG_REM                     DEF_BIT_15

#define A20S0PB12    (1<<12)
#define A20S1PD10    (1<<10)
#define A20S2PD11    (1<<11)

#define SA20S0PB12(x)     GPIOB->ODR = (GPIOB->ODR & ~A20S0PB12) | (x ? A20S0PB12 :0);
#define SA20S1PD10(x)     GPIOD->ODR = (GPIOD->ODR & ~A20S1PD10) | (x ? A20S1PD10 :0);
#define SA20S2PD11(x)     GPIOD->ODR = (GPIOD->ODR & ~A20S2PD11) | (x ? A20S2PD11 :0);

#define SA20S(x)       SA20S0PB12(x&0x01);SA20S1PD10((x&0x02)>>1);SA20S2PD11((x&0x04)>>2);   

#define ACK_ERROR       1
#define NO_ACK          1
#define ACK             0
#define HIGH            1
#define LOW             0
#define IIC2_SDA        PBout(7) 
#define READ2_SDA 	PBin(7) 
#define IIC2_SCL 	PBout(6)
#define I2C_delay()  (DelayNus(12))


#define CMOTOR1IN	(1<< 4) //CMOTOR4_1O PF.4
#define CMOTOR1IN_2IN	(1<< 2) //CMOTOR4_2O PF.2  
#define CMOTOR2IN       (1<< 3) //CMOTOR5_1O PF.3
#define CMOTOR2IN_2IN   (1<< 1) //CMOTOR5_2O PF.1
#define CMOTOR3IN       (1<< 15) //CMOTOR6_1O PC.15
#define CMOTOR3IN_2IN   (1<< 0)//CMOTOR6_2O PF.0


#define HC245EN1        (1<<1)  //PG.1
#define HC245EN2        (1<<5)  //PF.5
#define HC245EN3        (1<<15)  //PC.15

#define CMOTOR1(x)      GPIOF->ODR = (GPIOF->ODR & ~CMOTOR1IN)  | (x ? CMOTOR1IN: 0);
#define CMOTOR1_2(x)	GPIOF->ODR = (GPIOF->ODR & ~CMOTOR1IN_2IN)  | (x ? CMOTOR1IN_2IN: 0);
#define CMOTOR2(x)	GPIOF->ODR = (GPIOF->ODR & ~CMOTOR2IN)  | (x ? CMOTOR2IN: 0);
#define CMOTOR2_2(x)	GPIOF->ODR = (GPIOF->ODR & ~CMOTOR2IN_2IN)  | (x ? CMOTOR2IN_2IN: 0);
#define CMOTOR3(x)      GPIOC->ODR = (GPIOC->ODR & ~CMOTOR3IN)  | (x ? CMOTOR3IN: 0);
#define CMOTOR3_2(x)	GPIOF->ODR = (GPIOF->ODR & ~CMOTOR3IN_2IN)  | (x ? CMOTOR3IN_2IN: 0);


#define SHC245EN1(x)     GPIOG->ODR = (GPIOG->ODR & ~HC245EN1)  | (x ? HC245EN1: 0);
#define SHC245EN2(x)     GPIOF->ODR = (GPIOF->ODR & ~HC245EN2)  | (x ? HC245EN2: 0);
#define SHC245EN3(x)     GPIOC->ODR = (GPIOG->ODR & ~HC245EN3)  | (x ? HC245EN3: 0);

#define LED(x)   x?(GPIOG->BSRR = GPIO_Pin_10):(GPIOG->BRR = GPIO_Pin_10);

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void            Tick_Init               (void);
void            BSP_IntDisAll          (void);
CPU_INT32U      BSP_CPU_ClkFreq        (void);

void		    BSP_InitROB(void);
/*
*********************************************************************************************************
*                                             LED SERVICES
*********************************************************************************************************
*/

void            LED_On                 (CPU_INT08U led);
void            LED_Off                (CPU_INT08U led);
void            LED_Toggle             (CPU_INT08U led);

/*
*********************************************************************************************************
*                                             TICK SERVICES
*********************************************************************************************************
*/

void            Tmr_TickISR_Handler    (void);

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void DMA_Configuration(void);
void LcdShow_Init(void);
CPU_BOOLEAN  BSP_OS_SemWait (BSP_OS_SEM  sem, CPU_INT32U  dly_ms);
CPU_BOOLEAN  BSP_OS_SemPost (BSP_OS_SEM  sem);
void ADC_Configruation(void);

INT32U  SysGet100UsTick(void);
u16 RS485_Receive(USART_TypeDef* USARTx);
void RS485_Send(USART_TypeDef* USARTx,u16 data);
u16 UART_Receive(USART_TypeDef* USARTx);
void UART_Send(USART_TypeDef* USARTx,u16 data);

void ExternalIntInit(void);

void SPI_Configuration(void);

void RCC_ConfigurationROB(void);
void GPIO_ConfigurationROB(void);
void NVIC_ConfigurationROB(void);
void TIMEx_Base_Configruation(void);

void SetPWM(TIM_TypeDef* TIMx, u16 TIM_Channel_x, s16 eff);

void ADC_Configuration(void);
u16 ADC_GetConvertValue(ADC_TypeDef*  ADCx ,u8 adc_channel);
//u16 ADC_GetConvertValueAVE(ADC_TypeDef*  ADCx ,u8 adc_channel);

void UART2_Init(void);
void UART3_InitT(void);
void UART3_Init( u16 baud, u8 length, u8 parity, u8 stop);
void UART3_InitB( u16 baud, u8 length, u8 parity, u8 stop);
void UART3_Send(u16 data);
u16 UART3_Receive(void);
u16 UART3_ReceiveB(void);

void RxTxISR_Handler (u8 port);

void UXART_Init(USART_TypeDef* UXARTx,u32 USART_BaudRate,u16 USART_WordLength,u16 USART_StopBits,u16 USART_Parity,u8 IT_RX_EN,u8 IT_TX_EN);

u16 midavrfilter(u16 value_buf[], u8 size);

void DispParm(void);

void CreatTask(int i);

void start_mainX(void *p_arg);
//Flash operation
void SetParms(void);
void DispAllParms(void);
int  GetRemData(int Ax);
void SysDAC_Init(void);

int  UARTReceive(void);
void UARTSend(unsigned short data);
int  RS485Receive(int No);
void RS485Send(int No, unsigned short data);

void DelayNus(u16 us);
void DelayNms(u16 ms);

int GetVoiceIndex(void);
int SetDownLoadRecorder(void);
void led_flash(ELED en);


//ucModbus 中使用 
USART_TypeDef    *GetUSARTx(CPU_INT08U  port_nbr);


//FLASH 操作用
#if ROB_0
#define PAGE_SIEZ   ((u16)0x800)
#define FLASH_SIZE  (0x40000)     //256k
#else
#define PAGE_SIEZ   ((u16)0x800)
#define FLASH_SIZE  (0x80000)     //512k
#endif

#define AppBaseAddr	        0x8000000
#define AppEndAddr		(AppBaseAddr + FLASH_SIZE)
#define AppDownloadRecAddr	(AppEndAddr - PAGE_SIEZ)
#define ApplicationAddress      0x8003000       
#define ApplicationSize         (FLASH_SIZE - ( ApplicationAddress - AppBaseAddr))  

#define TOP_RAM_ADDR					(0x20000000+0x0C000)
#define TOP_RAM_ADDR_P1					(TOP_RAM_ADDR - 4)
#define TOP_RAM_ADDR_P2					(TOP_RAM_ADDR_P1 - 4)
#define TOP_RAM_ADDR_P3					(TOP_RAM_ADDR_P2 - 4)

#endif                                                          /* End of module include.     */
