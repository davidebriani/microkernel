#include "kernel/kb.h"
#include "kernel/shell.h"
#include "kernel/lib/string.h"
#include "lib/arch/x86/syscall.h"
#include "kernel/video/vga.h"

#define SHELL_BUFFER_SIZE 256
#define SHELL_PROMPT "Shell:/$ "
#define SHELL_MOTD "# Welcome! Type help for more information.\n\n"

int8_t shellBuffer[SHELL_BUFFER_SIZE];
uint32_t shellBufferSize = 0;

static void shell_buffer_write(int8_t c);
static int8_t *shell_buffer_read(void);
static void shell_buffer_clear(void);

static void shell_buffer_write(int8_t c) {
    /* save printable chars (any char greater than and including a space) */
    if (shellBufferSize < SHELL_BUFFER_SIZE && c >= ' ') {
	    shellBuffer[shellBufferSize] = c;
	    shellBufferSize++;
    }
    /* if c is a backspace remove the last saved character */
    if (shellBufferSize && c == '\b') {
	shellBuffer[--shellBufferSize] = 0x00;
    }
}

static int8_t *shell_buffer_read() {
    shellBuffer[shellBufferSize] = '\0';
    return shellBuffer;
}

static void shell_buffer_clear() {
    syscall_puts(SHELL_PROMPT);
    shellBufferSize = 0;
}

void shell_init() {
    syscall_puts(SHELL_MOTD);

    shell_buffer_clear();

    while(1) {
	if (kbd_buffer_size()) {
	    int8_t c = kbd_buffer_read();
	    /* print out c unless shellBuffer is empty and c is a backspace */
	    if (!(!shellBufferSize && c == '\b'))
		syscall_putc(c);
	    shell_buffer_write(c);

	    if (c == '\n') {
		int8_t *command = shell_buffer_read();
		if (!strcmp(command, "help"))
		    syscall_puts("- help\tPrint this text\n- clear\tClear the screen\n- exit\tExit the shell\n");
		else if (!strcmp(command, "exit"))
		    break;
		else if (!strcmp(command, "clear"))
		    vga_device_clear();
		else
		    if (strlen(command)) {
			syscall_puts(command);
			syscall_puts(": command not found.\n");
		    }
		shell_buffer_clear();
		continue;
	    }
	}
    }
}
