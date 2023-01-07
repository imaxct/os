ASPARAMS = --32
GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
LDPARAMS = -melf_i386

objects = obj/boot.o \
			obj/kernel.o

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) $< -o $@

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

obj/kernel.bin: link.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

kernel.iso: grub.cfg obj/kernel.bin
	mkdir -p iso/boot/grub
	cp obj/kernel.bin iso/boot
	cp grub.cfg iso/boot/grub/
	grub-mkrescue --output=$@ iso
	rm -rf iso

run: kernel.iso


clean:
	rm -rf obj/*.o
	rm -rf obj/*.bin
	rm -rf kernel.iso