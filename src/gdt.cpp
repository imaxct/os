#include <gdt.h>

GlobalDescriptorTable::GlobalDescriptorTable()
: nullSegmentDescriptor(0, 0, 0),
unusedSegmentDescriptor(0, 0, 0),
codeSegmentDescriptor(0, 64*1024*1024, 0x9A),
dataSegmentDescriptor(0, 64*1024*1024, 0x92)
{
    uint32_t i[2];
    i[0] = (uint32_t)this;
    i[1] = sizeof(GlobalDescriptorTable) << 16;

    asm volatile("lgdt (%0)": :"p" (((uint8_t*)i) + 2));
}

GlobalDescriptorTable::~GlobalDescriptorTable()
{}

uint16_t GlobalDescriptorTable::DataSegmentSelector()
{
    return (uint8_t*)&dataSegmentDescriptor - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::CodeSegmentSelector()
{
    return (uint8_t*)&codeSegmentDescriptor - (uint8_t*)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t access)
{
    uint8_t* target = (uint8_t*)this;

    // if the segment size is no more than 2-bytes size,
    // use byte granularity, 1 "limit" equals to 1 byte,
    // otherwise, use page granularity, 1 "limit" equals to 4 KiB.
    // see DB bit in Flags item.
    if (limit <= 0x10000)
    {
        // set flags
        target[6] = 0x40;
    }
    else
    {
        // 4KiB = 0xFFF = 2^12, so here uses 0xFFF and shifts 12.
        if ((limit & 0xFFF) != 0xFFF)
            limit = (limit >> 12) - 1;
        else
            limit = limit >> 12;
        
        // set flags
        target[6] = 0xC0;
    }

    // set limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

    // set base
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // set access
    target[5] = access;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Base()
{
    uint32_t base_addr = this->base_vhi;
    base_addr = (base_addr << 8) + this->base_hi;
    base_addr = (base_addr << 8) + this->base_lo;
    return base_addr;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit()
{
    uint32_t result = this->flags_limit_hi & 0xF;
    result = (result << 16) + this->limit_lo;

    if ( (this->flags_limit_hi & 0xC0) == 0xC0)
    {
        result = (result << 12) | 0xFFF;
    }

    return result;
}