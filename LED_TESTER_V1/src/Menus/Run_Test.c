/*
 * Run_Test.c
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

#define SaveLux "\
#!/bin/bash \n\
d=`date +%F_%T` \n\
lux=`cat /home/puka/Led_Tester_V1/LuxTimeRead/BuffLux.txt | tail -c 5` \n\
fn=`ls -lt ~/USBStick | head -2 | grep --color=AUTO \"LuxTimeRead.*.csv\"` \n\
fn=${fn#*LuxTimeRead} \n\
echo \"$lux,$d\" >> \"/root/USBStick/LuxTimeRead$fn\" \n\
"
#define luxreadpy "sudo /usr/bin/python /root/LED_Tester_V1/LuxRead/lux.py"

#define timestart "sudo echo `date +\"%S.%6N\"` > /root/LED_Tester_V1/RunTest/TimeStart"
#define timestop "sudo echo `date +\"%S.%6N\"` > /root/LED_Tester_V1/RunTest/TimeStop"

struct gpioID enabled_gpio[6];
int RTselectedPin[]={P8_14,P8_12,P8_11,P9_15,P9_13,P9_12};

void GPIO_init(void)
{
	iolib_setdir(8, 26, DIR_OUT);
	pin_low(8,26);
}

void interface_rt(void)
{
	char starttime[26];
	float ops;
	ops = 3;
	clear_Screen(enabled_gpio);

	while(ops >= 0)
	{
		sprintf(starttime,"Measurement in: %.0f ", ops);
		goto_ScreenLine(0, enabled_gpio);
		stringToScreen(starttime, enabled_gpio);
		ops--;
		sleep(1);
	}
}

float lux_run_read(void)
	{
		//int file;
		char *filename = "/root/LED_Tester_V1/LuxRead/buf";
		float lux=0;

		system(luxreadpy);

		FILE *file = fopen(filename, "r");
		fscanf(file, "%f", &lux);
		fclose(file);

		return(lux);
	}

void lux_avg(float *LuxAvg)
{
	float lux=0;
	float buffMax;
	float LuxMax;
	float buffMin;
	float LuxMin=5000;

	int i=0;
	float lux_all=0;
	float lux_avg=0;
	char LuxAvgPrint[21];

	float time_el=0;

	char lux_act_print[21];

	//Clean LCD
	clear_Screen(enabled_gpio);

	while (time_el<10)
	{
		//Read Lux
		lux=lux_run_read();
		sprintf(lux_act_print, "Lux: %.1f lx ", lux);
		goto_ScreenLine(0, enabled_gpio);
		stringToScreen(lux_act_print, enabled_gpio);

		//Lux Average
		lux_all=lux_all+lux;
		if(i==0)
		{
			lux_avg=lux_all;
			i++;
		}
		else
		{
			i++;
			lux_avg=lux_all/i;
		}

		sprintf(LuxAvgPrint, "Avg: %.1f lx ", lux_avg);
		goto_ScreenLine(3, enabled_gpio);
		stringToScreen(LuxAvgPrint, enabled_gpio);
		sleep(1);
		time_el=time_el+1;
	}
	*LuxAvg=lux_avg;
}

void Run_Test(void)
{
	float lux=0;
	float luxavg=0;
	float tstart=0;
	float tstop=0;
	float timeelapsed=0;
	int ts=0;

	GPIO_init();

	//Init Pendrive
	system(USBPath);

	pin_high(8,26);	//Turn On Lamp
	lux_avg(&luxavg);
	lux_avg(&luxavg);
	pin_low(8,26); //Turn Off Lamp

	interface_rt();

	while(lux+1<luxavg)
	{

		lux=lux_run_read();
		printf("\n Lux is: %.2f", lux);
		pin_high(8,26); //Turn On Lamp
		if(ts==0)
		{
			system(timestart);	//Save Time Start
			ts=1;
		}

	}
	system(timestop);  //Stop The Time
	pin_low(8,26); //Turn Off Lamp

	char *fntimestart = "/root/LED_Tester_V1/RunTest/TimeStart";
	char *fntimestop = "/root/LED_Tester_V1/RunTest/TimeStop";
	FILE *ftimestart = fopen(fntimestart, "r");
	FILE *ftimestop = fopen(fntimestop, "r");
	fscanf(ftimestart, "%f", &tstart);
	fscanf(ftimestop, "%f", &tstop);
	fclose(ftimestart);
	fclose(ftimestop);

	timeelapsed=tstop-tstart;

	while(ts<6)
	{
		printf("\n Time Start: %f \n Time Stop %f \n Time Elapsed: %f", tstart, tstop, timeelapsed);
		sleep(1);
		ts++;
	}


}

