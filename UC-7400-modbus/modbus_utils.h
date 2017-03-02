/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

#ifndef MODBUS_UTILS_H
#define MODBUS_UTILS_H
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
#define GETU16( var, buf) { (var) = (*(buf) << 8) & 0xFF00; (var) |= (*((buf)+1) & 0xFF); }
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
#define PUTU16( buf, var) { (*(buf)) = ((var) >> 8);  *((buf)+1) = ((var) & 0xFF); }

#define bcopy(from, to, count)  memcpy(to, from, count)


unsigned short     CRC16( const INT8U *buf, int len);
unsigned char      LRC( const INT8U *buf, int len);
int     MBASCIIToData(INT8U *buf, const INT8U *data, int len);
int     MBDataToASCII(INT8U *buf, const INT8U *data, int len);

int     MBMakeADU( INT8U *buf, int protocol, INT8U address, const INT8U *pdu, int len, INT16U tid);
int     MBGetPDU( INT8U *buf, INT8U *adu, int len);
int     MBMakePDU( INT8U *buf, INT8U function, INT8U *data, int len);
INT8U      MBGetAddress( const INT8U *adu);
INT8U      MBGetFunctionCode( const INT8U *pdu);

void    MBDataToBigEndian(INT8U *pdst, INT16U *psrc, INT16U no);

int     MBSerialExpectLenReadDecreteInputs(INT16U no);
//void    MBGetAddressRange(INT8U *pdu, INT16U *addr, INT16U *range);
//void    MBGetAddressRangeSize(INT8U *pdu, INT16U *addr, INT16U *range, INT8U *size);
//#define  MBGetAddressValue(pdu, addr, value) MBGetAddressRange(pdu, addr, value)


//int     MBSerialExpectLenReadFIFOQueue(INT16U no, INT16U count);
//int     MBSerialExpectLenReadExceptionStatus(INT8U status);
//int     MBSerialExpectLenDiagnostic(INT16U subfunc, INT16U data);
//int     MBSerialExpectLenGetCommEventCounter(INT16U status, INT16U eventcount);
//int     MBSerialExpectLenGetCommEventLog(INT8U no, INT16U status, INT16U eventcount, INT16U messagecount, INT8U *events);
//int     MBSerialRTULenToASCII(INT16U len);

#if MBF_READ_DECRETE_INPUTS_EN        != 0 
int     MBReadDecreteInputs(INT8U *buf, INT16U startdec, INT16U no);
int     MBResponseReadDecreteInputs(INT8U *buf, INT8U no, INT8U *value);
int     MBGetResponseReadDecreteInputs(INT8U *pdu, INT8U *no, INT8U *value);
int     MBSerialExpectLenReadDecreteInputs(INT16U no);
#endif

#if MBF_READ_COILS_EN                 != 0 
int     MBReadCoils(INT8U *buf, INT16U startcoils, INT16U no);
int     MBResponseReadCoils(INT8U *buf, INT8U no, INT8U *value);
int     MBGetResponseReadCoils(INT8U *pdu, INT8U *no, INT8U *value);
int     MBSerialExpectLenReadCoils(INT16U no);
//int     MBSerialExpectLenReadCoils(INT16U no);
#endif

#if MBF_READ_HOLDING_REGISTERS_EN     != 0 
int     MBReadHoldingRegisters(INT8U *buf, INT16U startreg, INT16U no);
int     MBResponseReadHoldingRegisters(INT8U *buf, INT8U no, INT16U *value);
int     MBGetResponseReadHoldingRegisters(INT8U *pdu, INT8U *no, INT16U *value);
int     MBSerialExpectLenReadHoldingRegisters(INT16U no);
#endif

#if MBF_READ_INPUT_REGISTERS_EN       != 0 
int     MBReadInputRegisters(INT8U *buf, INT16U startreg, INT16U no);
int     MBResponseReadInputRegisters(INT8U *buf, INT8U no, INT16U *value);
int     MBGetResponseReadInputRegisters(INT8U *pdu, INT8U *no, INT16U *value);
int     MBSerialExpectLenReadInputRegisters(INT16U no);
#endif

#if MBF_WRITE_SINGLE_COIL_EN          != 0 
int     MBWriteSingleCoil(INT8U *buf, INT16U coilreg, INT16U onoff);
int     MBResponseWriteSingleCoil(INT8U *buf, INT16U coilreg, INT16U onoff);
int     MBGetResponseWriteSingleCoil(INT8U *pdu, INT16U *address, INT16U *value);
int     MBSerialExpectLenWriteSingleCoil(void);
#endif

#if MBF_WRITE_SINGLE_REGISTER_EN      != 0 
int	MBWriteSingleRegister(u8 *buf, u16 devicereg, u16 value);
int	MBGetWriteSingleRegister(u8 *pdu, u16 *devicereg, u16 *value);
int     MBResponseWriteSingleRegister(INT8U *buf, INT16U reg, INT16U value);
int     MBGetResponseWriteSingleRegister(INT8U *pdu, INT16U *address, INT16U *value);
//int     MBSerialExpectLenWriteSingleRegister(void);
#endif

