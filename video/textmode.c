#include "stdint.h"
#include "textmode.h"
#include "string.h"
#include "ports.h"

/* These define our textpointer, our background and foreground
 * colors (attributes), and x and y cursor coordinates */
unsigned short *textmemptr;	/* pointer to video memory */
int attrib = 0x0F;		/* attribute to text */
uint32_t csr_x = 0, csr_y = 0;	/* cursor position */

/* Scrolls the screen */
void scroll(void) {
    unsigned blank, temp;

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    blank = 0x20 | (attrib << 8);

    /* Row 25 is the end, this means we need to scroll up */
    if(csr_y >= ROWS)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        temp = csr_y - ROWS + 1;
        memcpy (textmemptr, textmemptr + temp * COLS, (ROWS - temp) * COLS * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw (textmemptr + (ROWS - temp) * COLS, blank, COLS);
        csr_y = ROWS - 1;
    }
}

/* Updates the hardware cursor: the little blinking line
*  on the screen under the last character pressed! */
void move_csr(void) {
    unsigned temp;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    temp = csr_y * COLS + csr_x;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

/* Clears the screen */
void cls() {
    unsigned blank;
    uint32_t i;

    /* Again, we need the 'short' that will be used to
    *  represent a space with color */
    blank = 0x20 | (attrib << 8);

    /* Sets the entire screen to spaces in our current
    *  color */
    for(i = 0; i < ROWS; i++)
        memsetw (textmemptr + i * COLS, blank, COLS);

    /* Update out virtual cursor, and then move the
    *  hardware cursor */
    csr_x = 0;
    csr_y = 0;
    move_csr();
}

/* Combines background and foregorund color to attribute byte */
uint16_t textcolor(uint8_t forecolor, uint8_t backcolor) {
    /* Top 4 bytes are the background, bottom 4 bytes
     * are the foreground color */
    return (backcolor << 4) | (forecolor & 0x0F);
}

/* Sets the forecolor and backcolor that we will use */
void set_textcolor(uint8_t forecolor, uint8_t backcolor) {
    attrib = textcolor(forecolor, backcolor);
}

/* Sets our text-mode VGA pointer, then clears the screen for us */
void init_video(void) {
    textmemptr = (unsigned short *)0xB8000;
    cls();
}

/*
 * detects installed graphics card type
 * @return CARD_MONO if monochromatic card
 * CARD_COLOR if color card
 */
uint8_t detect_video(void) {
    uint8_t c = (*((uint16_t *) 0x410) & 0x30);
    return c?CARD_MONO:CARD_COLOR;
}
