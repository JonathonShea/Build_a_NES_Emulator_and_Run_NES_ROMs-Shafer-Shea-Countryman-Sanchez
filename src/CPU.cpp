// Created by Abel Sanchez on 11/17/2024
// Last updated: 1/21/2025

#include "CPU.h"
#include "Bus.h"
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

CPU::CPU() : stack_pointer(0xFF), memory(65536, 0) // Initialize 64KB of memory
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

///////////////////////////////////////////////////////////////////
// ADDRESSING MODES
///////////////////////////////////////////////////////////////////

// Implied 
uint16_t CPU::addr_implied()
{
    return 0;
}

// Accumulator 
uint16_t CPU::addr_accumulator()
{
    return 0; 
}

// Immediate 
uint16_t CPU::addr_immediate()
{
    return program_counter++; 
}

// Zero Page 
uint16_t CPU::addr_zero_page()
{
    return read(program_counter++);
}

// Zero Page X
uint16_t CPU::addr_zero_page_x()
{
    uint8_t base = read(program_counter++);
    
    // Zero page wraps around
    return (base + x) & 0xFF; 
}

// Zero Page Y 
uint16_t CPU::addr_zero_page_y()
{
    uint8_t base = read(program_counter++);

    // Zero page wraps around
    return (base + y) & 0xFF; 
}

// Absolute 
uint16_t CPU::addr_absolute()
{
    uint16_t low_byte = read(program_counter++);
    uint16_t high_byte = read(program_counter++);
    return (high_byte << 8) | low_byte; 
}

// Absolute X 
uint16_t CPU::addr_absolute_x()
{
    uint16_t low_byte = read(program_counter++);
    uint16_t high_byte = read(program_counter++);
    uint16_t addr = ((high_byte << 8) | low_byte) + x;

    return addr;
}

// Absolute Y - Use 16-bit address from next two bytes + Y
uint16_t CPU::addr_absolute_y()
{
    uint16_t low_byte = read(program_counter++);
    uint16_t high_byte = read(program_counter++);
    uint16_t addr = ((high_byte << 8) | low_byte) + y;

    return addr;
}

// Indirect
uint16_t CPU::addr_indirect()
{
    uint16_t ptr_low = read(program_counter++);
    uint16_t ptr_high = read(program_counter++);
    uint16_t ptr = (ptr_high << 8) | ptr_low;

    // Handle the 6502 JMP indirect bug
    // If the pointer is at the end of a page, it wraps around within the same page
    // instead of crossing to the next page
    uint16_t low_byte;
    uint16_t high_byte;

    if (ptr_low == 0xFF) {
        // Bug: high byte is fetched from the same page, not the next page
        low_byte = read(ptr);
        
        // Wrap around in the same page
        high_byte = read(ptr & 0xFF00); 
    }
    else {
        // Normal case
        low_byte = read(ptr);
        high_byte = read(ptr + 1);
    }

    return (high_byte << 8) | low_byte;
}

// Indexed Indirect (X) 
uint16_t CPU::addr_indexed_indirect_x()
{
    uint8_t base = read(program_counter++);

    // Zero page wraps around
    uint8_t ptr = (base + x) & 0xFF; 

    uint16_t low_byte = read(ptr);

    // Zero page wraps around
    uint16_t high_byte = read((ptr + 1) & 0xFF);

    return (high_byte << 8) | low_byte;
}

// Indirect Indexed (Y) 
uint16_t CPU::addr_indirect_indexed_y()
{
    uint8_t ptr = read(program_counter++);

    uint16_t low_byte = read(ptr);

    // Zero page wraps around
    uint16_t high_byte = read((ptr + 1) & 0xFF); 

    uint16_t addr = ((high_byte << 8) | low_byte) + y;

    return addr;
}

// Relative addressing mode
uint16_t CPU::addr_relative()
{
    int8_t offset = static_cast<int8_t>(read(program_counter++));
    return program_counter + offset;
}
///////////////////////////////////////////////////////////////////
// FLAG OPERATIONS
///////////////////////////////////////////////////////////////////

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

