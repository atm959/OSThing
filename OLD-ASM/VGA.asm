MousePointerRegWhite:
	db 01h, 01h, 01h, 01h, 00h, 00h, 00h, 00h
	db 01h, 10h, 10h, 01h, 00h, 00h, 00h, 00h
	db 01h, 10h, 10h, 10h, 01h, 00h, 00h, 00h
	db 01h, 01h, 10h, 10h, 10h, 01h, 00h, 00h
	db 00h, 00h, 01h, 10h, 10h, 10h, 01h, 00h
	db 00h, 00h, 00h, 01h, 10h, 10h, 10h, 01h
	db 00h, 00h, 00h, 00h, 01h, 10h, 01h, 00h
	db 00h, 00h, 00h, 00h, 00h, 01h, 00h, 00h
MousePointerRegGray:
	db 01h, 01h, 01h, 01h, 00h, 00h, 00h, 00h
	db 01h, 09h, 09h, 01h, 00h, 00h, 00h, 00h
	db 01h, 09h, 09h, 09h, 01h, 00h, 00h, 00h
	db 01h, 01h, 09h, 09h, 09h, 01h, 00h, 00h
	db 00h, 00h, 01h, 09h, 09h, 09h, 01h, 00h
	db 00h, 00h, 00h, 01h, 09h, 09h, 09h, 01h
	db 00h, 00h, 00h, 00h, 01h, 09h, 01h, 00h
	db 00h, 00h, 00h, 00h, 00h, 01h, 00h, 00h
MousePointerRegRed:
	db 01h, 01h, 01h, 01h, 00h, 00h, 00h, 00h
	db 01h, 0Dh, 0Dh, 01h, 00h, 00h, 00h, 00h
	db 01h, 0Dh, 0Dh, 0Dh, 01h, 00h, 00h, 00h
	db 01h, 01h, 0Dh, 0Dh, 0Dh, 01h, 00h, 00h
	db 00h, 00h, 01h, 0Dh, 0Dh, 0Dh, 01h, 00h
	db 00h, 00h, 00h, 01h, 0Dh, 0Dh, 0Dh, 01h
	db 00h, 00h, 00h, 00h, 01h, 0Dh, 01h, 00h
	db 00h, 00h, 00h, 00h, 00h, 01h, 00h, 00h
MousePointerProhibit:
	db 00h, 00h, 0Dh, 0Dh, 0Dh, 0Dh, 00h, 00h
	db 00h, 0Dh, 00h, 00h, 00h, 00h, 0Dh, 00h
	db 0Dh, 00h, 00h, 00h, 00h, 0Dh, 00h, 0Dh
	db 0Dh, 00h, 00h, 00h, 0Dh, 00h, 00h, 0Dh
	db 0Dh, 00h, 00h, 0Dh, 00h, 00h, 00h, 0Dh
	db 0Dh, 00h, 0Dh, 00h, 00h, 00h, 00h, 0Dh
	db 00h, 0Dh, 00h, 00h, 00h, 00h, 0Dh, 00h
	db 00h, 00h, 0Dh, 0Dh, 0Dh, 0Dh, 00h, 00h
MousePointerRoundedBox:
	db 00h, 01h, 01h, 01h, 01h, 01h, 01h, 00h
	db 01h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db 01h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db 01h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db 01h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db 01h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db 01h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db 00h, 01h, 01h, 01h, 01h, 01h, 01h, 00h
MousePointerSharpBox:
	db 01h, 01h, 01h, 01h, 01h, 01h, 01h, 01h
	db 01h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db 01h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db 01h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db 01h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db 01h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db 01h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db 01h, 01h, 01h, 01h, 01h, 01h, 01h, 01h
