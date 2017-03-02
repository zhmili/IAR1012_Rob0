//========================================================================
// 文件名: LCD_Driver_User.c
//========================================================================
//#include "LCD_Config.h"

#include <includes.h>

#define LCD_WR_DATA   LCD_DataWrite
#define LCD_WR_REG    LCD_RegWrite

extern const unsigned char Asii0816[];
extern const unsigned char gImage_logo[];

#if ILI9163C
//========================================================================
// 函数: void LCD_DataWrite(unsigned int Data)
// 描述: 写一个字节的显示数据至LCD中的显示缓冲RAM当中
// 参数: Data 写入的数据 
// 返回: 无
// 备注: 无
// 版本:
//      2007/01/09      First version
//========================================================================
void LCD_DataWrite(unsigned char u8_data)
{
  unsigned char Num;
  LCD_CS(0);
  LCD_A0(1);
            
  for(Num=0;Num<8;Num++)
  {
    if((u8_data&0x80) == 0)	
    {
      LCD_SDI(0);
    } else {
      LCD_SDI(1);
    }
    u8_data = u8_data << 1;
    LCD_CLK(0);
    LCD_CLK(1);
   }
   
  LCD_CS(1);
}



void LCD_DataWrite16Bit(unsigned short u16_data)
{
  //  LCD_DataWrite(u16_data>>8);
   // LCD_DataWrite(u16_data);
    
    
  unsigned char Num;
  LCD_CS(0);
  LCD_A0(1);
            
  for(Num=0;Num<16;Num++)
  {
    if((u16_data&0x8000) == 0)	
    {
      LCD_SDI(0);
    } else {
      LCD_SDI(1);
    }
    u16_data = u16_data << 1;
    LCD_CLK(0);
    LCD_CLK(1);
   }
   
  LCD_CS(1);
}


//========================================================================
// 函数: void LCD_RegWrite(unsigned char Command)
// 描述: 写一个字节的数据至LCD中的控制寄存器当中
// 参数: Command		写入的数据，低八位有效（byte） 
// 返回: 无
// 备注: 
// 版本:
//      2007/01/09      First version
//========================================================================
void LCD_RegWrite(unsigned char Command)
{
	unsigned char Num;
	LCD_CS(0);//LCD_CS(0);//LCD_CS = 0;
	LCD_A0(0);//LCD_A0(0);//LCD_A0 = 0;
 
	for(Num=0;Num<8;Num++)
	{
		if((Command&0x80) == 0)	
                {
                  LCD_SDI(0);//LCD_SDI(0);//LCD_SDI = 0;
                }
		else 
                {
                 LCD_SDI(1);//LCD_SDI(1);//LCD_SDI = 1;
                }
		Command = Command << 1;
		LCD_CLK(0);//LCD_CLK(0);//LCD_CLK = 0;
		LCD_CLK(1);//LCD_CLK(1);//LCD_CLK = 1;
	}
	LCD_CS(1);//LCD_CS(1);//LCD_CS = 1;
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_WR_REG(0x2a);
        LCD_DataWrite16Bit(x1);
        LCD_DataWrite16Bit(x2);
	LCD_WR_REG(0x2b);
        LCD_DataWrite16Bit(y1);
        LCD_DataWrite16Bit(y2);
	LCD_WR_REG(0x2c);
}



