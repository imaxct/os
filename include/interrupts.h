#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <types.h>
#include <port.h>
#include <gdt.h>

class InterruptManager
{
protected:
    struct GateDescriptor
    {
        uint16_t handlerAddrLowBits;
        uint16_t gdt_codeSegSelector;
        uint8_t reserved;
        uint8_t access;
        uint16_t handlerAddrHighBits;
    } __attribute__((packed));

    struct InterruptDescriptorTablePointer
    {
        uint16_t size;
        uint32_t base;
    } __attribute__((packed));
    
    static GateDescriptor interruptDescriptorTable[256];

    static void SetInterruptDescriptorTableEntry(
        uint8_t interruptNumber,
        uint16_t gdt_codeSegSelectorOffset,
        void (*handler)(),
        uint8_t descriptorPrivilegeLevel,
        uint8_t descriptorType
    );

public:
    InterruptManager(GlobalDescriptorTable* gdt);
    ~InterruptManager();

    void Activate();

    static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);

    static void IgnoreInterruptRequest();
    static void HandleInterruptRequest0x00();
    static void HandleInterruptRequest0x01();
};
#endif