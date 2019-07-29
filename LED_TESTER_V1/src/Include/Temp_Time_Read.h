/*
 * Temp_Time_Read.h
 *
 *  Created on: Jul 5, 2019
 *      Author: michal
 */

#ifndef INCLUDE_TEMP_TIME_READ_H_
#define INCLUDE_TEMP_TIME_READ_H_

void temptimeread(void);
void enter_time_values(int *mins_ex, int *hours_ex, int *days_ex);
int8_t set_refresh_rate();
void save_temp_readings(void);

#endif /* INCLUDE_TEMP_TIME_READ_H_ */
