#ifndef LCD_DIS_H
#define LCD_DIS_H


//LCD ��ʼ��������һ������LCD���ļĴ�����ʼ��
extern void LCD_Init(void);	//��LCD_Driver_User.c�ж���ĺ��������б�Ҫ�ú�����������˿ڳ�ʼ������λ�Ȳ���


void Writ_Dot(int x,int y,unsigned short Color);
//��ֱ�ߺ�����s_x��s_yΪ��ʼ�㣬e_x��e_yΪ������
//extern void Line(unsigned char s_x,unsigned char s_y,unsigned char e_x,unsigned char e_y);
//��׼�ַ����ã��������ͺ�ASCII��Ĵ�С�ߴ磬�Լ�������ɫ������
extern void FontSet(unsigned char Font_NUM,unsigned short Color);
//��x��y��������д��һ����׼�ַ�
extern void PutChar(unsigned char x,unsigned char y,char a);
//��x��y������Ϊ��ʼд��һ����׼�ַ���
extern void PutString(unsigned char x,unsigned char y,unsigned char *p);
//��x��y������Ϊ���ģ�����һ��Բ�߻�ʵ��Բ
//extern void PutPixel(unsigned char x,unsigned char y);
//extern void Circle(unsigned char x,unsigned char y,unsigned char r,unsigned char mode);
//����һ����left��top��right��bottomΪ�����Խǵľ��ο����ʵ�ľ���
extern void Rectangle(unsigned char left,unsigned char top,unsigned char right,unsigned char bottom);
//����������ִ��ȫ��Ļ��������ǰ��ɫ
void ClrScreen(unsigned short Mode);

void Put10Value(unsigned char x,unsigned char y, unsigned short value);

extern unsigned char X_Witch;					//�ַ�д��ʱ�Ŀ��
extern unsigned char Y_Witch;					//�ַ�д��ʱ�ĸ߶�
extern unsigned char Font_Wrod;				//����Ĵ�
extern unsigned char *Char_TAB;		//�ֿ�ָ��
extern unsigned short BMP_Color;
extern unsigned short Char_Color;

extern void PutIntValue(unsigned char x,unsigned char y,unsigned short value);
extern void PutIntValueB(unsigned char x,unsigned char y,s32 value);
extern void Put10Value(unsigned char x,unsigned char y, unsigned short value);
void Put100Value(unsigned char x,unsigned char y, unsigned short value);
void Show_Image(const unsigned char *p);
extern void Int2Str(u8* str, s32 intnum);
#endif
