#ifndef TEXTMODE_H
#define TEXTMODE_H

#include "stdint.h"

/* Sets our text-mode VGA pointer and clears the screen */
void init_textmode(void);

/* Sets the forecolor and backcolor that we will use */
void textmode_color(uint8_t forecolor, uint8_t backcolor);

/* Updates the hardware cursor */
void move_csr(void);

/* Scrolls the screen */
void scroll(void);

/* Clears the screen */
void cls(void);

/* Print a character on the screen */
void putch(const int8_t c);

/* Print an array of character (a string) on the screen */
void puts(const int8_t *str);

/* Simple implementation of the common printf */
void kprintf(const int8_t *format,...);

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
