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
	mov al, 10
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
 
global load_idt
 
global irq0_handler
global irq1_handler
global irq2_handler
global irq3_handler
global irq4_handler
global irq5_handler
global irq6_handler
global irq7_handler
global irq8_handler
global irq9_handler
global irq10_handler
global irq11_handler
global irq12_handler
global irq13_handler
global irq14_handler
global irq15_handler
 
extern irq0_handler
extern irq1_handler
extern irq2_handler
extern irq3_handler
extern irq4_handler
extern irq5_handler
extern irq6_handler
extern irq7_handler
extern irq8_handler
extern irq9_handler
extern irq10_handler
extern irq11_handler
extern irq12_handler
extern irq13_handler
extern irq14_handler
extern irq15_handler
 
irq0:
  pusha
  call irq0_handler
  popa
  iret
 
irq1:
  pusha
  call irq1_handler
  popa
  iret
 
irq2:
  pusha
  call irq2_handler
  popa
  iret
 
irq3:
  pusha
  call irq3_handler
  popa
  iret
 
irq4:
  pusha
  call irq4_handler
  popa
  iret
 
irq5:
  pusha
  call irq5_handler
  popa
  iret
 
irq6:
  pusha
  call irq6_handler
  popa
  iret
 
irq7:
  pusha
  call irq7_handler
  popa
  iret
 
irq8:
  pusha
  call irq8_handler
  popa
  iret
 
irq9:
  pusha
  call irq9_handler
  popa
  iret
 
irq10:
  pusha
  call irq10_handler
  popa
  iret
 
irq11:
  pusha
  call irq11_handler
  popa
  iret
 
irq12:
  pusha
  call irq12_handler
  popa
  iret
 
irq13:
  pusha
  call irq13_handler
  popa
  iret
 
irq14:
  pusha
  call irq14_handler
  popa
  iret
 
irq15:
  pusha
  call irq15_handler
  popa
  iret
 
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