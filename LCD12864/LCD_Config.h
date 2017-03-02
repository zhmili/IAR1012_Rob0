#ifndef LCD_CONFIG_H
#define LCD_CONFIG_H


//画笔颜色
#define LCD_INITIAL_COLOR	0xFFFF			//定义LCD屏初始化时的背景色
#define WHITE        	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 	         0XFFE0
#define GBLUE	         0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 		 0XBC40 //棕色
#define BRRED 		 0XFC07 //棕红色
#define GRAY  		 0X8430 //灰色

#if ILI9163C


#define LCD_X_MAX			128			//屏幕的X轴的物理宽度
#define LCD_Y_MAX			128			//屏幕的Y轴的物理宽度
#define LCD_XY_Switch		        0		        //显示时X轴和Y由交换
#define LCD_X_Rev			0			//显示时X轴反转
#define LCD_Y_Rev			0			//显示时Y轴反转

#if LCD_XY_Switch == 0
	#define Dis_X_MAX		LCD_X_MAX
	#define Dis_Y_MAX		LCD_Y_MAX	
#endif

#if LCD_XY_Switch == 1
	#define Dis_X_MAX		LCD_Y_MAX
	#define Dis_Y_MAX		LCD_X_MAX	
#endif

#endif


#if ST7735R
#define LCD_X_MAX			128			//屏幕的X轴的物理宽度
#define LCD_Y_MAX			128			//屏幕的Y轴的物理宽度
#define LCD_XY_Switch		        0		        //显示时X轴和Y由交换
#define LCD_X_Rev			0			//显示时X轴反转
#define LCD_Y_Rev			0			//显示时Y轴反转

#if LCD_XY_Switch == 0
	#define Dis_X_MAX		LCD_X_MAX
	#define Dis_Y_MAX		LCD_Y_MAX	
#endif

#if LCD_XY_Switch == 1
	#define Dis_X_MAX		LCD_Y_MAX
	#define Dis_Y_MAX		LCD_X_MAX	
#endif

#endif


#if ST7735S

#define LCD_X_MAX			128			//屏幕的X轴的物理宽度
#define LCD_Y_MAX			128			//屏幕的Y轴的物理宽度
#define LCD_XY_Switch		        0		        //显示时X轴和Y由交换
#define LCD_X_Rev			0			//显示时X轴反转
#define LCD_Y_Rev			0			//显示时Y轴反转

#if LCD_XY_Switch == 0
	#define Dis_X_MAX		LCD_X_MAX
	#define Dis_Y_MAX		LCD_Y_MAX	
#endif

#if LCD_XY_Switch == 1
	#define Dis_X_MAX		LCD_Y_MAX
	#define Dis_Y_MAX		LCD_X_MAX	
#endif

#endif



#if S6D0144

#define LCD_X_MAX			128-1			//屏幕的X轴的物理宽度
#define LCD_Y_MAX			160-1			//屏幕的Y轴的物理宽度
#define LCD_XY_Switch		        0		        //显示时X轴和Y由交换
#define LCD_X_Rev			0			//显示时X轴反转
#define LCD_Y_Rev			0			//显示时Y轴反转

#if LCD_XY_Switch == 0
	#define Dis_X_MAX		LCD_X_MAX
	#define Dis_Y_MAX		LCD_Y_MAX	
#endif
#if LCD_XY_Switch == 1
	#define Dis_X_MAX		LCD_Y_MAX
	#define Dis_Y_MAX		LCD_X_MAX	
#endif

#define LCD_INITIAL_COLOR	0x00			//定义LCD屏初始化时的背景色

#endif



#endif
