#include <kernel/video/vga.h>
#include <kernel/lib/string.h>
#include <kernel/arch/x86/io.h>

struct vga_device *device;

static uint8_t hexmap[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static uint8_t detect_video(void);
static uint16_t textcolor(uint8_t forecolor, uint8_t backcolor);

/* Detects installed graphics card type. Returns:
*  VGA_CARD_MONO	if monochromatic card
*  VGA_CARD_COLOR	if color card */
static uint8_t detect_video(void) {
    uint8_t c = (*((uint16_t *) 0x410) & 0x30);
    return c?VGA_CARD_MONO:VGA_CARD_COLOR;
}

/* Scrolls the screen */
void vga_device_scroll(void) {
    unsigned blank, temp;

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    blank = 0x20 | (device->attribute << 8);

    /* Row 25 is the end, this means we need to scroll up */
    if(device->cursorY >= VGA_ROWS)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        temp = device->cursorY - VGA_ROWS + 1;
        memcpy(device->address, device->address + temp * VGA_COLS, (VGA_ROWS - temp) * VGA_COLS * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw(device->address + (VGA_ROWS - temp) * VGA_COLS, blank, VGA_COLS);
        device->cursorY = VGA_ROWS - 1;
    }
}

/* Updates the hardware cursor: the little blinking line
*  on the screen under the last character pressed! */
void vga_device_cursor_move(void) {
    unsigned temp;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    temp = device->cursorY * VGA_COLS + device->cursorX;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    io_outb(0x3D4, 14);
    io_outb(0x3D5, temp >> 8);
    io_outb(0x3D4, 15);
    io_outb(0x3D5, temp);
}

/* Clears the screen */
void vga_device_clear() {
    unsigned blank;
    uint32_t i;

    /* Again, we need the 'short' that will be used to
    *  represent a space with color */
    blank = 0x20 | (device->attribute << 8);

    /* Sets the entire screen to spaces in our current
    *  color */
    for(i = 0; i < VGA_ROWS; i++)
        memsetw(device->address + i * VGA_COLS, blank, VGA_COLS);

    /* Update out virtual cursor, and then move the
    *  hardware cursor */
    device->cursorX = 0;
    device->cursorY = 0;
    vga_device_cursor_move();
}

/* Combines background and foregorund color to attribute byte */
static uint16_t textcolor(uint8_t forecolor, uint8_t backcolor) {
    /* Top 4 bytes are the background, bottom 4 bytes
     * are the foreground color */
    return (backcolor << 4) | (forecolor & 0x0F);
}

/* Sets the forecolor and backcolor that we will use */
void vga_device_cursor_set_color(uint8_t forecolor, uint8_t backcolor) {
    if (detect_video() == VGA_CARD_COLOR)
	device->attribute = textcolor(forecolor, backcolor);
}

/* Sets our text-mode VGA pointer, then clears the screen for us */
void vga_device_init(void) {
    device->address = (unsigned short *)VGA_FB_ADDRESS;
    device->attribute = 0x0F;
    device->cursorX = 0;
    device->cursorY = 0;
    vga_device_clear();
    vga_device_cursor_set_color(VGA_WHITE, VGA_BLACK);
}

/* Puts a single character on the screen */
void putc(const int8_t c) {
    uint16_t *where;
    uint32_t att = device->attribute << 8;

    /* Handle a backspace, by moving the cursor back one space */
    /* FIXME: not right when you try to remove a \t or something similar */
    if (c == '\b') {
        if (device->cursorX != 0) {
	    device->cursorX--;
	    where = device->address + (device->cursorY * VGA_COLS + device->cursorX);
	    *where = ' ' | att;
	}
    }
    /* Handles a tab by incrementing the cursor's x, but only
    *  to a point that will make it divisible by 8 */
    else if (c == '\t')
        device->cursorX = (device->cursorX + 8) & ~(8 - 1);
    /* Handles a 'Carriage Return', which simply brings the
    *  cursor back to the margin */
    else if (c == '\r')
	device->cursorX = 0;
    /* We handle our newlines the way DOS and the BIOS do: we
    *  treat it as if a 'CR' was also there, so we bring the
    *  cursor to the margin and we increment the 'y' value */
    else if (c == '\n') {
        device->cursorX = 0;
        device->cursorY++;
    }
    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    else if (c >= ' ') {
        where = device->address + (device->cursorY * VGA_COLS + device->cursorX);
        *where = c | att;	/* Character AND attributes: color */
        device->cursorX++;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if (device->cursorX >= VGA_COLS) {
        device->cursorX = 0;
        device->cursorY++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    vga_device_scroll();
    vga_device_cursor_move();
}

/* Uses the above routine to output a string... */
void puts(const int8_t *text) {
    uint32_t i;

    for (i = 0; i < strlen(text); i++)
        putc(text[i]);
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
		putc(*t++);
		break;

	    case 'c':	/* signle character */
		putc((int8_t) args[aktarg]);
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
		    putc('-');
		} else
		    u = n;

		i=9;
		do {
		    d[i--] = (u%10) + '0';
		    u /= 10;
		} while (u && i>=0);
		while (++i <= 10)
		    putc(d[i]);
		break;

	    case 'u':	/* unsigned integer */
		u = args[aktarg];
		i=9;
		do {
		    d[i--] = (u%10) + '0';
		    u /= 10;
		} while (u && i>=0);
		while (++i <= 10)
		putc(d[i]);
		break;

	    case 'X':	/* 8 bit hex */
		n = args[aktarg];
		putc(hexmap[(n & 0xF0) >> 4]);
		putc(hexmap[n & 0x0F]);
		break;

	    default:
		putc(*format);
	    }
	    ++aktarg;
	} else {
	    putc(*format);
	}
	++format;
    }
}
