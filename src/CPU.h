#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <vector>

class CPU
{
public:
	// Registers
	uint8_t accumulator = 0x00;				// 8-bit arithmetic register
	uint8_t x = 0x00;						// 8-bit general purpose register
	uint8_t y = 0x00;						// 8-bit general purpose register
	uint8_t stack_pointer = 0x0000;			// 8-bit register that contains lower 8 bits of stack
	uint16_t program_counter = 0x00;		// 16-bit register that contains a pointer to the next instruction
	uint8_t status = 0x00;					// 8-bit register that contains status flags

	CPU();

	void respTest();
	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);

	std::vector<uint8_t> getStackTESTING() const;
	void setStackBackTESTING(uint8_t value);

	uint8_t execute();

private:
	// Masks for status register
	static constexpr uint8_t negative_mask = 0x80;
	static constexpr uint8_t overflow_mask = 0x40;
	static constexpr uint8_t break_mask = 0x10;
	static constexpr uint8_t decimal_mode_mask = 0x08;
	static constexpr uint8_t interrupt_disable_mask = 0x04;
	static constexpr uint8_t zero_mask = 0x02;
	static constexpr uint8_t carry_mask = 0x01;

	// Stack and memory array
	std::vector<uint8_t> memory;
	std::vector<uint8_t> stack;

public: // Flag Operations - Sets, unsets, or clears status flags
	bool getOverFlowFlag() const;
	bool getNegativeFlag() const;
	void clearStatus();
	void setCarryFlag(bool value);
	bool getCarryFlag();
	void setZeroFlag(bool value);
	bool getZeroFlag() const;
	void setInterruptDisableFlag(bool value);
	void setDecimalModeFlag(bool value);
	bool getDecimalModeFlag() const;
	void setBreakCommandFlag(bool value);
	bool getBreakCommandFlag() const;
	void setOverflowFlag(bool value);
	void setNegativeFlag(bool value);

private: // Addressing Modes - Returns the effective address for each addressing mode
	uint16_t addr_implied();               // Implied
	uint16_t addr_accumulator();           // Accumulator
	uint16_t addr_immediate();             // Immediate 
	uint16_t addr_zero_page();             // Zero Page 
	uint16_t addr_zero_page_x();           // Zero Page X 
	uint16_t addr_zero_page_y();           // Zero Page Y 
	uint16_t addr_absolute();              // Absolute 
	uint16_t addr_absolute_x();            // Absolute X
	uint16_t addr_absolute_y();            // Absolute Y 
	uint16_t addr_indirect();              // Indirect 
	uint16_t addr_indexed_indirect_x();    // Indexed Indirect (X)
	uint16_t addr_indirect_indexed_y();    // Indirect Indexed (Y) 
	uint16_t addr_relative();              // Relative 

public: // 6502 Opcodes - Base implementation of each opcode
	// Shift Opcodes
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

	// Transfer opcodes
	void TXA();							// Transfer X to Accumulator
	void TYA();							// Transfer Y to Accumulator
	void TAX();							// Transfer Accumulator to X
	void TAY();							// Transfer Accumulator to Y

	// Stack OP codes
	void PHA();
	void PLA();
	void PHP();
	void PLP();
	void TXS();
	void TSX();

	// Jump opcodes
	void JMP_ABS(uint16_t addr);
	void JMP_IND(uint16_t addr);
	void JSR(uint16_t addr);
	void RTS(uint16_t addr);
	void BRK();							// Break (interrupt)

	// Branch Opcodes
	void BCC(uint16_t addr);			// Branch on carry clear
	void BCS(uint16_t addr);			// Branch on carry set
	void BMI(uint16_t addr);			// Branch on minus (negative set)
	void BPL(uint16_t addr);			// Branch on plus (negative clear)
	void BVC(uint16_t addr);			// Branch on overflow clear
	void BVS(uint16_t addr);			// Break on overflow set
	void RTI();							// Return from interrupt
	void BNE(uint16_t addr);			// Branch on not equal (zero clear)
	void BEQ(uint16_t addr);			// Branch on equal (zero set)

	// Flag Opcodes
	void CLC();							// Clear carry
	void CLD();							// Clear decimal
	void CLI();							// Clear interrupt disable
	void CLV();							// Clear overflow
	void SEC();							// Set carry
	void SED();							// Set decimal
	void SEI();							// Set interrupt disable
};

#endif