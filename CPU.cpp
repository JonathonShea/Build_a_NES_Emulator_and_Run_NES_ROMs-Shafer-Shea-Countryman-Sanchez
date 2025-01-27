// Created by Abel Sanchez on 11/17/2024
// Last updated: 1/21/2025

#include "CPU.h"
#include <iostream>

void CPU::respTest()
{
    uint16_t addr = 0x2000; // Example address
    uint8_t value = 0x55; // Example value

    // Write the value to memory
    write(addr, value);

    // Read and print the value before the shift
    uint8_t beforeShift = read(addr);
    std::cout << "Value before ASL: " << std::hex << static_cast<int>(beforeShift) << std::endl;

    // Perform the arithmetic shift left
    ASL(addr);

    // Read and print the value after the shift
    uint8_t afterShift = read(addr);
    std::cout << "Value after ASL: " << std::hex << static_cast<int>(afterShift) << std::endl;
    std::cout << "Expected value after ASL: " << std::hex << static_cast<int>(0xAA) << std::endl;

    // Perform the logical shift right
    write(addr, value);
    beforeShift = read(addr);
    std::cout << "Value before LSR: " << std::hex << static_cast<int>(beforeShift) << std::endl;
    LSR(addr);
    afterShift = read(addr);
    std::cout << "Value after LSR: " << std::hex << static_cast<int>(afterShift) << std::endl;
    std::cout << "Expected value after LSR: " << std::hex << static_cast<int>(0x2A) << std::endl;

    // Perform the rotate left
    write(addr, value);
    setCarryFlag(false); // Clear carry flag before ROL
    beforeShift = read(addr);
    std::cout << "Value before ROL: " << std::hex << static_cast<int>(beforeShift) << std::endl;
    ROL(addr);
    afterShift = read(addr);
    std::cout << "Value after ROL: " << std::hex << static_cast<int>(afterShift) << std::endl;
    std::cout << "Expected value after ROL: " << std::hex << static_cast<int>(0xAA) << std::endl;

    // Perform the rotate right
    write(addr, value);
    setCarryFlag(false); // Clear carry flag before ROR
    beforeShift = read(addr);
    std::cout << "Value before ROR: " << std::hex << static_cast<int>(beforeShift) << std::endl;
    ROR(addr);
    afterShift = read(addr);
    std::cout << "Value after ROR: " << std::hex << static_cast<int>(afterShift) << std::endl;
    std::cout << "Expected value after ROR: " << std::hex << static_cast<int>(0x2A) << std::endl;
}

CPU::CPU() : memory(65536, 0) // Initialize 64KB of memory
{
}


uint8_t CPU::read(uint16_t addr)
{
    return memory[addr];
}

void CPU::write(uint16_t addr, uint8_t data)
{
    memory[addr] = data;
}

// shift instructions
void CPU::ASL(uint16_t addr) // Arithmetic Shift Left
{
    uint8_t value = read(addr);
    setCarryFlag(value & 0x80);
    value <<= 1;
    write(addr, value);
    setZeroFlag(value == 0);
    setNegativeFlag(value & 0x80);
}

void CPU::LSR(uint16_t addr) // Logical Shift Right
{
    uint8_t value = read(addr);
    setCarryFlag(value & 0x01); 
    value >>= 1;            
    write(addr, value);
    setZeroFlag(value == 0); 
    setNegativeFlag(0);        
}

void CPU::ROL(uint16_t addr) // Rotate Left
{
    uint8_t value = read(addr);
    bool carry = getCarryFlag();
    setCarryFlag(value & 0x80);
    value = (value << 1) | carry; // shift left and add carry
    write(addr, value); 
    setZeroFlag(value == 0); 
    setNegativeFlag(value & 0x80);
}

void CPU::ROR(uint16_t addr) // Rotate Right
{
    uint8_t value = read(addr);
    bool carry = getCarryFlag();
    setCarryFlag(value & 0x01);
    value = (value >> 1) | (carry << 7);
    write(addr, value);
    setZeroFlag(value == 0);
    setNegativeFlag(value & 0x80);
}

// Bitwise
void CPU::AND(uint16_t addr) {
	uint8_t value = read(addr);
	accumulator = accumulator & value;

	setZeroFlag(accumulator == 0x00);
	setNegativeFlag(accumulator & 0x80);
}

void CPU::ORA(uint16_t addr) {
	uint8_t value = read(addr);
	accumulator = accumulator | value;

	setZeroFlag(accumulator == 0x00);
	setNegativeFlag(accumulator & 0x80);
}

void CPU::EOR(uint16_t addr) {
	uint8_t value = read(addr);
	accumulator = accumulator ^ value;

	setZeroFlag(accumulator == 0x00);
	setNegativeFlag(accumulator & 0x80);
}

void CPU::BIT(uint16_t addr) {
	uint8_t value = read(addr);
	result = accumulator & value;

	setZeroFlag((result & 0xFF) == 0x00);
	setOverflowFlag(value & (1 << 6));
	setNegativeFlag(value & (1 << 7));
}

// flags
void CPU::setCarryFlag(bool value) 
{
    if (value)
        status |= 0x01;
    else
        status &= ~0x01;
}

bool CPU::getCarryFlag()
{
    return status & 0x01;
}

void CPU::setZeroFlag(bool value)
{
    if (value)
        status |= 0x02;
    else
        status &= ~0x02;
}

void CPU::setInterruptDisableFlag(bool value)
{
    if (value)
        status |= 0x04;
    else
        status &= ~0x04;
}

void CPU::setDecimalModeFlag(bool value)
{
    if (value)
        status |= 0x08;
    else
        status &= ~0x08;
}

void CPU::setBreakCommandFlag(bool value)
{
    if (value)
        status |= 0x10;
    else
        status &= ~0x10;
}

void CPU::setOverflowFlag(bool value)
{
    if (value)
        status |= 0x40;
    else
        status &= ~0x40;
}

void CPU::setNegativeFlag(bool value)
{
    if (value)
        status |= 0x80;
    else
        status &= ~0x80;
}

int main()
{
    CPU testCPU;
    testCPU.respTest();
    return 0;
}
