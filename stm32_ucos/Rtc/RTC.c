#include "includes.h"
#include "rtc.h"

//#define RTCClockSource_LSI   /* Use the internal 32 KHz oscillator as RTC clock source */
#define RTCClockSource_LSE   /* Use the external 32.768 KHz oscillator as RTC clock source */
//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

static  const struct tm  s_stcDefaultTime = 
{
    0,    /* seconds after the minute, 0 to 60
                     (0 - 60 allows for the occasional leap second) */
    30,   /* minutes after the hour, 0 to 59 */
    13,   /* hours since midnight, 0 to 23 */
    26,   /* day of the month, 1 to 31 */
    11,   /* months since January, 0 to 11 */
    108,  /* years since 1900 */
    0,    /* days since Sunday, 0 to 6 */
};


/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : Configures the RTC.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  if (IsRealTimeActive())
  {	
    return;
  }

  /* Reset Backup Domain */
  BKP_DeInit();

  BKP_WriteBackupRegister(BKP_DR1, 0xA5A5); 

#ifdef RTCClockSource_LSI
  /* Enable LSI */ 
  RCC_LSICmd(ENABLE);
  /* Wait till LSI is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select LSI as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);  
#elif defined	RTCClockSource_LSE  
  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  
#endif


  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);


#ifdef RTCClockOutput_Enable  
  /* Disable the Tamper Pin */
  BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
                               functionality must be disabled */
                               
  /* Enable RTC Clock Output on Tamper Pin */
  BKP_RTCCalibrationClockOutputCmd(ENABLE);
#endif 

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  
  /* Enable the RTC Second */  
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  
  /* Set RTC prescaler: set RTC period to 1sec */
#ifdef RTCClockSource_LSI
  RTC_SetPrescaler(31999); /* RTC period = RTCCLK/RTC_PR = (32.000 KHz)/(31999+1) */
#elif defined	RTCClockSource_LSE
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
#endif
  
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  SetRealTime(&s_stcDefaultTime);
}


/*********************************************************************************************************
** 函数名称: IsRealTimeActive
** 功能描述: 判断实时时钟是否已在运行
** 输　入:   无
** 输　出:   无
** 返回值:   FALSE: 实时时钟尚未运行, 需重新设置新的时间
             TRUE:  实时时钟已运行
********************************************************************************************************/
BOOLEAN IsRealTimeActive(void)
{
  if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
	return FALSE;  
  }
  else
  {
    return TRUE; 
  }
}

/*********************************************************************************************************
** 函数名称: ReadRealTime
** 功能描述: 从实时时钟中读取时钟值
** 输　入:   
** 输　出:   pstcReg  指向读取时钟的存放地址
** 返回值:   TRUE:  读出时钟成功
             FALSE: 读取时钟失败
********************************************************************************************************/
BOOLEAN ReadRealTime(struct tm *pstcReg)
{
  time_t dwRtcCounter; 
  if (pstcReg == NULL)
  {
    return FALSE;
  }
  dwRtcCounter = (time_t)RTC_GetCounter();
  *pstcReg = *localtime(&dwRtcCounter);
  return TRUE;
}


/*********************************************************************************************************
** 函数名称: SetRealTime
** 功能描述: 设置实时时钟的时钟值
** 输　入:   pstcReg 要设置的时钟值结构
** 输  出:   无
** 返回值:   TRUE:  设置时钟成功
             FALSE: 设置时钟失败
********************************************************************************************************/
BOOLEAN SetRealTime(const struct tm *pstcReg)
{
  time_t dwRtcCounter; 
  struct tm stcTime;

  if (pstcReg == NULL)
  {
    return FALSE;
  }
  stcTime = *pstcReg;
  dwRtcCounter = mktime(&stcTime);
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask(); 
  /* Change the current time */
  RTC_SetCounter(dwRtcCounter);
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();   

  return TRUE;
}

/*********************************************************************************************************
** 函数名称: SaveParaToRTC
** 功能描述: 将4字节的值保存在RTC中, 作为掉电保存参数
********************************************************************************************************/
void SaveParaToRTC(INT32U dwPara)
{
   BKP_WriteBackupRegister(BKP_DR4, dwPara); 
   BKP_WriteBackupRegister(BKP_DR5, dwPara >> 16); 
}

/*********************************************************************************************************
** 函数名称: RestoreParaFromRTC
** 功能描述: 从RTC中读取4字节的掉电保存参数
********************************************************************************************************/
INT32U RestoreParaFromRTC(void)
{
   INT32U low;
   INT32U high;

   low = BKP_ReadBackupRegister(BKP_DR4); 
   high = BKP_ReadBackupRegister(BKP_DR5);
   return (low + high << 16);
}

