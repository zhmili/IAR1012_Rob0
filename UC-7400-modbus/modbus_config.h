/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : ModbusConfig.h
* Version       : V1.00
*********************************************************************************************************
*/

#ifndef  MODBUS_CONFIG_H
#define  MODBUS_CONFIG_H

#define MAX_BUFFER_SIZE                          128
#define MAX_MODBUS_PORT_NUM                        6
#define MAX_SUPPORT_SLAVE_PORT                     1    /* 最大支持的从机串口数 */

//MODBUS配置
#define MBF_READ_COILS_EN                          0   //
#define MBF_READ_DECRETE_INPUTS_EN                 0   //
#define MBF_READ_HOLDING_REGISTERS_EN              1
#define MBF_READ_INPUT_REGISTERS_EN                0   //
#define MBF_WRITE_SINGLE_COIL_EN                   0   //
#define MBF_WRITE_SINGLE_REGISTERS_EN              1
#define MBF_READ_EXCEPTION_STATUS_EN               0   //
#define MBF_DIAGNOSTIC_EN                          0   //
#define MBF_GET_COMM_EVENT_COUNTER_EN              0   //
#define MBF_GET_COMM_EVENT_LOG_EN                  0   //
#define MBF_WRITE_MULTIPLE_COILS_EN                0   //
#define MBF_WRITE_MULTIPLE_REGISTERS_EN            1
#define MBF_REPORT_SLAVE_ID_EN                     0   //
#define MBF_READ_FILE_RECORD_EN                    0   //
#define MBF_WRITE_FILE_RECORD_EN                   0   //
#define MBF_MASK_WRITE_REGISTER_EN                 0
#define MBF_READ_WRITE_MULTIPLE_REGISTERS_EN       1
#define MBF_READ_FIFO_QUEUE_EN                     0   //
#define MBF_REPORT_SLAVE_ID_EN                     0


#define U32_MAX_VALUE                           0xFFFFFFFF

#define MB_TCP_PORT                             502
#define MB_RTU_ADU_MAX_LENGTH                   128
#define MB_ASCII_ADU_MAX_LENGTH                 256

#define MB_RTU_PROTOCOL                         0
#define MB_ASCII_PROTOCOL                       1
#define MB_TCP_PROTOCOL                         2

#define MAX_BUFFER_SIZE                         128
#define MAX_485_NUM                             256

#define MBF_READ_DECRETE_INPUTS                 0x02
#define MBF_READ_COILS                          0x01
#define MBF_WRITE_SINGLE_COIL                   0x05
#define MBF_WRITE_MULTIPLE_COILS                0x0F
#define MBF_READ_INPUT_REGISTERS                0x04
#define MBF_READ_HOLDING_REGISTERS              0x03
#define MBF_WRITE_SINGLE_REGISTER               0x06
#define MBF_WRITE_MULTIPLE_REGISTERS            0x10
#define MBF_READ_WRITE_MULTIPLE_REGISTERS       0x17
#define MBF_MASK_WRITE_REGISTER                 0x16
#define MBF_READ_FIFO_QUEUE                     0x18
#define MBF_READ_FILE_RECORD                    0x14
#define MBF_WRITE_FILE_RECORD                   0x15
#define NBSF_FILE_RECORD                        0x06
#define MBF_READ_EXCEPTION_STATUS               0x07
#define MBF_DIAGNOSTIC                          0x08
#define MBF_GET_COMM_EVENT_COUNTER              0x0B
#define MBF_GET_COMM_EVENT_LOG                  0x0C
#define MBF_REPORT_SLAVE_ID                     0x11
#define MBF_READ_DEVICE_IDENTIFICATION          0x2B
#define MBF_HAS_EXCEPTION                       0x80
#define MBF_MASK                                0x7F

#define MB_ERROR_FREE                           0
#define MB_OK                                   0
#define MB_ERROR_FORMAT                         -1
#define MB_ERROR_LENGTH                         -2
#define MB_ERROR_PARAMETER                      -3
#define MB_ERROR_OPEN                           -4
#define MB_ERROR_TCP_NOT_CONNECTED              -5
#define MB_ERROR_NOT_OPENED                     -6
#define MB_ERROR_BUFFER_TOO_SHORT               -7
#define MB_ERROR_NO_FRAME                       -8
#define MB_ERROR_TIMEOUT                        -9
#define MB_ERROR_EXECPTION                      -10
#define MB_ERROR_BAD_FD                         -11
#define MB_ERROR_NET                            -12
#define MB_ERROR_NO_FILE_HANDLE                 -13
#define MB_ERROR_PROTOCOL                       -14
#define MB_ERROR_FUNCTION                       -15
#define MB_ERROR_MODE                           -16
#define MB_ERROR_ADU_LENTH                      -17

#endif
