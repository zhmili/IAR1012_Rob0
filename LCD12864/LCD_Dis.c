//========================================================================
// 文件名: LCD_Dis.c
// 作  者: Xinqiang Zhang(email: Xinqiang@Mzdesign.com.cn)
//			www.Mzdesign.com.cn
// 日  期: 2007/05/17
// 描  述: 串行接口版LCD用户接口层程序集
//
// 参  考: 请参考具体显示器的硬件描述说明书或者是DataSheet,并以此来编写加速
//			版接口程序;适用于MCS51系列MCU驱动串行的只写入无法读出的LCD
// 版  本:
//      2007/05/17      First version    Mz Design
//========================================================================
//#include "LCD_Driver_User.h"					//LCD底层驱动声明头文件
//#include "LCD_Config.h"							//LCD的配置文件，比如坐标轴是否倒置等的定义
#include <includes.h>

static int x,y;
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


//extern code unsigned char Asii0608[];			//6X8的ASII字符库
//extern code unsigned char Asii0816[];			//8X16的ASII字符库
//extern code unsigned char GB1616[];		        //16*16自定义的汉字库

extern  unsigned char Asii0608[];			//6X8的ASII字符库
extern  unsigned char Asii0816[];			//8X16的ASII字符库
extern  unsigned char GB1616[];				//16*16自定义的汉字库
extern  unsigned char font6x8[][6];


unsigned char X_Witch;					//字符写入时的宽度
unsigned char Y_Witch;					//字符写入时的高度
unsigned char Font_Wrod;				//字体的每个字模占用多少个存储单元数
unsigned char *Char_TAB;				//字库指针
//unsigned char Plot_Mode;				//绘图模式
unsigned short BMP_Color;
unsigned short Char_Color;
unsigned char Font_type=0;				//标识字符类型
unsigned char font_num;
//========================================================================
// 函数: void FontSet(int Font_NUM,unsigned int Color)
// 描述: 文本字体设置
// 参数: Font_NUM 字体选择,以驱动所带的字库为准
//		 Color  文本颜色,仅作用于自带字库  
// 返回: 无
// 备注: 
// 版本:
//      2006/10/15      First version
//========================================================================
void FontSet(unsigned char Font_NUM,unsigned short Color)
{
	switch(Font_NUM)
	{
		case 0:Font_Wrod = 16;	//ASII字符A
		       X_Witch = 8;
		       Y_Witch = 16;
		       Char_Color = Color;
		       Char_TAB = Asii0816;
                       Font_type=1;
                       font_num = 0;
		break;
                 /*
		case 1: Font_Wrod = 8;	//ASII字符B
				X_Witch = 6;
				Y_Witch = 8;
				Char_Color = Color;
				Char_TAB = Asii0608;
                                Font_type=1;	
                                font_num = 1;
		break;		
		case 2: Font_Wrod = 1;	//汉字A
				X_Witch = 16;
				Y_Witch = 16;
				Char_Color = Color;
				Char_TAB = (unsigned char *)GB1616;
                                Font_type = 0;
                                font_num = 2;
		break;
               
		case 3: Font_Wrod = 16;	//汉字B
				X_Witch = 16;
				Y_Witch = 2;
				Char_Color = Color;
				Char_TAB = GB16;
		break;
                */
		default: break;
	}
}
//========================================================================
// 函数: void PutChar(unsigned char x,unsigned char y,char a)  
// 描述: 写入一个标准字符
// 参数: x  X轴坐标     y  Y轴坐标(0~7页)
//		 a  要显示的字符在字库中的偏移量  
// 返回: 无
// 备注: ASCII字符可直接输入ASCII码即可
// 版本:
//      2006/10/15      First version
//		2007/05/17		V2.1  for seriea port LCD
//========================================================================
void PutChar(unsigned char x,unsigned char y,char a)       
{
  int i,j;//,K;		                //数据暂存
  unsigned char *p_data;
  unsigned char Temp;
  
  if(Font_type==1)
    p_data = Char_TAB + (a-32)*Font_Wrod;
  else
    p_data = Char_TAB + a*Font_Wrod;	//要写字符的首地址

  j = 0;
  
  while((j++) < Y_Witch)
  {
    if(y > Dis_Y_MAX) 
      break;
    i = 0;
    while(i < X_Witch)
    {
      if((i&0x07)==0)
      {
	Temp = *(p_data ++);
      }
        
      if((x+i) > Dis_X_MAX) 
        break;
      if((Temp & 0x80) > 0){ 
        Writ_Dot(x+i,y,Char_Color);
      } else {
        Writ_Dot(x+i,y,LCD_INITIAL_COLOR);
      }
        
      Temp = Temp << 1;
      i++;
    }
      y ++;
  }
  
}


