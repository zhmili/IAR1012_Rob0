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
                 pmsg:  �ط��͵���Ϣ֡
      TransmitMailbox:  �Ӹ����䷢��
  ����:  TRUE: ���������������Ϣ����
        FALSE: ���䱻ռ��
*******************************************************************************/
BOOLEAN CAN_COMsgTransmit(CO_MSG* pmsg, INT32U TransmitMailbox);


/*******************************************************************************
* Function Name  : CAN_COMsgReceive
       FIFONumber:  �Ӹ�FIFO�н���֡��Ϣ 
             pmsg:  �����յ���Ϣ֡
  ����:  TRUE: ���յ���Ч֡
         FALSE: û�н��յ���Ч֡
*******************************************************************************/
BOOLEAN CAN_COMsgReceive(u8 FIFONumber, CO_MSG* RxMessage);

#endif
