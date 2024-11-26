#include <vector>
#include <cstdint>
#include <inttypes.h>
#include <string>
#include "memory.h"

enum FLAG {
 C = 0b10000000,
 Z = 0b01000000,
 I = 0b00100000,
 D = 0b00010000,
 B = 0b00001000,
 U = 0b00000100,
 V = 0b00000010,
 N = 0b00000001,
};

typedef struct Registers {
    uint16_t programCounter; 
    uint8_t stackPointer;
    uint8_t accumulatorRegister; 
    uint8_t indexYRegister;
    uint8_t indexXRegister;
    uint8_t processorStatusRegister;

    Registers() {
    };
    uint16_t getPC(bool increase = true) {
        if (increase) {
            programCounter++;
            return programCounter-1;
        }
        return programCounter;
    }
    void setPC(uint16_t prgrmAddress) {
        programCounter = prgrmAddress;
    }

    uint8_t getSP() {
        return stackPointer;
    }
    void setSP(uint8_t val) {
        stackPointer = val;
    }

    void pull() {
        stackPointer++;
    }
    void push() {
        stackPointer--;
    }

    uint8_t getA() {
        return accumulatorRegister;
    }
    void setA(uint8_t a) {
        accumulatorRegister = a;
    }
    
    uint8_t getX() {
        return indexXRegister;
    }
    void setX(uint8_t x) {
        indexXRegister = x;
    }

    uint8_t getY() {
        return indexYRegister;
    }
    void setY(uint8_t y) {
        indexYRegister = y;
    }

    uint8_t getFlags() {
        return processorStatusRegister;
    }
    uint8_t getFlag(FLAG flag) {
        return (processorStatusRegister & flag);
    }
    void setFlag(FLAG flag, bool value) {
        processorStatusRegister = value ?  (processorStatusRegister | flag) :
                                            (processorStatusRegister & ~flag);
    }
    void setFlags(uint8_t val) {
        processorStatusRegister = val;
    }
    
} Registers;

class CPU6502 {

private:
    struct Instruction {
        std::string name;
        uint8_t cycles;
        uint8_t(CPU6502::*addressingMode)(void);
        uint8_t(CPU6502::*execution)(void);
    };
    Page zeroPage;
    Page systemStack;
    Bus* m_bus = nullptr;
    std::vector<Instruction> instructionSet;

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t value);

    uint8_t IMP();  uint8_t LDA(); uint8_t PHP(); uint8_t INX(); uint8_t BCS();
    uint8_t ACC();  uint8_t LDX(); uint8_t PLA(); uint8_t INY(); uint8_t BEQ();
    uint8_t IMM();  uint8_t LDY(); uint8_t PLP(); uint8_t DEC(); uint8_t BMI();
    uint8_t ZPG();  uint8_t STA(); uint8_t AND(); uint8_t DEX(); uint8_t BNE();
    uint8_t ZPX();  uint8_t STX(); uint8_t EOR(); uint8_t DEY(); uint8_t BPL();
    uint8_t ZPY();  uint8_t STY(); uint8_t ORA(); uint8_t ASL(); uint8_t BVC();
    uint8_t REL();  uint8_t TAX(); uint8_t BIT(); uint8_t LSR(); uint8_t BVS();
    uint8_t ABS();  uint8_t TAY(); uint8_t ADC(); uint8_t ROL(); uint8_t CLC();
    uint8_t ABX();  uint8_t TXA(); uint8_t SBC(); uint8_t ROR(); uint8_t CLD();
    uint8_t ABY();  uint8_t TYA(); uint8_t CMP(); uint8_t JMP(); uint8_t CLI();
    uint8_t IND();  uint8_t TSX(); uint8_t CPX(); uint8_t JSR(); uint8_t CLV();
    uint8_t IDX();  uint8_t TXS(); uint8_t CPY(); uint8_t RTS(); uint8_t SEC();
    uint8_t IDY();  uint8_t PHA(); uint8_t INC(); uint8_t BCC(); uint8_t SED();
                    uint8_t SEI(); uint8_t BRK(); uint8_t NOP(); uint8_t RTI();

public:
    uint64_t m_clk;
    Registers registers;
    
    uint8_t fetched;
    uint16_t argAddressAbs;
    uint16_t argAddressRel;
    uint8_t execCycles = 0;
    Instruction currentInstruction;

    CPU6502();
    void addBus(Bus* bus);
    void clock();
    void reset();
    void irq();
    void nmi();

    uint8_t fetch();

};
