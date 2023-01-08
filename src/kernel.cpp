#include <types.h>
#include <gdt.h>

void print(char *str)
{
    uint16_t* videoMemory = (uint16_t*)0xb8000;
    for (int i=0; str[i]!='\0'; ++i)
    {
        videoMemory[i] = (videoMemory[i] & 0xFF00) | str[i];
    }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void call_constructors()
{
    for (constructor* i = &start_ctors; i != &end_ctors; ++i) (*i)();
} 

extern "C" void kernel_main(void* multiboot_structure, uint32_t /*magic*/)
{
    print("Hello World");

    GlobalDescriptorTable gdt;
    
    while (1);
}