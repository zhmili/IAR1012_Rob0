/*
*********************************************************************************************************
*                                        ÐÞÕý  ParseToStru() ÖÐµÄactlenth 20080415
* lenth include : DATA
* FSC = DATA/256
* eg:0x68,8,8,0x68,1,2,0x40,4,4,4,4,4,86,0x16
*********************************************************************************************************
*/
#include"co_drv.h"

#ifndef RS232_CAN_H
#define RS232_CAN_H

/*
*********************************************************************************************************
*                                        PC command
*********************************************************************************************************
*/
typedef enum can232_command_e
{
  CCM_FILTER_RESET		= 0x00,
  CCM_FILTER_ALL		= 0x01,
  CCM_FILTER_ADD		= 0x02,
  CCM_FILTER_REMOVE		= 0x03,
  CCM_SEND_REPEAT		= 0x04,
  CCM_SEND_SINGLE		= 0x05,
  CCM_BAUD_SET			= 0x06,
  CCM_RESET			= 0x07,
  CCM_RECV_FRAME		= 0x08,
  CCM_RECV_ERR			= 0x09,
}CAN232_COMMAND_E;


s8 CreatCO_MSG(FRAM_T FramT,	CO_MSG COFramT);

void * GetAFram(void);

s8  ParseFrame( );

// return lenth
s16 CheckHeadAndGetLE( void );

s8 CheckFCSAndGetdata( u8 *pfram, u8 lenth );

s8 CheckED( void );

s16 ParseToStru( u8 *pfram,  FRAM_T stcFramT );

s8 CcmFilterReset(void );
s8 CcmFilterAll(void);
s8 CcmReset(void);

//s16 StruToCanStru(	);

#endif
