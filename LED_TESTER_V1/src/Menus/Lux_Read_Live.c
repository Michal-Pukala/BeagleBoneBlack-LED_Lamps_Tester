/*
 * Lux_Read_Live.c
 *
 *  Created on: Aug 8, 2019
 *      Author: michal
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>
#include <dirent.h>
#include <fcntl.h>

#include "../LCD/iolib.h"
#include "../LCD/beagle_gpio.h"
#include "../LCD/beagle_hd44780.h"

#define USBPath "\
#!/bin/bash \n\
path=`ls -l /dev/disk/by-id/usb* | tail -1 | tail -c 5` \n\
mkdir ~/USBStick \n\
mount /dev/$path ~/USBStick \n\
"

#define SaveTemp "\
#!/bin/bash \n\
d=`date +%F_%T` \n\
temp=`cat /home/puka/Led_Tester_V1/TempTimeRead/BuffTemp.txt | tail -c 5` \n\
fn=`ls -lt ~/USBStick | head -2 | grep --color=AUTO \"TempTimeRead.*.csv\"` \n\
fn=${fn#*TempTimeRead} \n\
echo \"$temp,$d\" >> \"/root/USBStick/TempTimeRead$fn\" \n\
cat \"/root/USBStick/TempTimeRead$fn\" \n\
"
#define luxreadpy "sudo /usr/bin/python /root/LED_Tester_V1/LuxRead/lux.py"

struct gpioID enabled_gpio[6];

//Buttons Define
int LXbuttonBack = 41;
int LXbuttonEnter = 17;
int LXbuttonPrev = 15;
int LXbuttonNext = 16;

float lux_read(float *luxexp)
{
	//int file;
	char *filename = "/root/LED_Tester_V1/LuxRead/buf";
	float lux=0;
	int i;

		system(luxreadpy);
		//sleep(1);
			FILE *file = fopen(filename, "r");
			fscanf(file, "%f", &lux);
			if(!feof (file))
			{
				//printf("\n Lux: %f", lux);
			}
			fclose(file);

			*luxexp=lux;

	return(lux);
}

void lux_live(void)
{
	float lux;
	float buffMax;
	float LuxMax;
	float buffMin;
	float LuxMin=5000;
	char LuxMaxPrint[21];
	char LuxMinPrint[21];
	int refresh_rate_LCD;

	int i=0;
	float lux_all=0;
	float lux_avg;
	char LuxAvgPrint[21];

	float time_el=0;
	int time_hours=0;
	int time_mins=0;

	char lux_act_print[21];

	//Clean LCD
	clear_Screen(enabled_gpio);

	while (is_low(9, LXbuttonBack))
	{
		//Read Lux
		lux=lux_read(&lux);
		system("date");
		//Refresh Rate
		refresh_rate_LCD = 1;
		i++;
		sleep(refresh_rate_LCD);

		//Actual Lux 1s interval
		printf("\n Lux Act: %.1f \n\n", lux);
		sprintf(lux_act_print, "Lux: %.1f lx ", lux);
		goto_ScreenLine(0, enabled_gpio);
		stringToScreen(lux_act_print, enabled_gpio);

		//Max Lux
		buffMax=lux;

		if(LuxMax<=buffMax)
		{
			LuxMax=buffMax;
			sprintf(LuxMaxPrint, "Max: %.1f lx ", LuxMax);
			printf("\n %s", LuxMaxPrint);
			goto_ScreenLine(1, enabled_gpio);
			stringToScreen(LuxMaxPrint, enabled_gpio);
		}
		else
		{
		sprintf(LuxMaxPrint, "Max: %.1f lx ", LuxMax);
		printf("\n %s", LuxMaxPrint);
		goto_ScreenLine(1, enabled_gpio);
		stringToScreen(LuxMaxPrint, enabled_gpio);
		}

		//Min Lux
		buffMin=lux;
		if(LuxMin>=buffMin)
		{
			LuxMin=buffMin;
			sprintf(LuxMinPrint, "Min: %.1f lx ", LuxMin);
			printf("\n %s", LuxMinPrint);
			goto_ScreenLine(2, enabled_gpio);
			stringToScreen(LuxMinPrint, enabled_gpio);
		}
		else
		{
			sprintf(LuxMinPrint, "Min: %.1f lx ", LuxMin);
			printf("\n %s", LuxMinPrint);
			goto_ScreenLine(2, enabled_gpio);
			stringToScreen(LuxMinPrint, enabled_gpio);
		}

		//Lux Average
		lux_all=lux_all+lux;
		lux_avg=lux_all/i;

		//Time Elapsed
		time_el=time_el+2.72;
		time_hours=time_el/3600;
		time_mins=(time_el-(time_hours*3600))/60;
		printf("\n Time Elapsed: %f Hours: %d Mins: %d", time_el, time_hours, time_mins);
		sprintf(LuxAvgPrint, "Avg: %.1f lx |%d:%d| ", lux_avg, time_hours, time_mins);
		printf("\n %s", LuxAvgPrint);
		goto_ScreenLine(3, enabled_gpio);
		stringToScreen(LuxAvgPrint, enabled_gpio);

	}
	//Press Back Button, return to KeyLoop
	KeyLoop();
}

void lux_live_read(void)
{
	lux_live();
}
