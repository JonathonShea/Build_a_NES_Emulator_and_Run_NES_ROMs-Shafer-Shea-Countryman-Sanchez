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

CPU::CPU(std::shared_ptr<Bus> bus, std::shared_ptr<Cartridge> cart, std::shared_ptr<OAM> oam) : stack_pointer(0xFF), memory(65536, 0), program_counter(reset_vector),
m_oam(oam), m_cart(cart), m_bus(bus) // Initialize 64KB of memory
{
    uint16_t temp = read(program_counter++);
  
    temp = temp << 8;
    temp |= read(program_counter);
    program_counter = Utilities::ByteSwap(temp); // Now we jump!!!!
    bus->memory = memory; // Initialize the bus memory with the CPU memory (hacky copies for now)
}

uint8_t CPU::read(uint16_t addr)
{
    if (addr == 0x4016) {
        // Controller 1 serial read
        uint8_t value = (controller1_shift & 1);
        if (!controller_strobe) {
            controller1_shift >>= 1;
        }
        return value | 0x40; // Often returns high bits set to 1 or open bus
    }
    else if (addr == 0x4017) {
        // (Optional) Controller 2 serial read (if you have a second controller)
        uint8_t value = (controller2_shift & 1);
        if (!controller_strobe) {
            controller2_shift >>= 1;
        }
        return value | 0x40;
    }
    else if (addr >= 0x8000) {
        return m_cart->ReadPrgRom(addr - 0x8000);
    }
    else {
        return memory[addr];
    }
}

