%define APM

; Error codes.
%define APM_ERROR_DISABLED 01h
%define APM_ERROR_CONNECTED 02h
%define APM_ERROR_NOT_CONNECTED 03h
%define APM_ERROR_RMODE_NOT_CONNECTED 04h
%define APM_ERROR_BAD_DEVICE_ID 09h
%define APM_ERROR_INVALID_CX 0Ah
%define APM_ERROR_NOT_PRESENT 86h

[GLOBAL apm_check_exist]
apm_check_exist:
    mov ax,5300h
    xor bx,bx
    int 15h
    jc apm_error
    mov [apm_flags],cx
    mov [apm_version_major],ah
    mov [apm_version_minor],al
    ret

[GLOBAL apm_connect]
apm_connect:
    mov ax,5303h ; 5301 Real Mode, 5302 16bit Pr. Mode, 5303 32bit Pr. Mode
    xor bx,bx
    int 15h
    jc apm_error
    ret

[GLOBAL apm_disconnect]
apm_disconnect:
    mov ax,5304h
    xor bx,bx
    int 15h
    jc .disconnect_error
    jmp .no_error
.disconnect_error:
    cmp ah,APM_ERROR_NOT_CONNECTED
    jne apm_error
.no_error:
    ret

[GLOBAL apm_get_power_status]
apm_get_power_status:
    mov ax,530Ah
    mov bx,0001h
    int 15h
    jc .done
    mov [apm_ac_status],bh
    mov [apm_battery_status],bl
    mov [apm_battery_percent],cl
.done:
    ret

[GLOBAL apm_enable_power_management]
apm_enable_power_management:
    mov ax,5308h
    mov bx,0001h
    mov cx,bx
    int 15h
    jc apm_error
    ret

[GLOBAL apm_enable_device_power_management]
apm_enable_device_power_management:
    mov ax,530Dh
    mov bx,0001h
    mov cx,bx
    int 15h

[GLOBAL apm_engage_power_management]
apm_engage_power_management:
    mov ax,530Fh
    mov bx,0001h
    mov cx,bx
    int 15h
    ret

[GLOBAL apm_driver_version]
apm_driver_version:
    mov ax,530Eh
    xor bx,bx
    pop cx
    int 15h
    ret

[GLOBAL apm_poweroff]
apm_poweroff:
    cmp byte [apm_version_minor],02h
    jge .go
    stc
    ret
.go:
    mov ax,5307h
    mov bx,0001h
    mov cx,0003h
    int 15h
    clc     ; Clear carry in case of failure.
    ret     ; It really shouldn't return...

[GLOBAL apm_suspend]
apm_suspend:
    cmp byte [apm_version_minor],02h
    jge .go
    stc
    ret
.go:
    mov ax,5307h
    mov bx,0001h
    mov cx,0002h
    int 15h
    clc     ; Clear carry in case of failure.
    ret     ; It really shouldn't return...

[GLOBAL apm_standby]
apm_standby:
    cmp byte [apm_version_minor],02h
    jge .go
    stc
    ret
.go:
    mov ax,5307h
    mov bx,0001h
    mov cx,0001h
    int 15h
    clc     ; Clear carry in case of failure.
    ret     ; It really shouldn't return...

apm_error:
	xor eax,eax
    ret

section .data
    apm_flags dw 0
    apm_version_major db 0
    apm_version_minor db 0
    apm_support_major db 01h
    apm_support_minor db 00h
    apm_ac_status db 0
    apm_battery_status db 0
    apm_battery_percent db 0
section .text
