[GLOBAL cli]
[GLOBAL sti]
[GLOBAL halt]

cli:
    cli		; Disable interrupts
    ret

sti:
    sti		; Enable interrupts
    ret

halt:
    hlt		; This should kernel return
    jmp halt
