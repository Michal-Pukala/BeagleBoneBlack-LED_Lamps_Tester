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

struct ds18b20
	{
	char devPath[128];
	char devID[16];
	char tempData[6];
	struct ds18b20 *next;
	};

float temperatureexport(void)
	{
	struct ds18b20 *rootNode;
	struct ds18b20 *devNode;
	struct ds18b20 *getTemp;
	char* tempAct[2];
	float tempActFL;

			// Load pin configuration. Ignore error if already loaded
			system("echo w1 > /sys/devices/bone_capemgr.9/slots>/dev/null");

				devNode = rootNode;
				getTemp = rootNode;
				findDevices(devNode);
				//printf("\n Found %d devices\n\n", devCnt);
				readTemp(getTemp, tempAct);
				//Actual Temperature Float
				sscanf(tempAct, "%f", &tempActFL);

			return(tempActFL);
	}

void enter_time_values(int *mins_ex, int *hours_ex, int *days_ex)
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


	//Export Times
	*mins_ex=i_mins;
	*hours_ex=i_hours;
	*days_ex=i_days;
}

}


//char set_refresh_rate(char refresh_rate_act[3])
int8_t set_refresh_rate()
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
	}
	return(refresh_rate);
}

void save_temp_readings(void)
{
	float temperature;

	int refresh_rate;

	int mins, hours, days;

	//Enter Times
	enter_time_values(&mins, &hours, &days);
	printf("\n Mins: %d \n Hours: %d \n Days: %d", mins, hours, days);

	//Set Refresh Rate
	refresh_rate=set_refresh_rate();
	printf("\n Refresh rate entered: %d", refresh_rate);

	//Set Temperature
	temperature=temperatureexport();
	printf("\n Temperature is: %f", temperature);

	//Save Date in CSV file
	char tempsave[100];
	float count_mins;
	float count_mins_plus=0;
	int fulltime=0;

	//Sum FullTime
	fulltime=(mins+(hours*60)+(days*24*60));
	//Create File
	system("echo TEMPERATURE,DATE > /home/puka/Led_Tester_V1/TempTimeRead/Data/TempTimeRead_$(date +%F_%T).csv");

	switch (refresh_rate)
	{
	case 1:
		while(fulltime>=count_mins)
		{
			//sleep(refresh_rate);
			count_mins_plus++;
			count_mins=count_mins_plus/60;
			temperature=temperatureexport();
			sprintf(tempsave,"echo Temp:%.1f > /home/puka/Led_Tester_V1/TempTimeRead/BuffTemp.txt",temperature);
			system(tempsave);
			//Save Temp and Date
			system("/bin/bash /home/puka/Led_Tester_V1/TempTimeRead/Data/TempTimeRead.sh");
			printf("\n Temperature is: %f", temperature);
			printf("\n Counted minutes: %f", count_mins);
		}
		break;

	case 10:
	while(fulltime>=count_mins)
	{
		sleep(refresh_rate-1);
		count_mins_plus=count_mins_plus+10;
		count_mins=(count_mins_plus)/60.0;
		temperature=temperatureexport();
		sprintf(tempsave,"echo Temp:%.1f > /home/puka/Led_Tester_V1/TempTimeRead/BuffTemp.txt",temperature);
		system(tempsave);
		//Save Temp and Date
		system("/bin/bash /home/puka/Led_Tester_V1/TempTimeRead/Data/TempTimeRead.sh");
		printf("\n Temperature is: %f", temperature);
		printf("\n Counted minutes: %f", count_mins);
	}
		break;
	case 60:
	while(fulltime>=count_mins)
		{
			sleep(refresh_rate-1);
			count_mins_plus=count_mins_plus+60;
			count_mins=(count_mins_plus)/60.0;
			temperature=temperatureexport();
			sprintf(tempsave,"echo Temp:%.1f > /home/puka/Led_Tester_V1/TempTimeRead/BuffTemp.txt",temperature);
			system(tempsave);
			//Save Temp and Date
			system("/bin/bash /home/puka/Led_Tester_V1/TempTimeRead/Data/TempTimeRead.sh");
			printf("\n Temperature is: %f", temperature);
			printf("\n Counted minutes: %f", count_mins);
			}
		break;
	}
}


void temptimeread(void)
{

	//Initialize Interface
	interface();
	sleep(1);



	//Save Temperature Readings
	save_temp_readings();


}


