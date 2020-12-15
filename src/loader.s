SECTION .boot
BITS 16
GLOBAL start

start:	
	mov [drive], dl

	mov ax, 8000h
	mov ss, ax
	mov sp, 4096
	mov ax, 07C0h
	mov ds, ax

	mov ah, 01h
	mov ch, 3Fh
	int 10h	;Turn off the cursor
	
	mov ah, 13h
	mov al, 01h
	mov bh, 00h
	mov bl, 0Fh
	mov cx, loadingEnd-loading
	mov dh, 00h
	mov dl, 00h
	mov bp, loading
	int 10h ;Place the loading message onto the screen
	
	mov dl, [drive]

	mov ah, 02h
	mov al, 63
	mov ch, 0
	mov dh, 0
	mov cl, 2
	mov bx, loadLocation
	int 13h ;Load from disk

	jc .loadFailed ;Carry flag is set if the load failed

	xor ax, ax
	push ax
	popf
	mov ds, ax
	lgdt [gdt_desc]
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp 08h:protectedModeBegin

.loadFailed:
	mov ah, 13h
	mov al, 01h
	mov bh, 00h
	mov bl, 0Fh
	mov cx, couldNotLoadEnd-couldNotLoad
	mov dh, 02h
	mov dl, 00h
	mov bp, couldNotLoad
	int 10h	; "Couldn't load" text
	mov ah, 13h
	mov al, 01h
	mov bh, 00h
	mov bl, 0Ch
	mov cx, driveStatusByteTextEnd-driveStatusByteText
	mov dh, 03h
	mov dl, 00h
	mov bp, driveStatusByteText
	int 10h ;"Drive Status Byte" text

	;Get the drive status, put into AH
	mov ah, 01h
	mov dl, [drive]
	int 13h

	mov dx, WORD 0000h

	mov bx, hexTable		;Address of the hex table
	mov cx, hexTable		;Address of the hex table

	mov dl, ah		;Copy the status byte into DX
	and dl, 0F0h	;Mask off the 4 most-significant bits
	shr dl, 04h		;Shift it right by 4 bits
	add bx, dx		;Add the value to the hex table address for the first digit

	mov dl, ah		;Copy the status byte into DX
	and dl, 0Fh		;Mask off the 4 most-significant bits
	add cx, dx		;Add the value to the hex table address for the second digit

	mov [temp], cx

	mov bp, bx
	mov ah, 13h
	mov al, 01h
	mov bh, 00h
	mov bl, 0Ch
	mov cx, 1
	mov dh, 03h
	mov dl, 13h
	int 10h ;Drive Status Byte first digit

	mov bx, [temp]

	mov bp, bx
	mov ah, 13h
	mov al, 01h
	mov bh, 00h
	mov bl, 0Ch
	mov cx, 1
	mov dh, 03h
	mov dl, 14h
	int 10h ;Drive Status Byte second digit

	jmp halt

loading:
	db "Loading..."
loadingEnd:

couldNotLoad:
	db "Couldn't load!"
couldNotLoadEnd:

driveStatusByteText:
	db "Drive Status Byte: "
driveStatusByteTextEnd:

hexTable:
	db "0123456789ABCDEF"
	
drive: db 0
temp: db 0

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

global zeroDivide
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

extern irq0_handler
 
global load_idt

extern clearScreen
extern placeText

zeroDivideText:
	db "DIVIDE-BY-ZERO EXCEPTION", 0

zeroDivide:
	call clearScreen
	push DWORD 0Ch
	push DWORD 0
	push DWORD 0
	mov eax, zeroDivideText
	push eax
	call placeText
	pop eax
	pop eax
	pop eax
	pop eax
	jmp halt

irq0:
  pusha
  
  popa
  iret
 
irq1:
  pusha

  popa
  iret
 
irq2:
  pusha
  
  popa
  iret
 
irq3:
  pusha
  
  popa
  iret
 
irq4:
  pusha
  
  popa
  iret
 
irq5:
  pusha
  
  popa
  iret
 
irq6:
  pusha
  
  popa
  iret
 
irq7:
  pusha
  
  popa
  iret
 
irq8:
  pusha
  
  popa
  iret
 
irq9:
  pusha
  
  popa
  iret
 
irq10:
  pusha
  
  popa
  iret
 
irq11:
  pusha
  
  popa
  iret
 
irq12:
  pusha
  
  popa
  iret
 
irq13:
  pusha
  
  popa
  iret
 
irq14:
  pusha
  
  popa
  iret
 
irq15:
  pusha
  
  popa
  iret

global halt

halt:
	cli
	hlt
 
load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti
	ret

SECTION .bss
ALIGN 4
kernelStackBottom: equ $
	RESB 16384
kernelStackTop: