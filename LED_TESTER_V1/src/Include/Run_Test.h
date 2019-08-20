/*
 * Run_Test.h
 *
 *  Created on: Aug 19, 2019
 *      Author: michal
 */

#ifndef INCLUDE_RUN_TEST_H_
#define INCLUDE_RUN_TEST_H_

float lux_run_read(void);
void GPIO_init(void);
void interface_rt(void);
void lux_avg(float *LuxAvg);
void Run_Test(void);


#endif /* INCLUDE_RUN_TEST_H_ */
