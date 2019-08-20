/*
 * Led_Tester_V1.c
 *
 *  Created on: May 11, 2019
 *      Author: michal
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>

#include "LCD/iolib.h"
#include "LCD/beagle_gpio.h"
#include "LCD/beagle_hd44780.h"
#include "LCD/MenuStruct.h"

//LCD Defines
int selectedPins[]={P8_14,P8_12,P8_11,P9_15,P9_13,P9_12};
const char *pinDescription[] = {"DB7","DB6","DB5","DB4","RS","E"};
struct gpioID enabled_gpio[6];

int main()
{

void LCDreset(void)
{
	initialize_Screen(enabled_gpio,selectedPins);
	clear_Screen(enabled_gpio);
	terminate_Screen(enabled_gpio,selectedPins);
	initialize_Screen(enabled_gpio,selectedPins);
}

void GPIOreset(void)
{
	iolib_init();
	iolib_free();
}

LCDreset();
GPIOreset();

sleep(1);
menu_refresh();

KeyLoop();

return 1;
}






