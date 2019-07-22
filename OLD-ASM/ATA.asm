;Detects if there is a drive connected
;AL - Result
;    00 - At least one drive exists
;    FF - No drives exist
detectDrive:
    mov dx, 1F7h        ;The ATA Status Register Port
    in al, dx           ;Into AL
    cmp al, 0FFh        ;Compare that with FFh, which is an invalid status byte
    je .driveIsInvalid  ;If they match, there are no drives(?)

    mov al, 00h         ;At least one drive exists(?)
    ret

.driveIsInvalid:
    mov al, 0FFh        ;No drives exist(?)
    ret

initDrive:
    mov dx, 1F6h
    mov al, 0A0h
    out dx, al

    mov al, 00h
    mov dx, 1F2h
    out dx, al
    inc dx
    out dx, al
    inc dx
    out dx, al
    inc dx
    out dx, al
    
    mov dx, 01F7h
    mov al, 0ECh
    out dx, al

    in al, dx
    cmp al, BYTE 00h
    je .driveDoesntExist

.loopUntilBSYClears:
    in al, dx
    cmp al, BYTE 80h
    je .loopUntilBSYClears

    mov dx, 1F4h
    in al, dx
    cmp al, BYTE 00h
    jne .driveIsntATA
    inc dx
    in al, dx
    cmp al, BYTE 00h
    jne .driveIsntATA

    mov dx, 1F7h

.loopUntilDRQOrERRSets:
    in al, dx
    cmp al, BYTE 0
    jne .loopUntilDRQOrERRSets

    ret

.driveDoesntExist:
    mov al, 0FFh
    ret

.driveIsntATA:
    mov al, 0FEh
    ret