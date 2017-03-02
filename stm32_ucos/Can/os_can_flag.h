/*
*********************************************************************************************************
*
* Filename      : os_can_flag.h
* Version       : V1.00
* Programmer(s) : shijiangxiao, 2008-04-01
*********************************************************************************************************
*/
#ifndef OS_CAN_FLAG_H
#define OS_CAN_FLAG_H

/* CAN Error Status Register bits */
#define CAN_FLAG_ESR_EWGF     ((u32)0x00000001)    /* Error warning flag */
#define CAN_FLAG_ESR_EPVF     ((u32)0x00000002)    /* Error passive flag */
#define CAN_FLAG_ESR_BOFF     ((u32)0x00000004)    /* Bus-off flag */

#define CAN_FLAG_STUFF_ERROR         0x0000010	   //����������
#define CAN_FLAG_FORM_ERROR          0x0000020
#define CAN_FLAG_ACK_ERROR           0x0000040     //ȷ�϶�ʧ
#define CAN_FLAG_BIT_RECESSIVE_ERROR 0x0000080
#define CAN_FLAG_BIT_DOMINANT_ERROR  0x0000100
#define CAN_FLAG_CRC_ERROR           0x0000200     //У�����
#define CAN_FLAG_ERROR               0x0001000     //ֻҪ��һ������, ��λ��1

#define CAN_FLAG_TX_ERR_REACH       0x00002000    //���ʹ�����������ﾯ��ֵ
#define CAN_FLAG_RX_ERR_REACH       0x00004000    //���մ�����������ﾯ��ֵ

#define CAN_FLAG_TME0               0x00010000	  //Transmit Mailbox 0 Empty
#define CAN_FLAG_TME1               0x00020000	  //Transmit Mailbox 1 Empty
#define CAN_FLAG_TME2               0x00040000	  //Transmit Mailbox 2 Empty

#define CAN_FLAG_TRANSMIT_RQCP0     0x00100000    //RQCP0: Request Completed Mailbox0
#define CAN_FLAG_TRANSMIT_TXOK0     0x00200000    //TXOK0: Transmission OK of Mailbox0
#define CAN_FLAG_TRANSMIT_ALOST0    0x00400000    //ALST0: Arbitration Lost for Mailbox0
#define CAN_FLAG_TRANSMIT_TERR0     0x00800000    //TERR0: Transmission Error of Mailbox0

#define CAN_FLAG_TRANSMIT_RQCP1     0x01000000    //RQCP1: Request Completed Mailbox1
#define CAN_FLAG_TRANSMIT_TXOK1     0x02000000    //TXOK1: Transmission OK of Mailbox1
#define CAN_FLAG_TRANSMIT_ALOST1    0x04000000    //ALST1: Arbitration Lost for Mailbox1
#define CAN_FLAG_TRANSMIT_TERR1     0x08000000    //TERR1: Transmission Error of Mailbox1

#define CAN_FLAG_TRANSMIT_RQCP2     0x10000000    //RQCP2: Request Completed Mailbox2
#define CAN_FLAG_TRANSMIT_TXOK2     0x20000000    //TXOK2: Transmission OK of Mailbox2
#define CAN_FLAG_TRANSMIT_ALOST2    0x40000000    //ALST2: Arbitration Lost for Mailbox2
#define CAN_FLAG_TRANSMIT_TERR2     0x80000000    //TERR2: Transmission Error of Mailbox2

#define CAN_FLAG_RECEIVE_VALID       0x0008000    //����֡��Ч

/*******************************************************************************
             CAN������ǳ�ʼ��
*******************************************************************************/
BOOLEAN      CAN_FlagInit(void);

/*******************************************************************************
             CAN������ǵȴ�, ���嶨�嶨���OSFlagPend
*******************************************************************************/
OS_FLAGS     CAN_FlagPend            (OS_FLAGS         flags,
                                     INT8U            wait_type,
                                     INT16U           timeout,
                                     INT8U           *perr);

/*******************************************************************************
             CAN�����������, ���嶨�嶨���OSFlagPost
*******************************************************************************/
OS_FLAGS     CAN_FlagPost            (OS_FLAGS         flags,
                                     INT8U            opt,
                                     INT8U           *perr);

/*******************************************************************************
             CAN������ǲ�ѯ, ���嶨�嶨���OSFlagQuery
*******************************************************************************/
OS_FLAGS     CAN_FlagQuery           (INT8U *perr);


#endif
