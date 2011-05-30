#include "textmode.h"
#include "string.h"
#include "ports.h"

/* These define our textpointer, our background and foreground
*  colors (attributes), and x and y cursor coordinates */
unsigned short *textmemptr;	/* pointer to video memory */
int attrib = 0x0F;		/* attribute to text */
uint32_t csr_x = 0, csr_y = 0;	/* cursor position */

static uint8_t hexmap[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};


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
    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
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

/* Detects installed graphics card type. Returns:
*  CARD_MONO	if monochromatic card
*  CARD_COLOR	if color card */
uint8_t detect_video(void) {
    uint8_t c = (*((uint16_t *) 0x410) & 0x30);
    return c?CARD_MONO:CARD_COLOR;
}

/* Puts a single character on the screen */
void putch(const int8_t c) {
    uint16_t *where;
    uint32_t att = attrib << 8;

    /* Handle a backspace, by moving the cursor back one space */
    if(c == 0x08) {
        if(csr_x != 0)
	    csr_x--;
    }
    /* Handles a tab by incrementing the cursor's x, but only
    *  to a point that will make it divisible by 8 */
    else if(c == 0x09)
        csr_x = (csr_x + 8) & ~(8 - 1);
    /* Handles a 'Carriage Return', which simply brings the
    *  cursor back to the margin */
    else if(c == '\r')
	csr_x = 0;
    /* We handle our newlines the way DOS and the BIOS do: we
    *  treat it as if a 'CR' was also there, so we bring the
    *  cursor to the margin and we increment the 'y' value */
    else if(c == '\n') {
        csr_x = 0;
        csr_y++;
    }
    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    else if(c >= ' ') {
        where = textmemptr + (csr_y * COLS + csr_x);
        *where = c | att;	/* Character AND attributes: color */
        csr_x++;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(csr_x >= COLS) {
        csr_x = 0;
        csr_y++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    scroll();
    move_csr();
}

/* Uses the above routine to output a string... */
void puts(const int8_t *text) {
    int32_t i;

    for (i = 0; i < strlen(text); i++)
        putch(text[i]);
}

void kprintf(const int8_t *format,...) {
    uint32_t *args = ((uint32_t*) &format) + 1;
    int8_t *t, d[11], xx[9];
    uint8_t aktarg=0;
    uint32_t u;
    int32_t n, i;

    xx[8] = 0;

    /* first argument in args[0] */
    while (*format) {
	if (*format == '%') {
	    ++format;
	    if (*format == 0) break;
	    switch (*format) {
	    case 's':	/* string */
		t = (int8_t *) args[aktarg];
		while (*t)
		putch(*t++);
		break;

	    case 'c':	/* signle character */
		putch((int8_t) args[aktarg]);
		break;

	    case 'x':	/* 32 bit hex */
		u = args[aktarg];
		for(i=7; i>=0; --i) {
		    xx[i]=hexmap[u & 0x0F];
		    u >>= 4;
		}
		puts(xx);
		break;

	    case 'd':	/* int32_t */
		n = args[aktarg];
		if (n < 0) {
		    u = -n;
		    putch('-');
		} else
		    u = n;

		i=9;
		do {
		    d[i--] = (u%10) + '0';
		    u /= 10;
		} while (u && i>=0);
		while (++i <= 10)
		    putch(d[i]);
		break;

	    case 'u':	/* unsigned integer */
		u = args[aktarg];
		i=9;
		do {
		    d[i--] = (u%10) + '0';
		    u /= 10;
		} while (u && i>=0);
		while (++i <= 10)
		putch(d[i]);
		break;

	    case 'X':	/* 8 bit hex */
		n = args[aktarg];
		putch(hexmap[(n & 0xF0) >> 4]);
		putch(hexmap[n & 0x0F]);
		break;

	    default:
		putch(*format);
	    }
	    ++aktarg;
	} else {
	    putch(*format);
	}
	++format;
    }
}
