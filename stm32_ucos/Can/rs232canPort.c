#include "includes.h"
#include "rs232canPort.h"
/*
s16 UART0Getch(	)  //use to test in the VC 
{
static s16 i = 0;
s16 out;

//u8 in[ ] = {0x10,'1','2',0x68,5,4,0x67,0x68,8,9,0x68,6,6,0x68,1,2,4,4,4,4,18,0x16};

//u8 in[ ] = {0x10,'1','2',0x68,5,4,0x67,0x68,8,7,0x68,8,8,0x68,1,2,128,4,4,4,4,4,150,0x16};

//u8 in[ ] = {0x10,'1','2',0x68,5,4,0x67,0x68,8,7,0x68,8,8,0x68,1,2,0,4,4,4,4,4,22,0x16};

u8 in[ ] = {0x10,'1','2',0x68,5,4,0x67,0x68,8,7,0x68,8,8,0x68,1,2,0x40,4,4,4,4,4,86,0x16 ,0x10,'1','2',0x68,5,4,0x67,0x68,8,7,0x68,10,10,0x68,1,2,159,129,136,4,4,4,4,4,190,0x16 };

out = in[i];

if( i > 10000 )
{
	return -1;

}else{

	i++;
	return out;

}

}
*/

/*
*********************************************************************************************************
*                                             INITIALIZATION
*
* Description: Get a Fram form the buffer
*
* Arguments  : none
*
* Returns    : if the fram exist return the fram's point , else return (void *)0
*********************************************************************************************************
*/
void * GetAFram(void)
{
	u8 b_LE;
	u8 b_cfg;
	s8  b_ce;
	u8 * pdata;
	
 b_LE = CheckHeadAndGetLE( );                     
 if( b_LE < 0)
	return (void *)0;

    pdata = (u8 *) malloc( b_LE ); 


  b_cfg = CheckFCSAndGetdata( pdata, b_LE );



 if( b_cfg < 0 )
	 return (void *)0;

 b_ce = CheckED( );
 if( b_ce < 0)
	 return (void *)0;

 return pdata;

}

/*
*********************************************************************************************************
*                                             INITIALIZATION
*
* Description: check head and get lenth
*
* Arguments  : none
*
* Returns    : if check head not correct it will return -1, else return the lenth
*********************************************************************************************************
*/
s16 CheckHeadAndGetLE( void )
{
	s16 SD;
	
	s16 LE;
	
	s16 LEr;

	s16 eSD;

//	do{
	do{
		do{
			do{			
				SD = UART0Getch( );
				if(SD < 0)
				return -1;

			   }while(SD != 0x68);

				LE  = UART0Getch( );
				if(LE < 0)
				return -1;
				LEr = UART0Getch( );
				if(LEr < 0)
				return -1;

			}while( LE != LEr);

				eSD = UART0Getch( );
				if(eSD < 0)
				return -1;
	   }while( eSD != 0x68);
//	   }while(SD != 0x68 || eSD != 0x68 || LE != LEr)

	return LE;	
}

/*
*********************************************************************************************************
*                                             INITIALIZATION
*
* Description: check the FSC and get the datas
*
* Arguments  : 
*             pfram : the fram's point which is get from the CheckHeadAndGetLE
*             lenth : the data lenth 
*           
* Returns    : if check FCS not correct it will return -1, else return the TRUE
*********************************************************************************************************
*/
s8 CheckFCSAndGetdata( u8 *pfram, u8 lenth )
{	
	s32 check;
	u8 TS;
	u8 FN;
	u8 DA;
	u8 FCS;
	s32 gFCS;
	u8 datalen;	
	int i;
	datalen = lenth ;
	check = 0;
        
        TS =(u8) UART0Getch( );	
	if( TS < 0 )
	   return -1;
	*pfram = TS;
	pfram++    ;
	FN = UART0Getch( );
	if( FN < 0)
		return -1;
	*pfram = FN ;
	pfram++	;
	check  = check + FN;

	for( i = 0; i < datalen; i++)
	{
		DA = UART0Getch( );
		if( DA < 0 )
			return -1;
		*pfram = DA ;
		check  = check + DA;
		pfram++ ;
	}
    
	gFCS = check % 256;

	FCS = UART0Getch( );
	if( FCS < 0)
		return -1;
	if( gFCS != FCS )
		return -1;	
	else 
		return TRUE;
}

/*
*********************************************************************************************************
*                                             INITIALIZATION
*
* Description: check the ED
*
* Arguments  : none
*           
* Returns    : if check ED not correct it will return -1, else return the TRUE
*********************************************************************************************************
*/
s8 CheckED( void )
{
	s16 ED;
	ED = UART0Getch( );
	if( ED < 0 )
	  return -1;
	if( ED != 0x16)
      return -1;
	else 
	  return TRUE;	
}

