#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

// struct to hold ds18b20 data for linked list
struct ds18b20 {
	char devPath[128];
	char devID[16];
	char tempData[6];
	struct ds18b20 *next;
};

// Find connected 1-wire devices. 1-wire driver creates entries for each device 
// in /sys/bus/w1/devices on Beaglebone Black.  Create linked list.
int8_t findDevices(struct ds18b20 *d)
{
	DIR *dir;
	struct dirent *dirent;
	struct ds18b20 *newDev;
	char path[] = "/sys/devices/w1_bus_master1";
	int8_t i = 0;
	dir = opendir(path);
	if (dir != NULL) {

		while ((dirent = readdir(dir))) {

			// 1-wire devices are links beginning with 28-
			if (dirent->d_type == 4) {
				if (strstr(dirent->d_name, "28-") != NULL) {
					newDev = malloc(sizeof(struct ds18b20));
					strcpy(newDev->devID, dirent->d_name);
					// Assemble path to OneWire device
					sprintf(newDev->devPath, "%s/%s/w1_slave", path,
							newDev->devID);
					i++;

					newDev->next = 0;
					d->next = newDev;
					d = d->next;
				}

			} else {

			}
		}
		(void) closedir(dir);
	} else {
		perror("Couldn't open the w1 devices directory");
		return 1;
	}
	return 1;
}

int8_t readTemp(struct ds18b20 *d, char tempOUT[2], char tempIN[2])
{
	int u=0;

	//struct ds18b20 *newTemp;
	while (d->next != NULL) {
		d = d->next;
		int fd = open(d->devPath, O_RDONLY);

		if (fd == -1) {
			perror("Couldn't open the w1 device.");
			return 1;
		}
		char buf[256];
		ssize_t numRead;
		while ((numRead = read(fd, buf, 256)) > 0) {
			if(u==0)  //TEMP OUT
			{
			strncpy(d->tempData, strstr(buf, "t=") + 2, 5);
			float tempC = strtof(d->tempData, NULL);
			sprintf(tempOUT, "%.1f", tempC / 1000);
			u=1;
			}
			else
			{
			//TEMP ON LAMP
			strncpy(d->tempData, strstr(buf, "t=") + 2, 5);
			float tempC = strtof(d->tempData, NULL);
			sprintf(tempIN, "%.1f", tempC / 1000);
			}
		}
		close(fd);
	}
	return (1);
}

/*
void getTempMain(void)
{
	// Temperature reading
		struct ds18b20 *rootNode;
		struct ds18b20 *devNode;
		struct ds18b20 *getTemp;
		char tempAct[6];
		// Load pin configuration. Ignore error if already loaded
		system("echo w1 > /sys/devices/bone_capemgr.9/slots>/dev/null");
		while (1)
		{
			rootNode = malloc(sizeof(struct ds18b20));
			devNode = rootNode;
			getTemp = rootNode;
			findDevices(devNode);
			//printf("\n Found %d devices\n\n", devCnt);
			readTemp(getTemp, tempAct);
			printf("\n Temp Act: %s \n\n", tempAct);
			free(rootNode);
		}
		//return 0;
}
*/
