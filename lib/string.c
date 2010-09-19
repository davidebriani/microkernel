#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "textmode.h"

/* These define our textpointer, our background and foreground
*  colors (attributes), and x and y cursor coordinates */
extern uint16_t *textmemptr;
extern int32_t attrib;
extern int32_t csr_x, csr_y;

static uint8_t hexmap[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

void *memcpy(void *dest, const void *src, int32_t count) {
    const uint8_t *sp = (const uint8_t *)src;
    uint8_t *dp = (uint8_t *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void *memset(void *dest, uint8_t val, int32_t count) {
    uint8_t *temp = (uint8_t *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

uint16_t *memsetw(uint16_t *dest, uint16_t val, int32_t count) {
    uint16_t *temp = (uint16_t *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

int32_t strlen(const int8_t *str) {
    int32_t retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

int8_t *strchr(const int8_t *str, int8_t c) {
    while (*str) {
	if (*str == c) return (char*)str;
	str++;
    }
    return NULL;
}

int8_t *strcpy(int8_t *dest, const int8_t *src) {
    memcpy(dest, src, strlen(src) + 1);
    return (int8_t *)src;
}

int8_t *strcat(int8_t *dest, const int8_t *src) {
    int8_t *dest2 = dest;
    dest2 += strlen(dest) - 1;
    while (*src) {
	*dest2 = *src;
	src++;
	dest2++;
    }
    *dest2 = 0;
    return dest;
}

int32_t strcmp(const int8_t *s1, const int8_t *s2) {
    while ((*s1) && (*s1 == *s2)) {
	s1++;
	s2++;
    }
    return (*(uint8_t *)s1 - *(uint8_t *)s2);
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
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;	/* Character AND attributes: color */
        csr_x++;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(csr_x >= 80) {
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

void kprintf(int8_t *format,...) {
    uint32_t *args = ((uint32_t*) &format) + 1;
    int8_t *t, d[11], xx[9];

    xx[8] = 0;
    uint8_t aktarg=0;
    uint32_t u;
    int32_t n, i;

    // first argument in args[0]
    while (*format) {
	if (*format == '%') {
	    ++format;
	    if (*format == 0) break;
	    switch (*format) {
	    case 's': //string
		t = (int8_t *) args[aktarg];
		while (*t)
		putch(*t++);
		break;

	    case 'c': //signle character
		putch((int8_t) args[aktarg]);
		break;

	    case 'x': //32 bit hex
		u = args[aktarg];
		for(i=7; i>=0; --i) {
		    xx[i]=hexmap[u & 0x0F];
		    u >>= 4;
		}
		puts(xx);
		break;

	    case 'd': //sint32_t
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

	    case 'u': //unsigned integer
		u = args[aktarg];
		i=9;
		do {
		    d[i--] = (u%10) + '0';
		    u /= 10;
		} while (u && i>=0);
		while (++i <= 10)
		putch(d[i]);
		break;

	    case 'X': //8 bit hex
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