void CPU::write(uint16_t addr, uint8_t data)
{
    if (addr == 0x4016) {
        controller_strobe = data & 1;
        if (controller_strobe) {
            // On strobe high (1), latch the current input state into the shift register
            controller1_shift = controller1_state;
            controller2_shift = controller2_state; // if you have controller 2
        }
    }
    else {
        memory[addr] = data;
    }
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

// Absolute Y 
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
    return program_counter++;
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
// INTERRUPT OPERATIONS
///////////////////////////////////////////////////////////////////

void CPU::setIRQ(bool state)
{
    irq_signal = state;
    if (state && !(status & interrupt_disable_mask)) {
        handleInterrupts();
    }
}

void CPU::setNMI(bool state)
{
    // NMI is edge-triggered (only triggered on transition)
    static bool previous_nmi_state = false;
    if (!previous_nmi_state && state) {
        nmi_signal = true;
        handleInterrupts();
    }

    previous_nmi_state = state;
}

void CPU::setRESET(bool state)
{
    reset_signal = state;
    if (state) {
        // Reset CPU state
        stack_pointer = 0xFF;
        program_counter = reset_vector;
        setInterruptDisableFlag(true);
        status = status & (~break_mask);  // Clear break flag

        // Reset signal flags
        irq_signal = false;
        nmi_signal = false;
        m_bus->nmi = false; // Clear NMI signal on the bus
        reset_signal = false;
    }
}

void CPU::handleInterrupts()
{
    // Process interrupts in priority order: RESET > NMI > IRQ
    if (reset_signal) {
        setRESET(true);
        return;
    }

    if (nmi_signal) {
        // Push program counter to the stack
        stack.push_back((program_counter >> 8) & 0xFF); 
        stack.push_back(program_counter & 0xFF);         

        // Push status to the stack
        uint8_t status_copy = status;
        status_copy &= ~break_mask;  // Clear break flag
        stack.push_back(status_copy);

        stack_pointer -= 3;
        setInterruptDisableFlag(true);
        program_counter = read(0xFFFA) | (read(0xFFFB) << 8); // NMI vector
        nmi_signal = false;
        m_bus->nmi = false; // Clear NMI signal on the bus
        return;
    }

    if (irq_signal && !(status & interrupt_disable_mask)) {
        // Push program counter to the stack
        stack.push_back((program_counter >> 8) & 0xFF); 
        stack.push_back(program_counter & 0xFF);        

        // Push status to the stack
        uint8_t status_copy = status;
        status_copy &= ~break_mask; // Clear break flag
        stack.push_back(status_copy);

        stack_pointer -= 3;
        setInterruptDisableFlag(true);
        program_counter = read(0xFFFE) | (read(0xFFFF) << 8); // IRQ vector

        return;
    }
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
    else{
        setOverflowFlag(false);
    }
    if (sum > 0xFF) {
        setCarryFlag(true);
    }
    else{
        setCarryFlag(false);
    }
    setZeroFlag(sum == 0);
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    else{
        setNegativeFlag(false);
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
    else{
        setOverflowFlag(false);
    }
    if (sum > 0xFF) {
        setCarryFlag(true);
    }
    else{
        setCarryFlag(false);
    }
    setZeroFlag(sum == 0);
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    else{
        setNegativeFlag(false);
    }
    
}

void CPU::INC(uint16_t addr)
{
    uint16_t sum = ++memory[addr];
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    else{
        setNegativeFlag(false);
    }
    setZeroFlag(sum == 0);
}

void CPU::DEC(uint16_t addr)
{
    uint16_t sum = --memory[addr];
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    else{
        setNegativeFlag(false);
    }
    setZeroFlag(sum == 0);
}

void CPU::INX()
{
    uint16_t sum = ++x;
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    else{
        setNegativeFlag(false);
    }
    setZeroFlag(sum == 0);
}

void CPU::DEX()
{
    uint16_t sum = --x;
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    else{
        setNegativeFlag(false);
    }
    setZeroFlag(sum == 0);
}

void CPU::INY()
{
    uint16_t sum = ++y;
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    else{
        setNegativeFlag(false);
    }
    setZeroFlag(sum == 0);
}

void CPU::DEY()
{
    uint16_t sum = --y;
    if (sum & negative_mask) {
        setNegativeFlag(true);
    }
    else{
        setNegativeFlag(false);
    }
    setZeroFlag(sum == 0);
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

    program_counter = addr;
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
    program_counter = addr;
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
        // Signed value, need to cast
        int8_t offset = static_cast<int8_t>(read(addr));
        program_counter += offset;
    }
}

void CPU::BCS(uint16_t addr)
{
    if (getCarryFlag())
    {
        // Signed value, need to cast
        int8_t offset = static_cast<int8_t>(read(addr));
        program_counter += offset;
    }
}

void CPU::BMI(uint16_t addr)
{
    if (getNegativeFlag()) 
    {
        // Signed value, need to cast
        int8_t offset = static_cast<int8_t>(read(addr));
        program_counter += offset;
    }
}

void CPU::BPL(uint16_t addr)
{
    if (!getNegativeFlag())
    {
        // Signed value, need to cast
        int8_t offset = static_cast<int8_t>(read(addr));
        program_counter += offset;
    }
}

void CPU::BVC(uint16_t addr)
{
    if (!getOverFlowFlag())
    {
        // Signed value, need to cast
        int8_t offset = static_cast<int8_t>(read(addr));
        program_counter += offset;
    }
}

void CPU::BVS(uint16_t addr)
{
    if (getOverFlowFlag())
    {
        // Signed value, need to cast
        int8_t offset = static_cast<int8_t>(read(addr));
        program_counter += offset;
    }
}

void CPU::BNE(uint16_t addr)
{
    if (!getZeroFlag()) {
        // Signed value, need to cast
        int8_t offset = static_cast<int8_t>(read(addr));
        program_counter += offset;
    }
}

void CPU::BEQ(uint16_t addr)
{
    if (getZeroFlag()) {
        // Signed value, need to cast
        int8_t offset = static_cast<int8_t>(read(addr));
        program_counter += offset;
    }

}

// NOP - No Operation
void CPU::NOP() {
    // Does nothing
}

///////////////////////////////////////////////////////////////////
// INSTRUCTION EXECUTION LOOP **UNHOLY**
///////////////////////////////////////////////////////////////////

uint8_t CPU::execute() {
    // Handle interrupts prior to executing instructions
    handleInterrupts();
    std::copy(m_bus->memory.begin(), m_bus->memory.end(), memory.begin()); // Copy bus to memory
    // Fetch the next instruction
    uint8_t opcode = read(program_counter++);
    //std::cout << opcodeMap[opcode] << '\n';
    uint16_t addr = 0;
    uint16_t addr_abs = 0;
    uint8_t cycles = 0;
    if(m_bus->nmi) {
        setNMI(true);
    }

    switch (opcode) {
        // ADC 
        case 0x69:
            addr = addr_immediate();
            ADC(addr);
            cycles = 2;
            break;

        case 0x65:
            addr = addr_zero_page();
            ADC(addr);
            cycles = 3;
            break;

        case 0x75:
            addr = addr_zero_page_x();
            ADC(addr);
            cycles = 4;
            break;

        case 0x6D:
            addr = addr_absolute();
            ADC(addr);
            cycles = 4;
            break;

        case 0x7D:
            addr = addr_absolute_x();
            ADC(addr);
            cycles = 4;
            if ((addr & 0xFF00) != ((addr - x) & 0xFF00)) {
                cycles += 1;
            }
            break;

        case 0x79:
            addr = addr_absolute_y();
            ADC(addr);
            cycles = 4;
            if ((addr & 0xFF00) != ((addr - x) & 0xFF00)) {
                cycles += 1;
            }
            break;

        case 0x61:
            addr = addr_indexed_indirect_x();
            ADC(addr);
            cycles = 6;
            break;

        case 0x71:
            addr = addr_indexed_indirect_x();
            ADC(addr);
            cycles = 5;
            if ((addr & 0xFF00) != ((addr - x) & 0xFF00)) {
                cycles += 1;
            }
            break;

        // AND
        case 0x29: // Immediate
            addr = addr_immediate();
            AND(addr);
            cycles = 2;
            break;
        
        case 0x25: // Zero Page
            addr = addr_zero_page();
            AND(addr);
            cycles = 3;
            break;
       
        case 0x35: // Zero Page X
            addr = addr_zero_page_x();
            AND(addr);
            cycles = 4;
            break;
        
        case 0x2D: // Absolute
            addr = addr_absolute();
            AND(addr);
            cycles = 4;
            break;
        
        case 0x3D: // Absolute X
            addr = addr_absolute_x();
            AND(addr);
            cycles = 4;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - x) & 0xFF00)) {
                cycles += 1;
            }
            break;
       
        case 0x39: // Absolute Y
            addr = addr_absolute_y();
            AND(addr);
            cycles = 4;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - x) & 0xFF00)) {
                cycles += 1;
            }
            break;
       
        case 0x21: // Indiredct X
            addr = addr_indexed_indirect_x();
            AND(addr);
            cycles = 6;
            break;
        
        case 0x31: // Indirect Y
            addr = addr_indirect_indexed_y();
            AND(addr);
            cycles = 5;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - x) & 0xFF00)) {
                cycles += 1;
            }
            break;

        // ASL
        case 0x0A: // Accumulator
            ASL(0); // Call with 0 to indicate accumulator mode
            cycles = 2;
            break;
        
        case 0x06: // Zero Page
            addr = addr_zero_page();
            ASL(addr);
            cycles = 5;
            break;

        case 0x16: // Zero Page X
            addr = addr_zero_page_x();
            ASL(addr);
            cycles = 6;
            break;

        case 0x0E: // Absolute
            addr = addr_absolute();
            ASL(addr);
            cycles = 6;
            break;

        case 0x1E: // Absolute X
            addr = addr_absolute_x();
            ASL(addr);
            cycles = 7;
            break;

        // BCC
        case 0x90: // Relative
            addr = addr_relative();
            BCC(addr);
            cycles = 2;
            // Check if carry flag is set
            if (!getCarryFlag()) {
                cycles += 1;
                // Check if page boundry was crossed
                if ((addr & 0xFF00) != (program_counter & 0xFF00)) {
                    cycles += 1;
                }
            }
            break;

        // BCS
        case 0xB0: // Relative
            addr = addr_relative();
            BCS(addr);
            cycles = 2;
            // Check if carry flag is set
            if (getCarryFlag()) {
                cycles += 1;
                // Check if page boundry was crossed
                if ((addr & 0xFF00) != (program_counter & 0xFF00)) {
                    cycles += 1;
                }
            }
            break;

            // BEQ
        case 0xF0: // Relative
            addr = addr_relative();
            BEQ(addr);
            cycles = 2;

            // Check if zero flag is set
            if (getZeroFlag()) {
                cycles += 1;
                // Check if page boundry was crossed
                if ((addr & 0xFF00) != (program_counter & 0xFF00)) {
                    cycles += 1;
                }
            }
            break;

        // BIT
        case 0x24: // Zero Page
            addr = addr_zero_page();
            BIT(addr);
            cycles = 3;
            break;

        case 0x2C: // Absolute
            addr = addr_absolute();
            BIT(addr);
            cycles = 4;
            break;

        // BMI
        case 0x30: // Relative
            addr = addr_relative();
            BMI(addr);
            cycles = 2; 
            // Check if negative flag is set
            if (getNegativeFlag()) {
                cycles += 1;
                // Check if page boundary is crossed
                if ((addr & 0xFF00) != (program_counter & 0xFF00)) {
                    cycles += 1;
                }
            }
            break;

        // BNE    
        case 0xD0: // Relative
            addr = addr_relative();
            BNE(addr);
            cycles = 2;
            // Check if zero flag is set
            if (!getZeroFlag()) {
                cycles += 1;
                // Check if page boundary is crossed
                if ((addr & 0xFF00) != (program_counter & 0xFF00)) {
                    cycles += 1;
                }
            }
            break;

        // BPL
        case 0x10: // Relative
            addr = addr_relative();
            BPL(addr);
            cycles = 2;
            // Check if negative flag is set
            if (!getNegativeFlag()) {
                cycles += 1;
                // Check if page boundary is crossed
                if ((addr & 0xFF00) != (program_counter & 0xFF00)) {
                    cycles += 1;
                }
            }
            break;

        // BRK
        case 0x00: // Implied
            BRK();
            cycles = 7;
            break;

        // BVC
        case 0x50: // Relative
            addr = addr_relative();
            BVC(addr);
            cycles = 2; 
            // Check if overflow flag is set
            if (!getOverFlowFlag()) {
                cycles += 1;
                // Check if page boundary is crossed
                if ((addr & 0xFF00) != (program_counter & 0xFF00)) {
                    cycles += 1;
                }
            }
            break;

        // BVS
        case 0x70: // Relative
            addr = addr_relative();
            BVS(addr);
            cycles = 2;
            // Check if overflow flag is set
            if (getOverFlowFlag()) {
                cycles += 1;
                // Check if page boundary is crossed
                if ((addr & 0xFF00) != (program_counter & 0xFF00)) {
                    cycles += 1;
                }
            }
            break;
        
        // CLC
        case 0x18: // Implied
            CLC();
            cycles = 2;
            break;

        // CLD
        case 0xD8: // Implied
            CLD();
            cycles = 2; 
            break;

        // CLI
        case 0x58: // Implied
            CLI();
            cycles = 2;
            break;

        // CLV
        case 0xB8: // Implied
            CLV();
            cycles = 2; 
            break;

        // CMP - Compare Accumulator
        case 0xC9: // Immediate
            addr = addr_immediate();
            CMP(addr);
            cycles = 2;
            break;

        case 0xC5: // Zero Page
            addr = addr_zero_page();
            CMP(addr);
            cycles = 3;
            break;

        case 0xD5: // Zero Page X
            addr = addr_zero_page_x();
            CMP(addr);
            cycles = 4;
            break;

        case 0xCD: // Absolute
            addr = addr_absolute();
            CMP(addr);
            cycles = 4;
            break;

        case 0xDD: // Absolute X
            addr = addr_absolute_x();
            CMP(addr);
            cycles = 4;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - x) & 0xFF00)) {
                cycles += 1;
            }
            break;

        case 0xD9: // Absolute Y
            addr = addr_absolute_y();
            CMP(addr);
            cycles = 4;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - y) & 0xFF00)) {
                cycles += 1;
            }
            break;

        case 0xC1: // Indirect X
            addr = addr_indexed_indirect_x();
            CMP(addr);
            cycles = 6;
            break;

        case 0xD1: // Indirect Y
            addr = addr_indirect_indexed_y();
            CMP(addr);
            cycles = 5;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - y) & 0xFF00)) {
                cycles += 1;
            }
            break;

        // CPX
        case 0xE0: // Immediate
            addr = addr_immediate();
            CPX(addr);
            cycles = 2;
            break;

        case 0xE4: // Zero Page
            addr = addr_zero_page();
            CPX(addr);
            cycles = 3;
            break;

        case 0xEC: // Absolute
            addr = addr_absolute();
            CPX(addr);
            cycles = 4;
            break;

        // CPY
        case 0xC0: // Immediate
            addr = addr_immediate();
            CPY(addr);
            cycles = 2;
            break;

        case 0xC4: // Zero Page
            addr = addr_zero_page();
            CPY(addr);
            cycles = 3;
            break;

        case 0xCC: // Absolute
            addr = addr_absolute();
            CPY(addr);
            cycles = 4;
            break;

        // DEC
        case 0xC6: // Zero Page
            addr = addr_zero_page();
            DEC(addr);
            cycles = 5;
            break;

        case 0xD6: // Zero Page X
            addr = addr_zero_page_x();
            DEC(addr);
            cycles = 6;
            break;

        case 0xCE: // Absolute
            addr = addr_absolute();
            DEC(addr);
            cycles = 6;
            break;

        case 0xDE: // Absolute X
            addr = addr_absolute_x();
            DEC(addr);
            cycles = 7;
            break;

        // DEX
        case 0xCA: // Implied
            DEX();
            cycles = 2;
            break;

        // DEY 
        case 0x88: // Implied
            DEY();
            cycles = 2;
            break;

        // EOR 
        case 0x49: // Immediate
            addr = addr_immediate();
            EOR(addr);
            cycles = 2;
            break;

        case 0x45: // Zero Page
            addr = addr_zero_page();
            EOR(addr);
            cycles = 3;
            break;

        case 0x55: // Zero Page X
            addr = addr_zero_page_x();
            EOR(addr);
            cycles = 4;
            break;

        case 0x4D: // Absolute
            addr = addr_absolute();
            EOR(addr);
            cycles = 4;
            break;

        case 0x5D: // Absolute X
            addr = addr_absolute_x();
            EOR(addr);
            cycles = 4;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - x) & 0xFF00)) {
                cycles += 1;
            }
            break;

        case 0x59: // Absolute Y
            addr = addr_absolute_y();
            EOR(addr);
            cycles = 4;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - y) & 0xFF00)) {
                cycles += 1;
            }
            break;

        case 0x41: // Indirect X
            addr = addr_indexed_indirect_x();
            EOR(addr);
            cycles = 6;
            break;

        case 0x51: // Indirect Y
            addr = addr_indirect_indexed_y();
            EOR(addr);
            cycles = 5;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - y) & 0xFF00)) {
                cycles += 1;
            }
            break;

        // INC
        case 0xE6: // Zero Page
            addr = addr_zero_page();
            INC(addr);
            cycles = 5;
            break;

        case 0xF6: // Zero Page X
            addr = addr_zero_page_x();
            INC(addr);
            cycles = 6;
            break;

        case 0xEE: // Absolute
            addr = addr_absolute();
            INC(addr);
            cycles = 6;
            break;

        case 0xFE: // Absolute X
            addr = addr_absolute_x();
            INC(addr);
            cycles = 7;
            break;

        // INX
        case 0xE8: // Implied
            INX();
            cycles = 2;
            break;

        // INY 
        case 0xC8: // Implied
            INY();
            cycles = 2;
            break;

        // JMP
        case 0x4C: // Absolute
            addr = addr_absolute();
            JMP_ABS(addr);
            cycles = 3;
            break;

        case 0x6C: // Indirect
            addr = addr_indirect();
            JMP_IND(addr);
            cycles = 5;
            break;

        // JSR
        case 0x20: // Absolute
            addr = addr_absolute();
            JSR(addr);
            cycles = 6;
            break;

        // LDA
        case 0xA9: // Immediate
            addr = addr_immediate();
            LDA(addr);
            cycles = 2;
            break;

        case 0xA5: // Zero Page
            addr = addr_zero_page();
            LDA(addr);
            cycles = 3;
            break;

        case 0xB5: // Zero Page X
            addr = addr_zero_page_x();
            LDA(addr);
            cycles = 4;
            break;

        case 0xAD: // Absolute
            addr = addr_absolute();
            LDA(addr);
            cycles = 4;
            break;

        case 0xBD: // Absolute X
            addr = addr_absolute_x();
            LDA(addr);
            cycles = 4;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - x) & 0xFF00)) {
                cycles += 1;
            }
            break;

        case 0xB9: // Absolute Y
            addr = addr_absolute_y();
            LDA(addr);
            cycles = 4;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - y) & 0xFF00)) {
                cycles += 1;
            }
            break;

        case 0xA1: // Indirect X
            addr = addr_indexed_indirect_x();
            LDA(addr);
            cycles = 6;
            break;

        case 0xB1: // Indirect Y
            addr = addr_indirect_indexed_y();
            LDA(addr);
            cycles = 5;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - y) & 0xFF00)) {
                cycles += 1;
            }
            break;

        // LDX
        case 0xA2: // Immediate
            addr = addr_immediate();
            LDX(addr);
            cycles = 2;
            break;

        case 0xA6: // Zero Page
            addr = addr_zero_page();
            LDX(addr);
            cycles = 3;
            break;

        case 0xB6: // Zero Page Y
            addr = addr_zero_page_y();
            LDX(addr);
            cycles = 4;
            break;

        case 0xAE: // Absolute
            addr = addr_absolute();
            LDX(addr);
            cycles = 4;
            break;

        case 0xBE: // Absolute Y
            addr = addr_absolute_y();
            LDX(addr);
            cycles = 4;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - y) & 0xFF00)) {
                cycles += 1;
            }
            break;

        // LDY
        case 0xA0: // Immediate
            addr = addr_immediate();
            LDY(addr);
            cycles = 2;
            break;

        case 0xA4: // Zero Page
            addr = addr_zero_page();
            LDY(addr);
            cycles = 3;
            break;

        case 0xB4: // Zero Page X
            addr = addr_zero_page_x();
            LDY(addr);
            cycles = 4;
            break;

        case 0xAC: // Absolute
            addr = addr_absolute();
            LDY(addr);
            cycles = 4;
            break;

        case 0xBC: // Absolute X
            addr = addr_absolute_x();
            LDY(addr);
            cycles = 4;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - x) & 0xFF00)) {
                cycles += 1;
            }
            break;

        // LSR
        case 0x4A: // Accumulator
            LSR(0); // Call with 0 to indicate accumulator mode
            cycles = 2;
            break;

        case 0x46: // Zero Page
            addr = addr_zero_page();
            LSR(addr);
            cycles = 5;
            break;

        case 0x56: // Zero Page X
            addr = addr_zero_page_x();
            LSR(addr);
            cycles = 6;
            break;

        case 0x4E: // Absolute
            addr = addr_absolute();
            LSR(addr);
            cycles = 6;
            break;

        case 0x5E: // Absolute X
            addr = addr_absolute_x();
            LSR(addr);
            cycles = 7;
            break;

        // NOP
        case 0xEA: // Implied
            NOP();
            cycles = 2;
            break;

        // ORA
        case 0x09: // Immediate
            addr = addr_immediate();
            ORA(addr);
            cycles = 2;
            break;

        case 0x05: // Zero Page
            addr = addr_zero_page();
            ORA(addr);
            cycles = 3;
            break;

        case 0x15: // Zero Page X
            addr = addr_zero_page_x();
            ORA(addr);
            cycles = 4;
            break;

        case 0x0D: // Absolute
            addr = addr_absolute();
            ORA(addr);
            cycles = 4;
            break;

        case 0x1D: // Absolute X
            addr = addr_absolute_x();
            ORA(addr);
            cycles = 4;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - x) & 0xFF00)) {
                cycles += 1;
            }
            break;

        case 0x19: // Absolute Y
            addr = addr_absolute_y();
            ORA(addr);
            cycles = 4;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - y) & 0xFF00)) {
                cycles += 1;
            }
            break;

        case 0x01: // Indirect X
            addr = addr_indexed_indirect_x();
            ORA(addr);
            cycles = 6;
            break;

        case 0x11: // Indirect Y
            addr = addr_indirect_indexed_y();
            ORA(addr);
            cycles = 5;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - y) & 0xFF00)) {
                cycles += 1;
            }
            break;

        // PHA
        case 0x48: // Implied
            PHA();
            cycles = 3;
            break;

        // PHP 
        case 0x08: // Implied
            PHP();
            cycles = 3;
            break;

        // PLA 
        case 0x68: // Implied
            PLA();
            cycles = 4;
            break;

        // PLP 
        case 0x28: // Implied
            PLP();
            cycles = 4;
            break;

        // ROL
        case 0x2A: // Accumulator
            ROL(0); // Call with 0 to indicate accumulator mode
            cycles = 2;
            break;
       

        case 0x26: // Zero Page
            addr = addr_zero_page();
            ROL(addr);
            cycles = 5;
            break;

        case 0x36: // Zero Page X
            addr = addr_zero_page_x();
            ROL(addr);
            cycles = 6;
            break;

        case 0x2E: // Absolute
            addr = addr_absolute();
            ROL(addr);
            cycles = 6;
            break;

        case 0x3E: // Absolute X
            addr = addr_absolute_x();
            ROL(addr);
            cycles = 7;
            break;

        // ROR
        case 0x6A: // Accumulator
            ROR(0); // Pass 0 to indicate accumulator mode
            cycles = 2;
            break;

        case 0x66: // Zero Page
            addr = addr_zero_page();
            ROR(addr);
            cycles = 5;
            break;

        case 0x76: // Zero Page X
            addr = addr_zero_page_x();
            ROR(addr);
            cycles = 6;
            break;

        case 0x6E: // Absolute
            addr = addr_absolute();
            ROR(addr);
            cycles = 6;
            break;

        case 0x7E: // Absolute X
            addr = addr_absolute_x();
            ROR(addr);
            cycles = 7;
            break;

        // RTI 
        case 0x40: // Implied
            RTI();
            cycles = 6;
            break;

        // RTS
        case 0x60: // Implied
            RTS(0);
            cycles = 6;
            break;

        // SBC
        case 0xE9: // Immediate
            addr = addr_immediate();
            SBC(addr);
            cycles = 2;
            break;

        case 0xE5: // Zero Page
            addr = addr_zero_page();
            SBC(addr);
            cycles = 3;
            break;

        case 0xF5: // Zero Page X
            addr = addr_zero_page_x();
            SBC(addr);
            cycles = 4;
            break;

        case 0xED: // Absolute
            addr = addr_absolute();
            SBC(addr);
            cycles = 4;
            break;

        case 0xFD: // Absolute X
            addr = addr_absolute_x();
            SBC(addr);
            cycles = 4;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - x) & 0xFF00)) {
                cycles += 1;
            }
            break;

        case 0xF9: // Absolute Y
            addr = addr_absolute_y();
            SBC(addr);
            cycles = 4;
            if ((addr & 0xFF00) != ((addr - y) & 0xFF00)) {
                cycles += 1;
            }
            break;

        case 0xE1: // Indirect X
            addr = addr_indexed_indirect_x();
            SBC(addr);
            cycles = 6;
            break;

        case 0xF1: // Indirect Y
            addr = addr_indirect_indexed_y();
            SBC(addr);
            cycles = 5;
            // Check if page boundry was crossed
            if ((addr & 0xFF00) != ((addr - y) & 0xFF00)) {
                cycles += 1;
            }
            break;

        // SEC
        case 0x38: // Implied
            SEC();
            cycles = 2;
            break;

        // SED 
        case 0xF8: // Implied
            SED();
            cycles = 2;
            break;

        // SEI
        case 0x78: // Implied
            SEI();
            cycles = 2;
            break;

        // STA
        case 0x85: // Zero Page
            addr = addr_zero_page();
            STA(addr);
            cycles = 3;
            break;

        case 0x95: // Zero Page X
            addr = addr_zero_page_x();
            STA(addr);
            cycles = 4;
            break;

        case 0x8D: // Absolute
            addr = addr_absolute();
            STA(addr);
            cycles = 4;
            break;

        case 0x9D: // Absolute X
            addr = addr_absolute_x();
            STA(addr);
            cycles = 5;
            break;

        case 0x99: // Absolute Y
            addr = addr_absolute_y();
            STA(addr);
            cycles = 5;
            break;

        case 0x81: // Indirect X
            addr = addr_indexed_indirect_x();
            STA(addr);
            cycles = 6;
            break;

        case 0x91: // Indirect Y
            addr = addr_indirect_indexed_y();
            STA(addr);
            cycles = 6; 
            break;

        // STX
        case 0x86: // Zero Page
            addr = addr_zero_page();
            STX(addr);
            cycles = 3;
            break;

        case 0x96: // Zero Page Y
            addr = addr_zero_page_y();
            STX(addr);
            cycles = 4;
            break;

        case 0x8E: // Absolute
            addr = addr_absolute();
            STX(addr);
            cycles = 4;
            break;

        // STY 
        case 0x84: // Zero Page
            addr = addr_zero_page();
            STY(addr);
            cycles = 3;
            break;

        case 0x94: // Zero Page X
            addr = addr_zero_page_x();
            STY(addr);
            cycles = 4;
            break;

        case 0x8C: // Absolute
            addr = addr_absolute();
            STY(addr);
            cycles = 4;
            break;

        // TAX
        case 0xAA: // Implied
            TAX();
            cycles = 2;
            break;

        // TAY
        case 0xA8: // Implied
            TAY();
            cycles = 2;
            break;

        // TSX
        case 0xBA: // Implied
            TSX();
            cycles = 2;
            break;

        // TXA
        case 0x8A: // Implied
            TXA();
            cycles = 2;
            break;

        // TXS
        case 0x9A: // Implied
            TXS();
            cycles = 2;
            break;

        // TYA 
        case 0x98: // Implied
            TYA();
            cycles = 2;
            break;
    }
    // Copy sprite data from memory into OAM
    if (m_oam != nullptr) {
        std::memcpy(m_oam->sprites.data(), memory.data() + oamAddr, oamSize);
        // for (const auto& val : m_oam->sprites) {
        //     std::cout << "Sprite: " << val << std::endl;
        // }
    }
    std::copy(memory.begin(), memory.end(), m_bus->memory.begin()); // Copy memory to bus
    return cycles;
}

void CPU::SetCartridge(std::shared_ptr<Cartridge> cartridge)
{
    this->m_cart = cartridge;
    uint16_t temp = read(program_counter++);
  
    temp = temp << 8;
    temp |= read(program_counter);
    program_counter = Utilities::ByteSwap(temp); // Now we jump!!!!
  
}

