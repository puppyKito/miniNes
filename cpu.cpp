#include <cstdint>
#include <inttypes.h>
#include <exception>
#include <iostream>
#include <stdexcept>
#include "cpu.h"

CPU6502::CPU6502() {
    using c = CPU6502;
    // Shamelessly "inspired" from One Line Coder
    instructionSet = {
    {"BRK", 7, &c::IMP, &c::BRK}, {"ORA", 6, &c::IDX, &c::ORA}, {"???", 1, &c::IMP, &c::NOP}, //$02
    {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, {"ORA", 3, &c::ZPG, &c::ORA}, //$05
    {"ASL", 5, &c::ZPG, &c::ASL}, {"???", 1, &c::IMP, &c::NOP}, {"PHP", 3, &c::IMP, &c::PHP}, //$08
    {"ORA", 2, &c::IMM, &c::ORA}, {"ASL", 2, &c::ACC, &c::ASL}, {"???", 1, &c::IMP, &c::NOP}, //$0B
    {"???", 1, &c::IMP, &c::NOP}, {"ORA", 4, &c::ABS, &c::ORA}, {"ASL", 6, &c::ABS, &c::ASL}, //$0E
    {"???", 1, &c::IMP, &c::NOP}, {"BPL", 2, &c::REL, &c::BPL}, {"ORA", 5, &c::IDY, &c::ORA}, //$11
    {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, //$14
    {"ORA", 4, &c::ZPX, &c::ORA}, {"ASL", 6, &c::ZPX, &c::ASL}, {"???", 1, &c::IMP, &c::NOP}, //$17
    {"CLC", 2, &c::IMP, &c::CLC}, {"ORA", 4, &c::ABY, &c::ORA}, {"???", 1, &c::IMP, &c::NOP}, //$1A
    {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, {"ORA", 4, &c::ABY, &c::ORA}, //$1D
    {"ASL", 7, &c::ABX, &c::ASL}, {"???", 1, &c::IMP, &c::NOP}, {"JSR", 6, &c::ABS, &c::JSR}, //$20
    {"AND", 6, &c::IDX, &c::AND}, {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, //$23
    {"BIT", 3, &c::ZPG, &c::BIT}, {"AND", 3, &c::ZPG, &c::AND}, {"ROL", 5, &c::ZPG, &c::ROL}, //$26
    {"???", 1, &c::IMP, &c::NOP}, {"PLP", 4, &c::IMP, &c::PLP}, {"AND", 2, &c::IMM, &c::AND}, //$29
    {"ROL", 2, &c::ACC, &c::ROL}, {"???", 1, &c::IMP, &c::NOP}, {"BIT", 4, &c::ABS, &c::BIT}, //$2C
    {"AND", 4, &c::ABS, &c::AND}, {"ROL", 6, &c::ABS, &c::ROL}, {"???", 1, &c::IMP, &c::NOP}, //$2F
    {"BMI", 2, &c::REL, &c::BMI}, {"AND", 5, &c::IDY, &c::AND}, {"???", 1, &c::IMP, &c::NOP}, //$32
    {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, {"AND", 4, &c::ZPX, &c::AND}, //$35
    {"ROL", 6, &c::ZPX, &c::ROL}, {"???", 1, &c::IMP, &c::NOP}, {"SEC", 2, &c::IMP, &c::SEC}, //$38
    {"AND", 4, &c::ABY, &c::AND}, {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, //$3B
    {"???", 1, &c::IMP, &c::NOP}, {"AND", 4, &c::ABX, &c::AND}, {"ROL", 7, &c::ABX, &c::ROL}, //$3E
    {"???", 1, &c::IMP, &c::NOP}, {"RTI", 4, &c::IMP, &c::RTI}, {"EOR", 6, &c::IDX, &c::EOR}, //$41
    {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, //$44
    {"EOR", 3, &c::ZPG, &c::EOR}, {"LSR", 5, &c::ZPG, &c::LSR}, {"???", 1, &c::IMP, &c::NOP}, //$47
    {"PHA", 3, &c::IMP, &c::PHA}, {"EOR", 2, &c::IMM, &c::EOR}, {"LSR", 2, &c::ACC, &c::LSR}, //$4A
    {"???", 1, &c::IMP, &c::NOP}, {"JMP", 3, &c::ABS, &c::JMP}, {"EOR", 4, &c::ABS, &c::EOR}, //$4D
    {"LSR", 6, &c::ABS, &c::LSR}, {"???", 1, &c::IMP, &c::NOP}, {"BVC", 2, &c::REL, &c::BVC}, //$50
    {"EOR", 5, &c::IDY, &c::EOR}, {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, //$53
    {"???", 1, &c::IMP, &c::NOP}, {"EOR", 4, &c::ZPX, &c::EOR}, {"LSR", 6, &c::ZPX, &c::LSR}, //$56
    {"???", 1, &c::IMP, &c::NOP}, {"CLI", 2, &c::IMP, &c::CLI}, {"EOR", 4, &c::ABY, &c::EOR}, //$59
    {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, //$5C
    {"EOR", 4, &c::ABX, &c::EOR}, {"LSR", 7, &c::ABX, &c::LSR}, {"???", 1, &c::IMP, &c::NOP}, //$5F
    {"RTS", 6, &c::IMP, &c::RTS}, {"ADC", 6, &c::IDX, &c::ADC}, {"???", 1, &c::IMP, &c::NOP}, //$62
    {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, {"ADC", 3, &c::ZPG, &c::ADC}, //$65
    {"ROR", 5, &c::ZPG, &c::ROR}, {"???", 1, &c::IMP, &c::NOP}, {"PLA", 4, &c::IMP, &c::PLA}, //$68
    {"ADC", 2, &c::IMM, &c::ADC}, {"ROR", 2, &c::ACC, &c::ROR}, {"???", 1, &c::IMP, &c::NOP}, //$6B
    {"JMP", 5, &c::IND, &c::JMP}, {"ADC", 4, &c::ABS, &c::ADC}, {"ROR", 6, &c::ABS, &c::ROR}, //$6E
    {"???", 1, &c::IMP, &c::NOP}, {"BVS", 2, &c::REL, &c::BVS}, {"ADC", 5, &c::IDY, &c::ADC}, //$71
    {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, //$74
    {"ADC", 4, &c::ZPX, &c::ADC}, {"ROR", 6, &c::ZPX, &c::ROR}, {"???", 1, &c::IMP, &c::NOP}, //$77
    {"SEI", 2, &c::IMP, &c::SEI}, {"ADC", 4, &c::ABY, &c::ADC}, {"???", 1, &c::IMP, &c::NOP}, //$7A
    {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, {"ADC", 4, &c::ABX, &c::ADC}, //$7D
    {"ROR", 7, &c::ABX, &c::ROR}, {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, //$80
    {"STA", 6, &c::IDX, &c::STA}, {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, //$83
    {"STY", 3, &c::ZPG, &c::STY}, {"STA", 3, &c::ZPG, &c::STA}, {"STX", 3, &c::ZPG, &c::STX}, //$86
    {"???", 1, &c::IMP, &c::NOP}, {"DEY", 2, &c::IMP, &c::DEY}, {"???", 1, &c::IMP, &c::NOP}, //$89
    {"TXA", 2, &c::IMP, &c::TXA}, {"???", 1, &c::IMP, &c::NOP}, {"STY", 4, &c::ABS, &c::STY}, //$8C
    {"STA", 4, &c::ABS, &c::STA}, {"STX", 4, &c::ABS, &c::STX}, {"???", 1, &c::IMP, &c::NOP}, //$8F
    {"BCC", 2, &c::REL, &c::BCC}, {"STA", 6, &c::IDY, &c::STA}, {"???", 1, &c::IMP, &c::NOP}, //$92
    {"???", 1, &c::IMP, &c::NOP}, {"STY", 4, &c::ZPX, &c::STY}, {"STA", 4, &c::ZPX, &c::STA}, //$95
    {"STX", 4, &c::ZPY, &c::STX}, {"???", 1, &c::IMP, &c::NOP}, {"TYA", 2, &c::IMP, &c::TYA}, //$98
    {"STA", 5, &c::ABY, &c::STA}, {"TXS", 2, &c::IMP, &c::TXS}, {"???", 1, &c::IMP, &c::NOP}, //$9B
    {"???", 1, &c::IMP, &c::NOP}, {"STA", 5, &c::ABX, &c::STA}, {"???", 1, &c::IMP, &c::NOP}, //$9E 
    {"???", 1, &c::IMP, &c::NOP}, {"LDY", 2, &c::IMM, &c::LDY}, {"LDA", 6, &c::IDX, &c::LDA}, //$A1
    {"LDX", 2, &c::IMM, &c::LDX}, {"???", 1, &c::IMP, &c::NOP}, {"LDY", 3, &c::ZPG, &c::LDY}, //$A4
    {"LDA", 3, &c::ZPG, &c::LDA}, {"LDX", 3, &c::ZPG, &c::LDX}, {"???", 1, &c::IMP, &c::NOP}, //$A7
    {"TAY", 2, &c::IMP, &c::TAY}, {"LDA", 2, &c::IMM, &c::LDA}, {"TAX", 2, &c::IMP, &c::TAX}, //$AA
    {"???", 1, &c::IMP, &c::NOP}, {"LDY", 4, &c::ABS, &c::LDY}, {"LDA", 4, &c::ABS, &c::LDA}, //$AD
    {"LDX", 4, &c::ABS, &c::LDX}, {"???", 1, &c::IMP, &c::NOP}, {"BCS", 2, &c::REL, &c::BCS}, //$B0
    {"LDA", 5, &c::IDY, &c::LDA}, {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, //$B3
    {"LDY", 4, &c::ZPX, &c::LDY}, {"LDA", 4, &c::ZPX, &c::LDA}, {"LDX", 4, &c::ZPY, &c::LDX}, //$B6 
    {"???", 1, &c::IMP, &c::NOP}, {"CLV", 2, &c::IMP, &c::CLV}, {"LDA", 4, &c::ABY, &c::LDA}, //$B9
    {"TSX", 2, &c::IMP, &c::TSX}, {"???", 1, &c::IMP, &c::NOP}, {"LDY", 4, &c::ABX, &c::LDY}, //$BC
    {"LDA", 4, &c::ABX, &c::LDA}, {"LDX", 4, &c::ABY, &c::LDX}, {"???", 1, &c::IMP, &c::NOP}, //$BF
    {"CPY", 2, &c::IMM, &c::CPY}, {"CMP", 6, &c::IDX, &c::CMP}, {"???", 1, &c::IMP, &c::NOP}, //$C2
    {"???", 1, &c::IMP, &c::NOP}, {"CPY", 3, &c::ZPG, &c::CPY}, {"CMP", 3, &c::ZPG, &c::CMP}, //$C5
    {"DEC", 5, &c::ZPG, &c::DEC}, {"???", 1, &c::IMP, &c::NOP}, {"INY", 2, &c::IMP, &c::INY}, //$C8
    {"CMP", 2, &c::IMM, &c::CMP}, {"DEX", 2, &c::IMP, &c::DEX}, {"???", 1, &c::IMP, &c::NOP}, //$CB
    {"CPY", 4, &c::ABS, &c::CPY}, {"CMP", 4, &c::ABS, &c::CMP}, {"DEC", 6, &c::ABS, &c::DEC}, //$CE
    {"???", 1, &c::IMP, &c::NOP}, {"BNE", 2, &c::REL, &c::BNE}, {"CMP", 5, &c::IDY, &c::CMP}, //$D1
    {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, //$D4
    {"CMP", 4, &c::ZPX, &c::CMP}, {"DEC", 6, &c::ZPX, &c::DEC}, {"???", 1, &c::IMP, &c::NOP}, //$D7
    {"CLD", 2, &c::IMP, &c::CLD}, {"CMP", 4, &c::ABY, &c::CMP}, {"???", 1, &c::IMP, &c::NOP}, //$DA
    {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, {"CMP", 4, &c::ABX, &c::CMP}, //$DD
    {"DEC", 7, &c::ABX, &c::DEC}, {"???", 1, &c::IMP, &c::NOP}, {"CPX", 2, &c::IMM, &c::CPX}, //$E0
    {"SBC", 6, &c::IDX, &c::SBC}, {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, //$E3
    {"CPX", 3, &c::ZPG, &c::CPX}, {"SBC", 3, &c::ZPG, &c::SBC}, {"INC", 5, &c::ZPG, &c::INC}, //$E6
    {"???", 1, &c::IMP, &c::NOP}, {"INX", 2, &c::IMP, &c::INX}, {"SBC", 2, &c::IMM, &c::SBC}, //$E9
    {"NOP", 2, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, {"CPX", 4, &c::ABS, &c::CPX}, //$EC
    {"SBC", 4, &c::ABS, &c::SBC}, {"INC", 6, &c::ABS, &c::INC}, {"???", 1, &c::IMP, &c::NOP}, //$EF 
    {"BEQ", 2, &c::REL, &c::BEQ}, {"SBC", 5, &c::IDY, &c::SBC}, {"???", 1, &c::IMP, &c::NOP}, //$F2
    {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, {"SBC", 4, &c::ZPX, &c::SBC}, //$F5
    {"INC", 6, &c::ZPX, &c::INC}, {"???", 1, &c::IMP, &c::NOP}, {"SED", 2, &c::IMP, &c::SED}, //$F8
    {"SBC", 4, &c::ABY, &c::SBC}, {"???", 1, &c::IMP, &c::NOP}, {"???", 1, &c::IMP, &c::NOP}, //$FB
    {"???", 1, &c::IMP, &c::NOP}, {"SBC", 4, &c::ABX, &c::SBC}, {"INC", 7, &c::ABX, &c::INC}, //$FE
    {"???", 1, &c::IMP, &c::NOP}
    };
}

void CPU6502::addBus(Bus* bus) {
    m_bus = bus;
}

uint8_t CPU6502::read(uint16_t addr) {
    if (m_bus == nullptr) throw std::exception(std::domain_error("No bus can be found, can't read."));
    return m_bus->read(addr);
}

void CPU6502::write(uint16_t addr, uint8_t value) {
    if (m_bus == nullptr) throw std::exception(std::domain_error("Bus not connected, can't write."));
    m_bus->write(addr, value);
}

uint8_t CPU6502::fetch() {
    if (!(currentInstruction.addressingMode == &CPU6502::IMP))
        fetched = read(argAddressAbs);
    return fetched;
}


uint8_t CPU6502::IMP() {
    return 0;
}   
uint8_t CPU6502::ACC() {
    return 0;
}   
uint8_t CPU6502::IMM() {
    argAddressAbs = registers.getPC();
    return 0;
}   
uint8_t CPU6502::ZPG() {
    uint8_t offset = read(registers.getPC());
    argAddressAbs = 0x00FF & offset;
    return 0;
}   
uint8_t CPU6502::ZPX() {
    uint8_t offset = read(registers.getPC());
    argAddressAbs = 0x00FF & ((offset + registers.getX()));
    return 0;
}   
uint8_t CPU6502::ZPY() {
    uint8_t offset = read(registers.getPC());
    argAddressAbs = (0x00FF & offset) + registers.getY();
    return 0;
}   
uint8_t CPU6502::REL() {
    argAddressRel = read(registers.getPC());
    if (argAddressAbs & 0x80)
        argAddressAbs |= 0xFF00;
    return 0;
}   
uint8_t CPU6502::ABS() {
    uint8_t addr_lo = read(registers.getPC());
    uint8_t addr_hi = read(registers.getPC());
    argAddressAbs = (addr_hi << 8) | addr_lo;
    return 0;
}   
uint8_t CPU6502::ABX() {
    uint8_t addr_lo = read(registers.getPC());
    uint8_t addr_hi = read(registers.getPC());
    argAddressAbs = (addr_hi << 8) | addr_lo;
    argAddressAbs += registers.getX();
    if ((argAddressAbs & 0xFF00) != (addr_hi << 8))
        return 1;
    return 0;
}   
uint8_t CPU6502::ABY() {
    uint8_t addr_lo = read(registers.getPC());
    uint8_t addr_hi = read(registers.getPC());
    argAddressAbs = (addr_hi << 8) | addr_lo;
    argAddressAbs += registers.getY();
    if ((argAddressAbs & 0xFF00) != (addr_hi << 8))
        return 1;
    return 0;
}   
uint8_t CPU6502::IND() {
    uint8_t addr_lo = read(registers.getPC());
    uint8_t addr_hi = read(registers.getPC());
    uint16_t addr = (addr_hi << 8) | addr_lo;

    addr_hi = read(addr+1);
    if (addr_lo == 0xFF) 
        addr_hi = read(addr & 0xFF00); //6502 bug simulation
    addr_lo = read(addr);
    argAddressAbs = (addr_hi << 8) | addr_lo;
    return 0;
}   
uint8_t CPU6502::IDX() {
    uint8_t addr_lo = read(registers.getPC());
    uint8_t addr_hi = read(registers.getPC());
    uint16_t addr = (addr_hi << 8) | addr_lo;
    addr_lo = read(addr);
    addr_hi = read(addr+1);
    argAddressAbs = (addr_hi << 8) | addr_lo;
    argAddressAbs += registers.getX();
    if ((argAddressAbs & 0xFF00) != (addr_hi >> 8))
        return 1;
    return 0;
}
uint8_t CPU6502::IDY() {
    uint8_t addr_lo = read(registers.getPC());
    uint8_t addr_hi = read(registers.getPC());
    uint16_t addr = (addr_hi << 8) | addr_lo;
    addr_lo = read(addr);
    addr_hi = read(addr+1);
    argAddressAbs = (addr_hi << 8) | addr_lo;
    argAddressAbs += registers.getY();
    if ((argAddressAbs & 0xFF00) != (addr_hi >> 8))
        return 1;
    return 0;
}   

uint8_t CPU6502::LDA() {
    fetch();
    registers.setA(fetched);
    registers.setFlag(Z, fetched == 0);
    registers.setFlag(N, fetched & 0x80);
    return 0;
} 
uint8_t CPU6502::LDX() {
    fetch();
    registers.setX(fetched);
    registers.setFlag(Z, fetched == 0);
    registers.setFlag(N, fetched & 0x80);
    return 0;
} 
uint8_t CPU6502::LDY() {
    fetch();
    registers.setY(fetched);
    registers.setFlag(Z, fetched == 0);
    registers.setFlag(N, fetched & 0x80);
    return 0;
} 
uint8_t CPU6502::STA() {
    write(argAddressAbs, registers.getA());
    return 0;
} 
uint8_t CPU6502::STX() {
    write(argAddressAbs, registers.getX());
    return 0;
} 
uint8_t CPU6502::STY() {
    write(argAddressAbs, registers.getY());
    return 0;
} 
uint8_t CPU6502::TAX() {
    uint8_t a = registers.getA();
    registers.setX(a);
    registers.setFlag(Z, a == 0);
    registers.setFlag(N, a & 0x80);
    return 0;
} 
uint8_t CPU6502::TAY() {
    uint8_t a = registers.getA();
    registers.setY(a);
    registers.setFlag(Z, a == 0);
    registers.setFlag(N, a & 0x80);
    return 0;
} 
uint8_t CPU6502::TXA() {
    uint8_t x = registers.getX();
    registers.setA(x);
    registers.setFlag(Z, x == 0);
    registers.setFlag(N, x & 0x80);
    return 0;
} 
uint8_t CPU6502::TYA() {
    uint8_t y = registers.getY();
    registers.setA(y);
    registers.setFlag(Z, y == 0);
    registers.setFlag(N, y & 0x80);
    return 0;
} 
uint8_t CPU6502::TSX() {
    uint8_t s = registers.getSP();
    registers.setX(s);
    registers.setFlag(Z, s == 0);
    registers.setFlag(N, s & 0x80);
    return 0;
} 
uint8_t CPU6502::TXS() {
    uint8_t x = registers.getX();
    registers.setSP(x);
    registers.setFlag(Z, x == 0);
    registers.setFlag(N, x & 0x80);
    return 0;
} 
uint8_t CPU6502::PHA() {
    uint8_t sp = registers.getSP();
    if (registers.getSP() <= 0)
        throw std::exception(std::overflow_error("Cannot push to stack."));
    uint16_t addr = systemStack.getPageAddress() + uint16_t(sp);
    write(addr, registers.getA());
    registers.push();
    return 0;
} 
uint8_t CPU6502::PHP() {
    uint8_t sp = registers.getSP();
    if (registers.getSP() <= 0)
        throw std::exception(std::overflow_error("Cannot push to stack."));
    uint16_t addr = systemStack.getPageAddress() + uint16_t(sp);
    write(addr, registers.getFlags());
    registers.push();
    return 0;
}
uint8_t CPU6502::PLA() {
    uint8_t sp = registers.getSP();
    if (registers.getSP() >= 0xFF)
        throw std::exception(std::overflow_error("Cannot pull from stack."));
    uint16_t addr = systemStack.getPageAddress() + uint16_t(sp);
    uint8_t a = read(addr);
    registers.setA(a);
    registers.setFlag(Z, a == 0);
    registers.setFlag(N, a & 0x80);
    registers.pull();
    return 0;
}
uint8_t CPU6502::PLP() {
    uint8_t sp = registers.getSP();
    if (registers.getSP() >= 0xFF)
        throw std::exception(std::overflow_error("Cannot pull from stack."));
    uint16_t addr = systemStack.getPageAddress() + uint16_t(sp);
    uint8_t a = read(addr);
    registers.setFlags(a);
    registers.pull();
    return 0;
}
uint8_t CPU6502::AND() {
    fetch();
    uint8_t a = registers.getA();
    registers.setA(a&fetched);
    a = registers.getA();
    registers.setFlag(Z, a == 0);
    registers.setFlag(N, a & 0x80);
    return 0;
}
uint8_t CPU6502::EOR() {
    fetch();
    uint8_t a = registers.getA();
    registers.setA(a^fetched);
    a = registers.getA();
    registers.setFlag(Z, a == 0);
    registers.setFlag(N, a & 0x80);
    return 0;
}
uint8_t CPU6502::ORA() {
    fetch();
    uint8_t a = registers.getA();
    registers.setA(a|fetched);
    a = registers.getA();
    registers.setFlag(Z, a == 0);
    registers.setFlag(N, a & 0x80);
    return 0;
}
uint8_t CPU6502::BIT() {
    fetch();
    uint8_t a = registers.getA() & fetched;
    registers.setFlag(Z, a == 0);
    registers.setFlag(V, a & 0x40);
    registers.setFlag(N, a & 0x80);
    return 0;
}
uint8_t CPU6502::ADC() {
    fetch();
    uint8_t a_old = registers.getA();
    uint16_t a = ((uint16_t)registers.getA() + (uint16_t)fetched + (uint16_t)registers.getFlag(C));
    registers.setFlag(Z, (a & 0x00FF) == 0);
    registers.setFlag(V, (((uint16_t)a_old ^ a) & ~((uint16_t)fetched ^ a_old)) & 0x0080);
    registers.setFlag(C, a > 0x00FF);
    registers.setFlag(N, a & 0x0080);
    registers.setA(a & 0x00FF);
    return 0;
}
uint8_t CPU6502::SBC() {
    fetch();
    uint8_t a_old = registers.getA();
    uint16_t tmp = (uint16_t)fetched ^ 0x00FF;
    uint16_t a = ((uint16_t)registers.getA() + (uint16_t)tmp + (uint16_t)registers.getFlag(C));
    registers.setFlag(Z, (a & 0x00FF) == 0);
    registers.setFlag(V, (((uint16_t)a_old ^ a) & ~((uint16_t)fetched ^ a_old)) & 0x0080);
    registers.setFlag(C, a > 0x00FF);
    registers.setFlag(N, a & 0x0080);
    registers.setA(a & 0x00FF);
    return 0;

}
uint8_t CPU6502::CMP() {
    fetch();
    uint8_t result = registers.getA() - fetched;
    registers.setFlag(Z, result == 0);
    registers.setFlag(C, result > 0xFF);
    registers.setFlag(N, result & 0x80);
    return 0;
}
uint8_t CPU6502::CPX() {
    fetch();
    uint8_t result = (uint8_t)registers.getX() - fetched;
    registers.setFlag(Z, result == 0);
    registers.setFlag(C, result > 0xFF);
    registers.setFlag(N, result & 0x80);
    return 0;
}
uint8_t CPU6502::CPY() {
    fetch();
    uint8_t result = (uint8_t)registers.getX() - fetched;
    registers.setFlag(Z, result == 0);
    registers.setFlag(C, result > 0xFF);
    registers.setFlag(N, result & 0x80);
    return 0;
}
uint8_t CPU6502::INC() {
    fetch();
    fetched++;
    registers.setFlag(Z, fetched == 0);
    registers.setFlag(N, fetched & 0x80);
    write(argAddressAbs, fetched);
    return 0;
}
uint8_t CPU6502::INX() {
    uint8_t x = registers.getX() + 1;
    registers.setFlag(Z, x == 0);
    registers.setFlag(N, x & 0x80);
    registers.setX(x);
    return 0;
} 
uint8_t CPU6502::INY() {
    uint8_t y = registers.getY() + 1;
    registers.setFlag(Z, y == 0);
    registers.setFlag(N, y & 0x80);
    registers.setY(y);
    return 0;
} 
uint8_t CPU6502::DEC() {
    fetch();
    fetched--;
    registers.setFlag(Z, fetched == 0);
    registers.setFlag(N, fetched & 0x80);
    write(argAddressAbs, fetched);
    return 0;
} 
uint8_t CPU6502::DEX() {
    uint8_t x = registers.getX() - 1;
    registers.setFlag(Z, x == 0);
    registers.setFlag(N, x & 0x80);
    registers.setX(x);
    return 0;
} 
uint8_t CPU6502::DEY() {
    uint8_t y = registers.getY() - 1;
    registers.setFlag(Z, y == 0);
    registers.setFlag(N, y & 0x80);
    registers.setY(y);
    return 0;
} 
uint8_t CPU6502::ASL() {
    fetch();
    if (currentInstruction.addressingMode == &CPU6502::ACC) 
        fetched = registers.getA();
    
    registers.setFlag(C, fetched & 0x80);
    fetched = fetched << 1;
    registers.setFlag(Z, fetched == 0);
    registers.setFlag(N, fetched & 0x80);

    if (currentInstruction.addressingMode == &CPU6502::ACC) 
        registers.setA(fetched);
    else
        write(argAddressAbs, fetched);

    return 0;
} 
uint8_t CPU6502::LSR() {
    fetch();
    if (currentInstruction.addressingMode == &CPU6502::ACC) 
        fetched = registers.getA();
    
    registers.setFlag(C, fetched & 0x01);
    fetched = fetched >> 1;
    registers.setFlag(Z, fetched == 0);
    registers.setFlag(N, fetched & 0x80);

    if (currentInstruction.addressingMode == &CPU6502::ACC) 
        registers.setA(fetched);
    else
        write(argAddressAbs, fetched);

    return 0;
} 
uint8_t CPU6502::ROL() {
    fetch();
    if (currentInstruction.addressingMode == &CPU6502::ACC) 
        fetched = registers.getA();
    
    uint8_t oldCarry = registers.getFlag(C) >> 7;
    registers.setFlag(C, fetched & 0x80);
    fetched = fetched << 1;
    fetched = (oldCarry) ?  (fetched | oldCarry) :
                            (fetched & ~oldCarry);
    registers.setFlag(Z, fetched == 0);
    registers.setFlag(N, fetched & 0x80);

    if (currentInstruction.addressingMode == &CPU6502::ACC) 
        registers.setA(fetched);
    else
        write(argAddressAbs, fetched);

    return 0;
} 
uint8_t CPU6502::ROR() {
    fetch();
    if (currentInstruction.addressingMode == &CPU6502::ACC) 
        fetched = registers.getA();
    
    uint8_t oldCarry = registers.getFlag(C);
    registers.setFlag(C, fetched & 0x80);
    fetched = fetched >> 1;
    fetched = (oldCarry) ?  (fetched | oldCarry) :
                            (fetched & ~oldCarry);
    registers.setFlag(Z, fetched == 0);
    registers.setFlag(N, fetched & 0x80);

    if (currentInstruction.addressingMode == &CPU6502::ACC) 
        registers.setA(fetched);
    else
        write(argAddressAbs, fetched);

    return 0;
} 
uint8_t CPU6502::JMP() {
    fetch();
    registers.setPC(fetched);
    return 0;
} 
uint8_t CPU6502::JSR() {
    fetch();
    uint16_t pc = registers.getPC(false)-1;
    uint8_t sp = registers.getSP();
    if (registers.getSP() <= 0)
        throw std::exception(std::overflow_error("Cannot push to stack."));
    uint16_t addr = systemStack.getPageAddress() + uint16_t(sp);
    write(addr, (pc & 0xFF00) >> 8);
    registers.push();
    write(addr-1, pc & 0x00FF);
    registers.push();
    registers.setPC(fetched);
    return 0;
} 
uint8_t CPU6502::RTS() {
    uint8_t sp = registers.getSP();
    if (registers.getSP() <= 0)
        throw std::exception(std::overflow_error("Cannot push to stack."));
    uint16_t addr = systemStack.getPageAddress() + uint16_t(sp);
    uint8_t addr_lo = read(addr);
    registers.pull();
    uint8_t addr_hi = read(addr+1);
    registers.pull();
    registers.setPC(((addr_hi << 8) | addr_lo));
    registers.getPC();
    return 0;
} 
uint8_t CPU6502::BCC() {
    uint16_t currentPC = registers.getPC(false);
    uint8_t pc_hi = (currentPC & 0xFF00);
    currentPC += argAddressAbs;
    if (registers.getFlag(C) == C)
        return 0;
    registers.setPC(currentPC);
    if (pc_hi != (currentPC & 0xFF00))
        return 2;
    return 1;
} 
uint8_t CPU6502::NOP() {
    return 0;
} 
uint8_t CPU6502::BCS() {
    uint16_t currentPC = registers.getPC(false);
    uint8_t pc_hi = (currentPC & 0xFF00);
    currentPC += argAddressAbs;
    if (registers.getFlag(C) != C)
        return 0;
    registers.setPC(currentPC);
    if (pc_hi != (currentPC & 0xFF00))
        return 2;
    return 1;
}
uint8_t CPU6502::BEQ() {
    uint16_t currentPC = registers.getPC(false);
    uint8_t pc_hi = (currentPC & 0xFF00);
    currentPC += argAddressAbs;
    if (registers.getFlag(Z) == Z)
        return 0;
    registers.setPC(currentPC);
    if (pc_hi != (currentPC & 0xFF00))
        return 2;
    return 1;
}
uint8_t CPU6502::BMI() {
    uint16_t currentPC = registers.getPC(false);
    uint8_t pc_hi = (currentPC & 0xFF00);
    currentPC += argAddressAbs;
    if (registers.getFlag(N) == N)
        return 0;
    registers.setPC(currentPC);
    if (pc_hi != (currentPC & 0xFF00))
        return 2;
    return 1;
}
uint8_t CPU6502::BNE() {
    uint16_t currentPC = registers.getPC(false);
    uint8_t pc_hi = (currentPC & 0xFF00);
    currentPC += argAddressAbs;
    if (registers.getFlag(Z) != Z)
        return 0;
    registers.setPC(currentPC);
    if (pc_hi != (currentPC & 0xFF00))
        return 2;
    return 1;
}
uint8_t CPU6502::BPL() {
    uint16_t currentPC = registers.getPC(false);
    uint8_t pc_hi = (currentPC & 0xFF00);
    currentPC += argAddressAbs;
    if (registers.getFlag(N) != N)
        return 0;
    registers.setPC(currentPC);
    if (pc_hi != (currentPC & 0xFF00))
        return 2;
    return 1;
}
uint8_t CPU6502::BVC() {
    uint16_t currentPC = registers.getPC(false);
    uint8_t pc_hi = (currentPC & 0xFF00);
    currentPC += argAddressAbs;
    if (registers.getFlag(V) != V)
        return 0;
    registers.setPC(currentPC);
    if (pc_hi != (currentPC & 0xFF00))
        return 2;
    return 1;
}
uint8_t CPU6502::BVS() {
    uint16_t currentPC = registers.getPC(false);
    uint8_t pc_hi = (currentPC & 0xFF00);
    currentPC += argAddressAbs;
    if (registers.getFlag(V) == V)
        return 0;
    registers.setPC(currentPC);
    if (pc_hi != (currentPC & 0xFF00))
        return 2;
    return 1;
}
uint8_t CPU6502::CLC() {
    registers.setFlag(C, false);
    return 0;
}
uint8_t CPU6502::CLD() {
    registers.setFlag(D, false);
    return 0;
}
uint8_t CPU6502::CLI() {
    registers.setFlag(I, 0);
    return 0;
}
uint8_t CPU6502::CLV() {
    registers.setFlag(V, 0);
    return 0;
}
uint8_t CPU6502::SEC() {

    registers.setFlag(C, 1);
    return 0;
}
uint8_t CPU6502::SED() {
    registers.setFlag(D, 1);
    return 0;
}
uint8_t CPU6502::SEI() {
    registers.setFlag(I, 1);
    return 0;
} 
uint8_t CPU6502::RTI() {
    uint8_t sp = registers.getSP();
    uint16_t addr = systemStack.getPageAddress() + (uint16_t)sp;
    uint8_t flags = read(addr); 
    addr++; registers.pull();
    uint8_t brPC_lo = read(addr);
    addr++; registers.pull();
    uint8_t brPC_hi = read(addr);
    addr++; registers.pull();

    registers.setPC(brPC_hi << 8 | brPC_lo);
    registers.setFlags(flags);
    return 0;
}
uint8_t CPU6502::BRK() {
    uint8_t sp = registers.getSP();
    uint16_t pc = registers.getPC(false);
    uint16_t addr = systemStack.getPageAddress() + (uint16_t)sp;
    write(addr, (pc & 0xFF00) >> 8);
    addr--; registers.push();
    write(addr, (pc & 0x00FF));
    addr--; registers.push();
    write(addr, registers.getFlags());
    addr--; registers.push();

    uint8_t brPC_lo = read(0xFFFE);
    uint8_t brPC_hi = read(0xFFFF);
    registers.setPC(brPC_hi << 8 | brPC_lo);
    registers.setFlag(B, 1);
    return 0;
}

int main() {
    CPU6502 cpu;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++)
            std::cout << cpu.instructionSet[i*16 + j].name << " | ";
        std::cout << std::endl;
    }
    return 0;
}
