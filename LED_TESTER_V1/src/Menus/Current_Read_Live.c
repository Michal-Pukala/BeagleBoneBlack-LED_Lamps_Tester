/*
 * Current_Read_Live.c
 *
 *  Created on: Aug 26, 2019
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
#include <errno.h>

#include "../LCD/iolib.h"
#include "../LCD/beagle_gpio.h"
#include "../LCD/beagle_hd44780.h"

#define AINpath "\
#!/bin/bash \n\
temp=`find /sys/ -name '*AIN*' | head -1 | sed 's/\AIN.*/AIN/'` \n\
echo $temp > /root/LED_Tester_V1/CurrentRead/pathbuf \n\
"
#define AINinit "\
#!/bin/bash \n\
path=`/sys/devices/bone_capemgr.*/slots` \n\
echo root | sudo -S echo cape-bone-iio > $path \n\
"

#define AIN 5

struct gpioID enabled_gpio[6];



char *init_current(void)
{

	system(AINpath);

	//int file;
	//char ainpath[33];
	char *filename = "/root/LED_Tester_V1/CurrentRead/pathbuf";
	char * buffer = 0;
	//char * ainexp = 0;
	long length;
	FILE * f = fopen (filename, "rb");

	if (f)
	{
	  fseek (f, 0, SEEK_END);
	  length = ftell (f);
	  fseek (f, 0, SEEK_SET);
	  //buffer = malloc (length);
	  buffer =  malloc(length+10);
	 // ainexp = malloc(length+10);
	  if (buffer)
	  {
	    fread (buffer, 1, length-1, f);
	    buffer[length-1] = 0;
	  }
	  fclose (f);
	  //free(f);
	}

	if (buffer)
	{
	  sprintf(buffer, "%s%d", buffer, AIN);
	  //free(buffer);

	}
	return buffer;
}

int current_live_read(int *current)
{

	char *ainpath;
	ainpath=init_current();
	int curr;
	//char currbuf[47];

	//sprintf(currbuf, "sudo cat %s >  ~/LED_Tester_V1/CurrentRead/currbuf", ainpath);
	//printf("%s", currbuf);

	FILE *file = fopen(ainpath, "r");
	free(ainpath);

	if(!file)
	{
		printf("Error opening file: %s\n", strerror(errno));
	}
	else
	{
		fscanf(file, "%4d", &curr);
		fclose(file);
		*current=curr;
	}

	free(file);


	return(0);
}

void current_read(void)
{
	system(AINinit);
	//system("sudo echo cape-bone-iio > /sys/devices/bone_capemgr.*/slots");  //Init ADC
	init_current();

	int current;
	float avgcurr=0;
	float allcurr=0;
	int i=0;

	while(1)
	{
	sleep(1);
	i++;
	current_live_read(&current);
	allcurr=allcurr+current;
	avgcurr=allcurr/i;
	printf("\n Current: %d AVG: %f", current, avgcurr);
	}

}


