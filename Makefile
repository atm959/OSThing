GCCPARAMS = -Iinclude -m32 -fno-pie -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
NASMPARAMS = -f elf32
LDPARAMS = -melf_i386
DDPARAMS = status=noxfer conv=notrunc

objects = obj/loader.o obj/kernel.o

obj/%.o: src/%.cpp
	g++ $(GCCPARAMS) -c -o $@ $<

obj/%.o: src/%.s
	nasm $(NASMPARAMS) -o $@ $<

built/atmos.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

built/atmos.img: built/atmos.bin
	dd $(DDPARAMS) if=$< of=$@