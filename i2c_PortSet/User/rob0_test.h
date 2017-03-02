
#ifndef __ROB0_TEST_H
#define __ROB0_TEST_H

void scan_sim(int h_num, int l_num);
void test_sim_forward(void);
void test_sim_reverse(void);
void judge_test(void);
void digital_out_test(int cha);
void analog_test(int cha);
void digital_test(int cha);
void button_test(void);
void servo_test(int cha);
void moto1_test(int cha);
void moto2_test(int cha);	
void test_all(void);
#endif

