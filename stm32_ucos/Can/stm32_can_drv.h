/*
*********************************************************************************************************
*
* Filename      : stm32_can_drv.h
* Version       : V1.00
* Programmer(s) : shijiangxiao, 2007-04-01
*********************************************************************************************************
*/

#ifndef STM32_CAN_DRV_H
#define STM32_CAN_DRV_H

#include "co_drv.h"

/*******************************************************************************
* Function Name  : CAN_COMsgTransmit
                 pmsg:  特发送的信息帧
      TransmitMailbox:  从该邮箱发送
  返回:  TRUE: 该邮箱可以允许信息发送
        FALSE: 邮箱被占用
*******************************************************************************/
BOOLEAN CAN_COMsgTransmit(CO_MSG* pmsg, INT32U TransmitMailbox);


/*******************************************************************************
* Function Name  : CAN_COMsgReceive
       FIFONumber:  从该FIFO中接收帧信息 
             pmsg:  待接收的信息帧
  返回:  TRUE: 接收到有效帧
         FALSE: 没有接收到有效帧
*******************************************************************************/
BOOLEAN CAN_COMsgReceive(u8 FIFONumber, CO_MSG* RxMessage);

#endif
