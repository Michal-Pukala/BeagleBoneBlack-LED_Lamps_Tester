/*
 * Temp_Time_Read.c
 *
 *  Created on: Jul 5, 2019
 *      Author: michal
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>

#include "../LCD/iolib.h"
#include "../LCD/beagle_gpio.h"
#include "../LCD/beagle_hd44780.h"

//Buttons Define
int TTRbuttonBack = 41;
int TTRbuttonEnter = 17;
int TTRbuttonPrev = 15;
int TTRbuttonNext = 16;

//LCD Define
struct gpioID enabled_gpio[6];

void interface(void)
{
	clear_Screen(enabled_gpio);
	//LCD Line 1
	goto_ScreenLine(0, enabled_gpio);
	stringToScreen(" SET TIME INTERVAL", enabled_gpio);
	//LCD Line 2
	goto_ScreenLine(1, enabled_gpio);
	stringToScreen("Minutes:", enabled_gpio);
	//LCD Line 3
	goto_ScreenLine(2, enabled_gpio);
	stringToScreen("Hours:", enabled_gpio);
	//LCD Line 4
	goto_ScreenLine(3, enabled_gpio);
	stringToScreen("Days:", enabled_gpio);
}

void enter_time_values(void)
{
	//Define Pointers
	char pc1[4];
	sprintf(pc1," ^^");
	//Define Numbers
	char* c_mins[2];
	char* c_hours[2];
	char* c_days[2];
	float i_mins=0;
	float i_hours;
	float i_days;
	char interface_mins[21];
	char interface_hours[21];
	char interface_days[21];

	int mins_i, hours_i, days_i;
	int Enter_minutes=0;
	int Enter_hours=0;
	int Enter_days=0;
	int full_input=0;

while(full_input==0)
{

while(Enter_minutes==0)
{
//Enter Mins
while(is_low(8, TTRbuttonEnter))
	{

		goto_ScreenLine(1, enabled_gpio);
		stringToScreen("Minutes: >>", enabled_gpio);
		//Press Add Number
		if(is_high(8, TTRbuttonNext))
		{
			//Button Press Prevent
			while(is_high(8, TTRbuttonNext));
			if(i_mins<=59)
			{
				i_mins++;
				if(i_mins<10)
				{
					sprintf(interface_mins,"Minutes: >> %.0f ", i_mins);
					goto_ScreenLine(1, enabled_gpio);
					stringToScreen(interface_mins, enabled_gpio);
				}
				else
				{
					sprintf(interface_mins,"Minutes: >> %.0f ", i_mins);
					goto_ScreenLine(1, enabled_gpio);
					stringToScreen(interface_mins, enabled_gpio);
				}
			}
			else
			{
				i_mins=0;
				if(i_mins<10)
				{
					sprintf(interface_mins,"Minutes: >> %.0f ", i_mins);
					goto_ScreenLine(1, enabled_gpio);
					stringToScreen(interface_mins, enabled_gpio);
				}
				else
				{
					sprintf(interface_mins,"Minutes: >> %.0f ", i_mins);
					goto_ScreenLine(1, enabled_gpio);
					stringToScreen(interface_mins, enabled_gpio);
				}
			}
		}else;

		//Press Deacrese Number
		if(is_high(8, TTRbuttonPrev))
		{
			//Button Press Prevent
			while(is_high(8, TTRbuttonPrev));
			if(i_mins <= 60 && i_mins >= 1)
			{
				i_mins--;
				if(i_mins<10)
				{
					sprintf(interface_mins,"Minutes: >> %.0f ", i_mins);
					goto_ScreenLine(1, enabled_gpio);
					stringToScreen(interface_mins, enabled_gpio);
				}
				else
				{
					sprintf(interface_mins,"Minutes: >> %.0f ", i_mins);
					goto_ScreenLine(1, enabled_gpio);
					stringToScreen(interface_mins, enabled_gpio);
				}
			}
			else
				{
				i_mins=60;
				sprintf(interface_mins,"Minutes: >> %.0f ", i_mins);
				goto_ScreenLine(1, enabled_gpio);
				stringToScreen(interface_mins, enabled_gpio);
				}
		}else;
	}
	while(is_high(8, TTRbuttonEnter))
		{
		Enter_minutes=1;
		sprintf(interface_mins,"Minutes: >> %.0f SET", i_mins);
		goto_ScreenLine(1, enabled_gpio);
		stringToScreen(interface_mins, enabled_gpio);
		}
}
	sleep(1);
	printf("\n Minutes Set");
	printf("\n Time Interval Mins: %.0f", i_mins);

while(Enter_hours==0)
{
//Enter Hours
while(is_low(8, TTRbuttonEnter))
	{
		goto_ScreenLine(2, enabled_gpio);
		stringToScreen("Hours: >>", enabled_gpio);
		//Press Add Number
		if(is_high(8, TTRbuttonNext))
		{
			//Button Press Prevent
			while(is_high(8, TTRbuttonNext));
			if(i_hours<=23)
			{
				i_hours++;
				if(i_hours<10)
				{
					sprintf(interface_hours,"Hours: >> %.0f ", i_hours);
					goto_ScreenLine(2, enabled_gpio);
					stringToScreen(interface_hours, enabled_gpio);
				}
				else
				{
					sprintf(interface_hours,"Hours: >> %.0f ", i_hours);
					goto_ScreenLine(2, enabled_gpio);
					stringToScreen(interface_hours, enabled_gpio);
				}
				}
				else
				{
					i_hours=0;
					if(i_hours<10)
					{
						sprintf(interface_hours,"Hours: >> %.0f ", i_hours);
						goto_ScreenLine(2, enabled_gpio);
						stringToScreen(interface_hours, enabled_gpio);
					}
					else
					{
						sprintf(interface_hours,"Hours: >> %.0f ", i_hours);
						goto_ScreenLine(2, enabled_gpio);
						stringToScreen(interface_hours, enabled_gpio);
					}
				}
			}else;

			//Press Deacrese Number
			if(is_high(8, TTRbuttonPrev))
			{
				//Button Press Prevent
				while(is_high(8, TTRbuttonPrev));
				if(i_hours <= 24 && i_hours >= 1)
				{
					i_hours--;
					if(i_hours<10)
					{
						sprintf(interface_hours,"Hours: >> %.0f ", i_hours);
						goto_ScreenLine(2, enabled_gpio);
						stringToScreen(interface_hours, enabled_gpio);
					}
					else
					{
						sprintf(interface_hours,"Hours: >> %.0f ", i_hours);
						goto_ScreenLine(2, enabled_gpio);
						stringToScreen(interface_hours, enabled_gpio);
					}
				}
				else
					{
					i_hours=24;
					sprintf(interface_hours,"Hours: >> %.0f ", i_hours);
					goto_ScreenLine(2, enabled_gpio);
					stringToScreen(interface_hours, enabled_gpio);
					}
			}else;
		}
		while(is_high(8, TTRbuttonEnter))
			{
			Enter_hours=1;
			sprintf(interface_hours,"Hours: >> %.0f SET", i_hours);
			goto_ScreenLine(2, enabled_gpio);
			stringToScreen(interface_hours, enabled_gpio);
			}
	}

	sleep(1);
	printf("\n Hours SET");
	printf("\n Time Interval Mins: %.0f", i_mins);
	printf("\n Time Interval Hours: %.0f", i_hours);

	while(Enter_days==0)
	{
	//Enter Days
	while(is_low(8, TTRbuttonEnter))
		{
			goto_ScreenLine(3, enabled_gpio);
			stringToScreen("Days: >>", enabled_gpio);
			//Press Add Number
			if(is_high(8, TTRbuttonNext))
			{
				//Button Press Prevent
				while(is_high(8, TTRbuttonNext));
				if(i_days<=29)
				{
					i_days++;
					if(i_days<10)
					{
						sprintf(interface_days,"Days: >> %.0f ", i_days);
						goto_ScreenLine(3, enabled_gpio);
						stringToScreen(interface_days, enabled_gpio);
					}
					else
					{
						sprintf(interface_days,"Days: >> %.0f ", i_days);
						goto_ScreenLine(3, enabled_gpio);
						stringToScreen(interface_days, enabled_gpio);
					}
					}
					else
					{
						i_days=0;
						if(i_days<10)
						{
							sprintf(interface_days,"Days: >> %.0f ", i_days);
							goto_ScreenLine(3, enabled_gpio);
							stringToScreen(interface_days, enabled_gpio);
						}
						else
						{
							sprintf(interface_days,"Days: >> %.0f ", i_days);
							goto_ScreenLine(3, enabled_gpio);
							stringToScreen(interface_days, enabled_gpio);
						}
					}
				}else;

				//Press Deacrese Number
				if(is_high(8, TTRbuttonPrev))
				{
					//Button Press Prevent
					while(is_high(8, TTRbuttonPrev));
					if(i_days <= 30 && i_days >= 1)
					{
						i_days--;
						if(i_days<10)
						{
							sprintf(interface_days,"Days: >> %.0f ", i_days);
							goto_ScreenLine(3, enabled_gpio);
							stringToScreen(interface_days, enabled_gpio);
						}
						else
						{
							sprintf(interface_days,"Days: >> %.0f ", i_days);
							goto_ScreenLine(3, enabled_gpio);
							stringToScreen(interface_days, enabled_gpio);
						}
					}
					else
						{
						i_days=30;
						sprintf(interface_days,"Days: >> %.0f ", i_days);
						goto_ScreenLine(3, enabled_gpio);
						stringToScreen(interface_days, enabled_gpio);
						}
				}else;
			}
			while(is_high(8, TTRbuttonEnter))
				{
				Enter_days=1;
				sprintf(interface_days,"Days: >> %.0f SET", i_days);
				goto_ScreenLine(3, enabled_gpio);
				stringToScreen(interface_days, enabled_gpio);
				}
		}
	sleep(1);
	full_input=1;

	printf("\n Days SET");
	printf("\n Time Interval Mins: %.0f", i_mins);
	printf("\n Time Interval Hours: %.0f", i_hours);
	printf("\n Time Interval Days: %.0f", i_days);
}

}


int8_t set_refresh_rate(char refresh_rate_act[])
{
	unsigned int full_input=0;
	unsigned int Enter_rr=0;
	unsigned int refresh_rate = 1;

	char one_s[] = "01 s";
	char ten_s[]="10 s";
	char sixty_s[]="60 s";
	char clear_s[]="   ";

	unsigned int pos=1;

	char pointer[]=" <<";
	char set[]="SET";
	char selected[8];
	char sclear[21];


	//Interface Screen
	clear_Screen(enabled_gpio);
	goto_ScreenLine(0, enabled_gpio);
	stringToScreen("  SET REFRESH RATE", enabled_gpio);
	goto_ScreenLine(1, enabled_gpio);
	stringToScreen(one_s, enabled_gpio);
	goto_ScreenLine(2, enabled_gpio);
	stringToScreen(ten_s, enabled_gpio);
	goto_ScreenLine(3, enabled_gpio);
	stringToScreen(sixty_s, enabled_gpio);
	sleep(1);
	sprintf(selected,"%s%s",one_s,pointer);
	goto_ScreenLine(1, enabled_gpio);
	stringToScreen(selected, enabled_gpio);

	//SET Loop
	while(full_input==0)
	{
	while(Enter_rr==0)
	{
	//SET confirmed
	while(is_low(8, TTRbuttonEnter))
		{
			//Press down
			if(is_high(8, TTRbuttonNext))
			{
				//Button Press Prevent
				while(is_high(8, TTRbuttonNext));

				switch (pos)
				{
				case 0:
					sprintf(selected,"%s%s",one_s,pointer);
					goto_ScreenLine(1, enabled_gpio);
					stringToScreen(selected, enabled_gpio);
					//Clear rest menu
					sprintf(sclear,"%s%s",ten_s,clear_s);
					goto_ScreenLine(2, enabled_gpio);
					stringToScreen(sclear, enabled_gpio);
					sprintf(sclear,"%s%s",sixty_s,clear_s);
					goto_ScreenLine(3, enabled_gpio);
					stringToScreen(sclear, enabled_gpio);
					pos++;
					refresh_rate = 1;
					break;
				case 1:
					sprintf(selected,"%s%s",ten_s,pointer);
					goto_ScreenLine(2, enabled_gpio);
					stringToScreen(selected, enabled_gpio);
					//Clear rest menu
					sprintf(sclear,"%s%s",one_s,clear_s);
					goto_ScreenLine(1, enabled_gpio);
					stringToScreen(sclear, enabled_gpio);
					sprintf(sclear,"%s%s",sixty_s,clear_s);
					goto_ScreenLine(3, enabled_gpio);
					stringToScreen(sclear, enabled_gpio);
					pos++;
					refresh_rate = 10;
					break;
				case 2:
					sprintf(selected,"%s%s",sixty_s,pointer);
					goto_ScreenLine(3, enabled_gpio);
					stringToScreen(selected, enabled_gpio);
					//Clear rest menu
					sprintf(sclear,"%s%s",one_s,clear_s);
					goto_ScreenLine(1, enabled_gpio);
					stringToScreen(sclear, enabled_gpio);
					sprintf(sclear,"%s%s",ten_s,clear_s);
					goto_ScreenLine(2, enabled_gpio);
					stringToScreen(sclear, enabled_gpio);
					//Reset position
					pos=0;
					refresh_rate = 60;
					break;
				}
			}
		}
	Enter_rr=1;
	}
	//Refresh Rate SET
	full_input=1;
	printf("\n Refresh rate is: %d s", refresh_rate);
	sprintf(refresh_rate_act, "%d", refresh_rate);
	}
	return(1);
}

void save_temp_readings(void)
{
	char* refresh_rate_act[21];
	int refresh_rate;

	//Set Refresh Rate
	set_refresh_rate(refresh_rate_act);
	sscanf(refresh_rate_act, "%d", &refresh_rate);


}

void temptimeread(void)
{

	//Initialize Interface
	interface();
	sleep(1);

	//Enter Time Values
	enter_time_values();
	printf("\n Times Entered");

	//Save Temperature Readings
	save_temp_readings();

}


