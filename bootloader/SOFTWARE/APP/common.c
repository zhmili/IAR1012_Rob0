/*-------------------------------------------------------------------------

                            接口部分
                            
                            
-------------------------------------------------------------------------*/

#include <string.h>

#include <includes.h>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FunVoidType JumpToApplication;
static FunVoidType FunReceEnter = NULL;
static FunVoidType FunReceExit = NULL;
static FunWriteType FunWrite = NULL;
static FunProcessType FunCurrentProcess = NULL;

static u32 m_JumpAddress;
static u32 m_ProgramAddr = ApplicationAddress;
static  SerialBuffType m_ReceData = SerialBuffDefault();

static volatile eCOM_STATUS m_Mode = eCOMChoose;
static vu32 m_FlashAddress = 0;
static vu32 m_ExtFlashCounter = 0;       //外部FLASH擦除的扇区号

static int tim3_err_tms = 0;

int buft[20] ={0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/*******************************************************************************
* Function Name :static void Print(u8 *str)
* Description   :打印消息  串口发送
* Input         :
* Output        :
* Other         :
* Date          :2013.03.01
*******************************************************************************/
void Print(u8 *str)
{
#if 1
    u16 len = 0;

    len = strlen((const char *)str);
#ifdef USE_USART1
    while (BspUsart1Send(str, len) != TRUE);
#endif

#ifdef USE_UART5
   while (BspUart5Send(str, len) != TRUE);
#endif

#endif
}


/*******************************************************************************
* Function Name :void ReceOneChar(u8 ReceCharacter)
* Description   :接收到一个字符
* Input         :
* Output        :
* Other         :
* Date          :2013.02.19
*******************************************************************************/
static void ReceOneChar(u8 ReceCharacter)
{
    if (m_ReceData.ind >= USART1_BUFF_LANGTH)
        return;
        
    if (m_ReceData.len > 0)
        return;
        
    m_ReceData.buf[m_ReceData.ind++] = ReceCharacter;
		tim3_err_tms = 0;
    BspTim3Open();      //定时器重新计数
}

/*******************************************************************************
* Function Name :static void TimEndHandle(void)
* Description   :接收字符超时回调函数
* Input         :
* Output        :
* Other         :
* Date          :2013.02.19
*******************************************************************************/
static void TimEndHandle(void)
{
	  buft[2] = m_ReceData.ind;
	  if(m_Mode != eCOMReceive)
		{
    BspTim3Close();
		
    m_ReceData.len = m_ReceData.ind;
    m_ReceData.ind = 0;
		}
		else
		{
			if (m_ReceData.buf[0] == SOH)
			{
				if(m_ReceData.ind >= (PACKET_SIZE-1))
				{
					BspTim3Close();
					m_ReceData.len = m_ReceData.ind;
					m_ReceData.ind = 0;
				}
				else
				{
					BspTim3Open();
					tim3_err_tms++;
					buft[0] = tim3_err_tms;
					if(tim3_err_tms > 30)
					{
						m_ReceData.len = m_ReceData.ind;
						m_ReceData.ind = 0;
					}
				}
			}
			else if (m_ReceData.buf[0] == STX)
			{
				if(m_ReceData.ind >= (PACKET_1K_SIZE-1))
				{
					BspTim3Close();
					m_ReceData.len = m_ReceData.ind;
					m_ReceData.ind = 0;
				}
				else
				{
					BspTim3Open();
					tim3_err_tms++;
					buft[1] = tim3_err_tms;
					buft[4] = m_ReceData.ind;
					if(tim3_err_tms > 30)
					{
						m_ReceData.len = m_ReceData.ind;
						m_ReceData.ind = 0;
					}
				}
			
			}
			else
			{
			    BspTim3Close();
		
					m_ReceData.len = m_ReceData.ind;
					m_ReceData.ind = 0;
			}
		}
}



/*******************************************************************************
* Function Name :void JumpToApp(void)
* Description   :跳转到应用程序区
* Input         :None
* Output        :None
* Other         :None
* Date          :2013.02.19
*******************************************************************************/
static void JumpToApp(void)
{
    if (((*(vu32*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
    { 
        BspClose();
        /* Jump to user application */
        m_JumpAddress = *(vu32*) (ApplicationAddress + 4);
        JumpToApplication = (FunVoidType) m_JumpAddress;
				
        /* Initialize user application's Stack Pointer */
        __MSR_MSP(*(vu32*) ApplicationAddress);
        JumpToApplication();
    }
}


/*******************************************************************************
* Function Name :static void AppChoose(u8 *pData, u32 *pLen, volatile eCOM_STATUS *peStat)
* Description   :操作选择   300ms内有 收到 字符C 进入bootload区
* Input         :
* Output        :
* Other         :
* Date          :2013.02.26
*******************************************************************************/
static void AppChoose(u8 *pData, u32 *pLen, volatile eCOM_STATUS *peStat)
{
    static u8 flg = 0;
		static vu32 get72;
#if 0
    if (*pLen > 0)
    {
        if ((*pData == 'C') || (*pData == 'c'))
        {
            if (flg == 0)
                flg++;

            if (flg && IS_TIMEOUT_1MS(eTim1, 200))  //二次确认
                *peStat = eCOMDisplay;
        }
        *pLen = 0;
    }
    if (IS_TIMEOUT_1MS(eTim2, 320))
    {
        JumpToApp(); 
 //       Print("\n\r运行失败!");
        while (1);
    }
#endif
		
		//if (((*(volatile vu32*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
    {
			////如果是中途拔线不能跳入 app
#if ROB_NEW

			if((left_button_read() == 0)&&(middle_button_read() == 0))
			{
				PutString(5,0,"ForceDownLoad...!");
				
				*peStat = eCOMInput;
			}			
			else 
				
			if((((*(volatile vu8*)TOP_RAM_ADDR_P2) == 'c')&&((*(volatile vu8*)TOP_RAM_ADDR_P3) = 'r'))||((*(volatile vu8*)AppDownloadRecAddr) ) != 'r')
			{
				(*(volatile vu8*)TOP_RAM_ADDR_P2) = '0';
				(*(volatile vu8*)TOP_RAM_ADDR_P3) = '0';
				PutString(5,0,"DownLoad...!");
				*peStat = eCOMInput;
			}
			else
			{
				PutString(5,0,"To App!");
				JumpToApp();
			}
#else
			if((left_button_read() == 0)&&(middle_button_read() == 0))
			{
				PutString(5,0,"ForceDownLoad...!");
				
				*peStat = eCOMInput;
			}
			else if ((((*(volatile vu8*)AppDownloadRecAddr) ) == 'r')&&(((*(volatile vu8*)(AppDownloadRecAddr+4)) ) == 'l'))
			{
			PutString(5,0,"To App!");

			JumpToApp();
			} else {
			PutString(5,0,"DownLoad...!");
			*peStat = eCOMInput;
			}
#endif		
    } 
		
		//else {
		//	*peStat = eCOMInput;
    //} 
}

/*******************************************************************************
* Function Name :static void DisplayMessage(u8 *pData, u32 *pLen, volatile eCOM_STATUS *peStat)
* Description   :显示提示消息
* Input         :
* Output        :
* Other         :
* Date          :2013.02.26
*******************************************************************************/
static void DisplayMessage(u8 *pData, u32 *pLen, volatile eCOM_STATUS *peStat)
{

    *pLen = 1;
    strcpy((char *)pData, "\r*********************************************************\r\n");
    strcat((char *)pData, "1.更新应用区程序；\r\n");  
    strcat((char *)pData, "2.运行APP程序。\r\n");
    strcat((char *)pData, "*********************************************************\r\n");
    strcat((char *)pData, "请选择:");
    *peStat = eCOMInput;
    
//  Print(pData);
    *pLen = 0;

}


/*******************************************************************************
* Function Name :static void InputSelect(u8 *pData, u32 *pLen, volatile eCOM_STATUS *peStat)
* Description   :操作选择
* Input         :
* Output        :
* Other         :
* Date          :2013.02.26
*******************************************************************************/
static void InputSelect(u8 *pData, u32 *pLen, volatile eCOM_STATUS *peStat)
{
    if (*pLen > 0)
    {
        switch (*pData)
        {
        case '1': 
            m_ProgramAddr = ApplicationAddress; 
            FunReceEnter = FLASH_ProgramStart;
            FunWrite = FLASH_WriteBank;
            FunReceExit = FLASH_ProgramDone;
            *peStat = eCOMReceive;
						
						Print("1");				//回馈信息
						
						//SetDownLoadRecorder('c','r');
				    //FLASH_ProgramHalfWord(AppDownloadRecAddr, 'c');			//修改，再下载前 将顶部数据改为d n, 作用防止中间断线等操作
            //FLASH_ProgramHalfWord(AppDownloadRecAddr+4, 'r');
						
 //         Print("1\r\n请选择要发送文件");
            break;    
            
        case '2': 
            *peStat = eCOMChoose; 
 //           Print("2\r\n运行程序...");
            break;    
				
		case '5':
			Print("b");
			break;
            
        default :break;
        }
        *pLen = 0;
    }
}



/*******************************************************************************
* Function Name :static void ReceiveData(u8 *pData, u32 *pLen, volatile eCOM_STATUS *peStat)
* Description   :YModem协议接收数据，并存储数据
* Input         :
* Output        :
* Other         :
* Date          :2013.02.26
*******************************************************************************/
static void ReceiveData(u8 *pData, u32 *pLen, volatile eCOM_STATUS *peStat)
{
    u8 pArray[1028] = {0,};
    int len = 0;
    
    switch (YmodemReceive((char *)(pData), (int *)pLen, (char *)pArray, (int *)&len))
    {
    case YM_FILE_INFO: 
        if (FunReceEnter) (*FunReceEnter)();    								//开始函数
        break;
        
    case YM_FILE_DATA: 
        if (FunWrite) (*FunWrite)(pArray, m_ProgramAddr, len);  //接收数据函数
        m_ProgramAddr += len;
        break;
        
    case YM_EXIT: 
        if (FunReceExit) (*FunReceExit)();      								//接收完毕函数
        
        FunReceEnter = NULL;
        FunWrite = NULL;
        FunReceExit = NULL;
				//*peStat = eCOMChoose;	
        //Print("\r\n运行程序...");
				ClrScreen(0);
				PutString(5,0,"Recv End!");
				JumpToApp();							//烧写完毕后 直接进入app
		    
//				*peStat = eCOMInput;		//烧写完毕后继续进入烧写状态
//				Print("\r\n烧写完毕...");
				
        break;
    }

}

/*******************************************************************************
* Function Name :void CommonInit(void)
* Description   :接口初始化
* Input         :
* Output        :
* Other         :
* Date          :2013.02.19
*******************************************************************************/
void CommonInit(void)
{
    BspTim3SetIRQCallBack(TimEndHandle);
#ifdef USE_USART1
    BspUsart1IRQCallBack(ReceOneChar);
#endif

#ifdef USE_UART5
    BspUart5IRQCallBack(ReceOneChar);
#endif
    
}


/*******************************************************************************
* Function Name :void CommonExec(void)
* Description   :接口函数
* Input         :
* Output        :
* Other         :
* Date          :2013.02.20
*******************************************************************************/
void CommonExec(void)
{

    switch (m_Mode)
    {
    case eCOMChoose:    //判断进入 IAP程序 还是APP程序
        FunCurrentProcess = AppChoose;
#if U_DBG
		PutString(5,20,"AppChoose!");
	//Print("m_Mode  eCOMChoose \r\n\n");
#endif
				//FunCurrentProcess = InputSelect;
        break;
        
    case eCOMDisplay:   //IAP操作显示
        FunCurrentProcess = DisplayMessage;
        break;
        
    case eCOMInput:     //IAP操作选择
        FunCurrentProcess = InputSelect;
#if U_DBG
		PutString(5,20,"InputSelect!");
	//Print("m_Mode  eCOMChoose \r\n\n");
#endif
        break;
		
    case eCOMReceive:   //YMODEM 接收数据
        FunCurrentProcess = ReceiveData;
        break;
        
    default:
        m_Mode = eCOMChoose;
        break;
    }
    (*FunCurrentProcess)((u8 *)(m_ReceData.buf), (u32 *)&(m_ReceData.len), &m_Mode);
}




/*********************************** END **************************************/