MousePointerRedExclamation:
	db 00h, 00h, 00h, 0Dh, 0Dh, 00h, 00h, 00h
	db 00h, 00h, 00h, 0Dh, 0Dh, 00h, 00h, 00h
	db 00h, 00h, 00h, 0Dh, 0Dh, 00h, 00h, 00h
	db 00h, 00h, 00h, 0Dh, 0Dh, 00h, 00h, 00h
	db 00h, 00h, 00h, 0Dh, 0Dh, 00h, 00h, 00h
	db 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db 00h, 00h, 00h, 0Dh, 0Dh, 00h, 00h, 00h
	db 00h, 00h, 00h, 0Dh, 0Dh, 00h, 00h, 00h
MousePointerBlackExclamation:
	db 00h, 00h, 00h, 01h, 01h, 00h, 00h, 00h
	db 00h, 00h, 00h, 01h, 01h, 00h, 00h, 00h
	db 00h, 00h, 00h, 01h, 01h, 00h, 00h, 00h
	db 00h, 00h, 00h, 01h, 01h, 00h, 00h, 00h
	db 00h, 00h, 00h, 01h, 01h, 00h, 00h, 00h
	db 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db 00h, 00h, 00h, 01h, 01h, 00h, 00h, 00h
	db 00h, 00h, 00h, 01h, 01h, 00h, 00h, 00h
MousePointerCheckmark:
	db 00h, 00h, 00h, 00h, 00h, 00h, 03h, 03h
	db 00h, 00h, 00h, 00h, 00h, 00h, 03h, 03h
	db 00h, 00h, 00h, 00h, 00h, 03h, 03h, 00h
	db 03h, 03h, 00h, 00h, 00h, 03h, 03h, 00h
	db 03h, 03h, 03h, 00h, 03h, 03h, 00h, 00h
	db 00h, 03h, 03h, 03h, 03h, 03h, 00h, 00h
	db 00h, 00h, 03h, 03h, 03h, 00h, 00h, 00h
	db 00h, 00h, 00h, 03h, 03h, 00h, 00h, 00h

pointerX: dw 0
pointerY: dw 0
calculatedX: dw 0
calculatedY: dw 0

drawPointer:
	mov [tempX], WORD 0
.outerLoop:
	cmp [tempX], WORD 8
	je .done
	mov [tempY], WORD 0
.innerLoop:

	mov ecx, MousePointerProhibit
	mov edx, 0A0000h

	mov ax, [pointerX]
	mov bx, [tempX]
	add ax, bx
	mov [calculatedX], ax

	mov ax, [pointerY]
	mov bx, [tempY]
	add ax, bx
	mov [calculatedY], ax

	mov eax, DWORD 0
	mov ax, [calculatedY]
	mov bx, 320
	mul bx
	mov bx, [calculatedX]
	add ax, bx
	add edx, eax

	mov eax, DWORD 0
	mov ax, [tempY]
	mov bl, 8
	mul bl
	mov bx, [tempX]
	add ax, bx
	add ecx, eax

	mov cl, BYTE [ecx]
	cmp cl, 00
	je .transparent
	sub cl, BYTE 1
	mov [edx], cl

.transparent:

	cmp [tempY], WORD 7
	je .innerLoopDone
	inc WORD [tempY]
	jmp .innerLoop

.innerLoopDone:
	inc WORD [tempX]
	jmp .outerLoop
.done:
	ret

counter2: dw 0

copyImageToVGABuff:
	mov eax, start
	mov ebx, 0A0000h
	mov [counter2], WORD CodeEnd-start
.copyImageLoop:
	mov cl, [eax]
	mov [ebx], cl

	sub [counter2], WORD 1
	add eax, DWORD 1
	add ebx, DWORD 1

	cmp [counter2], WORD -1
	jne .copyImageLoop

	ret

clearVGABuff:
	mov ecx, 0A0000h
	mov ax, (320*200)
.loop:
	mov [ecx], BYTE 0Fh
	add ecx, 01h
	sub ax, 01h
	cmp ax, 00h
	je .doneClearingVGABuff
	jmp .loop
.doneClearingVGABuff:
	ret