#ifndef __TEXTMODE_H
#define __TEXTMODE_H

#include "stdint.h"

void cls();
uint16_t textcolor(uint8_t forecolor, uint8_t backcolor);
void set_textcolor(uint8_t forecolor, uint8_t backcolor);
void init_video();
uint8_t detect_video();
void move_csr();
void scroll();

/* default pagination */
#define COLS		80U
#define ROWS		25U

/* type of video card */
#define CARD_COLOR	1
#define CARD_MONO	0

/* text color attribute */
#define BLACK		0
#define BLUE		1
#define GREEN		2
#define CYAN		3
#define RED		4
#define MAGENTA		5
#define BROWN		6
#define LIGHTGREY	7
#define GREY		8
#define LIGHTBLUE	9
#define LIGHTGREEN	10
#define LIGHTCYAN	11
#define LIGHTRED	12
#define LIGHTMAGENTA	13
#define YELLOW		14
#define WHITE		15

#endif
