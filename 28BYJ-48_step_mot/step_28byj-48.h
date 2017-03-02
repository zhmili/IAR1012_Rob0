#ifndef __STEP_28BYJ_48_H
#define __STEP_28BYJ_48_H

#define STEP_LLOW  500
#define STEP_ML0W  1000
#define STEP_M     1500
#define STEP_MM    2000
#define STEP_H     2500

#define DSTEP_LLOW  35
#define DSTEP_ML0W  (DSTEP_LLOW - 5)   //30
#define DSTEP_M     (DSTEP_ML0W -5)    //25
#define DSTEP_MM    (DSTEP_M - 5)      //20
#define DSTEP_H     (DSTEP_MM - 5)     //15

void step_init(float wh_r_mm);
void out(unsigned char ctdt);
void double_four_step(int dir_op0_p1, u16 dly_us);
void run_dist(u32 mm, int dir);

#endif

