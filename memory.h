#include <inttypes.h>
#include <cstdlib>
#include <stdexcept>


struct Page {
    uint16_t startAddress;
    Page() {}
    Page(uint16_t address) {
        startAddress = address;
    }
    uint16_t getPageAddress() {
        return startAddress;
    }
    uint16_t getPageAddressEnd() {
        return startAddress + 0xFF;
    }
};

class Memory {

    private:
    uint32_t size = 0x10000;
    uint8_t* memorymap;

    public:
    Page zeroPage;
    Page systemStack;
    Memory (); 
    Memory(uint32_t nbytes); 

    uint8_t* getMemory();
    uint8_t& operator[] (uint16_t addr); 
};

struct Bus {
    Memory* ram;
    uint8_t read(uint16_t addr) {
        if (addr >= 0x0000 && addr <= 0xFFFF)
            return (*ram)[addr];
        return 0x00;
    };
    void write(uint16_t addr, uint8_t value) {
        if (addr >= 0x0000 && addr <= 0xFFFF)
            ram[addr] = value;
    }
    void setRam(Memory* memory) {
        ram = memory;    
    }
};

