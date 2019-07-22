SECTION .boot
BITS 16
GLOBAL start

start:
	cli
	
	mov [drive], dl
	
	mov ah, 01h
	mov ch, 3Fh
	int 10h	;Turn off the cursor
	
	mov ah, 13h
	mov al, 01h
	mov bh, 00h
	mov bl, 0Fh
	mov cx, 0Ah
	mov dh, 00h
	mov dl, 00h
	mov bp, loading
	int 10h ;Place the loading message onto the screen
	
	mov dl, [drive]
	
	mov ah, 02h
	mov al, 6
	mov ch, 0
	mov dh, 0
	mov cl, 2
	mov bx, loadLocation
	int 13h ;Load from disk

	mov ah, 00h
	mov al, 13h
	int 10h	;Graphics mode 13h (VGA 320x200)
	
	xor ax, ax
	push ax
	popf
	mov ds, ax
	lgdt [gdt_desc]
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp 08h:protectedModeBegin

loading:
	db "Loading...", 0
	
drive: db 0

BITS 32

protectedModeBegin:
	mov ax, 10h
	mov ds, ax
	mov ss, ax
	
	jmp loadLocation	

gdt:
gdt_null:
	dq 0
gdt_code:
	dw 0FFFFh
	dw 0
	db 0
	db 10011010b
	db 11001111b
	db 0
gdt_data:
	dw 0FFFFh
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0
gdt_end:

gdt_desc:
	dw gdt_end-gdt-1
	dd gdt

	times 510-($-$$) db 0
	dw 0AA55h
	
loadLocation:
	mov esp, kernelStackTop
	extern kernelMain
	call kernelMain
	cli
	hlt
	
SECTION .bss
ALIGN 4
kernelStackBottom: equ $
	RESB 16384
kernelStackTop: