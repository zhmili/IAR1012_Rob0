/*---------------------------------------------------------------------------*/
/**
  @file		modbus_utils.c
  @brief	Example modbus utilities define file

  This file contains much modbus utility functions.
  It helps to merge modbus function code and data to a PDU and split PDU
  to modbus function code and data.
  We split one request and response into four functions, for example if modbus
  master sends Read Coils request to slave and gets slave's response, we might
  have to use these four functions.

  Modbus Master					Modbus Slave
  1. MBReadCoils()		---->		2. MBGetReadCoils()
  4. MBGetResponseReadCoils()	<----		3. MBResponseReadCoils
  
  History:
  Date		Author			Comment
  08-01-2005	AceLan Kao.		Create it.

  @author AceLan Kao.(acelan_kao@moxa.com.tw)
 */
/*---------------------------------------------------------------------------*/

#include <string.h>
#include "includes.h"

static const unsigned char	CRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
};
static const unsigned char	CRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};
/*---------------------------------------------------------------------------*/
/**
  @brief	calculate the CRC16 value of the input data
  @param	buf		input data
  @param	len		length of the input data
  @return	CRC16 value of the input data
 */
/*---------------------------------------------------------------------------*/
unsigned short	CRC16( const u8 *buf, int len)
{
	unsigned char	hi=0xFF;
	unsigned char	lo=0xFF;
	unsigned short	index;

	if ( buf == 0 || len <= 0 )
		return(MB_ERROR_PARAMETER);

	while ( len-- ) {
		index = lo ^ (u8)*buf++;
		lo = hi ^ CRCHi[index];
		hi = CRCLo[index];
	}

	index = lo << 8;
	index |= hi;
	return(index);
}

/*---------------------------------------------------------------------------*/
/**
  @brief	calculate the checksum value of the input data
  @param	buf		input data
  @param	len		length of the input data
  @return	checksum value of the input data
 */