//========================================================================
// 函数: void PutString(unsigned char x,unsigned char y,char *p)
// 描述: 在x、y为起始坐标处写入一串标准字符
// 参数: x  X轴坐标     y  Y轴坐标(0~7页)
//		 p  要显示的字符串  
// 返回: 无
// 备注: 仅能用于自带的ASCII字符串显示
// 版本:
//      2006/10/15      First version
//========================================================================
void PutString(unsigned char x,unsigned char y,unsigned char *p)
{
  while(*p!=0)
  {
    PutChar(x,y,*p);
    x += X_Witch;
    if((x + X_Witch) > Dis_X_MAX)
    {
      x = 0;
      if((Dis_Y_MAX - y) < Y_Witch) 
        break;
      else 
        y += Y_Witch;
    }
    p++;
   }
}

//========================================================================
// 函数: void Rectangle(unsigned char left, unsigned char top, unsigned char right,
//						 unsigned char bottom, unsigned char Mode)
// 描述: 以x,y为圆心R为半径画一个圆(mode = 0) or 圆面(mode = 1)
// 参数: left - 矩形的左上角横坐标，范围0到126
//		 top - 矩形的左上角纵坐标(0~7页)，范围0到6
//		 right - 矩形的右下角横坐标，范围1到127
//		 bottom - 矩形的右下角纵坐标(0~7页)，范围0到7
// 返回: 无
// 备注: 画圆函数执行较慢，如果MCU有看门狗，请作好清狗的操作
// 版本:
//      2005/05/21      First version
//		2007/05/17		V2.1  for seriea port LCD
//========================================================================
void Rectangle(unsigned char left, unsigned char top, unsigned char right, unsigned char bottom)
{
#if 0
	unsigned char uiTemp,ucTemp2;
	if(left>right)
	{
		uiTemp = left;
		left = right;
		right = uiTemp;
	}
	if(top>bottom)
	{
		uiTemp = top;
		top = bottom;
		bottom = uiTemp;
	}
	for(uiTemp=top;uiTemp<=bottom;uiTemp++)
	{
		LCD_RegWrite(uiTemp+0xb0);
		LCD_RegWrite(((left&0xf0) >> 4)|0x10);	//设置初始地址 
		LCD_RegWrite(left&0x0f);
		for(ucTemp2=left;ucTemp2<=right;ucTemp2++)
		{
			LCD_DataWrite(BMP_Color);
		}
	}
#endif
        
}
//========================================================================
// 函数: void ClrScreen(unsigned int Mode)
// 描述: 清屏函数，执行全屏幕清除或填充前景色
// 参数: Mode   0:全屏除屏
//				1:全屏填充前景色
// 返回: 无
// 备注: 
// 版本:
//      2007/01/11      First version
//========================================================================
void ClrScreen(unsigned short Mode)
{
	if(Mode==0)
		LCD_Fill(LCD_INITIAL_COLOR);
	else
		LCD_Fill(BMP_Color);
}
//========================================================================
// 函数: void PutIntValue(unsigned short value)
// 描述: 显示整数
// 参数: 
//				
// 返回: 无
// 备注: 
// 版本:
//      2007/01/11      First version
//========================================================================
void PutIntValue(unsigned char x,unsigned char y,unsigned short value)
{
  unsigned char display[5]={0,0};
  u8  num[4],i,j;
  num[3] = value/1000;
  num[2] = (value - num[3]*1000)/100;
  num[1] = (value - num[3]*1000 - num[2]*100)/10;
  num[0] = value - num[3]*1000 - num[2]*100 - num[1]*10;
  j= 4;
                
  for(i = 0 ;i < j ;i++)
    display[3-i] = num[i]+'0';
                
    display[j]  = '\0';
  PutString( x, y,display);	
}


