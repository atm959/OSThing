export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

nasm -f elf32 main.asm -o main.o
$TARGET-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
$TARGET-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib main.o kernel.o -lgcc
dd status=noxfer conv=notrunc if=myos.bin of=osthing.img