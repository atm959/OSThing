BITS 16
ORG 7C00h

start:
	cli
	
	mov [drive], dl
	
	mov ah, 01h
	mov ch, 3Fh
	int 10h
	
	mov ah, 13h
	mov al, 01h
	mov bh, 00h
	mov bl, 0Fh
	mov cx, 0Ah
	mov dh, 00h
	mov dl, 00h
	mov bp, loading
	int 10h

	;mov ah, 00h
	;mov al, 13h
	;int 10h
	
	mov dl, [drive]
	
	mov ah, 02h
	mov al, (CodeEnd-start)/512
	mov ch, 0
	mov dh, 0
	mov cl, 2
	mov bx, loadLocation
	int 13h
	
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
	mov esp, 90000h
	
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

idtbegin:
zerodivide:
	dw 0000h
	dw 0008h
	db 00h
	db 8Eh
	dw 0000h
idtend:

idt:
	dw idtend-idtbegin-1
	dd idtbegin

	times 510-($-$$) db 0
	dw 0AA55h
	
loadLocation:
	mov eax, zeroDivide_exception
	and eax, 0000FFFFh
	mov [zerodivide], ax
	mov eax, zeroDivide_exception
	and eax, 0FFFF0000h
	mov [zerodivide+6], ax
	lidt [idt]
	
	mov bl, 00h
	;div bl		;Cause a divide-by-zero exception ;)
	
	call clearTextScreen

	mov eax, 00000000h

	call detectDrive
	call initDrive

	mov ecx, eax
	mov ax, 00h
	mov bx, 00h
	call print32Bit
.loop:
	
	jmp .loop

lp0onfire:
	db "Test of using a hard disc drive.", 0

tempx: dw 0
tempy: dw 0
tempattrib: db 0
tempval: dq 0

;esi - Pointer to text
;ax - X Pos.
;bx - Y Pos.
;cx - Attrib.
placeText:
	mov [tempattrib], cl
	mov [tempx], ax
	mov ax, bx
	mov bx, 160
	mul bx
	mov [tempy], ax
	mov ax, [tempx]
	mov bx, 2
	mul bx
	mov bx, ax
	mov ax, [tempy]
	add eax, ebx
	
	mov ecx, 0B8000h
	add ecx, eax
.placeTextLoop:
	lodsb
	cmp al, 00h
	je .donePlacingText
	mov [ecx], BYTE al
	add ecx, 1h
	mov dx, [tempattrib]
	mov [ecx], dx
	add ecx, 1h
	jmp .placeTextLoop
.donePlacingText:
	ret
	
clearTextScreen:
	mov ecx, 0B8000h
	mov ax, (80*25)*2
.loop:
	mov [ecx], BYTE 00h
	add ecx, 01h
	sub ax, 01h
	cmp ax, 00h
	je .doneClearingTextScreen
	jmp .loop
.doneClearingTextScreen:
	ret
	
divideByZero:
	db "DIVIDE-BY-ZERO EXCEPTION", 0
pcTxt:
	db "PC: ", 0
espTxt:
	db "ESP: ", 0
eaxTxt:
	db "EAX: ", 0
ebxTxt:
	db "EBX: ", 0
ecxTxt:
	db "ECX: ", 0
edxTxt:
	db "EDX: ", 0
hexTable:
	db "0123456789ABCDEF", 0
	
temppc: dd 0
tempesp: dd 0
tempeax: dd 0
tempebx: dd 0
tempecx: dd 0
tempedx: dd 0

copyRegsToMem:
	mov [tempeax], eax
	mov [tempebx], ebx
	mov [tempecx], ecx
	mov [tempedx], edx
	pop ebx ;Pop the return address from calling this subroutine into EBX
	pop eax
	mov [temppc], eax
	push ebx ;Push the return address back onto the stack
	mov [tempesp], esp
	ret

zeroDivide_exception:
	call copyRegsToMem

	call clearTextScreen
	mov si, divideByZero
	mov ax, 00h
	mov bx, 00h
	mov cx, 0Ch
	call placeText

	call showRegisters
	
	jmp hang
	
showRegisters:
	mov si, pcTxt
	mov ax, 00h
	mov bx, 01h
	mov cx, 0Ch
	call placeText
	mov si, espTxt
	mov ax, 00h
	mov bx, 02h
	mov cx, 0Ch
	call placeText
	mov si, eaxTxt
	mov ax, 00h
	mov bx, 03h
	mov cx, 0Ch
	call placeText
	mov si, ebxTxt
	mov ax, 00h
	mov bx, 04h
	mov cx, 0Ch
	call placeText
	mov si, ecxTxt
	mov ax, 00h
	mov bx, 05h
	mov cx, 0Ch
	call placeText
	mov si, edxTxt
	mov ax, 00h
	mov bx, 06h
	mov cx, 0Ch
	call placeText

	mov ax, 05h
	mov bx, 01h
	mov ecx, [temppc]
	call print32Bit
	mov ax, 05h
	mov bx, 02h
	mov ecx, [tempesp]
	call print32Bit
	mov ax, 05h
	mov bx, 03h
	mov ecx, [tempeax]
	call print32Bit
	mov ax, 05h
	mov bx, 04h
	mov ecx, [tempebx]
	call print32Bit
	mov ax, 05h
	mov bx, 05h
	mov ecx, [tempecx]
	call print32Bit
	mov ax, 05h
	mov bx, 06h
	mov ecx, [tempedx]
	call print32Bit
	ret

tempVal: dd 0
andVal: dd 0
shiftVal: db 0
counter: dd 0

tempX: dw 0
tempY: dw 0

hexString: db "00000000H", 0

;ax - X Pos.
;bx - Y Pos.
;ecx - Value to print
print32Bit:
	mov [tempX], ax
	mov [tempY], bx
	mov [tempVal], ecx
	mov [shiftVal], BYTE 28
	mov [andVal], DWORD 0F0000000h
	mov [counter], BYTE 00h

.copyToStringLoop:
	mov eax, [tempVal]
	and eax, [andVal]
	mov cl, [shiftVal]
	shr DWORD eax, cl

	mov ebx, hexTable
	add ebx, eax

	mov ecx, hexString
	mov edx, [counter]
	add ecx, edx
	mov dl, [ebx]
	mov [ecx], dl

	sub [shiftVal], BYTE 4
	shr DWORD [andVal], BYTE 4
	add [counter], DWORD 1

	cmp [counter], DWORD 8
	jne .copyToStringLoop
	
	mov ax, [tempX]
	mov bx, [tempY]
	mov cl, 0Fh
	mov si, hexString
	call placeText

	ret

hang:
	jmp hang

%INCLUDE "VGA.asm"
%INCLUDE "ATA.asm"

CodeEnd: