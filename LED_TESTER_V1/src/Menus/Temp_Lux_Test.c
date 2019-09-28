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
#define SaveData "\
#!/bin/bash \n\
d=`date +%F_%T` \n\
lux=`cat /home/puka/Led_Tester_V1/Test1/BufLux.txt | tail -c 5` \n\
tempout=`cat /home/puka/Led_Tester_V1/Test1/BufTempOut.txt | tail -c 5` \n\
tempin=`cat /home/puka/Led_Tester_V1/Test1/BufTempIn.txt | tail -c 5` \n\
curr=`cat /home/puka/Led_Tester_V1/Test1/BufCurr.txt | tail -c 3` \n\
fn=`ls -lt ~/USBStick | head -2 | grep --color=AUTO \"Test1DATA.*.csv\"` \n\
fn=${fn#*Test1DATA} \n\
echo \"$lux,$curr,$tempout,$tempin,$d\" >> \"/root/USBStick/Test1DATA$fn\" \n\
"
//Buttons Define
int TLTbuttonBack = 41;
int TLTbuttonEnter = 17;
int TLTbuttonPrev = 15;
int TLTbuttonNext = 16;

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
		system("sudo echo LUX,TEMPOUT,TEMPIN,CURR,DATE > ~/USBStick/Test1DATA_$(date +%F).csv");

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
				//system(SaveData);
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
			system(SaveData);
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
				system(SaveData);
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

int set_tempstart(void)
{
	LCDReset();

	int full_input=0;

	//Temperatures
	float tempOUT, tempIN;
	float set_temp;
	char i_tempOUT[21];
	char i_tempset[21];

	//LCD Line 1
	goto_ScreenLine(0, enabled_gpio);
	stringToScreen(" SET START TEMP", enabled_gpio);
	//LCD Line 2
	goto_ScreenLine(1, enabled_gpio);
	stringToScreen("Start Temp: ", enabled_gpio);
	//LCD Line 3
	//LCD Line 4
	goto_ScreenLine(3, enabled_gpio);
	stringToScreen("OUT Temp= ", enabled_gpio);

	//Check OUT Temp
	temperatureexport(&tempOUT, &tempIN);
	sprintf(i_tempOUT,"OUT Temp=  %.1f ", tempOUT);
	goto_ScreenLine(3, enabled_gpio);
	stringToScreen(i_tempOUT, enabled_gpio);

	//Set Temp to TempOUT
	set_temp=tempOUT;
	sprintf(i_tempset,"Start Temp: >> %.1f ", set_temp);
	goto_ScreenLine(1, enabled_gpio);
	stringToScreen(i_tempset, enabled_gpio);

	//Enter Start Temperature
	while(is_low(8, TLTbuttonEnter) && full_input==0)
		{
			//Press Add Number
			if(is_high(8, TLTbuttonNext))
			{
				//Button Press Prevent
				while(is_high(8, TLTbuttonNext));
				if(set_temp<=80.0)
				{
				set_temp=set_temp+0.5;
				sprintf(i_tempset,"Start Temp: >> %.1f ", set_temp);
				goto_ScreenLine(1, enabled_gpio);
				stringToScreen(i_tempset, enabled_gpio);
				}else;
			}else;

			//Press Deacrese Number
			if(is_high(8, TLTbuttonPrev))
			{
				//Button Press Prevent
				while(is_high(8, TLTbuttonPrev));

				if(set_temp>tempOUT)
				{
				set_temp=set_temp-0.5;
				sprintf(i_tempset,"Start Temp: >> %.1f ", set_temp);
				goto_ScreenLine(1, enabled_gpio);
				stringToScreen(i_tempset, enabled_gpio);
				}else;
			}else;

		while(is_high(8, TLTbuttonEnter))
			{
			full_input=1;
			sprintf(i_tempset,"Start Temp: %.1f SET", set_temp);
			goto_ScreenLine(1, enabled_gpio);
			stringToScreen(i_tempset, enabled_gpio);
			}
		}
	printf("\n TEMP SET: %.1f", set_temp);
	return set_temp;
}

void heat_temp(float heat_temp)
{
	LCDReset();

	char i_acttemp[21];
	char i_starttemp[21];

	//LCD Line 1
	goto_ScreenLine(0, enabled_gpio);
	stringToScreen(" HEATING TEMP", enabled_gpio);
	//LCD Line 2
	goto_ScreenLine(1, enabled_gpio);
	stringToScreen("Temp: ", enabled_gpio);
	//LCD Line 3
	//LCD Line 4
	goto_ScreenLine(3, enabled_gpio);
	stringToScreen("Start Temp = ", enabled_gpio);

	float tempOUT, tempIN;
	int done=0;

	//Check OUT Temp
	temperatureexport(&tempOUT, &tempIN);

	sprintf(i_acttemp,"Temp:  %.1f ", tempOUT);
	goto_ScreenLine(1, enabled_gpio);
	stringToScreen(i_acttemp, enabled_gpio);

	sprintf(i_starttemp,"Start Temp =  %.1f ", heat_temp);
	goto_ScreenLine(3, enabled_gpio);
	stringToScreen(i_starttemp, enabled_gpio);


	//Set GPIO to Heater
	iolib_setdir(8, 8, DIR_OUT);
	pin_low(8,8);

	if(heat_temp-0.5 > tempOUT && done==0)
	{
		// Turn ON Heater
		pin_high(8,8);
		goto_ScreenLine(2, enabled_gpio);
		stringToScreen("       Heating...", enabled_gpio);
		while(heat_temp-0.5 > tempOUT)
		{
		temperatureexport(&tempOUT, &tempIN);
		sprintf(i_acttemp,"Temp:  %.1f ", tempOUT);
		goto_ScreenLine(1, enabled_gpio);
		stringToScreen(i_acttemp, enabled_gpio);
		}
	//Turn OFF Heater
	pin_low(8,8);
	}
	LCDReset();
	goto_ScreenLine(0, enabled_gpio);
	stringToScreen(" HEATING COMPLETE", enabled_gpio);
	sleep(1);
	LCDReset();

	//Screen Setup
	//LCD Line 1
	goto_ScreenLine(0, enabled_gpio);
	stringToScreen("SAVING DATA", enabled_gpio);
	//LCD Line 2
	goto_ScreenLine(1, enabled_gpio);
	sprintf(i_starttemp,"Set Temp: %.1f ", heat_temp);
	stringToScreen(i_starttemp , enabled_gpio);
	//LCD Line 3
	//LCD Line 4
	goto_ScreenLine(3, enabled_gpio);
	stringToScreen("Actual Temp: ", enabled_gpio);

	//Turn On Lamp
	pin_high(8,26);
	sleep(1);

	//Set Temp Real Time and Heat
	int button=0;
	while(is_low(8, TLTbuttonBack))
	{
		if(is_high(8, TLTbuttonNext))
		{
		while(is_high(8, TLTbuttonNext));
		button=1;
		}
		if(is_high(8, TLTbuttonPrev))
		{
		while(is_high(8, TLTbuttonPrev));
		button=2;
		}

		switch(button)
		{
		case 1:
			heat_temp=heat_temp+0.5;
			goto_ScreenLine(1, enabled_gpio);
			sprintf(i_starttemp,"Set Temp: %.1f ", heat_temp);
			stringToScreen(i_starttemp , enabled_gpio);
			button=0;
			break;
		case 2:
			heat_temp=heat_temp-0.5;
			goto_ScreenLine(1, enabled_gpio);
			sprintf(i_starttemp,"Set Temp: %.1f ", heat_temp);
			stringToScreen(i_starttemp , enabled_gpio);
			button=0;
			break;
		}

		//Check OUT Temp
		temperatureexport(&tempOUT, &tempIN);
		if(heat_temp-0.5 > tempOUT)
			{
			// Turn ON Heater
			goto_ScreenLine(1, enabled_gpio);
			sprintf(i_starttemp,"Set Temp: %.1f ", heat_temp);
			stringToScreen(i_starttemp , enabled_gpio);
			goto_ScreenLine(2, enabled_gpio);
			stringToScreen("Heating..." , enabled_gpio);
			sprintf(i_acttemp,"Actual Temp: %.1f ", tempOUT);
			goto_ScreenLine(3, enabled_gpio);
			stringToScreen(i_acttemp, enabled_gpio);
			pin_high(8,8);
			}
		else
		{
			pin_low(8,8);
			goto_ScreenLine(2, enabled_gpio);
			stringToScreen("          " , enabled_gpio);
			sprintf(i_acttemp,"Actual Temp: %.1f ", tempOUT);
			goto_ScreenLine(3, enabled_gpio);
			stringToScreen(i_acttemp, enabled_gpio);
		}

		//Saving Data
		save_readings();


	}
}

void save_readings(void)
{
	float lux=0;
	char luxsave[100];

	float tempOUT, tempIN;
	char tempOUTsave[100];
	char tempINsave[100];

	int current;
	char currentsave[100];
	char *ainpath;
	ainpath=init_current();
	temperatureexport(&tempOUT, &tempIN);
	lux_read(&lux);
	current_live_read(ainpath, &current);

	//Save Temps

	sprintf(tempOUTsave,"echo TempOUT:%.1f > /home/puka/Led_Tester_V1/Test1/BufTempOut.txt", tempOUT);
	sprintf(tempINsave,"echo TempIN:%.1f > /home/puka/Led_Tester_V1/Test1/BufTempIn.txt", tempIN);
	system(tempOUTsave);
	system(tempINsave);

	//Save Lux

	sprintf(luxsave,"echo Lux:%.1f > /home/puka/Led_Tester_V1/Test1/BufLux.txt", lux);
	system(luxsave);
	//Save Curr

	sprintf(currentsave,"echo Curr:%d > /home/puka/Led_Tester_V1/Test1/BufCurr.txt", current);
	system(currentsave);

	//Save all data
	system(SaveData);
}

void templuxtest(void)
{
	float tempstart=0;

	//Mount USBPath
	system(USBPath);
	//Create File
	system("sudo echo LUX,TEMPOUT,TEMPIN,CURR,DATE > ~/USBStick/Test1DATA_$(date +%F).csv");


	//Set Start Temperature
	tempstart=set_tempstart();

	//Heat OUT Temp
	heat_temp(tempstart);

	//save_temp_lux();
}
