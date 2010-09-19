; Service Routines (ISRs)
[GLOBAL isr0]
[GLOBAL isr1]
[GLOBAL isr2]
[GLOBAL isr3]
[GLOBAL isr4]
[GLOBAL isr5]
[GLOBAL isr6]
[GLOBAL isr7]
[GLOBAL isr8]
[GLOBAL isr9]
[GLOBAL isr10]
[GLOBAL isr11]
[GLOBAL isr12]
[GLOBAL isr13]
[GLOBAL isr14]
[GLOBAL isr15]
[GLOBAL isr16]
[GLOBAL isr17]
[GLOBAL isr18]
[GLOBAL isr19]
[GLOBAL isr20]
[GLOBAL isr21]
[GLOBAL isr22]
[GLOBAL isr23]
[GLOBAL isr24]
[GLOBAL isr25]
[GLOBAL isr26]
[GLOBAL isr27]
[GLOBAL isr28]
[GLOBAL isr29]
[GLOBAL isr30]
[GLOBAL isr31]
[GLOBAL isr128]

;  0: Divide By Zero Exception
isr0:
    cli
    push byte 0		; A normal ISR stub that pops a dummy error code
    push 0		; to keep a uniform stack frame
    jmp isr_common_stub

;  1: Debug Exception
isr1:
    cli
    push byte 0
    push 1
    jmp isr_common_stub

;  2: Non Maskable Interrupt Exception
isr2:
    cli
    push byte 0
    push 2
    jmp isr_common_stub

;  3: Breakpoint Exception
isr3:
    cli
    push byte 0
    push 3
    jmp isr_common_stub

;  4: Into Detected Overflow Exception
isr4:
    cli
    push byte 0
    push 4
    jmp isr_common_stub

;  5: Out of Bounds Exception
isr5:
    cli
    push byte 0
    push 5
    jmp isr_common_stub

;  6: Invalid Opcode Exception
isr6:
    cli
    push byte 0
    push 6
    jmp isr_common_stub

;  7: No Coprocessor Exception
isr7:
    cli
    push byte 0
    push 7
    jmp isr_common_stub

;  8: Double Fault Exception (With Error Code!)
isr8:
    cli
    push 8		; We DON'T push a value on the stack here, it pushes one already!
    jmp isr_common_stub

;  9: Coprocessor Segment Overrun Exception
isr9:
    cli
    push byte 0
    push 9
    jmp isr_common_stub

;  10: Bad TSS Exception
isr10:
    cli
    push 10	; We DON'T push a value on the stack here, it pushes one already!
    jmp isr_common_stub

;  11: Segment Not Present Exception
isr11:
    cli
    push 11	; We DON'T push a value on the stack here, it pushes one already!
    jmp isr_common_stub

;  12: Stack Fault Exception
isr12:
    cli
    push 12	; We DON'T push a value on the stack here, it pushes one already!
    jmp isr_common_stub

;  13: General Protection Fault Exception
isr13:
    cli
    push 13	; We DON'T push a value on the stack here, it pushes one already!
    jmp isr_common_stub

;  14: Page Fault Exception
isr14:
    cli
    push 14	; We DON'T push a value on the stack here, it pushes one already!
    jmp isr_common_stub

;  15: Unknown Interrupt Exception
isr15:
    cli
    push byte 0
    push 15
    jmp isr_common_stub

;  16: Coprocessor Fault Exception
isr16:
    cli
    push byte 0
    push 16
    jmp isr_common_stub

;  17: Alignment Check Exception (486+)
isr17:
    cli
    push byte 0
    push 17
    jmp isr_common_stub

;  18: Machine Check Exception (Pentium/586+)
isr18:
    cli
    push byte 0
    push 18
    jmp isr_common_stub

; From here, Reversed Exceptions
isr19:
    cli
    push byte 0
    push 19
    jmp isr_common_stub

isr20:
    cli
    push byte 0
    push 20
    jmp isr_common_stub

isr21:
    cli
    push byte 0
    push 21
    jmp isr_common_stub

isr22:
    cli
    push byte 0
    push 22
    jmp isr_common_stub

isr23:
    cli
    push byte 0
    push 23
    jmp isr_common_stub

isr24:
    cli
    push byte 0
    push 24
    jmp isr_common_stub

isr25:
    cli
    push byte 0
    push 25
    jmp isr_common_stub

isr26:
    cli
    push byte 0
    push 26
    jmp isr_common_stub

isr27:
    cli
    push byte 0
    push 27
    jmp isr_common_stub

isr28:
    cli
    push byte 0
    push 28
    jmp isr_common_stub

isr29:
    cli
    push byte 0
    push 29
    jmp isr_common_stub

isr30:
    cli
    push byte 0
    push 30
    jmp isr_common_stub

isr31:
    cli
    push byte 0
    push 31
    jmp isr_common_stub

isr128:
    cli
    push byte 0
    push 128
    jmp isr_common_stub

; We call a C function in here. We need to let the assembler know
; that 'isr_handler' exists in another file
[EXTERN isr_handler]

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level isr_handler,
; and finally restores the stack frame.
isr_common_stub:
    pusha
    mov ax, ds
    push eax

    mov ax, 0x10	; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

call isr_handler
    ;mov eax, isr_handler
    ;call eax		; A special call, preserves the 'eip' register

    pop eax
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa
    add esp, 8		; Cleans up the pushed error code and pushed ISR number
    sti
    iret		; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!
