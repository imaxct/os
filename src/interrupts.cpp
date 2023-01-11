#include <interrupts.h>
void print(char *str);

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

void InterruptManager::SetInterruptDescriptorTableEntry(
        uint8_t interruptNumber,
        uint16_t gdt_codeSegSelectorOffset,
        void (*handler)(),
        uint8_t descriptorPrivilegeLevel,
        uint8_t descriptorType
    )
{
    /*0x80 equals to 0b10000000, this is to set present bit as 1 to indicate this entry is valid.*/ 
    const uint8_t IDT_DESC_PRESENT = 0x80;

    // see https://wiki.osdev.org/IDT for details about interrupt descriptor table entry structure.
    interruptDescriptorTable[interruptNumber].handlerAddrLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handlerAddrHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].gdt_codeSegSelector = gdt_codeSegSelectorOffset;
    interruptDescriptorTable[interruptNumber].access 
        = IDT_DESC_PRESENT | descriptorType | ((descriptorPrivilegeLevel & 0b11) << 5);
    interruptDescriptorTable[interruptNumber].reserved = 0;
}


InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
{
    uint16_t codeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for (uint16_t i = 0; i < 256; ++i)
    {
        SetInterruptDescriptorTableEntry(i, codeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }

    SetInterruptDescriptorTableEntry(0x20, codeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x21, codeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

    InterruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt));
}

InterruptManager::~InterruptManager()
{}

void InterruptManager::Activate()
{
    asm("sti");
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    print("INTERRUPT\n");
    return esp;
}
