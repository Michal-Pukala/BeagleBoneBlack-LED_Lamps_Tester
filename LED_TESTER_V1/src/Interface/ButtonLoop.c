/*
 * ButtonLoop.c
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
#include "../LCD/MenuStruct.h"

void KeyLoop(void)
{
		while (1)
			    {
					key_next_press();
					key_prev_press();
					key_enter_press();
					key_back_press();
			    }
}

