#include <kernel/kb.h>
#include <kernel/shell.h>
#include <lib/string.h>
#include <lib/arch/x86/syscall.h>

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
    call_puts(SHELL_PROMPT);
    shellBufferSize = 0;
}

void shell_init() {
    /* Use system calls as this we'll be an executable program someday */
    call_puts(SHELL_MOTD);

    shell_buffer_clear();

    while(1) {
	if (kbd_buffer_size()) {
	    int8_t c = kbd_buffer_read();
	    /* print out c unless shellBuffer is empty and c is a backspace */
	    if (!(!shellBufferSize && c == '\b'))
		call_putc(c);
	    shell_buffer_write(c);

	    if (c == '\n') {
		int8_t *command = shell_buffer_read();
		if (!strcmp(command, "help"))
		    call_puts("- help\t\tPrint this text\n"
			    "- exit\t\tExit the shell\n"
			    "- halt\t\tHalt the kernel\n"
			    "- reboot\tReboot the kernel\n");
		else if (!strcmp(command, "exit"))
		    break;
		else if (!strcmp(command, "halt"))
		    call_halt();
		else if (!strcmp(command, "reboot"))
		    call_reboot();
		else if (!strcmp(command, "test"))
		    call_load("/ramdisk/mods/test.ko");
		else
		    if (strlen(command)) {
			call_puts(command);
			call_puts(": command not found.\n");
		    }
		shell_buffer_clear();
		continue;
	    }
	}
    }
    call_puts("Shell closed: bye bye!\n");
}