/*
*********************************************************************************************************
*                                             INITIALIZATION
*
* Description: parse a fram to the struct
*
* Arguments  : 
*              pfram    : the point of fram which get from the buf
*              stcFramT : the struct which to be store
*           
* Returns    : if check ED not correct it will return -1, else return the TRUE
*********************************************************************************************************
*/
s16 ParseToStru( u8 * pfram, CO_MSG stcCO_MSGT )
{
	u8   i;
	u8   FN;
	u32  movemid;
	s8   command;
	s16	 lenth;
	s16  actlenth;
	lenth = *pfram ;
	pfram++ ;

	stcFramT.headSD  = 0x68   ;
	stcFramT.lenthLE = lenth  ;
	stcFramT.rlenthLER = lenth;
	stcFramT.framED    = 0x68 ;
		
	stcFramT.timeTS    = *pfram ;
	pfram++ ;

	stcFramT.functionFN = *pfram ;
	FN = *pfram ;
	pfram++ ;

	actlenth = lenth - 2	  ;

	switch( FN )
	{
		case CCM_FILTER_RESET :
	      //        command = CcmFilterReset( );
			return command ;

		case CCM_FILTER_ALL :	
		//	command = CcmFilterAll( );
			return command ;
			
		case CCM_FILTER_ADD :
			break;

		case CCM_FILTER_REMOVE :
			break;

		case   CCM_SEND_REPEAT :
			stcFramT.OTHER_U.rtime = ( *pfram << 8 );							//高8 位
			pfram++;
			stcFramT.OTHER_U.rtime = stcFramT.OTHER_U.rtime + *pfram;			//低8 位
			pfram++ ;
			actlenth = actlenth - 2;
			break;

		case CCM_SEND_SINGLE :	

			stcFramT.OTHER_U.otime =  ( *pfram << 8 );	
			pfram++ ;
			stcFramT.OTHER_U.otime = stcFramT.OTHER_U.otime + *pfram ;
			pfram++ ;
			actlenth = actlenth - 2;
			break;

		case CCM_BAUD_SET	:
			stcFramT.OTHER_U.baud = *pfram;
			pfram++ ;
			actlenth = actlenth - 1;
			break;

		case   CCM_RESET :	
		//	command = CcmReset( );
			return command ;

		case CCM_RECV_FRAME :
			
			break;

		case CCM_RECV_ERR	:	
			stcFramT.OTHER_U.erract = *pfram;
			pfram++ ;
			stcFramT.errcode = *pfram;
			pfram++ ;
			return TRUE;

		default : 
			return -1 ;
	
	}
			if( *fram & 0x0080)   //注意实际有用数值是U8
			stcFramT.CanFramT.EXT = 1;
		else 
			stcFramT.CanFramT.EXT = 0;

			stcFramT.CanFramT.RTR =  ( ( (  *fram << 9 ) >> 15)  & 0x0001);
			stcFramT.CanFramT.COBID = ( ( *fram << 11 )<< 24 )	;							// get 5 bits to COBID
			fram++;
			movemid = *fram ;
			movemid = movemid << 16 ;																// get 8 bits to COBID 
			stcFramT.CanFramT.COBID = stcFramT.CanFramT.COBID + movemid ;			// 
			
			fram++ ;
			movemid = *fram++ ;
			movemid = movemid << 8 ;													
			stcFramT.CanFramT.COBID = stcFramT.CanFramT.COBID + movemid ;			// get 8 bits to COBID
			fram++ ;
			stcFramT.CanFramT.COBID = stcFramT.CanFramT.COBID + *fram;				// get 8 bits to COBID
			fram++ ;

			for( i = 0; i < actlenth - 4; i++ , fram++ )
				stcFramT.CanFramT.data[i] = *fram ;			

	return TRUE;
}

/*
*********************************************************************************************************
*                                             INITIALIZATION
*
* Description: parse a fram to the struct
*
* Arguments  : 
*              pfram    : the point of fram which get from the buf
*              stcFramT : the struct which to be store
*           
* Returns    : if check ED not correct it will return -1, else return the TRUE
*********************************************************************************************************
*/
s8 CreatCO_MSG(FRAM_T FramT,	CO_MSG COFramT)
{
	//if( FramT.can_u.CanExtendT.CEXT)
	//COFramT.COMsgId = 0x80000000	;
	//COFramT.COMsgId = FramT.can_u.CanExtendT.
}

s8 CcmFilterReset( )
{


	return TRUE;
}



s8 CcmFilterAll( )
{

	return TRUE;
}

s8 CcmReset( )
{

	return TRUE;
}
