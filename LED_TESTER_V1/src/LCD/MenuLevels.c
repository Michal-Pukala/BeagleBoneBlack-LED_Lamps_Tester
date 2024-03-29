/*
 * MenuLevels.c
 *
 *  Created on: May 13, 2019
 *      Author: michal
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>

#include "iolib.h"
#include "beagle_gpio.h"
#include "beagle_hd44780.h"
#include "MenuStruct.h"
#include "../Include/Led_Tester_V1_Inc.h"
#include "../Include/Temp_Time_Read.h"
#include "../Include/Lux_Read_Live.h"
#include "../Include/Lux_Time_Read.h"
#include "../Include/Temp_Lux_Test.h"
#include "../Include/Run_Test.h"
#include "../Include/Current_Read_Live.h"

#define LCD_ROWS 4


// definition of menu's components: (*name, *next, *prev, *child, *parent, (*menu_function))
menu_t menu1 = { "Temp Test", &menu2, &menu6, &sub_menu1_1, NULL, NULL };
	menu_t sub_menu1_1 = { "Temp Live Read", &sub_menu1_2, &sub_menu1_2, NULL, &menu1, templiveread };
	menu_t sub_menu1_2 = { "Temp Time Read", NULL, &sub_menu1_1, NULL, &menu1, temptimeread };
menu_t menu2 = { "Lux Test", &menu3, &menu1, &sub_menu2_1, NULL, NULL };
	menu_t sub_menu2_1 = { "Lux Live Read", &sub_menu2_2, &sub_menu2_2, NULL, &menu2, lux_live_read };
	menu_t sub_menu2_2 = { "Lux Time Read", NULL, &sub_menu2_1, NULL, &menu2, luxtimeread };
		//menu_t sub_menu2_2_5 = { "ELEMENT 2_2_5", NULL, &sub_menu2_2_4, NULL, &sub_menu2_2, NULL };
menu_t menu3 = { "TESTS", &menu4, &menu2, &sub_menu3_1, NULL, NULL };
	menu_t sub_menu3_1 = { "Test 1", &sub_menu3_2, &sub_menu3_2, NULL, &menu3, templuxtest };
	menu_t sub_menu3_2 = { "Run Lux Test", NULL, &sub_menu3_1, NULL, &menu3, Run_Test };
menu_t menu4 = { "Current Test", &menu5, &menu3, NULL, NULL, current_read };
menu_t menu5 = { "ELEMENT 5", &menu6, &menu4, NULL, NULL, NULL };
menu_t menu6 = { "ELEMENT 6", NULL, &menu5, NULL, NULL, NULL };

menu_t *currentPointer = &menu1;

uint8_t menu_index;
uint8_t lcd_row_pos;
uint8_t lcd_row_pos_level_1, lcd_row_pos_level_2;

volatile uint8_t timer_flag;

struct gpioID enabled_gpio[6];

    int buttonBack = 41;
    //iolib_setdir(8, buttonBack, DIR_IN);

    int buttonEnter = 17;
   //iolib_setdir(8, buttonEnter, DIR_IN);

    int buttonPrev = 15;
    //iolib_setdir(8, buttonPrev, DIR_IN);

    int buttonNext = 16;
    //iolib_setdir(8, buttonNext, DIR_IN);


void menu_refresh(void)
{
    menu_t *temp;
    uint8_t i;
    i=0;

    if(currentPointer->parent) temp = (currentPointer->parent)->child;
    else temp = &menu1;


    for (i=0; i != menu_index - lcd_row_pos; i++)
    {

        temp = temp->next;
    }

    //clear_Screen(enabled_gpio);
    for (i=0; i< LCD_ROWS; i++)
    {


        goto_ScreenLocation(i,0,enabled_gpio);

		if (temp == currentPointer) charToScreen(62,enabled_gpio);

		else stringToScreen(" ",enabled_gpio);

	    goto_ScreenLocation(i,2,enabled_gpio);

	    stringToScreen(temp->name,enabled_gpio);

		temp = temp->next;

		if (!temp) break;
    }

}

uint8_t menu_get_index(menu_t *q)
{
    menu_t *temp;
    uint8_t i = 0;

    if(q->parent) temp = (q->parent)->child;
    else temp = &menu1;

    while (temp != q)
    {
        temp = temp->next;
        i++;
    }
    return i;
}

uint8_t menu_get_level(menu_t *q)
{
    menu_t *temp = q;
    uint8_t i=0;

    if(!q->parent) return 0;

    while (temp->parent != NULL)
    {
        temp = temp->parent;
        i++;
    }
    return i;
}



void menu_next(void)
{

    if(currentPointer->next)
    {
        currentPointer = currentPointer->next;
        menu_index++;
        if(++lcd_row_pos > LCD_ROWS - 1) lcd_row_pos = LCD_ROWS - 1;
    }

    else
    {
        menu_index = 0;
        lcd_row_pos = 0;
        if (currentPointer->parent) currentPointer = (currentPointer->parent)->child;
        else currentPointer = &menu1;
    }

    menu_refresh();
}

void menu_prev(void)
{
    currentPointer = currentPointer->prev;

    if(menu_index)
    {
        menu_index--;
        if(lcd_row_pos > 0) lcd_row_pos--;
    }
    else
    {
        menu_index = menu_get_index(currentPointer);

        if(menu_index >= LCD_ROWS -1) lcd_row_pos = LCD_ROWS - 1;
        else lcd_row_pos = menu_index;
    }
    menu_refresh();
}

void menu_enter(void)
{
    if(currentPointer->menu_function) currentPointer->menu_function();

    if(currentPointer->child)
    {
        switch (menu_get_level(currentPointer))
        {
            case 0:
            lcd_row_pos_level_1 = lcd_row_pos;
            break;

            case 1:
            lcd_row_pos_level_2 = lcd_row_pos;
            break;
        }

        menu_index = 0;
        lcd_row_pos = 0;

        currentPointer = currentPointer->child;

        menu_refresh();
    }
}

void menu_back(void)
{

	if (currentPointer->parent)
	{

		switch (menu_get_level(currentPointer))
		{
			case 1:
				lcd_row_pos = lcd_row_pos_level_1;
				break;

			case 2:
				lcd_row_pos = lcd_row_pos_level_2;
				break;
		}

		currentPointer = currentPointer->parent;
		menu_index = menu_get_index(currentPointer);

		menu_refresh();

	}
}

void key_next_press(void)
{

	static uint8_t key_next_lock=0;

	if( !key_next_lock && (is_high(8, buttonNext)))
	{

		key_next_lock=1;

		menu_next();

	} else if( key_next_lock && !(is_high(8, buttonNext) )) key_next_lock++;

}

void key_prev_press(void)
{

	static uint8_t key_prev_lock=0;

	if( !key_prev_lock && (is_high(8, buttonPrev) )) {

		key_prev_lock=1;

		menu_prev();

	} else if( key_prev_lock && !(is_high(8, buttonPrev) )) key_prev_lock++;

}

void key_enter_press(void)
{

	static uint8_t key_enter_lock=0;

	if( !key_enter_lock && (is_high(8, buttonEnter) )) {

		clear_Screen(enabled_gpio);

		key_enter_lock=1;

		menu_enter();

	} else if( key_enter_lock && !(is_high(8, buttonEnter) )) key_enter_lock++;

}

void key_back_press(void)
{

	static uint8_t key_back_lock=0;

	if( !key_back_lock && (is_high(9, buttonBack) ))
	{
		clear_Screen(enabled_gpio);

		key_back_lock=1;

		menu_back();

	} else if( key_back_lock && !(is_high(9, buttonBack) )) key_back_lock++;
}

struct ds18b20
	{
	char devPath[128];
	char devID[16];
	char tempData[6];
	struct ds18b20 *next;
	};

int8_t templiveread(void)
	{
	struct ds18b20 *rootNode;
	struct ds18b20 *devNode;
	struct ds18b20 *getTemp;
	char* tempOUT[2];
	float tempOUTFL;
	char* tempIN[2];
	float tempINFL;
	char tempOUTPrint[18];
	char tempINPrint[18];
	int refresh_rate_LCD;
	//MaxTemp Definition
	char tempMaxPrint[18];
	float TempMax=-99;
	float buffMax;
	//MinTemp Definition
	char tempMinPrint[18];
	float TempMin=-99;
	float buffMin;

			// Load pin configuration. Ignore error if already loaded
			system("echo w1 > /sys/devices/bone_capemgr.9/slots>/dev/null");
			while (is_low(9, buttonBack))
			{
				rootNode = malloc(sizeof(struct ds18b20));
				devNode = rootNode;
				getTemp = rootNode;
				findDevices(devNode);
				//printf("\n Found %d devices\n\n", devCnt);
				readTemp(getTemp, tempOUT, tempIN);

				//Refresh Rate
				refresh_rate_LCD = 1;
				sleep(refresh_rate_LCD);
				//Actual Temperature Float
				sscanf(tempOUT, "%f", &tempOUTFL);
				printf("\n TempOUT: %.1f \n\n", tempOUTFL);
				sscanf(tempIN, "%f", &tempINFL);
				printf("\n TempIN: %.1f \n\n", tempINFL);

				//Actual Temperature 1s interval
				sprintf(tempOUTPrint, "TempOUT: %s C", tempOUT);
				goto_ScreenLine(0, enabled_gpio);
				stringToScreen(tempOUTPrint, enabled_gpio);
				sprintf(tempINPrint, "TempIN: %s C", tempIN);
				goto_ScreenLine(1, enabled_gpio);
				stringToScreen(tempINPrint, enabled_gpio);

				//Max Temperature
				buffMax=tempOUTFL;
				if(TempMax==-99)
				{
					TempMax=buffMax;
				}
				else
				{
					if(TempMax<=buffMax)
					{
						TempMax=buffMax;

					}
					else;
				}
				sprintf(tempMaxPrint, "Max=%.1f C", TempMax);
				printf("\n %s", tempMaxPrint);
				goto_ScreenLine(2, enabled_gpio);
				stringToScreen(tempMaxPrint, enabled_gpio);

				//Min Temperature
				buffMin=tempOUTFL;
				if(TempMin==-99)
					{
						TempMin=buffMin;
					}
				else
					{
						if(TempMin>=buffMin)
							{
								TempMin=buffMin;
							}
						else;
					}
				sprintf(tempMinPrint, "Min=%.1f C", TempMin);
				printf("\n %s", tempMinPrint);
				goto_ScreenLine(3, enabled_gpio);
				stringToScreen(tempMinPrint, enabled_gpio);

				//Free Temperature Sens Memory
				free(rootNode);
	}

//Press Back Button, return to KeyLoop
KeyLoop();
}




