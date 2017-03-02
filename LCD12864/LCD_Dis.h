#ifndef LCD_DIS_H
#define LCD_DIS_H


//LCD 初始化函数，一般用于LCD屏的寄存器初始化
extern void LCD_Init(void);	//在LCD_Driver_User.c中定义的函数，如有必要该函数还会包含端口初始化、复位等操作


void Writ_Dot(int x,int y,unsigned short Color);
//画直线函数，s_x、s_y为起始点，e_x、e_y为结束点
//extern void Line(unsigned char s_x,unsigned char s_y,unsigned char e_x,unsigned char e_y);
//标准字符设置，包括两型号ASCII码的大小尺寸，以及字体颜色的设置
extern void FontSet(unsigned char Font_NUM,unsigned short Color);
//于x、y的坐标上写入一个标准字符
extern void PutChar(unsigned char x,unsigned char y,char a);
//于x、y的坐标为起始写入一串标准字符串
extern void PutString(unsigned char x,unsigned char y,unsigned char *p);
//于x、y的坐标为中心，绘制一个圆边或实心圆
//extern void PutPixel(unsigned char x,unsigned char y);
//extern void Circle(unsigned char x,unsigned char y,unsigned char r,unsigned char mode);
//绘制一个以left、top和right、bottom为两个对角的矩形框或者实心矩形
extern void Rectangle(unsigned char left,unsigned char top,unsigned char right,unsigned char bottom);
//清屏函数，执行全屏幕清除或填充前景色
void ClrScreen(unsigned short Mode);

void Put10Value(unsigned char x,unsigned char y, unsigned short value);

extern unsigned char X_Witch;					//字符写入时的宽度
extern unsigned char Y_Witch;					//字符写入时的高度
extern unsigned char Font_Wrod;				//字体的大
extern unsigned char *Char_TAB;		//字库指针
extern unsigned short BMP_Color;
extern unsigned short Char_Color;

extern void PutIntValue(unsigned char x,unsigned char y,unsigned short value);
extern void PutIntValueB(unsigned char x,unsigned char y,s32 value);
extern void Put10Value(unsigned char x,unsigned char y, unsigned short value);
void Put100Value(unsigned char x,unsigned char y, unsigned short value);
void Show_Image(const unsigned char *p);
extern void Int2Str(u8* str, s32 intnum);
#endif
