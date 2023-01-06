ASPARAMS = --32
GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
objects = obj/boot.o

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) $< -o $@

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	gcc $(CPPPARAMS) -c $< -o $@

build: $(objects)