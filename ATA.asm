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
