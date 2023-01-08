#include <types.h>
#include <gdt.h>
#include <port.h>

void print(char *str)
{
    static uint16_t* videoMemory = (uint16_t*)0xb8000;
    static uint8_t x = 0, y = 0;
    static uint8_t col = 80, line = 25;
    for (int i=0; str[i]!='\0'; ++i)
    {
        switch (str[i])
        {
            case '\n':
                y++;
                x = 0;
                break;
            default:
                videoMemory[col * y + x] = (videoMemory[col * y + x] & 0xFF00) | str[i];
                x++;
                break;
        }
        
        if (x >= col)
        {
            x = 0;
            y++;
        }

        if (y >= line)
        {
            for (y = 0; y < line; y++)
                for (x = 0; x < col; x++)
                    videoMemory[col * y + x] = (videoMemory[col * y + x] & 0xFF00) | ' ';
            
            y = 0;
            x = 0;
        }
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
    print("Hello World\n");
    print("Hello World\nThe third line.");

    GlobalDescriptorTable gdt;

    while (1);
}