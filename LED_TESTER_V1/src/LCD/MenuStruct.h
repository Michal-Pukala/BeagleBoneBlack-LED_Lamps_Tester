#ifndef MENUSTRUCT_H_
#define MENUSTRUCT_H_

typedef struct menu_struct menu_t;

struct menu_struct
{
    const char *name;
    menu_t *next;
    menu_t *prev;
    menu_t *child;
    menu_t *parent;
    void (*menu_function)(void);
};

menu_t menu1;
	menu_t sub_menu1_1;
		menu_t sub_menu1_1_1;
	menu_t sub_menu1_2;
menu_t menu2;
	menu_t sub_menu2_1;
	menu_t sub_menu2_2;
		menu_t sub_menu2_2_1, sub_menu2_2_2, sub_menu2_2_3, sub_menu2_2_4, sub_menu2_2_5;
	menu_t sub_menu2_3;
	menu_t sub_menu2_4;
	menu_t sub_menu2_5;
menu_t menu3;
menu_t menu4;
menu_t menu5;
menu_t menu6;

void menu_refresh(void);
void menu_next(void);
void menu_prev(void);
void menu_enter(void);
void menu_back(void);
void key_next_press(void);
void key_prev_press(void);
void key_enter_press(void);
void key_back_press(void);
int8_t templiveread(void);

#endif /* MENUSTRUCT_H_ */
