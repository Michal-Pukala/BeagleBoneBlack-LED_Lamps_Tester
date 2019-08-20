/*
 * Temp_Lux_Test.c
 *
 *  Created on: Aug 19, 2019
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
#include "../Include/Lux_Time_Read.h"

#define USBPath "\
#!/bin/bash \n\
path=`ls -l /dev/disk/by-id/usb* | tail -1 | tail -c 5` \n\
mkdir ~/USBStick \n\
mount /dev/$path ~/USBStick \n\
"
#define SaveLux "\
#!/bin/bash \n\
d=`date +%F_%T` \n\
lux=`cat /home/puka/Led_Tester_V1/LuxTimeRead/BuffLux.txt | tail -c 5` \n\
temp=`cat /home/puka/Led_Tester_V1/TempTimeRead/BuffTemp.txt | tail -c 5` \n\
fn=`ls -lt ~/USBStick | head -2 | grep --color=AUTO \"TempLuxTest.*.csv\"` \n\
fn=${fn#*TempLuxTest} \n\
echo \"$lux,$temp,$d\" >> \"/root/USBStick/TempLuxTest$fn\" \n\
"

struct gpioID enabled_gpio[6];

void save_temp_lux(void)
{
		float lux=0;
		float temperature=0;

		int refresh_rate;

		int mins, hours, days;

		//Save Date in CSV file
		char luxsave[100];
		char tempsave[100];
		float count_mins=0;
		float count_mins_plus=0;
		int fulltime=0;

		void luxtimereadscreen(void)
			{
				goto_ScreenLine(0, enabled_gpio);
				stringToScreen("Complete...", enabled_gpio);
			}

		//Enter Times
		enter_time_values(&mins, &hours, &days);
		printf("\n Mins: %d \n Hours: %d \n Days: %d", mins, hours, days);

		//Set Refresh Rate
		refresh_rate=set_refresh_rate();
		printf("\n Refresh rate entered: %d", refresh_rate);

		LCDReset();
		Startscreen();
		//Set Lux
		lux_read(&lux);
		printf("\n Lux is: %.1lf", lux);
		//Set Temp
		temperatureexport(&temperature);
		printf("\n Temperature is: %f", temperature);

		//Sum FullTime
		fulltime=(mins+(hours*60)+(days*24*60));
		//int addusleep=262500;
		//Mount USBPath
		system(USBPath);
		//Create File
		system("sudo echo LUX,TEMP,DATE > ~/USBStick/TempLuxTest_$(date +%F).csv");

		switch (refresh_rate)
		{
		case 1:
			while(fulltime>=count_mins)
			{
				//usleep(addusleep);
				count_mins_plus++;
				count_mins=count_mins_plus/60;
				free(lux_read(&lux));
				lux_read(&lux);
				temperatureexport(&temperature);
				sprintf(luxsave,"echo Lux:%.1f > /home/puka/Led_Tester_V1/LuxTimeRead/BuffLux.txt",lux);
				system(luxsave);
				sprintf(tempsave,"echo Temp:%.1f > /home/puka/Led_Tester_V1/TempTimeRead/BuffTemp.txt",temperature);
				system(tempsave);
				//Save Temp Lux Date
				system(SaveLux);
				printf("\n Lux is: %f", lux);
				printf("\n Temp is: %f", temperature);
				printf("\n Counted minutes: %f", count_mins);
			}
			break;

		case 10:
		while(fulltime>=count_mins)
		{
			//usleep((refresh_rate*1000000)-(1000000-addusleep));
			count_mins_plus=count_mins_plus+10;
			count_mins=(count_mins_plus)/60.0;
			free(lux_read(&lux));
			lux_read(&lux);
			temperatureexport(&temperature);
			sprintf(luxsave,"echo Lux:%.1f > /home/puka/Led_Tester_V1/LuxTimeRead/BuffLux.txt",lux);
			system(luxsave);
			sprintf(tempsave,"echo Temp:%.1f > /home/puka/Led_Tester_V1/TempTimeRead/BuffTemp.txt",temperature);
			system(tempsave);
			//Save Temp Lux Date
			system(SaveLux);
			printf("\n Lux is: %f", lux);
			printf("\n Temp is: %f", temperature);
			printf("\n Counted minutes: %f", count_mins);
		}
			break;
		case 60:
		while(fulltime>=count_mins)
			{
				//usleep((refresh_rate*1000000)-(1000000-addusleep));
				count_mins_plus=count_mins_plus+60;
				count_mins=(count_mins_plus)/60.0;
				free(lux_read(&lux));
				lux_read(&lux);
				temperatureexport(&temperature);
				sprintf(luxsave,"echo Lux:%.1f > /home/puka/Led_Tester_V1/LuxTimeRead/BuffLux.txt",lux);
				system(luxsave);
				sprintf(tempsave,"echo Temp:%.1f > /home/puka/Led_Tester_V1/TempTimeRead/BuffTemp.txt",temperature);
				system(tempsave);
				//Save Temp Lux Date
				system(SaveLux);
				printf("\n Lux is: %f", lux);
				printf("\n Temp is: %f", temperature);
				printf("\n Counted minutes: %f", count_mins);
				}
			break;
		}
		free(lux_read(&lux));
		LCDReset();
		luxtimereadscreen();
		KeyLoop();
}



void templuxtest(void)
{


	//Initialize Interface
	interface();
	sleep(1);

	save_temp_lux();
}
