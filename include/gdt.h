#ifndef __GDT_H__
#define __GDT_H__

#include <types.h>

class GlobalDescriptorTable
{
public:
    /// @brief see https://wiki.osdev.org/Global_Descriptor_Table for details about the 
    /// structure of segment descriptor.
    class SegmentDescriptor
    {

    private:
        uint16_t limit_lo;
        uint16_t base_lo;
        uint8_t base_hi;
        uint8_t type; /*access*/
        uint8_t flags_limit_hi;
        uint8_t base_vhi;

    public:
        SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t access);
        uint32_t Base();
        uint32_t Limit();
    } __attribute__((packed));

    SegmentDescriptor nullSegmentDescriptor;
    SegmentDescriptor unusedSegmentDescriptor;
    /*kernel code segment*/
    SegmentDescriptor kernelCodeSegmentDescriptor;
    /*kernel data segment*/
    SegmentDescriptor kernelDataSegmentDescriptor;
public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();

    uint16_t CodeSegmentSelector();
    uint16_t DataSegmentSelector();
};

#endif