#if MBF_READ_EXCEPTION_STATUS_EN      != 0 
int     MBReadExceptionStatus(INT8U *buf);
int     MBResponseReadExceptionStatus(INT8U *buf, INT8U status);
int     MBGetResponseReadExceptionStatus(INT8U *buf, INT8U *status);
#endif

#if MBF_DIAGNOSTIC_EN                 != 0 
int     MBDiagnostic(INT8U *buf, INT16U subfunc, INT16U data);
int     MBGetDiagnostic(INT8U *pdu, INT16U *subfunc, INT16U *data);      // just for serial port
int     MBResponseDiagnostic(INT8U *buf, INT16U subfunc, INT16U data);
int     MBGetResponseDiagnostic(INT8U *pdu, INT16U *subfunc, INT16U *data);
#endif

#if MBF_GET_COMM_EVENT_COUNTER_EN     != 0 
int     MBGetCommEventCounter(INT8U *buf);
int     MBResponseGetCommEventCounter(INT8U *buf, INT16U status, INT16U eventcount);
int     MBGetResponseGetCommEventCounter(INT8U *pdu, INT16U *status, INT16U *eventcount);
#endif

#if MBF_GET_COMM_EVENT_LOG_EN         != 0 
int     MBGetCommEventLog(INT8U *buf);
int     MBResponseGetCommEventLog(INT8U *buf, INT8U no, INT16U status, INT16U eventcount, INT16U messagecount, INT8U *events);
int     MBGetResponseGetCommEventLog(INT8U *buf, INT16U *status, INT16U *eventcount, INT16U *messagecount, INT8U *events);
#endif

#if MBF_WRITE_MULTIPLE_COILS_EN       != 0 
int     MBWriteMultipleCoils(INT8U *buf, INT16U startcoils, INT16U no, INT8U count, INT8U *data);
int     MBGetResponseWriteMultipleCoils(INT8U *pdu, INT16U *address, INT16U *value);
int	MBGetWriteMultipleCoils(u8 *pdu, u16 *startcoils, u16 *no)
int     MBSerialExpectLenWriteMultipleCoils(void);
#endif

#if MBF_WRITE_MULTIPLE_REGISTERS_EN   != 0 
int     MBWriteMultipleRegisters(INT8U *buf, INT16U startreg, INT16U noreg, INT8U count, INT16U *no);
int     MBGetResponseWriteMultipleRegisters(INT8U *pdu, INT16U *address, INT16U *no);
int     MBResponseWriteMultipleRegisters(INT8U *buf, INT16U startdevreg, INT16U no);
int     MBSerialExpectLenWriteMultipleRegisters(void);
#endif

#if MBF_REPORT_SLAVE_ID_EN            != 0 
int     MBReportSlaveID(INT8U *buf);
int     MBResponseReportSlaveID(INT8U *pdu, INT8U *buf, int len);
int     MBGetResponseReportSlaveID(INT8U *pdu, INT8U *slave_id, INT8U *status);
#endif

#if MBF_READ_FILE_RECORD_EN           != 0 
#endif

#if MBF_WRITE_FILE_RECORD_EN          != 0 
#endif

#if MBF_MASK_WRITE_REGISTER_EN        != 0 
int     MBMaskWriteRegister(INT8U *buf, INT16U reg, INT16U andmask,INT16U ormask);
int     MBGetMaskWriteRegister(INT8U *pdu, INT16U *reg, INT16U *andmask, INT16U *ormask);
int     MBGetResponseMaskWriteRegister(INT8U *pdu, INT16U *reg, INT16U *andmask, INT16U *ormask);
int     MBSerialExpectLenMaskWriteRegister(void);
#endif

#if MBF_READ_WRITE_MULTIPLE_REGISTERS_EN  != 0 
int     MBReadWriteMultipleRegisters(INT8U *buf, INT16U rsreg, INT16U rno, INT16U wsreg, INT16U wno, INT8U count, INT16U *wv);
int     MBGetReadWriteMultipleRegisters(INT8U *pdu, INT16U *rsreg, INT16U *rno, INT16U *wsreg, INT16U *wno, INT8U *count, INT16U *wv);
int     MBGetResponseReadWriteMultipleRegisters(INT8U *pdu, INT8U *no, INT16U *value);
#endif

#if MBF_READ_FIFO_QUEUE_EN            != 0 
int     MBReadFIFOQueue(INT8U *buf, INT16U FIFOAddr);
int     MBGetReadFIFOQueue(INT8U *pdu, INT16U *FIFOAddr);
int     MBResponseReadFIFOQueue(INT8U *buf, INT16U no, INT16U count, INT16U *value);
int     MBGetResponseReadFIFOQueue(INT8U *buf, INT16U *count, INT16U *value);
#endif

#if MBF_REPORT_SLAVE_ID_EN            != 0
int	MBReportSlaveID(u8 *buf);
int	MBGetResponseReportSlaveID(u8 *pdu, u8 *slave_id, u8 *status);
#endif

// MBF_READ_FILE_RECORD                 0x14
// MBF_WRITE_FILE_RECORD                0x15

#endif
