#ifndef LCD_DRIVER_USER_H
#define LCD_DRIVER_USER_H


void LCD_Init(void);

void LCD_DataWrite16Bit(unsigned short u16_data);
void LCD_DataWrite(unsigned char u8_data);
void Gui_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s);

#if S6D0144

#define YX_ADDR  0x21
void CPU_LCDPowerOn(void);
void Cpu_Initializing_Function_1 (void);
void CPU_Power_Setting_Function (void);
void CPU_Initializing_Function_2 (void);
void CPU_Display_On_Function(void);
void LCD_DataWrite(unsigned int Data);
void LCD_Reg22(void);
void Fill_Dot_LCD(unsigned short Color);
void LCD_Fill(unsigned int a);
void Write_Dot_LCD(unsigned char x,unsigned int y,unsigned int i);
void LCD_RegWrite(unsigned char Reg_Addr,unsigned int Data);
void LCD_Image(void);

#endif

void TimeDelay(int Time);
#endif
