#include "memory.h"

Memory::Memory() {
    memorymap = (uint8_t*)malloc(sizeof(uint8_t)*size);
    zeroPage = Page(0x0000);
    systemStack = Page(0x0100);
}

Memory::Memory(uint32_t nbytes) {
    size = nbytes;
    memorymap = (uint8_t*)malloc(sizeof(uint8_t)*size);
}

uint8_t* Memory::getMemory() {
    return memorymap;
}
uint8_t& Memory::operator[] (uint16_t addr) {
    if (addr >= 0x0 && addr <= size)
        return memorymap[addr];
    throw std::exception(std::out_of_range("Address out of range"));
}