bool CPU::getDecimalModeFlag() const
{
    return status & decimal_mode_mask;
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

bool CPU::getZeroFlag() const
{
    return status & zero_mask;
}

bool CPU::getBreakCommandFlag() const
{
    return status & break_mask;
}

void CPU::clearStatus()
{
    status = 0;
}

std::vector<uint8_t> CPU::getStackTESTING() const {
    return stack;
}

void CPU::setStackBackTESTING(uint8_t value) {
    stack.push_back(value);
    stack_pointer--;
}

///////////////////////////////////////////////////////////////////
// OPCODES
///////////////////////////////////////////////////////////////////

// Shift Opcodes
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

// Arithmetic Opcodes
// Add with carry OP code
void CPU::ADC(uint16_t addr) {
    uint16_t sum = accumulator + memory[addr] + getCarryFlag();
    accumulator = (sum & 0xFF);
    if (~((sum ^ accumulator) & (sum ^ memory[addr]) & negative_mask) & 0x80)
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
    
}

// Add with carry OP code
void CPU::SBC(uint16_t addr) {
    uint16_t inverse = memory[addr] ^ 0xFF;
    uint16_t sum = accumulator + inverse + getCarryFlag();
    accumulator = (sum & 0xFF);
    if (~((sum ^ accumulator) & (sum ^ inverse) & negative_mask) & 0x80)
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

void CPU::INX()
{
    uint16_t sum = ++x;
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    if ((sum && 0) == 0) {
        setZeroFlag(true);
    }
}

void CPU::DEX()
{
    uint16_t sum = --x;
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    if ((sum && 0) == 0) {
        setZeroFlag(true);
    }
}

void CPU::INY()
{
    uint16_t sum = ++y;
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    if ((sum && 0) == 0) {
        setZeroFlag(true);
    }
}

void CPU::DEY()
{
    uint16_t sum = --y;
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    if ((sum && 0) == 0) {
        setZeroFlag(true);
    }
}

// Flag Opcodes
void CPU::CLC() // Clear Carry Flag
{
    setCarryFlag(false);
}

void CPU::SEC() // Set Carry Flag
{
    setCarryFlag(true);
}

void CPU::CLI() // Clear Interrupt Disable Flag
{
    setInterruptDisableFlag(false);
}

void CPU::SEI() // Set Interrupt Disable Flag
{
    setInterruptDisableFlag(true);
}

void CPU::CLV() // Clear Overflow Flag
{
    setOverflowFlag(false);
}

void CPU::CLD() // Clear Decimal Mode Flag
{
    setDecimalModeFlag(false);
}

void CPU::SED() // Set Decimal Mode Flag
{
    setDecimalModeFlag(true);
}

// Bitwise Opcodes
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
    uint8_t result = accumulator & value;

    setZeroFlag(result == 0x00);
    setOverflowFlag(value & 0x40);
    setNegativeFlag(value & 0x80);
}

// Compare Opcodes
void CPU::CMP(uint16_t addr) {
    uint8_t value = read(addr);
    uint8_t result = accumulator - value;

    setCarryFlag(accumulator >= value);
    setZeroFlag(result == 0);
    setNegativeFlag(result & 0x80);
}

void CPU::CPX(uint16_t addr) {
    uint8_t value = read(addr);
    uint8_t result = x - value;

    setCarryFlag(x >= value);
    setZeroFlag(result == 0);
    setNegativeFlag(result & 0x80);
}

void CPU::CPY(uint16_t addr) {
    uint8_t value = read(addr);
    uint8_t result = y - value;

    setCarryFlag(y >= value);
    setZeroFlag(result == 0);
    setNegativeFlag(result & 0x80);
}

// Access Opcodes
void CPU::LDA(uint16_t addr) {
    uint8_t value = read(addr);
    accumulator = value;

    setZeroFlag(value == 0);
    setNegativeFlag(value & 0x80);
}

void CPU::STA(uint16_t addr) {
    write(addr, accumulator);
}

void CPU::LDX(uint16_t addr) {
    uint8_t result = read(addr);
    x = result;

    setZeroFlag(result == 0);
    setNegativeFlag(result & 0x80);
}

void CPU::STX(uint16_t addr) {
    write(addr, x);
}

void CPU::LDY(uint16_t addr) {
    uint8_t result = read(addr);
    y = result;

    setZeroFlag(result == 0);
    setNegativeFlag(result & 0x80);
}

void CPU::STY(uint16_t addr) {
    write(addr, y);
}

// Stack Opcodes
void CPU::PHA() { //Push accumulator value onto stack
    stack.push_back(accumulator);
    stack_pointer --;
}

void CPU::PLA() { //If stack not empty set accumulator to value at back of stack and set flags based on new accumulator value
    if (!stack.empty()) {
        accumulator = stack.back();
        stack.pop_back();
        stack_pointer++;

        setZeroFlag(accumulator & zero_mask);
        setNegativeFlag(accumulator & negative_mask);
    }
}

void CPU::PHP() { //Set Break flag to 1 and push status register onto stack. Initialization of the extrabit found in status truncated due to being unnecessary
    setBreakCommandFlag(1);
    stack.push_back(status);
    stack_pointer --;
}

void CPU::PLP() { //If stack not empty assign status to value at back of stack and set flags based on new status value
    if (!stack.empty()) {
        status = stack.back();
        stack.pop_back();
        stack_pointer++;

        setCarryFlag(status & carry_mask);
        setZeroFlag(status & zero_mask);
        setInterruptDisableFlag(status & interrupt_disable_mask);
        setDecimalModeFlag(status & decimal_mode_mask);
        setOverflowFlag(status & overflow_mask);
        setNegativeFlag(status & negative_mask);
    }
}

void CPU::TXS() { //Transfer X to Stack Pointer
    stack_pointer = x;
}

void CPU::TSX() { //Transfer Stack Pointer to x
    x = stack_pointer;

    setZeroFlag(x & zero_mask);
    setNegativeFlag(x & negative_mask);
}

// Transfer Opcodes
void CPU::TXA()
{
    accumulator = x;
    setZeroFlag(accumulator == 0x00);
    setNegativeFlag(accumulator & negative_mask);
}

void CPU::TYA()
{
    accumulator = y;
    setZeroFlag(accumulator == 0x00);
    setNegativeFlag(accumulator & negative_mask);
}

void CPU::TAX()
{
    x = accumulator;
    setZeroFlag(x == 0x00);
    setNegativeFlag(x & negative_mask);

}

void CPU::TAY()
{
    y = accumulator;
    setZeroFlag(y == 0x00);
    setNegativeFlag(y & negative_mask);
}

// Jump Opcodes
void CPU::JMP_ABS(uint16_t addr)
{
    // Program counter is 2 bytes, need to read in value in the next address as well.
    std::memcpy(&program_counter, memory.data() + addr, sizeof(program_counter));
}

void CPU::JMP_IND(uint16_t addr)
{

    uint16_t jmp_addr;
    std::memcpy(&jmp_addr, memory.data() + addr, sizeof(addr));
    std::memcpy(&program_counter, memory.data() + jmp_addr, sizeof(jmp_addr));

    // CPU bug when crossing a page boundary, "For example, JMP ($03FF) reads $03FF and $0300 instead of $0400"
    (program_counter & 0xFF) == 0xFF ? program_counter -= 0xFF : program_counter;
}

void CPU::JSR(uint16_t addr)
{
    stack.push_back(program_counter >> 1); // MSB
    stack.push_back(program_counter & 0xFF); // LSB
    stack_pointer -= 2;
    JMP_ABS(addr);
}

void CPU::RTS(uint16_t addr)
{
    // Pop program counter bytes into program counter
    program_counter = 0;
    program_counter = stack.back() << 1;
    stack.pop_back();
    program_counter |= stack.back();
    stack.pop_back();
    stack_pointer += 2;

    program_counter++; // We return to the next address after the JMP that brought us here (otherwise this becomes a portal emulator)

}

void CPU::BRK()
{
    status |= break_mask; // b flag is set prior to pushing status to the stack.
    program_counter += 2;
    stack.push_back(program_counter & 0x0F); // low byte
    stack.push_back((program_counter & 0xF0) >> 1); // high byte
    stack.push_back(status);
    status |= interrupt_disable_mask; // irq disable flag is set after pushing status to the stack.
    program_counter = 0xFFFE;
}

void CPU::RTI()
{
    status = stack.back();
    stack.pop_back();
    program_counter = stack.back() << 1; // high byte
    stack.pop_back();
    program_counter |= stack.back(); // low byte
    stack.pop_back();
}

// Branch Opcodes
void CPU::BCC(uint16_t addr)
{
    if (!getCarryFlag())
    {
        program_counter += static_cast<int8_t>(memory[addr]) + 2;
    }
}

void CPU::BCS(uint16_t addr)
{
    if (getCarryFlag())
    {
        program_counter += static_cast<int8_t>(memory[addr]) + 2;
    }
}

void CPU::BMI(uint16_t addr)
{
    if (getNegativeFlag()) 
    {
        program_counter += static_cast<int8_t>(memory[addr]) + 2;
    }
}

void CPU::BPL(uint16_t addr)
{
    if (!getNegativeFlag())
    {
        program_counter += static_cast<int8_t>(memory[addr]) + 2;
    }
}

void CPU::BVC(uint16_t addr)
{
    if (!getOverFlowFlag())
    {
        program_counter += static_cast<int8_t>(memory[addr]) + 2;
    }
}

void CPU::BVS(uint16_t addr)
{
    if (getOverFlowFlag())
    {
        program_counter += static_cast<int8_t>(memory[addr]) + 2;
    }
}

void CPU::BNE(uint16_t addr)
{
    if (!getZeroFlag()) {
        // Signed value, need to cast
        program_counter += static_cast<int8_t>(memory[addr]) + 2;
    }
}

void CPU::BEQ(uint16_t addr)
{
    if (getZeroFlag()) {
        // Signed value, need to cast
        program_counter += static_cast<int8_t>(memory[addr]) + 2;
    }
}