void LCD_Init(void)
{
        //LCD_CS(1);
        //LCD_RES(0);
	//LCD_RES(1);
 
	LCD_CS(0);  //打开片选使能
        LCD_RegWrite(0x01);
        OSTimeDlyHMSM(0,0,0,5);
   	LCD_RegWrite(0x11); //Sleep out
        OSTimeDlyHMSM(0,0,0,5);
        
	LCD_WR_REG(0x26); //Set Default Gamma
	LCD_WR_DATA(0x04);
	LCD_WR_REG(0xB1);//Set Frame Rate
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x14);
	LCD_WR_REG(0xC0); //Set VRH1[4:0] & VC 
	
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x00);    //0X00  0x02
	LCD_WR_REG(0xC1); //Set BT[2:0] for AVDD & 
	
	LCD_WR_DATA(0x05);
	LCD_WR_REG(0xC5); //Set VMH[6:0] & VML
	
	LCD_WR_DATA(0x46);   //0X46 3A
	LCD_WR_DATA(0x40);   //0X40 32
	LCD_WR_REG(0xC7);// Set VMF
	LCD_WR_DATA(0xC2);   // 0XC2 C0
	LCD_WR_REG(0x3a); //Set Color Format
	LCD_WR_DATA(0x05);
	LCD_WR_REG(0x2A); //Set Column Address
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x7F);
	LCD_WR_REG(0x2B); //Set Page Address
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x7F);
	LCD_WR_REG(0xB4);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xf2); //Enable Gamma bit
	LCD_WR_DATA(0x01);
        
 	LCD_WR_REG(0xEC);
	LCD_WR_DATA(0x0C);//	LCD_WriteData(0x08); 0xC0       
        
	LCD_WR_REG(0x36);
	LCD_WR_DATA(0xD8);//	LCD_WriteData(0x08); 0xC0
        
	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0x3F);//p1
	LCD_WR_DATA(0x26);//p2
	LCD_WR_DATA(0x23);//p3
	LCD_WR_DATA(0x30);//p4
	LCD_WR_DATA(0x28);//p5
	LCD_WR_DATA(0x10);//p6
	LCD_WR_DATA(0x55);//p7
	LCD_WR_DATA(0xB7);//p8
	LCD_WR_DATA(0x40);//p9
	LCD_WR_DATA(0x19);//p10
	LCD_WR_DATA(0x10);//p11
	LCD_WR_DATA(0x1E);//p12
	LCD_WR_DATA(0x02);//p13
	LCD_WR_DATA(0x01);//p14
	LCD_WR_DATA(0x00);//p15
	LCD_WR_REG(0xE1);
	LCD_WR_DATA(0x00);//p1
	LCD_WR_DATA(0x19);//p2
	LCD_WR_DATA(0x1C);//p3
	LCD_WR_DATA(0x0F);//p4
	LCD_WR_DATA(0x14);//p5
	LCD_WR_DATA(0x0F);//p6
	LCD_WR_DATA(0x2A);//p7
	LCD_WR_DATA(0x48);//p8
	LCD_WR_DATA(0x3F);//p9
	LCD_WR_DATA(0x06);//p10
	LCD_WR_DATA(0x1D);//p11
	LCD_WR_DATA(0x21);//p12
	LCD_WR_DATA(0x3d);//p13
	LCD_WR_DATA(0x3e);//p14
	LCD_WR_DATA(0x3f);//p15
	LCD_WR_REG(0x29); // Display On

}


void LCD_Fill(unsigned int Data)
{
  u16 i,j,index;	
  Address_set(0,0,LCD_X_MAX-1,LCD_Y_MAX-1);//设置光标位置
  for(i=0;i<LCD_X_MAX;i++)
  {
    for (j=0;j<LCD_Y_MAX;j++)
    {
      LCD_DataWrite16Bit(Data);
    }
  }
}


void LCD_Image()
{
  u16 i,j,index,k;
  k = 0;	
  Address_set(0,0,LCD_X_MAX-1,LCD_Y_MAX-1);//设置光标位置
  for(i=0;i<LCD_X_MAX;i++)
  {
    for (j=0;j<LCD_Y_MAX;j++)
    {
        LCD_DataWrite(gImage_logo[k++]);
        LCD_DataWrite(gImage_logo[k++]);
    }
  }
}



//========================================================================
// 函数: void Write_Dot_LCD(int x,int y,unsigned int i)
// 描述: 在LCD的真实坐标系上的X、Y点绘制填充色为i的点
// 参数: x 		X轴坐标
//		 y 		Y轴坐标
//		 i 		要填充的点的颜色 
// 返回: 无
// 备注: 
// 版本:
//      2007/04/10      First version
//========================================================================
void Write_Dot_LCD(unsigned char x,unsigned int y,unsigned short i)
{
  Address_set(x,y,x,y);//设置光标位置
  LCD_DataWrite16Bit(i);
}


void Gui_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
  unsigned char i,j;
  unsigned short k,x0;
  x0=x;

  while(*s) 
  {	
    if((*s) < 128) 
    {
      k=*s;
      if (k == 13) 
      {
	  x=x0;
	  y+=16;
      } else {
	if (k>32)
          k-=32; 
        else k=0;
	
	for(i=0;i<16;i++)
	  for(j=0;j<8;j++) 
	  {
	    if(Asii0816[k*16+i]&(0x80>>j))	
              Write_Dot_LCD(x+j,y+i,fc);
	    else 
	    {
	      if (fc!=bc) 
                Write_Dot_LCD(x+j,y+i,bc);
	    }
	  }
	x+=8;
      }
	s++;
    }
		else 
		{
#if 0
			for (k=0;k<hz16_num;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
								else {
									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
								}
							}
				    }
				}
			  }
			s+=2;x+=16;
#endif
		} 
 	
	}

}




#endif


#if ST7735R

extern const unsigned char Asii0816[];

//========================================================================
// 函数: void LCD_DataWrite(unsigned int Data)
// 描述: 写一个字节的显示数据至LCD中的显示缓冲RAM当中
// 参数: Data 写入的数据 
// 返回: 无
// 备注: 无
// 版本:
//      2007/01/09      First version
//========================================================================
void LCD_DataWrite(unsigned char u8_data)
{
  unsigned char Num;
  LCD_CS(0);
  LCD_A0(1);
            
  for(Num=0;Num<8;Num++)
  {
    if((u8_data&0x80) == 0)	
    {
      LCD_SDI(0);
    } else {
      LCD_SDI(1);
    }
    u8_data = u8_data << 1;
    LCD_CLK(0);
    LCD_CLK(1);
    }
    LCD_CS(1);
}



