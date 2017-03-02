 /*---------------------------------------------------------------------------*/
 /*---------------------------------------------------------------------------*/
#ifndef MBSERIAL_H
#define MBSERIAL_H

struct MB_SERIAL_INFO
{
         char    Protocol;                       
         u32     FrameTimeout;                   
         u32     CharTimeout;                    
         u32     Timeout;                        
};

typedef struct recv_buf
{
  u16 buf[MAX_BUFFER_SIZE];
  u16 act_lenth;
  
}TRecvBuf;


u16 SerialDataInInputQueue(int port);
u16 SerialBlockRead(int port, u8* buf, int len);
u16 SerialWrite( int port, u8* adu, int adu_len);

void    MBSerialInit(void);
int     MBSerialOpen(int port, int protocol);

int     MBSerialBlockRead(int port, u8 *buf, int len);          
int     MBSerialNonBlockRead(int port, u8 *buf, int len);
int     MBSerialWrite(int port, u8 *pdu, int len, u8 address);  
int     MBSerialClose(int port);
int     MBSerialFlowCtrl(int port, int control);
//int     MBSerialSendAndWaitResponse(int port, u8 *buf, u8 *pdu, int len, u8 address);
int	MBSerialSendAndWaitResponse(int port, u8 *buf, int expect_len, u8 *pdu, int len, u8 address);
 
u32     MBSerialGetFrameTimeout(int port);
void    MBSerialSetFrameTimeout(int port, u32 timeout);
u32     MBSerialGetCharTimeout(int port);
void    MBSerialSetCharTimeout(int port, u32 timeout);
u32     MBSerialGetTimeout(int port);
void    MBSerialSetTimeout(int port, u32 timeout);


int     MBSerialReadRTU( int port, u8 *buf, int len);
int     MBSerialReadASCII( int port, u8 *buf, int len);

u8      MBSerialGetAddress( const u8 *adu);
int     MBSerialGetPDU( u8 *buf, u8 *adu, int len);

int     MBSerialSetMode( int port, unsigned int mode);
int     MBSerialSetSpeed( int port, unsigned int speed);

#if MBF_READ_DECRETE_INPUTS_EN        != 0 
int     MBSerialReadDecreteInputs(int port,u8 address, u16 startdec, u16 no, u8 *value);
int     MBSerialSendReadDecreteInputs(int port, u8 address, u16 no, u8 *value);
#endif 

#if MBF_READ_COILS_EN                 != 0
int     MBSerialReadCoils(int port, u8 address, u16 startcoils, u16 no, u8 *value);
int     MBSerialSendReadCoils(int port, u8 address, u16 no, u8 *value);
#endif

#if MBF_WRITE_SINGLE_COIL_EN          != 0 
int     MBSerialWriteSingleCoil(int port, u8 address, u16 coilreg, u16 onoff);
int     MBSerialSendWriteSingleCoil(int port, u8 address, u16 coilreg, u16 onoff);
#endif 

#if MBF_WRITE_MULTIPLE_COILS_EN       != 0 
int     MBSerialWriteMultipleCoils(int port, u8 address, u16 startcoils, u16 no, u8 *value);
#endif

#if MBF_READ_INPUT_REGISTERS_EN       != 0 
int     MBSerialReadInputRegisters(int port, u8 address, u16 startreg, u16 no, u16 *value);
int     MBSerialSendReadInputRegisters(int port, u8 address, u16 no, u16 *value);
#endif 

#if MBF_READ_HOLDING_REGISTERS_EN     != 0 
int     MBSerialReadHoldingRegisters(int port, u8 address, u16 startreg, u16 no, u16 *value);
int     MBSerialSendReadHoldingRegisters(int port, u8 address, u16 no, u16 *value);
#endif

#if MBF_WRITE_SINGLE_REGISTER_EN      != 0
int     MBSerialWriteSingleRegister(int port, u8 address, u16 devicereg, u16 value);
#endif 


#if MBF_WRITE_MULTIPLE_REGISTERS_EN   != 0
int     MBSerialWriteMultipleRegisters(int port, u8 address, u16 startdevreg, u16 noreg, u8 count, u16 *value);
int     MBSerialSendWriteMultipleRegisters(int port, u8 address, u16 startdevreg, u16 noreg);
#endif 

#if MBF_READ_WRITE_MULTIPLE_REGISTERS_EN  != 0 
int     MBSerialReadWriteMultipleRegisters(int port, u8 address, u16 rsreg, u16 rno, u16 *rv, u16 wsreg, u16 wno, u8 count, u16 *wv);
#endif 

#if MBF_MASK_WRITE_REGISTER_EN        != 0 
int     MBSerialMaskWriteRegister(int port,u8 address, u16 reg, u16 andmask,u16 ormask);
#endif

#if MBF_READ_FIFO_QUEUE_EN            != 0 
int     MBSerialReadFIFOQueue(int port, u8 address, u16 FIFOAddr, u16 *FIFOCount, u16 *FIFOValue);
#endif

#if MBF_READ_EXCEPTION_STATUS_EN      != 0 
int     MBSerialReadExceptionStatus(int port, u8 address, u8 *status);
#endif

#if MBF_DIAGNOSTIC_EN                 != 0 
int     MBSerialDiagnostic(int port, u8 address, u16 subfunc, u16 data);
#endif

#if MBF_GET_COMM_EVENT_COUNTER_EN     != 0 
int     MBSerialGetCommEventCounter(int port, u8 address, u16 *status, u16 *eventcount);
#endif

#if MBF_GET_COMM_EVENT_LOG_EN         != 0 
int     MBSerialGetCommEventLog(int port, u8 address, u16 *status, u16 *eventcount, u16 *messagecount, u8 *events);
#endif

#if MBF_REPORT_SLAVE_ID_EN            != 0 
int     MBSerialReportSlaveID(int port, u8 address, u8 *slave_id, u8 *status);
#endif


int     MBSerialSendWriteSingleRegister(int port, u8 address, u16 addr, u16 value);

int     MBSerialSendReadFIFOueue(int port, u8 address, u16 no, u16 count, u16 *value);

int     MBSerialSendReadExecptionStatus(int port, u8 address, u8 status);

int     MBSerialSendDiagnostic(int port, u8 address, u16 subfunc, u16 data);

int     MBSerialSendGetCommEventCounter(int port, u8 address, u16 status, u16 eventcount);

int     MBSerialSendGetCommEventLog(int port, u8 address, u8 no, u16 status, u16 eventcount, u16 messagecount, u8 *events);

#endif
