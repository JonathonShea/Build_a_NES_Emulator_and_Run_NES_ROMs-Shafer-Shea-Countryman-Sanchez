// Created by Abel Sanchez on 11/17/2024
// Last updated: 1/21/2025

#include "CPU.h"
#include <iostream>

void CPU::respTest()
{
    std::cout << "The CPU says hi! Say hi back or else..." << std::endl;
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
void CPU::ASL(uint16_t addr)
{
    uint8_t value = read(addr);
    setCarryFlag(value & 0x80);
    value <<= 1;
    write(addr, value);
    setZeroFlag(value == 0);
    setNegativeFlag(value & 0x80);
}

void CPU::LSR(uint16_t addr)
{
    uint8_t value = read(addr);
    setCarryFlag(value & 0x01); 
    value >>= 1;            
    write(addr, value);
    setZeroFlag(value == 0); 
    setNegativeFlag(0);        
}

void CPU::ROL(uint16_t addr)
{
    uint8_t value = read(addr);
    bool carry = getCarryFlag();
    setCarryFlag(value & 0x80);
    value = (value << 1) | carry;
    write(addr, value);
    setZeroFlag(value == 0);
    setNegativeFlag(value & 0x80);
}

void CPU::ROR(uint16_t addr)
{
    uint8_t value = read(addr);
    bool carry = getCarryFlag();
    setCarryFlag(value & 0x01);
    value = (value >> 1) | (carry << 7);
    write(addr, value);
    setZeroFlag(value == 0);
    setNegativeFlag(value & 0x80);
}

// Add with carry OP code
void CPU::ADC(uint16_t addr) {
    uint16_t sum = accumulator + memory[addr] + getCarryFlag();
    if ((~(sum ^ accumulator) & (sum ^ memory[addr]) & negative_mask) > 0)
    {
        setOverflowFlag(true);
    }
    if (sum > 0xFF) {
        setCarryFlag(true);
    }
    if (sum == 0) {
        setZeroFlag(0);
    }
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    accumulator += (sum & 0xFF);
}

void CPU::INC(uint16_t addr)
{
    uint16_t sum = ++memory[addr];
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    if ((sum & 0xFF) == 0) {
        setZeroFlag(true);
    }
}

void CPU::DEC(uint16_t addr)
{
    uint16_t sum = --memory[addr];
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    if ((sum & 0xFF) == 0) {
        setZeroFlag(true);
    }
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


bool CPU::getOverFlowFlag() const
{
    return status & overflow_mask;
}

bool CPU::getNegativeFlag() const
{
    return status & negative_mask;
}

int main()
{
    CPU testCPU;
    testCPU.respTest();
    return 0;
}