void LCD_DataWrite16Bit(unsigned short u16_data)
{
    LCD_DataWrite(u16_data>>8);
    LCD_DataWrite(u16_data);
}


//========================================================================
// 函数: void LCD_RegWrite(unsigned char Command)
// 描述: 写一个字节的数据至LCD中的控制寄存器当中
// 参数: Command		写入的数据，低八位有效（byte） 
// 返回: 无
// 备注: 
// 版本:
//      2007/01/09      First version
//========================================================================
void LCD_RegWrite(unsigned char Command)
{
	unsigned char Num;
	LCD_CS(0);//LCD_CS(0);//LCD_CS = 0;
	LCD_A0(0);//LCD_A0(0);//LCD_A0 = 0;
 
	for(Num=0;Num<8;Num++)
	{
		if((Command&0x80) == 0)	
                {
                  LCD_SDI(0);//LCD_SDI(0);//LCD_SDI = 0;
                }
		else 
                {
                 LCD_SDI(1);//LCD_SDI(1);//LCD_SDI = 1;
                }
		Command = Command << 1;
		LCD_CLK(0);//LCD_CLK(0);//LCD_CLK = 0;
		LCD_CLK(1);//LCD_CLK(1);//LCD_CLK = 1;
	}
	LCD_CS(1);//LCD_CS(1);//LCD_CS = 1;
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{  
        LCD_RegWrite(0x2a);
	LCD_DataWrite(0x00);
	LCD_DataWrite(x1+2);
	LCD_DataWrite(0x00);
	LCD_DataWrite(x2+2);

	LCD_RegWrite(0x2b);
	LCD_DataWrite(0x00);
	LCD_DataWrite(y1+3);
	LCD_DataWrite(0x00);
	LCD_DataWrite(y2+3);
	
	LCD_RegWrite(0x2c);

}

#define Lcd_WriteData   LCD_DataWrite
#define Lcd_WriteIndex  LCD_RegWrite

void LCD_Init(void)
{
        //LCD_CS(1);
        //LCD_RES(0);
        OSTimeDlyHMSM(0,0,0,60);
	//LCD_RES(1);
        OSTimeDlyHMSM(0,0,0,20);
	LCD_CS(0);  //打开片选使能
      
   	LCD_RegWrite(0x11); //Sleep out
        OSTimeDlyHMSM(0,0,0,120);
        
	//ST7735R Frame Rate
	Lcd_WriteIndex(0xB1); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB2); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB3); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	
	Lcd_WriteIndex(0xB4); //Column inversion 
	Lcd_WriteData(0x07); 
	
	//ST7735R Power Sequence
	Lcd_WriteIndex(0xC0); 
	Lcd_WriteData(0xA2); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x84); 
	Lcd_WriteIndex(0xC1); 
	Lcd_WriteData(0xC5); 

	Lcd_WriteIndex(0xC2); 
	Lcd_WriteData(0x0A); 
	Lcd_WriteData(0x00); 

	Lcd_WriteIndex(0xC3); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0x2A); 
	Lcd_WriteIndex(0xC4); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0xEE); 
	
	Lcd_WriteIndex(0xC5); //VCOM 
	Lcd_WriteData(0x0E); 
	
	Lcd_WriteIndex(0x36); //MX, MY, RGB mode 
	Lcd_WriteData(0xC8); 
	
	//ST7735R Gamma Sequence
	Lcd_WriteIndex(0xe0); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1a); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x18); 
	Lcd_WriteData(0x2f); 
	Lcd_WriteData(0x28); 
	Lcd_WriteData(0x20); 
	Lcd_WriteData(0x22); 
	Lcd_WriteData(0x1f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x23); 
	Lcd_WriteData(0x37); 
	Lcd_WriteData(0x00); 	
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x10); 

	Lcd_WriteIndex(0xe1); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x17); 
	Lcd_WriteData(0x33); 
	Lcd_WriteData(0x2c); 
	Lcd_WriteData(0x29); 
	Lcd_WriteData(0x2e); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x39); 
	Lcd_WriteData(0x3f); 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x03); 
	Lcd_WriteData(0x10);  
	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x7f);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x9f);
	
	Lcd_WriteIndex(0xF0); //Enable test command  
	Lcd_WriteData(0x01); 
	Lcd_WriteIndex(0xF6); //Disable ram power save mode 
	Lcd_WriteData(0x00); 
	
	Lcd_WriteIndex(0x3A); //65k mode 
	Lcd_WriteData(0x05); 
	
	Lcd_WriteIndex(0x29);//Display on    
}


