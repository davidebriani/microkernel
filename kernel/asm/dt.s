; This will set up our new segment registers. We need to do
; something special in order to set CS. We do what is called a
; far jump. A jump that includes a segment as well as an offset.

[GLOBAL gdt_flush]	; Allows the C code to link to this

gdt_flush:
    mov eax, [esp+4]	; Get the pointer to the GDT, passed as a parameter
    lgdt [eax]		; Load the GDT

    mov ax, 0x10	; 0x10 is the offset in the GDT to our data segment
    mov ds, ax		; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush	; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret			; Returns back to the C code!


[GLOBAL idt_load]

idt_load:
    mov eax, [esp+4]	; Get the pointer to the IDT, passed as parameter
    lidt [eax]		; Load the IDT pointer
    ret
