#ifndef KERNEL_VIDEO_VGA_H
#define KERNEL_VIDEO_VGA_H

#include <kernel/stdint.h>

/* Sets our text-mode VGA pointer and clears the screen */
void vga_device_init(void);

/* Sets the forecolor and backcolor that we will use */
void vga_device_cursor_set_color(uint8_t forecolor, uint8_t backcolor);

/* Updates the hardware cursor */
void vga_device_cursor_move(void);

/* Scrolls the screen */
void vga_device_scroll(void);

/* Clears the screen */
void vga_device_clear(void);

/* Print a character on the screen */
void putc(const int8_t c);

/* Print an array of character (a string) on the screen */
void puts(const int8_t *str);

/* Simple implementation of the common printf */
void kprintf(const int8_t *format,...);

struct vga_device
{
    uint16_t *address;    /* pointer to video memory */
    uint32_t cursorX;     /* cursor X position */
    uint32_t cursorY;     /* cursor Y position */
    uint16_t attribute;   /* text attribute */
    unsigned char cursorColor;
    unsigned short cursorOffset;
    unsigned int (*read_framebuffer)(struct vga_device *self, unsigned int offset, unsigned int count, void *buffer);
    unsigned int (*write_framebuffer)(struct vga_device *self, unsigned int offset, unsigned int count, void *buffer);
    void (*set_cursor_color)(struct vga_device *self, unsigned char fg, unsigned char bg);
    void (*set_cursor_offset)(struct vga_device *self, unsigned short offset);
};

#define VGA_FB_ADDRESS 0xB8000

/* default pagination */
#define VGA_COLS	80U
#define VGA_ROWS	25U

/* type of video card */
#define VGA_CARD_COLOR	1
#define VGA_CARD_MONO	0

/* text color attribute */
#define VGA_BLACK	    0
#define VGA_BLUE	    1
#define VGA_GREEN	    2
#define VGA_CYAN	    3
#define VGA_RED		    4
#define VGA_MAGENTA	    5
#define VGA_BROWN	    6
#define VGA_LIGHTGREY	    7
#define VGA_GREY	    8
#define VGA_LIGHTBLUE       9
#define VGA_LIGHTGREEN	    10
#define VGA_LIGHTCYAN	    11
#define VGA_LIGHTRED	    12
#define VGA_LIGHTMAGENTA    13
#define VGA_YELLOW	    14
#define VGA_WHITE	    15

#endif