/*---------------------------------------------------------------------------*/
unsigned char	LRC( const u8 *buf, int len)
{
	unsigned char	ch=0;

	if ( len <= 0 || buf == 0 )
		return(MB_ERROR_PARAMETER);

	while ( len-- )
		ch += (unsigned char)*buf++;

	return(ch);
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse ASCII mode data to binary data
  @param	buf		output buffer
  @param	data		input ASCII data
  @param	len		length of the input data
  @return	the length of data after parse. On error return error code

  This function read the PDU part as input, parse it as Modbus ASCII protocol,
  and produce the binary data.
 */
/*---------------------------------------------------------------------------*/
int	MBASCIIToData( u8 *buf, const u8 *data, int len)
{
	int i= 0;
	u8 ch1, ch2;

	/* to do length error check */
	if ( len & 1 )
		return( MB_ERROR_LENGTH);
	if ( len <=1 || len >= MB_ASCII_ADU_MAX_LENGTH )
		return( MB_ERROR_LENGTH);
        
	for ( i=0; i<len; i+=2 )
	{
		ch2 = 0;
		ch1 = *data++;
		if ( ch1 >= '0' && ch1 <= '9' )
			ch1 -= '0';
		else if ( ch1 >= 'A' && ch1 <= 'F' )
			ch1 = ch1 - 'A' + 10;
		else
			return(MB_ERROR_FORMAT);
		ch2 = ch1 << 4;
		ch1 = *data++;
		if ( ch1 >= '0' && ch1 <= '9' )
			ch1 -= '0';
		else if ( ch1 >= 'A' && ch1 <= 'F' )
			ch1 = ch1 - 'A' + 10;
		else
			return(MB_ERROR_FORMAT);
		ch2 |= ch1;
		*buf++ = ch2;
	}
	return(len/2);
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse binary data to ASCII mode data
  @param	buf		output buffer
  @param	data		input binary data
  @param	len		length of the input data
  @return	the length of data after parse. On error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBDataToASCII( u8 *buf, const u8 *data, int len)
{
	
	int i= 0, j= 0;

	for( i= 0; i< len; i++)
	{
		u8 ch= ((data[ i] & 0xF0) >> 4);
		if( ch <= 9) // 去除警告 if( ch >= 0 && ch <= 9) ch 本来就 >= 0
			ch+= '0';
		else
			ch+= ('A'- 10);
		buf[ j++]= ch;

		ch= (data[ i] & 0x0F);
		if( ch <= 9) // 去除警告 if( ch >= 0 && ch <= 9) ch 本来就 >= 0
			ch+= '0';
		else
			ch+= ('A'-10);
		buf[ j++]= ch;
	}
	return j;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	assemble address, PDU and error checking code to ADU
  @param	buf		output buffer
  @param	protocol	which Modbus protocol to use
  @param	address		address of the destination device
  @param	pdu		PDU part
  @param	len		length of the PDU part
  @param	tid		transaction used for Modbus TCP protocol
  @return	the length of data after assemble. On error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBMakeADU( u8 *buf, int protocol, u8 address, const u8 *pdu, int len, u16 tid)
{
	u8 tmp[ MB_ASCII_ADU_MAX_LENGTH];
	vs16 lrc;
	vs32 crc;
	int  tmp_len= 0, buf_len= 0;

	switch( protocol)
	{
		case MB_ASCII_PROTOCOL:
			buf[ 0]= ':';

			tmp[ 0]= address;
			tmp_len++;

			bcopy( (const char *)pdu, (char *)(tmp+ tmp_len), len);
			tmp_len+= len;

			lrc= LRC( tmp, tmp_len);
			tmp[ tmp_len++]= lrc;

			buf_len= MBDataToASCII( buf+ 1, tmp, tmp_len);
			buf_len++;

			buf[ buf_len++]= 0xd;
			buf[ buf_len++]= 0xa;
			break;
		case MB_RTU_PROTOCOL:
			buf[ 0]= address;
			buf_len++;

			bcopy( (const char *)pdu, (char *)(buf+ 1), len);
			buf_len+= len;

			crc= CRC16( buf, buf_len);
			buf[ buf_len++]= crc >> 8;
			buf[ buf_len++]= crc & 0x00FF;
			break;
		case MB_TCP_PROTOCOL:
			buf[ 0]= tid >> 8;
			buf[ 1]= tid & 0x00FF;

			buf[ 2]= 0;
			buf[ 3]= 0;

			buf[ 4]= (len+1) >> 8;		// length of address + pdu
			buf[ 5]= (len+1) & 0x00FF;	// length of address + pdu

			buf[ 6]= address;

			bcopy( (const char *)pdu, (char *)(buf+ 7), len);
			buf_len= 7+ len;
			break;
		default:
			return MB_ERROR_PROTOCOL;
	}

	return buf_len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	get PDU part from ADU data
  @param	buf		output buffer
  @param	adu		ADU data with error check code
  @param	len		length of the adu data
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBGetPDU( u8 *buf, u8 *adu, int len)
{
	int minus= 2;
	if( adu[ 0] == ':')
		minus= 1;
	bcopy( (const char *)(adu+1), (char *)buf, len- 1- minus);

	return len- 1- minus;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	assemble function code and data to ADU
  @param	buf		output buffer
  @param	function	function code of the data
  @param	data		data
  @param	len		length of the data
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBMakePDU( u8 *buf, u8 function, u8 *data, int len)
{
	int ret_len= 0;

	buf[ 0]= function;
	bcopy( (const char *)data, (char *)(buf+ 1), len);
	ret_len= len+ 1;

	return ret_len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	get device address from ADU
  @param	adu		ADU data
  @return	none
 */
/*---------------------------------------------------------------------------*/
u8	MBGetAddress( const u8 *adu)
{
	return adu[ 0];
}

/*---------------------------------------------------------------------------*/
/**
  @brief	get function code from PDU
  @param	pdu		PDU data
  @return	none
 */
/*---------------------------------------------------------------------------*/
u8	MBGetFunctionCode( const u8 *pdu)
{
	return pdu[ 0];
}

// Master function
#if MBF_READ_DECRETE_INPUTS_EN != 0
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Read Decrete Inputs function(0x02)
 @param	buf		output buffer
  @param	startdec	starting decrete inputs register number
  @param	no		number of registers, between 1 ~ 2000
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBReadDecreteInputs(u8 *buf, u16 startdec, u16 no)
{
	int len= 0;
	buf[ len]= MBF_READ_DECRETE_INPUTS;
	len++;
	PUTU16( buf+ len, startdec);		// don't put ++ in the macro
	len+= 2;
	PUTU16( buf+ len, no);
	len+= 2;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	make response PDU packet of Read Decrete Inputs request(0x02)
  @param	buf		output buffer
  @param	no		number of registers
  @param	value		registers status
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBResponseReadDecreteInputs(u8 *buf, u8 no, u8 *value)
{
	int len= 0, i= 0;
	buf[ len]= MBF_READ_DECRETE_INPUTS;
	len++;
	buf[ len]= no;
	len++;
	if( no & 0x07)			// test if no could be divied by 8
		no= (no >> 3) + 1;
	else
		no>>= 3;

	for( i= 0; i< no/2; i++, len+= 2)
		PUTU16( buf+len, value[ i]);


	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse PDU packet of Read Decrete Inputs function(0x02)
  @param	pdu		the request PDU data
  @param	startdec	starting decrete inputs register number
  @param	no		number of registers, between 1 ~ 2000
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetReadDecreteInputs(u8 *pdu, u16 *startdec, u16 *no)
{
	if( pdu[ 0] != MBF_READ_DECRETE_INPUTS)
		return MB_ERROR_FUNCTION;

	GETU16( *startdec, pdu+ 1);
	GETU16( *no, pdu+ 3);

	return MB_OK;
}
/*---------------------------------------------------------------------------*/
/**
  @brief	parse response PDU packet of Read Decrete Inputs request(0x02)
  @param	pdu		the request PDU data
  @param	no		number of registers
  @param	value		registers status
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseReadDecreteInputs(u8 *pdu, u8 *no, u8 *value)
{
	int i= 0;
	if( pdu[ 0] != MBF_READ_DECRETE_INPUTS)
		return MB_ERROR_FUNCTION;

	*no= pdu[ 1];
	for( i= 0; i< *no; i++)
		value[ i]= pdu[ 2+ i];

	return MB_OK;
}


/*---------------------------------------------------------------------------*/
/**
 @brief    得到命令期待的返回帧长度
  @param    no  读取数据个数
  @return      期待的RTU返回帧长度
 */
/*---------------------------------------------------------------------------*/
int     MBSerialExpectLenReadDecreteInputs(INT16U no)
{
   return (5 +  (no + 7) / 8);   //地址(1) + 功能码(1) + 字节数(1) + 线圈数据((N + 7) / 8) + 校验(2)
}

#endif //#if MBF_READ_DECRETE_INPUTS_EN != 0


#if MBF_READ_SINGLE_COIL_EN          != 0
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Read Coils function(0x01)
  @param	buf		output buffer
  @param	startcoils	starting coil register number
  @param	no		number of registers, between 1 ~ 2000
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBReadCoils(u8 *buf, u16 startcoils, u16 no)
{
	int len= 0;
	buf[ len]= MBF_READ_COILS;
	len++;
	PUTU16( buf+ len, startcoils);		// don't put ++ in the macro
	len+= 2;
	PUTU16( buf+ len, no);
	len+= 2;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	make response PDU packet of Response Read Coils request(0x01)
  @param	buf		output buffer
  @param	no		number of registers
  @param	value		registers status
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBResponseReadCoils(u8 *buf, u8 no, u8 *value)
{
	int len= 0, i= 0;
	buf[ len]= MBF_READ_COILS;
	len++;
	buf[ len]= no;
	len++;
	if( no & 0x07)			// test if no could be divied by 8
		no= (no >> 3) + 1;
	else
		no>>= 3;

	for( i= 0; i< no/2; i++, len+= 2)
		PUTU16( buf+len, value[ i]);


	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse PDU packet of Read Coils function(0x01)
  @param	pdu		the request PDU data
  @param	startcoils	starting coil register number
  @param	no		number of registers, between 1 ~ 2000
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetReadCoils(u8 *pdu, u16 *startcoils, u16 *no)
{
	if( pdu[ 0] != MBF_READ_COILS)
		return MB_ERROR_FUNCTION;

	GETU16( *startcoils, pdu+ 1);
	GETU16( *no, pdu+ 3);

	return MB_OK;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse response PDU packet of Response Read Coils request(0x01)
  @param	pdu		output buffer
  @param	no		number of registers
  @param	value		registers status
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseReadCoils(u8 *pdu, u8 *no, u8 *value)
{
	int i= 0;
	if( pdu[ 0] != MBF_READ_COILS)
		return MB_ERROR_FUNCTION;

	*no= pdu[ 1];
	for( i= 0; i< *no; i++)
		value[ i]= pdu[ 2+ i];

	return MB_OK;
}


/*---------------------------------------------------------------------------*/
/**
 @brief    得到命令期待的返回帧长度
  @param    no  读取数据个数
  @return      期待的RTU返回帧长度
 */
/*---------------------------------------------------------------------------*/
int     MBSerialExpectLenReadCoils(INT16U no)
{
   return (5 +  (no + 7) / 8);   //地址(1) + 功能码(1) + 字节数(1) + 线圈数据((N + 7) / 8) + 校验(2)
}



#endif //#if MBF_READ_SINGLE_COIL_EN          != 0

#if MBF_WRITE_SIGNLE_COIL_EN  !=0
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Write Single Coil function(0x05)
  @param	buf		output buffer
  @param	coilreg		coil register number
  @param	onoff		0xFF00 for on,0x0000 for off
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBWriteSingleCoil(u8 *buf, u16 coilreg, u16 onoff)
{
	int len= 0;
	buf[ len]= MBF_WRITE_SINGLE_COIL;
	len++;
	PUTU16( buf+ len, coilreg);		// don't put ++ in the macro
	len+= 2;
	PUTU16( buf+ len, onoff);
	len+= 2;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	make response PDU packet of Write Single Coil request(0x05)
  @param	buf		output buffer
  @param	address		start address of register
  @param	value		output value
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBResponseWriteSingleCoil(u8 *buf, u16 address, u16 value)
{
	int len= 0;
	buf[ len]= MBF_WRITE_SINGLE_COIL;
	len++;
	PUTU16( buf+len, address);
	len+= 2;
	PUTU16( buf+len, value);
	len+= 2;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse PDU packet of Write Single Coil function(0x05)
  @param	pdu		the request PDU data
  @param	coilreg		coil register number
  @param	onoff		0xFF00 for on,0x0000 for off.
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetWriteSingleCoil(u8 *pdu, u16 *coilreg, u16 *onoff)
{
	if( pdu[ 0] != MBF_WRITE_SINGLE_COIL)
		return MB_ERROR_FUNCTION;

	GETU16( *coilreg, pdu+ 1);
	GETU16( *onoff, pdu+ 3);

	return MB_OK;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse response PDU packet of Write Single Coil request(0x05)
  @param	pdu		output buffer
  @param	address		start address of register
  @param	value		output value
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseWriteSingleCoil(u8 *pdu, u16 *address, u16 *value)
{
	if( pdu[ 0] != MBF_WRITE_SINGLE_COIL)
		return MB_ERROR_FUNCTION;

	GETU16( *address, pdu+ 1);
	GETU16( *value, pdu+ 3);

	return MB_OK;
}


/*---------------------------------------------------------------------------*/
/**
 @brief    得到命令期待的返回帧长度
  @return      期待的RTU返回帧长度
 */
/*---------------------------------------------------------------------------*/
int     MBSerialExpectLenWriteSingleCoil(void)
{
   return  8;     //地址(1) + 功能码(1) + 输出地址(2) + 输出值(2) + 校验(2)
}

#endif //#if MBF_WRITE_SIGNLE_COIL_EN  !=0


#if MBF_WRITE_MULTIPLE_COILS_EN       != 0
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Write Multiple Coils function(0x0F)
  @param	buf		output buffer
  @param	startcoils	starting coil register number
  @param	no		number of registers, between 1 ~ 2000
  @param	count		equal to no/8
  @param	data		output data
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBWriteMultipleCoils( u8 *buf, u16 startcoils, u16 no, u8 count, u8 *data)
{
	int len= 0;
	buf[ len]= MBF_WRITE_MULTIPLE_COILS;
	len++;
	PUTU16( buf+ len, startcoils);		// don't put ++ in the macro
	len+= 2;
	PUTU16( buf+ len, no);
	len+= 2;
	buf[ len]= count;
	len++;
	if( count & 0x07)			// test if no could be divied by 8
		count = ( count >> 3)+ 1;
	else
		count >>= 3;
	bcopy( (const char *)data, (char *)(buf+ len), count);
	len+= count;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse response PDU packet of Write Multiple Coil request(0x0F)
  @param	pdu		output buffer
  @param	address		start address of register
  @param	value		number of modified coils
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseWriteMultipleCoils(u8 *pdu, u16 *address, u16 *value)
{
	if( pdu[ 0] != MBF_WRITE_MULTIPLE_COILS)
		return MB_ERROR_FUNCTION;

	GETU16( *address, pdu+ 1);
	GETU16( *value, pdu+ 3);

	return MB_OK;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse PDU packet of Write Multiple Coils function(0x0F)
  @param	pdu		the request PDU data
  @param	startcoils	starting coil register number
  @param	no		number of registers, between 1 ~ 2000
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetWriteMultipleCoils(u8 *pdu, u16 *startcoils, u16 *no)
{
	if( pdu[ 0] != MBF_WRITE_MULTIPLE_COILS)
		return MB_ERROR_FUNCTION;

	GETU16( *startcoils, pdu+ 1);
	GETU16( *no, pdu+ 3);

	return MB_OK;
}


/*---------------------------------------------------------------------------*/
/**
 @brief    得到命令期待的返回帧长度
  @return      期待的RTU返回帧长度
 */
/*---------------------------------------------------------------------------*/
int     MBSerialExpectLenWriteMultipleCoils(void)
{
   return 8;     //地址(1) + 功能码(1) + 起始地址(2) + 输出数量(2) + 校验(2)
}


#endif //#if MBF_WRITE_MULTIPLE_COILS_EN       != 0

#if MBF_READ_INPUT_REGISTERS_EN       != 0 
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Read Input Registers function(0x04)
  @param	buf		output buffer
  @param	startreg	starting coil register number
  @param	no		number of registers, between 1 ~ 125
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBReadInputRegisters(u8 *buf, u16 startreg, u16 no)
{
	int len= 0;
	buf[ len]= MBF_READ_INPUT_REGISTERS;
	len++;
	PUTU16( buf+ len, startreg);		// don't put ++ in the macro
	len+= 2;
	PUTU16( buf+ len, no);
	len+= 2;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	make response PDU packet of Read Input Registers request(0x04)
  @param	buf		output buffer
  @param	no		number of registers
  @param	value		registers status
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBResponseReadInputRegisters(u8 *buf, u8 no, u16 *value)
{
	int len= 0, i= 0;
	buf[ len]= MBF_READ_INPUT_REGISTERS;
	len++;
	buf[ len]= no;
	len++;

	for( i= 0; i< no/2; i++, len+= 2)
		PUTU16( buf+len, value[ i]);

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse response PDU packet of Read Input Registers request(0x04)
  @param	pdu		output buffer
  @param	no		number of registers
  @param	value		registers status
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseReadInputRegisters(u8 *pdu, u8 *no, u16 *value)
{
	int i= 0;
	if( pdu[ 0] != MBF_READ_INPUT_REGISTERS)
		return MB_ERROR_FUNCTION;

	*no= pdu[ 1];
	for( i= 0; i< *no/2; i++)
		GETU16( value[ i], pdu+ 2+ i*2);

	return MB_OK;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse PDU packet of Read Input Registers function(0x04)
  @param	pdu		the request PDU data
  @param	startreg	starting coil register number
  @param	no		number of registers, between 1 ~ 125
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetReadInputRegisters(u8 *pdu, u16 *startreg, u16 *no)
{
	if( pdu[ 0] != MBF_READ_INPUT_REGISTERS)
		return MB_ERROR_FUNCTION;

	GETU16( *startreg, pdu+ 1);
	GETU16( *no, pdu+ 3);

	return MB_OK;
}



/*---------------------------------------------------------------------------*/
/**
 @brief    得到命令期待的返回帧长度
  @param    no  读取数据个数
  @return      期待的RTU返回帧长度
 */
/*---------------------------------------------------------------------------*/
int     MBSerialExpectLenReadInputRegisters(INT16U no)
{
   return (5 + no * 2);   //地址(1) + 功能码(1) + 字节数(1) + 数据(N * 2) + 校验(2)
}
#endif //#if MBF_READ_INPUT_REGISTERS_EN       != 0 

#if MBF_READ_HOLDING_REGISTERS_EN !=0
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Read Holding Registers function(0x03)
  @param	buf		output buffer
  @param	startreg	starting coil register number
  @param	no		number of registers, between 1 ~ 125
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBReadHoldingRegisters(u8 *buf, u16 startreg, u16 no)
{
	int len= 0;
	buf[ len]= MBF_READ_HOLDING_REGISTERS;
	len++;
	PUTU16( buf+ len, startreg);		// don't put ++ in the macro
	len+= 2;
	PUTU16( buf+ len, no);
	len+= 2;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	make response PDU packet of Read Holding Registers request(0x03)
  @param	buf		output buffer
  @param	no		number of registers
  @param	value		registers status
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBResponseReadHoldingRegisters(u8 *buf, u8 no, u16 *value)
{
	int len= 0, i= 0;
	buf[ len]= MBF_READ_HOLDING_REGISTERS;
	len++;
	buf[ len]= no;
	len++;

	for( i= 0; i< no/2; i++, len+= 2)
		PUTU16( buf+len, value[ i]);

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse PDU packet of Read Holding Registers function(0x03)
  @param	pdu		the request PDU data
  @param	startreg	starting coil register number
  @param	no		number of registers, between 1 ~ 125
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetReadHoldingRegisters(u8 *pdu, u16 *startreg, u16 *no)
{
	if( pdu[ 0] != MBF_READ_HOLDING_REGISTERS)
		return MB_ERROR_FUNCTION;

	GETU16( *startreg, pdu+ 1);
	GETU16( *no, pdu+ 3);

	return MB_OK;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse response PDU packet of Read Holding Registers request(0x03)
  @param	pdu		output buffer
  @param	no		number of registers
  @param	value		registers status
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseReadHoldingRegisters(u8 *pdu, u8 *no, u16 *value)
{
	int i= 0;
	if( pdu[ 0] != MBF_READ_HOLDING_REGISTERS)
		return MB_ERROR_FUNCTION;

	*no= pdu[ 1];
	for( i= 0; i< *no/2; i++)
		GETU16( value[ i], ( pdu+ 2+ i*2));

	return MB_OK;
}

/*---------------------------------------------------------------------------*/
/**
 @brief    得到命令期待的返回帧长度
  @param    no  读取数据个数
  @return      期待的RTU返回帧长度
 */
/*---------------------------------------------------------------------------*/
int     MBSerialExpectLenReadHoldingRegisters(INT16U no)
{
   return (5 + no * 2);   //地址(1) + 功能码(1) + 字节数(1) + 数据(N * 2) + 校验(2)
}

#endif //#if MBF_READ_HOLDING_REGISTERS_EN !=0

#if MBF_WRITE_SINGLE_REGISTERS_EN !=0
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Write Single Register function(0x06)
  @param	buf		output buffer
  @param	devicereg	register number
  @param	value		value to write
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBWriteSingleRegister(u8 *buf, u16 devicereg, u16 value)
{
	int len= 0;
	buf[ len]= MBF_WRITE_SINGLE_REGISTER;
	len++;
	PUTU16( buf+ len, devicereg);		// don't put ++ in the macro
	len+= 2;
	PUTU16( buf+ len, value);
	len+= 2;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse PDU packet of Write Single Register function(0x06)
  @param	pdu		the request PDU data
  @param	devicereg	register number
  @param	value		value to write
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetWriteSingleRegister(u8 *pdu, u16 *devicereg, u16 *value)
{
	if( pdu[ 0] != MBF_WRITE_SINGLE_REGISTER)
		return MB_ERROR_FUNCTION;

	GETU16( *devicereg, pdu+ 1);
	GETU16( *value, pdu+ 3);

	return MB_OK;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	make response PDU packet of Write Single Register request(0x06)
  @param	buf		output buffer
  @param	address		start address of register
  @param	value		output value
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBResponseWriteSingleRegister(u8 *buf, u16 address, u16 value)
{
	int len= 0;
	buf[ len]= MBF_WRITE_SINGLE_REGISTER;
	len++;
	PUTU16( buf+len, address);
	len+= 2;
	PUTU16( buf+len, value);
	len+= 2;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse response PDU packet of Write Single Register request(0x06)
  @param	pdu		output buffer
  @param	address		start address of register
  @param	value		output value
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseWriteSingleRegister(u8 *pdu, u16 *address, u16 *value)
{
	if( pdu[ 0] != MBF_WRITE_SINGLE_REGISTER)
		return MB_ERROR_FUNCTION;

	GETU16( *address, pdu+ 1);
	GETU16( *value, pdu+ 3);

	return MB_OK;
}



/*---------------------------------------------------------------------------*/
/**
 @brief    得到命令期待的返回帧长度
  @return      期待的RTU返回帧长度
 */
/*---------------------------------------------------------------------------*/
int     MBSerialExpectLenWriteSingleRegister(void)
{
   return  8;     //地址(1) + 功能码(1) + 寄存器地址(2) + 寄存器值(2) + 校验(2)
}
#endif //#if MBF_WRITE_SINGLE_REGISTERS_EN !=0

#if  MBF_WRITE_MULTIPLE_REGISTERS_EN !=0
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Write Multiple Registers function(0x10)
  @param	buf		output buffer
  @param	startreg	starting register number
  @param	noreg		number of registers, between 1 ~ 120
  @param	count		value counter
  @param	value		value to write
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBWriteMultipleRegisters(u8 *buf, u16 startreg, u16 noreg, u8 count, u16 *value)
{
	int len= 0, i= 0;
	buf[ len]= MBF_WRITE_MULTIPLE_REGISTERS;
	len++;
	PUTU16( buf+ len, startreg);		// don't put ++ in the macro
	len+= 2;
	PUTU16( buf+ len, noreg);
	len+= 2;
	buf[ len]= count;
	len++;

	for( i= 0; i< count/2; i++, len+= 2)
		PUTU16( buf+len, value[ i]);


	return len;
}
/*---------------------------------------------------------------------------*/
/**
  @brief	make response PDU packet of Write Multiple Registers request(0x10)
  @param	buf		output buffer
  @param	address		start address of register
  @param	no		number of registers(1 ~ 123)
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBResponseWriteMultipleRegisters(u8 *buf, u16 address, u16 no)
{
	int len= 0;
	buf[ len]= MBF_WRITE_MULTIPLE_REGISTERS;
	len++;
	PUTU16( buf+len, address);
	len+= 2;
	PUTU16( buf+len, no);
	len+= 2;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse response PDU packet of Write Multiple Registers request(0x10)
  @param	pdu		output buffer
  @param	address		start address of register
  @param	no		number of registers(1 ~ 123)
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseWriteMultipleRegisters(u8 *pdu, u16 *address, u16 *no)
{
	if( pdu[ 0] != MBF_WRITE_MULTIPLE_REGISTERS)
		return MB_ERROR_FUNCTION;

	GETU16( *address, pdu+ 1);
	GETU16( *no, pdu+ 3);

	return MB_OK;
}



/*---------------------------------------------------------------------------*/
/**
 @brief    得到命令期待的返回帧长度
  @return      期待的RTU返回帧长度
 */
/*---------------------------------------------------------------------------*/
int     MBSerialExpectLenWriteMultipleRegisters(void)
{
   return  8; //地址(1) + 功能码(1) + 起始地址(2) + 寄存器数量(2) + 校验(2)
}
#endif //#ifdef MBF_WRITE_MULTIPLE_REGISTERS_EN !=0

#if MBF_READ_WRITE_MULTIPLE_REGISTERS_EN  != 0 
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Read Write Multiple Registers function(0x17)
  @param	buf		output buffer
  @param	rsreg		starting register number to read
  @param	rno		number of registers ro read, between 1 ~ 118
  @param	wsreg		starting register number to write
  @param	wno		number of registers ro write, between 1 ~ 118
  @param	count		value counter
  @param	wv		value to write
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBReadWriteMultipleRegisters(u8 *buf, u16 rsreg, u16 rno, u16 wsreg, u16 wno, u8 count, u16 *wv)
{
	int len= 0, i= 0;
	buf[ len]= MBF_READ_WRITE_MULTIPLE_REGISTERS;
	len++;
	PUTU16( buf+ len, rsreg);		// don't put ++ in the macro
	len+= 2;
	PUTU16( buf+ len, rno);
	len+= 2;
	PUTU16( buf+ len, wsreg);
	len+= 2;
	PUTU16( buf+ len, wno);
	len+= 2;
	buf[ len++]= count;

	for( i= 0; i< wno/2; i++, len+= 2)
		PUTU16( buf+len, wv[ i]);

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse PDU packet of Read Write Multiple Registers function(0x17)
  @param	pdu		the request PDU data
  @param	rsreg		starting register number to read
  @param	rno		number of registers ro read, between 1 ~ 120
  @param	wsreg		starting register number to write
  @param	wno		number of registers ro write, between 1 ~ 120
  @param	count		value count
  @param	wv		value to write
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetReadWriteMultipleRegisters(u8 *pdu, u16 *rsreg, u16 *rno, u16 *wsreg, u16 *wno, u8 *count, u16 *wv)
{
	int i= 0;
	if( pdu[ 0] != MBF_READ_WRITE_MULTIPLE_REGISTERS)
		return MB_ERROR_FUNCTION;

	GETU16( *rsreg, pdu+ 1);
	GETU16( *rno, pdu+ 3);
	GETU16( *wsreg, pdu+ 5);
	GETU16( *wno, pdu+ 7);
	*count= pdu[ 9];
	for( i= 0; i< (*count)/2; i++)
		GETU16( wv[ i], pdu+ 10+ i*2 );

	return MB_OK;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse response PDU packet of Response Read Coils request(0x17)
  @param	pdu		output buffer
  @param	no		number of registers
  @param	value		registers status
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseReadWriteMultipleRegisters(u8 *pdu, u8 *no, u16 *value)
{
	int i= 0;
	if( pdu[ 0] != MBF_READ_WRITE_MULTIPLE_REGISTERS)
		return MB_ERROR_FUNCTION;

	*no= pdu[ 1];
	for( i= 0; i< *no/2; i++)
		GETU16( value[ i], pdu+ 2+ i*2);

	return MB_OK;
}



#endif //#if MBF_READ_WRITE_MULTIPLE_REGISTERS_EN  != 0 

#if MBF_MASK_WRITE_REGISTER_EN        != 0
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Mask Write Register function(0x16)
  @param	buf		output buffer
  @param	reg		register number
  @param	andmask		and mask
  @param	ormask		or mask
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBMaskWriteRegister(u8 *buf, u16 reg, u16 andmask, u16 ormask)
{
	int len= 0;
	buf[ len]= MBF_MASK_WRITE_REGISTER;
	len++;
	PUTU16( buf+ len, reg);			// don't put ++ in the macro
	len+= 2;
	PUTU16( buf+ len, andmask);
	len+= 2;
	PUTU16( buf+ len, ormask);
	len+= 2;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse PDU packet of Mask Write Register function(0x16)
  @param	pdu		the request PDU data
  @param	reg		register number
  @param	andmask		and mask
  @param	ormask		or mask
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetMaskWriteRegister(u8 *pdu, u16 *reg, u16 *andmask, u16 *ormask)
{
	if( pdu[ 0] != MBF_MASK_WRITE_REGISTER)
		return MB_ERROR_FUNCTION;

	GETU16( *reg, pdu+ 1);
	GETU16( *andmask, pdu+ 3);
	GETU16( *ormask, pdu+ 5);

	return MB_OK;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse response PDU packet of Mask Write Register function(0x16)
  @param	pdu		output buffer
  @param	reg		register number
  @param	andmask		and mask
  @param	ormask		or mask
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseMaskWriteRegister(u8 *pdu, u16 *reg, u16 *andmask, u16 *ormask)
{
	if( pdu[ 0] != MBF_MASK_WRITE_REGISTER)
		return MB_ERROR_FUNCTION;

	GETU16( *reg, pdu+ 1);
	GETU16( *andmask, pdu+ 3);
	GETU16( *ormask, pdu+ 5);

	return MB_OK;
}


/*---------------------------------------------------------------------------*/
/**
 @brief    得到命令期待的返回帧长度
  @return      期待的RTU返回帧长度
 */
/*---------------------------------------------------------------------------*/
int     MBSerialExpectLenMaskWriteRegister(void)
{
   return 10;     //地址(1) + 功能码(1) + 参考地址(2) + And_Mask(2) + Or_Mask(2) + 校验(2)
}

#endif //#if MBF_MASK_WRITE_REGISTER_EN        != 0


#if MBF_READ_FIFO_QUEUE_EN            != 0 
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Read FIFO Queue function(0x18)
  @param	buf		output buffer
  @param	FIFOAddr	FIFO address
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBReadFIFOQueue(u8 *buf, u16 FIFOAddr)
{
	int len= 0;
	buf[ len]= MBF_READ_FIFO_QUEUE;
	len++;
	PUTU16( buf+ len, FIFOAddr);		// don't put ++ in the macro
	len+= 2;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	make response PDU packet of Read FIFO Queue request(0x18)
  @param	buf		output buffer
  @param	no		bytes of the following data
  @param	count		FIFO value buffer size(<=31)
  @param	value		values of FIFO register
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBResponseReadFIFOQueue(u8 *buf, u16 no, u16 count, u16 *value)
{
	int len= 0, i= 0;
	buf[ len]= MBF_READ_FIFO_QUEUE;
	len++;
	PUTU16( buf+len, no);
	len+= 2;
	PUTU16( buf+len, count);
	len+= 2;

	for( i= 0; i< (no- 2)/2; i++, len+= 2)
		PUTU16( buf+len, value[ i]);

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse PDU packet of Read FIFO Queue function(0x18)
  @param	pdu		the request PDU data
  @param	FIFOAddr	FIFO address
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetReadFIFOQueue(u8 *pdu, u16 *FIFOAddr)
{
	if( pdu[ 0] != MBF_READ_FIFO_QUEUE)
		return MB_ERROR_FUNCTION;

	GETU16( *FIFOAddr, pdu+ 1);

	return MB_OK;
}

#endif //#if MBF_READ_FIFO_QUEUE_EN            != 0 
// MBF_READ_FILE_RECORD			0x14
// MBF_WRITE_FILE_RECORD		0x15

#if MBF_READ_EXCEPTION_STATUS_EN      != 0
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Read Exception Status function(0x07)
  @param	buf		output buffer
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBReadExceptionStatus(u8 *buf)
{
	int len= 0;
	buf[ len]= MBF_READ_EXCEPTION_STATUS;
	len++;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	make response PDU packet of Read FIFO Queue request(0x07)
  @param	buf		output buffer
  @param	status		eight exception status in one byte
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBResponseReadExceptionStatus(u8 *buf, u8 status)
{
	int len= 0;
	buf[ len]= MBF_READ_EXCEPTION_STATUS;
	len++;
	buf[ len]= status;
	len++;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse response PDU packet of Read FIFO Queue request(0x07)
  @param	pdu		output buffer
  @param	status		eight exception status in one byte
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseReadExceptionStatus(u8 *pdu, u8 *status)
{
	if( pdu[ 0] != MBF_READ_EXCEPTION_STATUS)
		return MB_ERROR_FUNCTION;

	*status= pdu[ 1];

	return MB_OK;
}

#endif //#if MBF_READ_EXCEPTION_STATUS_EN      != 0


#if MBF_DIAGNOSTIC_EN                 != 0 
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Diagnostic function(0x08)
  @param	buf		output buffer
  @param	subfunc		subfunction code
  @param	data		data to be sent to slave device
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBDiagnostic(u8 *buf, u16 subfunc, u16 data)
{
	int len= 0;
	buf[ len]= MBF_DIAGNOSTIC;
	len++;
	PUTU16( buf+ len, subfunc);		// don't put ++ in the macro
	len+= 2;
	PUTU16( buf+ len, data);		// all data of subfunc is 16bits long
	len+= 2;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	make response PDU packet of Diagnostic request(0x08)
  @param	buf		output buffer
  @param	subfunc		subfunction code
  @param	data		response data of request
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBResponseDiagnostic(u8 *buf, u16 subfunc, u16 data)
{
	int len= 0;
	buf[ len]= MBF_DIAGNOSTIC;
	len++;
	PUTU16( buf+len, subfunc);
	len+= 2;
	PUTU16( buf+len, data);
	len+= 2;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse response PDU packet of Diagnostic request(0x08)
  @param	pdu		output buffer
  @param	subfunc		subfunction code
  @param	data		response data of request
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseDiagnostic(u8 *pdu, u16 *subfunc, u16 *data)
{
	if( pdu[ 0] != MBF_DIAGNOSTIC)
		return MB_ERROR_FUNCTION;

	GETU16( *subfunc, pdu+ 1);
	GETU16( *data, pdu+ 3);

	return MB_OK;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse PDU packet of Diagnostic function(0x08)
  @param	pdu		the request PDU data
  @param	subfunc		subfunction code
  @param	data		data to be sent to slave device
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetDiagnostic(u8 *pdu, u16 *subfunc, u16 *data)
{
	if( pdu[ 0] != MBF_DIAGNOSTIC)
		return MB_ERROR_FUNCTION;

	GETU16( *subfunc, pdu+ 1);
	GETU16( *data, pdu+ 3);

	return MB_OK;
}

#endif //#if MBF_DIAGNOSTIC_EN                 != 0 


#if MBF_GET_COMM_EVENT_COUNTER_EN     != 0
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Get Comm Event Counter function(0x0B)
  @param	buf		output buffer
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBGetCommEventCounter(u8 *buf)
{
	int len= 0;
	buf[ len]= MBF_GET_COMM_EVENT_COUNTER;
	len++;

	return len;
}
/*---------------------------------------------------------------------------*/
/**
  @brief	make response PDU packet of Get Comm Event Counter request(0x0B)
  @param	buf		output buffer
  @param	status		status of Comm Conuter
  @param	eventcount	event counter
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBResponseGetCommEventCounter(u8 *buf, u16 status, u16 eventcount)
{
	int len= 0;
	buf[ len]= MBF_GET_COMM_EVENT_COUNTER;
	len++;
	PUTU16( buf+len, status);
	len+= 2;
	PUTU16( buf+len, eventcount);
	len+= 2;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse response PDU packet of Get Comm Event Counter request(0x0B)
  @param	pdu		output buffer
  @param	status		status of Comm Conuter
  @param	eventcount	event counter
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseGetCommEventCounter(u8 *pdu, u16 *status, u16 *eventcount)
{
	if( pdu[ 0] != MBF_GET_COMM_EVENT_COUNTER)
		return MB_ERROR_FUNCTION;

	GETU16( *status, pdu+ 1);
	GETU16( *eventcount, pdu+ 3);

	return MB_OK;
}


#endif //#if MBF_GET_COMM_EVENT_COUNTER_EN     != 0

#if MBF_GET_COMM_EVENT_LOG_EN         != 0 
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Get Comm Event Log function(0x0C)
  @param	buf		output buffer
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBGetCommEventLog(u8 *buf)
{
	int len= 0;
	buf[ len]= MBF_GET_COMM_EVENT_LOG;
	len++;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	make response PDU packet of Get Comm Event Counter request(0x0C)
  @param	buf		output buffer
  @param	no		number of the following data
  @param	status		status of Comm Conuter
  @param	eventcount	event counter
  @param	messagecount	counter for processed message
  @param	events		new events
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBResponseGetCommEventLog(u8 *buf, u8 no, u16 status, u16 eventcount, u16 messagecount, u8 *events)
{
	int len= 0;
	buf[ len]= MBF_GET_COMM_EVENT_LOG;
	len++;
	buf[ len]= no;
	len++;
	PUTU16( buf+len, status);
	len+= 2;
	PUTU16( buf+len, eventcount);
	len+= 2;
	PUTU16( buf+len, messagecount);
	len+= 2;

	bcopy( (const char *)events, (char *)(buf+ len), no- 6);

	return len;
}

/*---------------------------------------------------------------------------*/
/**
  @brief	parse response PDU packet of Get Comm Event Counter request(0x0C)
  @param	pdu		output buffer
  @param	status		status of Comm Conuter
  @param	eventcount	event counter
  @param	messagecount	counter for processed message
  @param	events		new events
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseGetCommEventLog(u8 *pdu, u16 *status, u16 *eventcount, u16 *messagecount, u8 *events)
{
	u8 no;
	if( pdu[ 0] != MBF_GET_COMM_EVENT_LOG)
		return MB_ERROR_FUNCTION;

	no= pdu[ 1];
	GETU16( *status, pdu+ 2);
	GETU16( *eventcount, pdu+ 4);
	GETU16( *messagecount, pdu+ 6);

	bcopy( (const char *)(pdu+ 8), (char *)events, no- 6);

	return MB_OK;
}

#endif //#if MBF_GET_COMM_EVENT_LOG_EN         != 0 


#if  MBF_REPORT_SLAVE_ID_EN            != 0
/*---------------------------------------------------------------------------*/
/**
  @brief	make PDU packet of Report Slave ID function(0x11)
  @param	buf		output buffer
  @return	the length of PDU data
 */
/*---------------------------------------------------------------------------*/
int	MBReportSlaveID(u8 *buf)
{
	int len= 0;
	buf[ len]= MBF_REPORT_SLAVE_ID;
	len++;

	return len;
}

/*---------------------------------------------------------------------------*/
/**
 @brief	parse response PDU packet of Report Slave ID function(0x16)
  @param	pdu		output buffer
  @param	slave_id	slave id
  @param	status		slave device status
  @return	MB_OK for success, on error return error code
 */
/*---------------------------------------------------------------------------*/
int	MBGetResponseReportSlaveID(u8 *pdu, u8 *slave_id, u8 *status)
{
	if( pdu[ 0] != MBF_REPORT_SLAVE_ID)
		return MB_ERROR_FUNCTION;

	*slave_id= pdu[ 2];
	*status= pdu[ 3];

	return MB_OK;
}








#endif //#if MBF_REPORT_SLAVE_ID_EN            ! = 0
