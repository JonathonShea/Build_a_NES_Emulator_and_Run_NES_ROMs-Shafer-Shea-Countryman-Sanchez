// Created by Abel Sanchez om 11/17/2024
// Last updated: 1/21/2025

#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <vector>
class Bus;

class CPU
{
public:
	uint8_t accumulator = 0x00; // 8-bit arithmetic register
	uint8_t x = 0x00; // 8-bit general purpose register
	uint8_t y = 0x00; // 8-bit general purpose register
	uint8_t stack_pointer = 0x0000; // 8-bit register that contains lower 8 bits of stack
	uint16_t program_counter = 0x00; // 16-bit register that contains a pointer to the next instruction
	uint8_t status = 0x00; // 8-bit register that contains status flags

	// Connect CPU to the bus
	void connect_bus(Bus *n) {
		bus = n;
	}

	CPU();

	void respTest();
	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);
	void ASL(uint16_t addr);
	void LSR(uint16_t addr);
	void ROL(uint16_t addr);
	void ROR(uint16_t addr);

	// Arithmetic OP codes.
	void ADC(uint16_t addr);
	void SBC(uint16_t addr);
	void INC(uint16_t addr);
	void DEC(uint16_t addr);
	void INX();
	void DEX();
	void INY();
	void DEY();

	bool getOverFlowFlag() const;
	bool getNegativeFlag() const;
	void clearStatus();
	void setCarryFlag(bool value);
    bool getCarryFlag();
    void setZeroFlag(bool value);
	bool getZeroFlag() const;
    void setInterruptDisableFlag(bool value);
    void setDecimalModeFlag(bool value);
    void setBreakCommandFlag(bool value);
    void setOverflowFlag(bool value);
    void setNegativeFlag(bool value);

	// Bitwise OP codes
	void AND(uint16_t addr);
	void ORA(uint16_t addr);
	void EOR(uint16_t addr);
	void BIT(uint16_t addr);

	// Compare OP codes
	void CMP(uint16_t addr);
	void CPX(uint16_t addr);
	void CPY(uint16_t addr);

	// Access OP codes
	void LDA(uint16_t addr);
	void STA(uint16_t addr);
	void LDX(uint16_t addr);
	void STX(uint16_t addr);
	void LDY(uint16_t addr);
	void STY(uint16_t addr);

	// JMP can be an absolute address or indirect.
	void JMP_ABS(uint16_t addr);
	void JMP_IND(uint16_t addr);
	void JSR(uint16_t addr);
	void RTS(uint16_t addr);


private:
	// Masks for status register
	static constexpr uint8_t overflow_mask = 0x40;
	static constexpr uint8_t negative_mask = 0x80;
	static constexpr uint8_t carry_mask = 0x01;
	static constexpr uint8_t zero_mask = 0x02;
	std::vector<uint8_t> memory;
	std::vector<uint8_t> stack;
	Bus *bus = nullptr;
	uint8_t bus_read(uint16_t address);
	void bus_write(uint16_t address, uint8_t data);

private: // Addressing Modes
	uint8_t ACC(); // Accumulator
	uint8_t ABS(); // Absolute
	uint8_t ABSX(); // Absolute X
	uint8_t ABSY(); // Absolute Y
	uint8_t IMM(); // Immediate
	uint8_t IMP(); // Implied
	uint8_t IND(); // Indirect
	uint8_t XIND(); // X, Indirect
	uint8_t INDY(); // Indirect, Y
	uint8_t REL(); // Relative
	uint8_t ZP(); // Zero Page
	uint8_t ZPX(); // Zero Page, X
	uint8_t ZPY(); // Zero Page, Y

private: // Opcodes
	uint8_t ADC(); // Add with carry
	//uint8_t AND(); // AND (with accumulator)
	uint8_t ASL(); // Arithmetic shift left
	uint8_t BCC(); // Branch on carry clear
	uint8_t BCS(); // Branch on carry set
	uint8_t BEQ(); // Branch on equal (zero set)
	//uint8_t BIT(); // Bit test
	uint8_t BMI(); // Branch on minus (negative set)
	uint8_t BNE(); // Branch on not equal (zero clear)
	uint8_t BPL(); // Branch on plus (negative clear)
	uint8_t BRK(); // Break (interrupt)
	uint8_t BVC(); // Branch on overflow clear
	uint8_t BVS(); // Break on overflow set
	void CLC(); // Clear carry
	void CLD(); // Clear decimal
	void CLI(); // Clear interrupt disable
	void CLV(); // Clear overflow
	//uint8_t CMP(); // Compare (with accumlulator)
	//uint8_t CPX(); // Compare with X
	//uint8_t CPY(); // Compare with Y
	uint8_t DEC(); // Decrement
	// uint8_t DEX(); // Decrement X
	// uint8_t DEY(); // Decrement Y
	//uint8_t EOR(); // Exclusive OR (with accumulator)
	uint8_t INC(); // Increment 
	// uint8_t INX(); // Increment X
	// uint8_t INY(); // Increment Y
	uint8_t JMP(); // Jump
	uint8_t JSR(); // Jump subroutine
	//uint8_t LDA(); // Load accumulaotr
	//uint8_t LDX(); // Load X
	//uint8_t LDY(); // Load Y
	uint8_t LSR(); // Logical shift right
	uint8_t NOP(); // No operation
	//uint8_t ORA(); // Or with accumulator
	uint8_t PHA(); // Push accumulator
	uint8_t PHP(); // Push processor status
	uint8_t PLA(); // Pull accumulator
	uint8_t PLP(); // Pull procesor status
	uint8_t ROL(); // Rotate left
	uint8_t ROR(); // Rotate Right
	uint8_t RTI(); // Return from interrupt
	uint8_t RTS(); // Return from subroutine
	uint8_t SBC(); // Subtract with carry
	void SEC(); // Set carry
	void SED(); // Set decimal
	void SEI(); // Set interrupt disable
	//uint8_t STA(); // Store accumulator
	//uint8_t STX(); // Store X
	//uint8_t STY(); // Store Y
	uint8_t TAX(); // Transfer Accumulator to X
	uint8_t TAY(); // Transfer Accumulator to Y
	uint8_t TSX(); // Transfer stack pointer to X
	uint8_t TXA(); // Transfer X to accumulator
	uint8_t TXS(); // Transfer X to stack pointer
	uint8_t TYA(); // Transfer Y to accumulator
	
};

#endif