void LCD_Fill(unsigned int Data)
{
    	u8 VH,VL;
	u16 i,j;
	VH=Data>>8;
	VL=Data;	
	Address_set(0,0,LCD_X_MAX-1,LCD_Y_MAX-1);
        
        for(i=0;i<LCD_X_MAX;i++)
	{
	  for (j=0;j<LCD_Y_MAX;j++)
	   {
            LCD_DataWrite(VH);
	    LCD_DataWrite(VL);	
	   }
	}
}


//========================================================================
// 函数: void Write_Dot_LCD(int x,int y,unsigned int i)
// 描述: 在LCD的真实坐标系上的X、Y点绘制填充色为i的点
// 参数: x 		X轴坐标
//		 y 		Y轴坐标
//		 i 		要填充的点的颜色 
// 返回: 无
// 备注: 
// 版本:
//      2007/04/10      First version
//========================================================================
void Write_Dot_LCD(unsigned char x,unsigned int y,unsigned short i)
{
  Address_set(x,y,x,y);//设置光标位置   
  LCD_DataWrite16Bit(i);
}


void Gui_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
  unsigned char i,j;
  unsigned short k,x0;
  x0=x;

  while(*s) 
  {	
    if((*s) < 128) 
    {
      k=*s;
      if (k == 13) 
      {
	  x=x0;
	  y+=16;
      } else {
	if (k>32)
          k-=32; 
        else k=0;
	
	for(i=0;i<16;i++)
	  for(j=0;j<8;j++) 
	  {
	    if(Asii0816[k*16+i]&(0x80>>j))	
              Write_Dot_LCD(x+j,y+i,fc);
	    else 
	    {
	      if (fc!=bc) 
                Write_Dot_LCD(x+j,y+i,bc);
	    }
	  }
	x+=8;
      }
	s++;
    }
		else 
		{
#if 0
			for (k=0;k<hz16_num;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
								else {
									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
								}
							}
				    }
				}
			  }
			s+=2;x+=16;
#endif
		} 
 	
	}

}



#endif




#if ST7735S

//========================================================================
// 函数: void LCD_DataWrite(unsigned int Data)
// 描述: 写一个字节的显示数据至LCD中的显示缓冲RAM当中
// 参数: Data 写入的数据 
// 返回: 无
// 备注: 无
// 版本:
//      2007/01/09      First version
//========================================================================
void LCD_DataWrite(unsigned char u8_data)
{
  unsigned char Num;
  LCD_CS(0);
  LCD_A0(1);
            
  for(Num=0;Num<8;Num++)
  {
    if((u8_data&0x80) == 0)	
    {
      LCD_SDI(0);
    } else {
      LCD_SDI(1);
    }
    u8_data = u8_data << 1;
    LCD_CLK(0);
    LCD_CLK(1);
  }
  LCD_CS(1);
}



void LCD_DataWrite16Bit(unsigned short u16_data)
{
  // LCD_DataWrite(u16_data>>8);
  // LCD_DataWrite(u16_data);
    
  unsigned char Num;
  LCD_CS(0);
  LCD_A0(1);
            
  for(Num=0;Num<16;Num++)
  {
    if((u16_data&0x8000) == 0)	
    {
      LCD_SDI(0);
    } else {
      LCD_SDI(1);
    }
    u16_data = u16_data << 1;
    LCD_CLK(0);
    LCD_CLK(1);
  }
   
  LCD_CS(1);
}


//========================================================================
// 函数: void LCD_RegWrite(unsigned char Command)
// 描述: 写一个字节的数据至LCD中的控制寄存器当中
// 参数: Command		写入的数据，低八位有效（byte） 
// 返回: 无
// 备注: 
// 版本:
// 2007/01/09      First version
//========================================================================
void LCD_RegWrite(unsigned char Command)
{
	unsigned char Num;
	LCD_CS(0);//LCD_CS(0);//LCD_CS = 0;
	LCD_A0(0);//LCD_A0(0);//LCD_A0 = 0;
        
	for(Num=0;Num<8;Num++)
	{
		if((Command&0x80) == 0)	
                {
                  LCD_SDI(0);//LCD_SDI(0);//LCD_SDI = 0;
                }
		else 
                {
                 LCD_SDI(1);//LCD_SDI(1);//LCD_SDI = 1;
                }
		Command = Command << 1;
		LCD_CLK(0);//LCD_CLK(0);//LCD_CLK = 0;
		LCD_CLK(1);//LCD_CLK(1);//LCD_CLK = 1;
	}
	LCD_CS(1);//LCD_CS(1);//LCD_CS = 1;
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_WR_REG(0x2a);
        LCD_DataWrite16Bit(x1 + 0x02);
        LCD_DataWrite16Bit(x2 + 0x02);
	LCD_WR_REG(0x2b);
        LCD_DataWrite16Bit(y1 + 0x03);
        LCD_DataWrite16Bit(y2 + 0x03);
	LCD_WR_REG(0x2c);
}




#define LCD_WriteCommand  LCD_RegWrite
#define LCD_WriteData   LCD_DataWrite

