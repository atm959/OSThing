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

	jc .loadFailed

	mov ah, 00h
	mov al, 13h
	int 10h
	
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
	mov dh, 01h
	mov dl, 00h
	mov bp, couldNotLoad
	int 10h

	jmp halt

loading:
	db "Loading..."
loadingEnd:

couldNotLoad:
	db "Couldn't load... (Not booting from floppy disk?)"
couldNotLoadEnd:
	
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
	sti
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
	call copyState
	call clearScreen
	call dumpState
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
  call irq0_handler
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

eaxRAM: dd 0
ebxRAM: dd 0
ecxRAM: dd 0
edxRAM: dd 0
espRAM: dd 0

copyState:
	mov [eaxRAM], eax
	mov [ebxRAM], ebx
	mov [ecxRAM], ecx
	mov [edxRAM], edx
	mov [espRAM], esp
	ret

eaxText: db "EAX: ", 0
ebxText: db "EBX: ", 0
ecxText: db "ECX: ", 0
edxText: db "EDX: ", 0
espText: db "ESP: ", 0
instructionAddressText: db "'Timeframes are the best!' - MYCRAFTisbest", 0
stackTraceText: db "STACK TRACE: ", 0

extern print32Bit

dumpState:
	push DWORD 0Fh
	push DWORD 1
	push DWORD 0
	mov eax, eaxText
	push eax
	call placeText
	pop eax
	pop eax
	pop eax
	pop eax
	push DWORD 1
	push DWORD 5
	mov eax, [eaxRAM]
	push eax
	call print32Bit
	pop eax
	pop eax
	pop eax
	
	push DWORD 0Fh
	push DWORD 2
	push DWORD 0
	mov eax, ebxText
	push eax
	call placeText
	pop eax
	pop eax
	pop eax
	pop eax
	push DWORD 2
	push DWORD 5
	mov eax, [ebxRAM]
	push eax
	call print32Bit
	pop eax
	pop eax
	pop eax

	push DWORD 0Fh
	push DWORD 3
	push DWORD 0
	mov eax, ecxText
	push eax
	call placeText
	pop eax
	pop eax
	pop eax
	pop eax
	push DWORD 3
	push DWORD 5
	mov eax, [ecxRAM]
	push eax
	call print32Bit
	pop eax
	pop eax
	pop eax

	push DWORD 0Fh
	push DWORD 4
	push DWORD 0
	mov eax, edxText
	push eax
	call placeText
	pop eax
	pop eax
	pop eax
	pop eax
	push DWORD 4
	push DWORD 5
	mov eax, [edxRAM]
	push eax
	call print32Bit
	pop eax
	pop eax
	pop eax

	push DWORD 0Fh
	push DWORD 5
	push DWORD 0
	mov eax, espText
	push eax
	call placeText
	pop eax
	pop eax
	pop eax
	pop eax
	push DWORD 5
	push DWORD 5
	mov eax, [espRAM]
	push eax
	call print32Bit
	pop eax
	pop eax
	pop eax

	push DWORD 0Fh
	push DWORD 6
	push DWORD 0
	mov eax, instructionAddressText
	push eax
	call placeText
	pop eax
	pop eax
	pop eax
	pop eax

	push DWORD 0Fh
	push DWORD 8
	push DWORD 0
	mov eax, stackTraceText
	push eax
	call placeText
	pop eax
	pop eax
	pop eax
	pop eax
	ret
 
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