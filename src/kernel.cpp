void print(char *str)
{
    unsigned short* videoMemory = (unsigned short*)0xb8000;
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

extern "C" void kernel_main(void* multiboot_structure, unsigned int magic)
{
    print("Hello World");
    while (1);
}