/*
  LCD_RES(0);
  DelayNms(20);
  LCD_RES(1);
  DelayNms(120);
  LCD_RegWrite(0x11); //Sleep out
  DelayNms(20);	
	//------------------------------------ST7735S Frame Rate-----------------------------------------//

	LCD_RegWrite(0xB1);
	LCD_DataWrite(0x05);
	LCD_DataWrite(0x3C);
	LCD_DataWrite(0x3C);
	LCD_RegWrite(0xB2);
	LCD_DataWrite(0x05);
	LCD_DataWrite(0x3C);
	LCD_DataWrite(0x3C);
	LCD_RegWrite(0xB3);
	LCD_DataWrite(0x05);
	LCD_DataWrite(0x3C);
	LCD_DataWrite(0x3C);
	LCD_DataWrite(0x05);
	LCD_DataWrite(0x3C);
	LCD_DataWrite(0x3C);
	
	//------------------------------------End ST7735S Frame Rate-----------------------------------------//
	LCD_RegWrite(0xB4); //Dot inversion
	LCD_DataWrite(0x03);
	LCD_RegWrite(0xC0);
	LCD_DataWrite(0x28);
	LCD_DataWrite(0x08);
	LCD_DataWrite(0x04);
	LCD_RegWrite(0xC1);
	LCD_DataWrite(0XC0);
	LCD_RegWrite(0xC2);
	LCD_DataWrite(0x0D);
	LCD_DataWrite(0x00);
	LCD_RegWrite(0xC3);
	LCD_DataWrite(0x8D);
	LCD_DataWrite(0x2A);
	LCD_RegWrite(0xC4);
	LCD_DataWrite(0x8D);
	LCD_DataWrite(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	LCD_RegWrite(0xC5); //VCOM
	LCD_DataWrite(0x1A);
	LCD_RegWrite(0x36); //MX, MY, RGB mode
	LCD_DataWrite(0xC0);
	//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
	LCD_RegWrite(0xE0);
	LCD_DataWrite(0x04);
	LCD_DataWrite(0x22);
	LCD_DataWrite(0x07);
	LCD_DataWrite(0x0A);
	LCD_DataWrite(0x2E);
	LCD_DataWrite(0x30);
	LCD_DataWrite(0x25);
	LCD_DataWrite(0x2A);
	LCD_DataWrite(0x28);
	LCD_DataWrite(0x26);
	LCD_DataWrite(0x2E);
	LCD_DataWrite(0x3A);
	LCD_DataWrite(0x00);
	LCD_DataWrite(0x01);
	LCD_DataWrite(0x03);
	LCD_DataWrite(0x13);
	LCD_RegWrite(0xE1);
	LCD_DataWrite(0x04);
	LCD_DataWrite(0x16);
	LCD_DataWrite(0x06);
	LCD_DataWrite(0x0D);
	LCD_DataWrite(0x2D);
	LCD_DataWrite(0x26);
	LCD_DataWrite(0x23);
	LCD_DataWrite(0x27);
	LCD_DataWrite(0x27);
	LCD_DataWrite(0x25);
	LCD_DataWrite(0x2D);
	LCD_DataWrite(0x3B);
	LCD_DataWrite(0x00);
	LCD_DataWrite(0x01);
	LCD_DataWrite(0x04);
	LCD_DataWrite(0x13);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
	LCD_RegWrite(0x3A); //65k mode
	LCD_DataWrite(0x05);
	LCD_RegWrite(0x29); //Display on
*/


