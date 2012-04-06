#include <kernel/kb.h>
#include <kernel/arch/x86/isr.h>
#include <kernel/arch/x86/io.h>

/* US lowercase keyboard keymap */
uint8_t kbdMapLowerUS[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',				/* Backspace */
  '\t',							/* Tab */
  'q', 'w', 'e', 'r',					/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',		/* Enter key */
    0,							/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,					/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

/* US uppercase keyboard keymap */
uint8_t kbdMapUpperUS[128] = {
    0,  27, '!', ' ', '#', ' ', '%', '&', '/', '(',	/* 9 */
  ')', '=', '-', '=', '\b',				/* Backspace */
  '\t',							/* Tab */
  'Q', 'W', 'E', 'R',					/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',		/* Enter key */
    0,							/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',	/* 39 */
 '\'', '`',   0,					/* Left shift */
 '\\', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

#define KBD_BUFFER_SIZE 256

uint8_t kbdBuffer[KBD_BUFFER_SIZE];
uint32_t kbdBufferSize = 0;
uint8_t kbdToggleShift = 0;
uint8_t kbdToggleCtrl = 0;
uint8_t kbdToggleAlt = 0;

static void kbd_buffer_write(uint8_t c) {
    if (kbdBufferSize < KBD_BUFFER_SIZE) {
	kbdBuffer[kbdBufferSize] = c;
	kbdBufferSize++;
    }
}

uint8_t kbd_buffer_read(void) {
    uint8_t c;

    if (kbdBufferSize) {
	kbdBufferSize--;
	c = kbdBuffer[kbdBufferSize];
    }
    return c;
}

uint32_t kbd_buffer_size(void) {
    return kbdBufferSize;
}

/* Handles the keyboard interrupt */
static void keyboard_handler(registers_t *regs) {
    uint8_t scancode;

    /* Read from the keyboard's data buffer */
    scancode = io_inb(0x60);

    if (scancode == 0x2A)
	kbdToggleShift = 1;

    if (scancode == 0xAA)
	kbdToggleShift = 0;

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80) {
        /* We can use this one to see if the user released the
        *  shift, alt, or control keys... */
    }
    else {
        /* Here, a key was just pressed. Please note that if you
        *  hold a key down, you will get repeated key press
        *  interrupts. */

        /* Just to show you how this works, we simply translate
        *  the keyboard scancode into an ASCII value, and then
        *  display it to the screen. You can get creative and
        *  use some flags to see if a shift is pressed and use a
        *  different layout, or you can add another 128 entries
        *  to the above layout to correspond to 'shift' being
        *  held. If shift is held using the larger lookup table,
        *  you would add 128 to the scancode when you look for it */
	if (kbdToggleShift)
	    kbd_buffer_write(kbdMapUpperUS[scancode]);
	else
	    kbd_buffer_write(kbdMapLowerUS[scancode]);
    }
}

/* Installs the keyboard handler into IRQ1 */
void keyboard_init() {
    irq_register_handler(IRQ_KBD, &keyboard_handler);
}
