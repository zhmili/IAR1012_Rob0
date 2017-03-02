
#include "includes.h"

unsigned char  d_ccw[4]={0x0c,0x06,0x03,0x09};//1100 0110 0011 1001 
GPIO_TypeDef* pport[4];
u16 pin[4];
u16 wheel_r_mm;

float pi = 3.1416;
float cyc_mm ;

float step_angle = 5.625;
u8    ratio = 64;
float steps_per_360d;             //运行360度 需要多少步
float steps_per_mm;               //运行1mm 需要多少步
float step_m4_per_mm;             //

//600 转 每分钟 ，步进电机极限    600*4096 - 60s 
//10*4096 - s -> 40960step - s = 0.0244 ms -> 2.4us 
//极限情况下 间隔不低于2.4us
//
//
void step_init(float wh_r_mm)
{
#if ROB_0 == 0

  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin     =  PS_DO_PIN|PS_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin     =  PS_DI_PIN;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin     =  PS_CS_PIN;
  GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode 		=  GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  
  pport[0] = GPIOB;
  pport[1] = GPIOB;
  pport[2] = GPIOB;
  pport[3] = GPIOC;
  pin[0] = DEF_BIT_13;
  pin[1] = DEF_BIT_14; 
  pin[2] = DEF_BIT_15; 
  pin[3] = DEF_BIT_09; 
#endif
  wheel_r_mm = wh_r_mm;
  
  cyc_mm = 2*pi*wheel_r_mm;
  
  steps_per_360d = 360/(step_angle/ratio);
  
  steps_per_mm = steps_per_360d/cyc_mm;
  
  step_m4_per_mm = steps_per_mm/4;
}


void out(unsigned char ctdt)
{
  int i; 
  int x;
  for(i = 0; i < 4; i++)
  {
    x = ((ctdt>>i)&(0x01));
    x?(pport[i]->BSRR = pin[i]):(pport[i]->BRR = pin[i]);
  }
}

//4*5.625 = 22.5 
//22.5 / 64 = 0.3515625 drg
//
void double_four_step(int dir_op0_p1, u16 dly_us)
{
  int i;
  for(i = 0; i < 4; i++)
  {
    if(dir_op0_p1)
    {
    out(~d_ccw[i]);
    }
    else
    {
    out(~d_ccw[3-i]);
    }
    DelayNus(dly_us);
  }
}


void run_dist(u32 mm, int dir)
{
  u32 step_m4;
  u32 i;
  step_m4 = mm/step_m4_per_mm;
  
  for(i = 0; i < step_m4; i++)
  {
#if 0
    if(step_m4 < STEP_LLOW)
    {
    double_4_step(dir, DSTEP_LLOW);
    }
    else if(step_m4 < STEP_ML0W)
    {
    double_4_step(dir, DSTEP_ML0W);
    }
    else if(step_m4 < STEP_M)
    {
    double_4_step(dir, DSTEP_M);
    }
    else if(step_m4 < STEP_MM)
    {
    double_4_step(dir, DSTEP_MM);
    }
    else if(step_m4 < STEP_H)
    {
    double_4_step(dir, DSTEP_H);
    }
#else
    if(step_m4 < STEP_LLOW)
    {
    double_4_step(dir, DSTEP_LLOW);
    }
    else if(step_m4 < STEP_M)
    {
    double_4_step(dir, DSTEP_M);
    }
    else if(step_m4 < STEP_H)
    {
    double_4_step(dir, DSTEP_H);
    }
#endif    
  }
}