void LCD_Init(void)
{
  LCD_RES(0);
  DelayNms(20);
  LCD_RES(1);
  DelayNms(100);
  LCD_RegWrite(0x11); //Sleep out
  DelayNms(20);

  DelayNms(120);
  LCD_RegWrite(0x11); //Sleep out
  DelayNms(20);


#if 0
	//STM32F103ZET6 用此版本 没有问题
  //------------------------------------ST7735S Frame rate-------------------------------------------------//
	LCD_WriteCommand(0xB1); //Frame rate 80Hz
	LCD_WriteData(0x02);
	LCD_WriteData(0x35);
	LCD_WriteData(0x36);
	LCD_WriteCommand(0xB2); //Frame rate 80Hz
	LCD_WriteData(0x02);
	LCD_WriteData(0x35);
	LCD_WriteData(0x36);
	LCD_WriteCommand(0xB3); //Frame rate 80Hz
	LCD_WriteData(0x02);
	LCD_WriteData(0x35);
	LCD_WriteData(0x36);
	LCD_WriteData(0x02);
	LCD_WriteData(0x35);
	LCD_WriteData(0x36);
#else
	LCD_RegWrite(0xB1);
	LCD_DataWrite(0x05);
	LCD_DataWrite(0x3C);
	LCD_DataWrite(0x3C);
	LCD_RegWrite(0xB2);
	LCD_DataWrite(0x05);
	LCD_DataWrite(0x3C);
	LCD_DataWrite(0x3C);
	LCD_RegWrite(0xB3);
	LCD_DataWrite(0x05);
	LCD_DataWrite(0x3C);
	LCD_DataWrite(0x3C);
	LCD_DataWrite(0x05);
	LCD_DataWrite(0x3C);
	LCD_DataWrite(0x3C);
#endif

//------------------------------------End ST7735S Frame rate-------------------------------------------//
LCD_WriteCommand(0xB4); //Dot inversion
LCD_WriteData(0x03);
//------------------------------------ST7735S Power Sequence-----------------------------------------//
LCD_WriteCommand(0xC0);
LCD_WriteData(0xA2);
LCD_WriteData(0x02);
LCD_WriteData(0x84);
LCD_WriteCommand(0xC1);
LCD_WriteData(0xC5);
LCD_WriteCommand(0xC2);
LCD_WriteData(0x0D);
LCD_WriteData(0x00);
LCD_WriteCommand(0xC3);
LCD_WriteData(0x8D);
LCD_WriteData(0x2A);
LCD_WriteCommand(0xC4);
LCD_WriteData(0x8D);
LCD_WriteData(0xEE);
//---------------------------------End ST7735S Power Sequence---------------------------------------//
LCD_WriteCommand(0xC5); //VCOM
LCD_WriteData(0x0a);
LCD_WriteCommand(0x36); //MX, MY, RGB mode
LCD_WriteData(0xC8);
//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
LCD_WriteCommand(0XE0);
LCD_WriteData(0x12);
LCD_WriteData(0x1C);
LCD_WriteData(0x10);
LCD_WriteData(0x18);
LCD_WriteData(0x33);
LCD_WriteData(0x2C);
LCD_WriteData(0x25);
LCD_WriteData(0x28);
LCD_WriteData(0x28);
LCD_WriteData(0x27);
LCD_WriteData(0x2F);
LCD_WriteData(0x3C);
LCD_WriteData(0x00);
LCD_WriteData(0x03);
LCD_WriteData(0x03);
LCD_WriteData(0x10);
LCD_WriteCommand(0XE1);
LCD_WriteData(0x12);
LCD_WriteData(0x1C);
LCD_WriteData(0x10);
LCD_WriteData(0x18);
LCD_WriteData(0x2D);
LCD_WriteData(0x28);
LCD_WriteData(0x23);
LCD_WriteData(0x28);
LCD_WriteData(0x28);
LCD_WriteData(0x26);
LCD_WriteData(0x2F);
LCD_WriteData(0x3B);
LCD_WriteData(0x00);
LCD_WriteData(0x03);
LCD_WriteData(0x03);
LCD_WriteData(0x10);
//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
LCD_WriteCommand(0x3A); //65k mode
LCD_WriteData(0x05);
LCD_WriteCommand(0x29); //Display on 

}


void LCD_Fill(unsigned int Data)
{
  u16 i,j;	
  Address_set(0,0,LCD_X_MAX-1,LCD_Y_MAX-1);//设置光标位置
  for(i=0;i<LCD_X_MAX;i++)
  {
    for (j=0;j<LCD_Y_MAX;j++)
    {
      LCD_DataWrite16Bit(Data);
    }
  }
}


void LCD_Image()
{
  u16 i,j,k;
  k = 0;	
  Address_set(0,0,LCD_X_MAX-1,LCD_Y_MAX-1);//设置光标位置
  for(i=0;i<LCD_X_MAX;i++)
  {
    for (j=0;j<LCD_Y_MAX;j++)
    {
        LCD_DataWrite(gImage_logo[k++]);
        LCD_DataWrite(gImage_logo[k++]);
    }
  }
}



//========================================================================
// 函数: void Write_Dot_LCD(int x,int y,unsigned int i)
// 描述: 在LCD的真实坐标系上的X、Y点绘制填充色为i的点
// 参数: x 		X轴坐标
//		 y 		Y轴坐标
//		 i 		要填充的点的颜色 
// 返回: 无
// 备注: 
// 版本:
//      2007/04/10      First version
//========================================================================
void Write_Dot_LCD(unsigned char x,unsigned int y,unsigned short i)
{
  Address_set(x,y,x,y);//设置光标位置
  LCD_DataWrite16Bit(i);
}


void Gui_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
  unsigned char i,j;
  unsigned short k,x0;
  x0=x;

  while(*s) 
  {	
    if((*s) < 128) 
    {
      k=*s;
      if (k == 13) 
      {
	  x=x0;
	  y+=16;
      } else {
	if (k>32)
          k-=32; 
        else k=0;
	
	for(i=0;i<16;i++)
	  for(j=0;j<8;j++) 
	  {
	    if(Asii0816[k*16+i]&(0x80>>j))	
              Write_Dot_LCD(x+j,y+i,fc);
	    else 
	    {
	      if (fc!=bc) 
                Write_Dot_LCD(x+j,y+i,bc);
	    }
	  }
	x+=8;
      }
	s++;
    }
		else 
		{
#if 0
			for (k=0;k<hz16_num;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
								else {
									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
								}
							}
				    }
				}
			  }
			s+=2;x+=16;
