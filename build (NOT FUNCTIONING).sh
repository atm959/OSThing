(This build script is non-functioning)

nasm -f elf32 main.asm -o main.o
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib main.o kernel.o -lgcc
dd status=noxfer conv=notrunc if=myos.bin of=osthing.img
