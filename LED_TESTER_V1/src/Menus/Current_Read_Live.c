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
#include <sys/stat.h>

#include "../LCD/iolib.h"
#include "../LCD/beagle_gpio.h"
#include "../LCD/beagle_hd44780.h"

#define AINpath "\
#!/bin/bash \n\
temp=`find /sys/ -name '*AIN*' | head -1 | sed 's/\AIN.*/AIN/'` \n\
echo $temp > /root/LED_Tester_V1/CurrentRead/pathbuf \n\
"
#define AINinit "sudo bash /root/LED_Tester_V1/CurrentRead/AINon.sh"

#define pathslots "\
sudo find /sys/devices/bone_capemgr.*/slots > /root/LED_Tester_V1/CurrentRead/pathslots"

#define AIN 5

#define ms 1000000

struct gpioID enabled_gpio[6];

char *init_current(void)
{

	system(AINpath);

	char *filename = "/root/LED_Tester_V1/CurrentRead/pathbuf";
	char * buffer = 0;
	long length;

	FILE *f = fopen (filename, "rb");

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

	}

	if (buffer)
	{
	  sprintf(buffer, "%s%d", buffer, AIN);


	}
	return buffer;
}

int *current_live_read(char *ainpath)
{

	//ainpath=init_current();
	int *curr;

	FILE *file = fopen(ainpath, "r");
	//free(ainpath);

	if(!file)
	{
		printf("Error opening file: %s\n", strerror(errno));
	}
	else
	{
		if(fscanf(file, "%4d", curr))
		{
			fclose(file);
		}
		else
		{
			printf("Error fscanf: %s\n", strerror(errno));
		}
		//*current=curr;
	}

	//free(file);


	return curr;
}

void current_read(void)
{

	system(AINinit);
	sleep(1);

	char *ainpath;

	ainpath=init_current();

	int *current;
	float avgcurr=0;
	float allcurr=0;
	int i=0;

	while(i<10)
	{
	sleep(1);
	i++;
	current=current_live_read(ainpath);
	allcurr=allcurr+*current;
	avgcurr=allcurr/i;
	printf("\n Current: %d AVG: %f", *current, avgcurr);
	//free(current);
	}

}