#endif
		} 
 	
	}

}


#endif


#if S6D0144

void LCD_Init(void)
{
   CPU_LCDPowerOn( );
   LCD_Fill(LCD_INITIAL_COLOR);  
}

//========================================================================
// 函数: void Write_Dot_LCD(int x,int y,unsigned int i)
// 描述: 在LCD的真实坐标系上的X、Y点绘制填充色为i的点
// 参数: x 		X轴坐标
//		 y 		Y轴坐标
//		 i 		要填充的点的颜色 
// 返回: 无
// 备注: 
// 版本:
//      2007/04/10      First version
//========================================================================
void Write_Dot_LCD(unsigned char x,unsigned int y,unsigned int i)
{
	unsigned int yx,z;
	
	z = y<<8;
	yx = x+ z;

        LCD_RegWrite(YX_ADDR,yx);
	    
	LCD_DataWrite(i);
}


//========================================================================
// 函数: void Set_Dot_Addr_LCD(int x,int y)
// 描述: 设置在LCD的真实坐标系上的X、Y点对应的RAM地址
// 参数: x 		X轴坐标
//		 y 		Y轴坐标
// 返回: 无
// 备注: 仅设置当前操作地址，为后面的连续操作作好准备
// 版本:
//      2007/04/10      First version
//========================================================================
void Set_Dot_Addr_LCD(int x,int y)
{
	unsigned int yx;
	yx = x+ (y<<8);

        LCD_RegWrite(YX_ADDR,yx);

	LCD_Reg22();
}


//========================================================================
// 函数: void LCD_Reg22(void)
// 描述: MzT24-1 LCD模块当中，依据控制器SPFD5408的特性，将操作寄存器指向数
//		据寄存器R22h当中，以便于后面的数据连续写操作
// 参数:
// 返回: 无
// 备注: 
// 版本:
//      2007/03/15      First version
//========================================================================
void LCD_Reg22(void)
{
	LCD_RS(0);
	LCD_CS(0);
	LCD_WR(0);
	LCD_RD(1);
	LCD_Data_BUS_Set(0x00);
	LCD_WR(1);
	LCD_WR(0);
	LCD_Data_BUS_Set(0x22);
	LCD_WR(1);
	LCD_CS(1);
}


//========================================================================
// 函数: void Fill_Dot_LCD(unsigned int Color)
// 描述: 填充一个点到LCD显示缓冲RAM当中，而不管当前要填充的地址如何
// 参数: Color 		要填充的点的颜色 
// 返回: 无
// 备注: 
// 版本:
//      2007/04/10      First version
//========================================================================
void Fill_Dot_LCD(unsigned short Color)
{
	LCD_RS(1);
	LCD_CS(0);
	LCD_WR(0);
	LCD_RD(1);
	LCD_Data_BUS_Set(Color>>8);
	LCD_WR(1);
	LCD_WR(0);
	LCD_Data_BUS_Set((unsigned char)(Color&0x00ff));
	LCD_WR(1);
	LCD_CS(1);
}

//========================================================================
// 函数: void LCD_Fill(unsigned int Data)
// 描述: 会屏填充以Data的数据至各点中
// 参数: Data   要填充的颜色数据
// 返回: 无
// 备注: 仅在LCD初始化程序当中调用
// 版本:
//      2006/10/15      First version
//========================================================================
void LCD_Fill(unsigned int Data)
{
  unsigned int  i;
  unsigned char j;
  Set_Dot_Addr_LCD(0,0);
  for(i=0;i<LCD_X_MAX;i++)
  {
    for(j=0;j<LCD_Y_MAX;j++)
    {
      Fill_Dot_LCD(Data);
    }
  }
}


void CPU_LCDPowerOn(void)
{
  SetLcdReg_Cpu(0x00,0x0001);
  delay_nms(10);
  
  Cpu_Initializing_Function_1 ();
  CPU_Power_Setting_Function  ();
  CPU_Initializing_Function_2 ();
  CPU_Display_On_Function ();
  
  SetLcdReg_Cpu(0x21,0x0000);
}

void Cpu_Initializing_Function_1 (void)
{ 
  SetLcdReg_Cpu(0x01,0x0114);
  //SetLcdReg_Cpu(0x02,0x0000);
  SetLcdReg_Cpu(0x02,0x0100);
  SetLcdReg_Cpu(0x03,0x0030);
  //SetLcdReg_Cpu(0x03,0x1030);
  SetLcdReg_Cpu(0x07,0x0000);
  SetLcdReg_Cpu(0x08,0x0302);
  //SetLcdReg_Cpu(0x0B,0x0005);
  SetLcdReg_Cpu(0x0B,0x0000);
  SetLcdReg_Cpu(0x0C,0x0002);
  SetLcdReg_Cpu(0x21,0X0000);// SetLcdReg_Cpu(0x21,S_R21);
 
  SetLcdReg_Cpu(0x40,0x0000);
  SetLcdReg_Cpu(0x42,0x9F00);
  SetLcdReg_Cpu(0x43,0x9F00);
  SetLcdReg_Cpu(0x44,0x7F00);
  SetLcdReg_Cpu(0x45,0x9F00);

  SetLcdReg_Cpu(0x61,0x0018);
  SetLcdReg_Cpu(0x69,0x0000);
  SetLcdReg_Cpu(0xB4,0x0010);
 }