void Put10Value(unsigned char x,unsigned char y, unsigned short value)
{
   if(value >= 0 && value <= 9)
      PutChar(x, y, value + '0');
}


/*******************************************************************************
* Function Name  : PUTCHAR_PROTOTYPE
* Description    : Retargets the C library printf function to the LCD128*64
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
 PUTCHAR_PROTOTYPE
{
#if UART_PRINTF 

    USART_SendData(UART5, (u8) ch);
		
    while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);		
	
    return (ch);
#else
  
  if((x + X_Witch) > (Dis_X_MAX ))
  {
      x = 0;
      if(((Dis_Y_MAX) - y) < (Y_Witch))
      {       
        y = 0;
        x = 0;
      } else 
        y += Y_Witch;
   }
   
   PutChar(x,y,ch);
   x += X_Witch;
              
   return ch;
#endif
}



//========================================================================
// 函数: void Writ_Dot(int x,int y,unsigned int Color)
// 描述: 填充以x,y为坐标的象素
// 参数: x  X轴坐标     y  Y轴坐标      Color  像素颜色 
// 返回: 无
// 备注: 这里以及之前的所有x和y坐标系都是用户层的，并不是实际LCD的坐标体系
//		 本函数提供可进行坐标变换的接口
// 版本:
//      2006/10/15      First version
//========================================================================
void Writ_Dot(int x,int y,unsigned short Color)
{
#if	LCD_XY_Switch == 0
	#if (LCD_X_Rev == 0)&&(LCD_Y_Rev == 0)
		Write_Dot_LCD(x,y,Color);
	#endif
	#if (LCD_X_Rev == 1)&&(LCD_Y_Rev == 0)
		Write_Dot_LCD(LCD_X_MAX - x,y,Color);
	#endif
	#if (LCD_X_Rev == 0)&&(LCD_Y_Rev == 1)
		Write_Dot_LCD(x,LCD_Y_MAX - y,Color);
	#endif
	#if (LCD_X_Rev == 1)&&(LCD_Y_Rev == 1)
		Write_Dot_LCD(LCD_X_MAX - x,LCD_Y_MAX - y,Color);
	#endif
#endif
#if	LCD_XY_Switch == 1
	#if (LCD_X_Rev == 0)&&(LCD_Y_Rev == 0)
		Write_Dot_LCD(y,x,Color);
	#endif
	#if (LCD_X_Rev == 1)&&(LCD_Y_Rev == 0)
		Write_Dot_LCD(y,LCD_Y_MAX - x,Color);
	#endif
	#if (LCD_X_Rev == 0)&&(LCD_Y_Rev == 1)
		Write_Dot_LCD(LCD_X_MAX - y,x,Color);
	#endif
	#if (LCD_X_Rev == 1)&&(LCD_Y_Rev == 1)
		Write_Dot_LCD(LCD_X_MAX - y,LCD_Y_MAX - x,Color);
	#endif
#endif	
}


//========================================================================


//========================================================================
void Show_Image(const unsigned char *p)
{
  	int i,j,k; 
	unsigned char picH,picL;
  
	
	for(k=0;k<3;k++)
	{
	   	for(j=0;j<3;j++)
		{	
		Address_set(x,y,x,y);(0,0,0,0);		//坐标设置
		    for(i=0;i<1896;i++)
			 {	
			 	picL=*(p+i*2);	//数据低位在前
				picH=*(p+i*2+1);				
				LCD_DataWrite16Bit(picH<<8|picL);  						
			 }	
		 }
	}		
}




