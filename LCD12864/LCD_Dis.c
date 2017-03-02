//========================================================================
// �ļ���: LCD_Dis.c
// ��  ��: Xinqiang Zhang(email: Xinqiang@Mzdesign.com.cn)
//			www.Mzdesign.com.cn
// ��  ��: 2007/05/17
// ��  ��: ���нӿڰ�LCD�û��ӿڲ����
//
// ��  ��: ��ο�������ʾ����Ӳ������˵���������DataSheet,���Դ�����д����
//			��ӿڳ���;������MCS51ϵ��MCU�������е�ֻд���޷�������LCD
// ��  ��:
//      2007/05/17      First version    Mz Design
//========================================================================
//#include "LCD_Driver_User.h"					//LCD�ײ���������ͷ�ļ�
//#include "LCD_Config.h"							//LCD�������ļ��������������Ƿ��õȵĶ���
#include <includes.h>

static int x,y;
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


//extern code unsigned char Asii0608[];			//6X8��ASII�ַ���
//extern code unsigned char Asii0816[];			//8X16��ASII�ַ���
//extern code unsigned char GB1616[];		        //16*16�Զ���ĺ��ֿ�

extern  unsigned char Asii0608[];			//6X8��ASII�ַ���
extern  unsigned char Asii0816[];			//8X16��ASII�ַ���
extern  unsigned char GB1616[];				//16*16�Զ���ĺ��ֿ�
extern  unsigned char font6x8[][6];


unsigned char X_Witch;					//�ַ�д��ʱ�Ŀ��
unsigned char Y_Witch;					//�ַ�д��ʱ�ĸ߶�
unsigned char Font_Wrod;				//�����ÿ����ģռ�ö��ٸ��洢��Ԫ��
unsigned char *Char_TAB;				//�ֿ�ָ��
//unsigned char Plot_Mode;				//��ͼģʽ
unsigned short BMP_Color;
unsigned short Char_Color;
unsigned char Font_type=0;				//��ʶ�ַ�����
unsigned char font_num;
//========================================================================
// ����: void FontSet(int Font_NUM,unsigned int Color)
// ����: �ı���������
// ����: Font_NUM ����ѡ��,�������������ֿ�Ϊ׼
//		 Color  �ı���ɫ,���������Դ��ֿ�  
// ����: ��
// ��ע: 
// �汾:
//      2006/10/15      First version
//========================================================================
void FontSet(unsigned char Font_NUM,unsigned short Color)
{
	switch(Font_NUM)
	{
		case 0:Font_Wrod = 16;	//ASII�ַ�A
		       X_Witch = 8;
		       Y_Witch = 16;
		       Char_Color = Color;
		       Char_TAB = Asii0816;
                       Font_type=1;
                       font_num = 0;
		break;
                 /*
		case 1: Font_Wrod = 8;	//ASII�ַ�B
				X_Witch = 6;
				Y_Witch = 8;
				Char_Color = Color;
				Char_TAB = Asii0608;
                                Font_type=1;	
                                font_num = 1;
		break;		
		case 2: Font_Wrod = 1;	//����A
				X_Witch = 16;
				Y_Witch = 16;
				Char_Color = Color;
				Char_TAB = (unsigned char *)GB1616;
                                Font_type = 0;
                                font_num = 2;
		break;
               
		case 3: Font_Wrod = 16;	//����B
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
// ����: void PutChar(unsigned char x,unsigned char y,char a)  
// ����: д��һ����׼�ַ�
// ����: x  X������     y  Y������(0~7ҳ)
//		 a  Ҫ��ʾ���ַ����ֿ��е�ƫ����  
// ����: ��
// ��ע: ASCII�ַ���ֱ������ASCII�뼴��
// �汾:
//      2006/10/15      First version
//		2007/05/17		V2.1  for seriea port LCD
//========================================================================
void PutChar(unsigned char x,unsigned char y,char a)       
{
  int i,j;//,K;		                //�����ݴ�
  unsigned char *p_data;
  unsigned char Temp;
  
  if(Font_type==1)
    p_data = Char_TAB + (a-32)*Font_Wrod;
  else
    p_data = Char_TAB + a*Font_Wrod;	//Ҫд�ַ����׵�ַ

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
// ����: void PutString(unsigned char x,unsigned char y,char *p)
// ����: ��x��yΪ��ʼ���괦д��һ����׼�ַ�
// ����: x  X������     y  Y������(0~7ҳ)
//		 p  Ҫ��ʾ���ַ���  
// ����: ��
// ��ע: ���������Դ���ASCII�ַ�����ʾ
// �汾:
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
// ����: void Rectangle(unsigned char left, unsigned char top, unsigned char right,
//						 unsigned char bottom, unsigned char Mode)
// ����: ��x,yΪԲ��RΪ�뾶��һ��Բ(mode = 0) or Բ��(mode = 1)
// ����: left - ���ε����ϽǺ����꣬��Χ0��126
//		 top - ���ε����Ͻ�������(0~7ҳ)����Χ0��6
//		 right - ���ε����½Ǻ����꣬��Χ1��127
//		 bottom - ���ε����½�������(0~7ҳ)����Χ0��7
// ����: ��
// ��ע: ��Բ����ִ�н��������MCU�п��Ź����������幷�Ĳ���
// �汾:
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
		LCD_RegWrite(((left&0xf0) >> 4)|0x10);	//���ó�ʼ��ַ 
		LCD_RegWrite(left&0x0f);
		for(ucTemp2=left;ucTemp2<=right;ucTemp2++)
		{
			LCD_DataWrite(BMP_Color);
		}
	}
#endif
        
}
//========================================================================
// ����: void ClrScreen(unsigned int Mode)
// ����: ����������ִ��ȫ��Ļ��������ǰ��ɫ
// ����: Mode   0:ȫ������
//				1:ȫ�����ǰ��ɫ
// ����: ��
// ��ע: 
// �汾:
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
// ����: void PutIntValue(unsigned short value)
// ����: ��ʾ����
// ����: 
//				
// ����: ��
// ��ע: 
// �汾:
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
// ����: void Writ_Dot(int x,int y,unsigned int Color)
// ����: �����x,yΪ���������
// ����: x  X������     y  Y������      Color  ������ɫ 
// ����: ��
// ��ע: �����Լ�֮ǰ������x��y����ϵ�����û���ģ�������ʵ��LCD��������ϵ
//		 �������ṩ�ɽ�������任�Ľӿ�
// �汾:
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
		Address_set(x,y,x,y);(0,0,0,0);		//��������
		    for(i=0;i<1896;i++)
			 {	
			 	picL=*(p+i*2);	//���ݵ�λ��ǰ
				picH=*(p+i*2+1);				
				LCD_DataWrite16Bit(picH<<8|picL);  						
			 }	
		 }
	}		
}