void  CPU_Power_Setting_Function (void)
{ 
 SetLcdReg_Cpu(0x12,0x0000);
 SetLcdReg_Cpu(0x11,0x0210);
 SetLcdReg_Cpu(0x13,0x080C);
 delay_nms(40);
 SetLcdReg_Cpu(0x11,0x0210);
 
 delay_nms(40);
 SetLcdReg_Cpu(0x12,0x0071);
 SetLcdReg_Cpu(0x13,0x081D);
 delay_nms(40);
 SetLcdReg_Cpu(0x10,0x1910);
 delay_nms(40);
 SetLcdReg_Cpu(0x14,0x7FE5);
 delay_nms(96);
}

void CPU_Initializing_Function_2 (void)
{ 
                                          ;
 SetLcdReg_Cpu (0x0030,0x0700)             ;// Gamma Control1, KP1KP0 
 SetLcdReg_Cpu (0x0031,0x0307)             ;// Gamma Control2, KP3KP2
 SetLcdReg_Cpu (0x0032,0x0100)             ;// Gamma Control3, KP5KP4
 SetLcdReg_Cpu (0x0033,0x0000)             ;// Gamma Control4, RP1RP0
 SetLcdReg_Cpu (0x0034,0x0605)             ;// Gamma Control5, VRP1VRP0
 SetLcdReg_Cpu (0x0035,0x0104)             ;// Gamma Control6, KN1KN0
 SetLcdReg_Cpu (0x0036,0x0707)            ;// Gamma Control7, KN3KN2
 SetLcdReg_Cpu (0x0037,0x0000)             ;// Gamma Control8, KN5KN4
 SetLcdReg_Cpu (0x0038,0x000D)             ;// Gamma Control9, RN1RN0
 SetLcdReg_Cpu (0x0011,0x0004)              ;  
}

void CPU_Display_On_Function(void)
{
  SetLcdReg_Cpu(0x07,0x0001);
  delay_nms(40);
  SetLcdReg_Cpu(0x07,0x0021);
  //delay_nms(40);
  SetLcdReg_Cpu(0x07,0x0023);
  delay_nms(40);
  SetLcdReg_Cpu(0x07,0x0037);
  delay_nms(10);
}

//========================================================================
// 函数: void LCD_DataWrite(unsigned char Data)
// 描述: 写一个字的显示数据至LCD中的显示缓冲RAM当中
// 参数: Data 写入的数据 
// 返回: 无
// 备注: 无
// 版本:
//      2007/04/10      First version
//========================================================================
void LCD_DataWrite(unsigned int Data)
{
	LCD_RS(0);
	LCD_CS(0);
	LCD_WR(0);
	LCD_RD(1);
	LCD_Data_BUS_Set(0x00);
	LCD_WR(1);
	LCD_WR(0);
	LCD_Data_BUS_Set(0x22);
	LCD_WR(1);
	LCD_RS(1);
	LCD_WR(0);
	LCD_Data_BUS_Set(Data>>8);
	LCD_WR(1);
	LCD_WR(0);
	LCD_Data_BUS_Set((unsigned char)(Data&0x00ff));
        LCD_WR(1);
        LCD_CS(1);
}

//========================================================================
// 函数: void LCD_RegWrite(unsigned char Reg_Addr,unsigned int Data)
// 描述: 写一个字节的数据至LCD中的控制寄存器当中
// 参数: Reg_Addr 	寄存器地址
//		 Data 		写入的数据 
// 返回: 无
// 备注: 
// 版本:
//      2007/04/10      First version
//========================================================================
void LCD_RegWrite(unsigned char Reg_Addr,unsigned int Data)
{
	LCD_RS(0);
	LCD_CS(0);
	LCD_WR(0);
	LCD_RD(1);
	LCD_Data_BUS_Set(0x00);
	LCD_WR(1);
	LCD_WR(0);
	LCD_Data_BUS_Set(Reg_Addr);
	LCD_WR(1);
	LCD_RS(1);
	LCD_WR(0);
	LCD_Data_BUS_Set(Data>>8); //Lcd_Data_H( data1/256);

	LCD_WR(1);
	LCD_WR(0);
	LCD_Data_BUS_Set((unsigned char)(Data&0x00ff));	//Lcd_Data_H( data1%256);
	LCD_WR(1);
	LCD_CS(1);
}


#endif















