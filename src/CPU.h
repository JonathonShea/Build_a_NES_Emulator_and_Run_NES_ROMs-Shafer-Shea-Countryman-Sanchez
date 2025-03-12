#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <vector>
#include <memory>
#include "Cartridge.h"
#include "Utilities.h"
#include <unordered_map>

class CPU
{
public:
	// Registers
	uint8_t accumulator = 0x00;				// 8-bit arithmetic register
	uint8_t x = 0x00;						// 8-bit general purpose register
	uint8_t y = 0x00;						// 8-bit general purpose register
	uint8_t stack_pointer;					// 8-bit register that contains lower 8 bits of stack
	uint16_t program_counter;				// 16-bit register that contains a pointer to the next instruction
	uint8_t status = 0x00;					// 8-bit register that contains status flags

	// Interrupt signals
	bool irq_signal = false;
	bool nmi_signal = false;
	bool reset_signal = false;

	CPU();

	void respTest();
	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);

	std::vector<uint8_t> getStackTESTING() const;
	void setStackBackTESTING(uint8_t value);

	// Execution 
	uint8_t execute();
	void SetCartridge(std::shared_ptr<Cartridge> cartridge);

	// Interrupt signal setters and handler
	void setIRQ(bool state);   
	void setNMI(bool state);    
	void setRESET(bool state);  
	void handleInterrupts();

private:
	// Masks for status register
	static constexpr uint8_t negative_mask = 0x80;
	static constexpr uint8_t overflow_mask = 0x40;
	static constexpr uint8_t break_mask = 0x10;
	static constexpr uint8_t decimal_mode_mask = 0x08;
	static constexpr uint8_t interrupt_disable_mask = 0x04;
	static constexpr uint8_t zero_mask = 0x02;
	static constexpr uint8_t carry_mask = 0x01;

	static constexpr uint16_t reset_vector = 0xFFFc; // It all starts here!!!
	static constexpr uint16_t irq_vector = 0xFFFE;   // IRQ/BRK vector
	static constexpr uint16_t nmi_vector = 0xFFFA;   // NMI vector

	std::vector<uint8_t> memory;
	std::vector<uint8_t> stack;
	std::shared_ptr<Cartridge> cart;

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

public: // Addressing Modes - Returns the effective address for each addressing mode
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

	// No Operation
	void NOP();

	private:
    std::unordered_map<uint8_t, std::string> opcodeMap = {
    {0x00, "BRK"},
    {0x01, "ORA"},
    {0x05, "ORA"},
    {0x06, "ASL"},
    {0x08, "PHP"},
    {0x09, "ORA"},
    {0x0A, "ASL"},
    {0x0D, "ORA"},
    {0x0E, "ASL"},
    {0x10, "BPL"},
    {0x11, "ORA"},
    {0x15, "ORA"},
    {0x16, "ASL"},
    {0x18, "CLC"},
    {0x19, "ORA"},
    {0x1D, "ORA"},
    {0x1E, "ASL"},
    {0x20, "JSR"},
    {0x21, "AND"},
    {0x24, "BIT"},
    {0x25, "AND"},
    {0x26, "ROL"},
    {0x28, "PLP"},
    {0x29, "AND"},
    {0x2A, "ROL"},
    {0x2C, "BIT"},
    {0x2D, "AND"},
    {0x2E, "ROL"},
    {0x30, "BMI"},
    {0x31, "AND"},
    {0x35, "AND"},
    {0x36, "ROL"},
    {0x38, "SEC"},
    {0x39, "AND"},
    {0x3D, "AND"},
    {0x3E, "ROL"},
    {0x40, "RTI"},
    {0x41, "EOR"},
    {0x45, "EOR"},
    {0x46, "LSR"},
    {0x48, "PHA"},
    {0x49, "EOR"},
    {0x4A, "LSR"},
    {0x4C, "JMP"},
    {0x4D, "EOR"},
    {0x4E, "LSR"},
    {0x50, "BVC"},
    {0x51, "EOR"},
    {0x55, "EOR"},
    {0x56, "LSR"},
    {0x58, "CLI"},
    {0x59, "EOR"},
    {0x5D, "EOR"},
    {0x5E, "LSR"},
    {0x60, "RTS"},
    {0x61, "ADC"},
    {0x65, "ADC"},
    {0x66, "ROR"},
    {0x68, "PLA"},
    {0x69, "ADC"},
    {0x6A, "ROR"},
    {0x6C, "JMP"},
    {0x6D, "ADC"},
    {0x6E, "ROR"},
    {0x70, "BVS"},
    {0x71, "ADC"},
    {0x75, "ADC"},
    {0x76, "ROR"},
    {0x78, "SEI"},
    {0x79, "ADC"},
    {0x7D, "ADC"},
    {0x7E, "ROR"},
    {0x81, "STA"},
    {0x84, "STY"},
    {0x85, "STA"},
    {0x86, "STX"},
    {0x88, "DEY"},
    {0x8A, "TXA"},
    {0x8C, "STY"},
    {0x8D, "STA"},
    {0x8E, "STX"},
    {0x90, "BCC"},
    {0x91, "STA"},
    {0x94, "STY"},
    {0x95, "STA"},
    {0x96, "STX"},
    {0x98, "TYA"},
    {0x99, "STA"},
    {0x9A, "TXS"},
    {0xA0, "LDY"},
    {0xA1, "LDA"},
    {0xA2, "LDX"},
    {0xA4, "LDY"},
    {0xA5, "LDA"},
    {0xA6, "LDX"},
    {0xA8, "TAY"},
    {0xA9, "LDA"},
    {0xAA, "TAX"},
    {0xAC, "LDY"},
    {0xAD, "LDA"},
    {0xAE, "LDX"},
    {0xB0, "BCS"},
    {0xB1, "LDA"},
    {0xB4, "LDY"},
    {0xB5, "LDA"},
    {0xB6, "LDX"},
    {0xB8, "CLV"},
    {0xB9, "LDA"},
    {0xBA, "TSX"},
    {0xBC, "LDY"},
    {0xBD, "LDA"},
    {0xBE, "LDX"},
    {0xC0, "CPY"},
    {0xC1, "CMP"},
    {0xC4, "CPY"},
    {0xC5, "CMP"},
    {0xC6, "DEC"},
    {0xC8, "INY"},
    {0xC9, "CMP"},
    {0xCA, "DEX"},
    {0xCC, "CPY"},
    {0xCD, "CMP"},
    {0xCE, "DEC"},
    {0xD0, "BNE"},
    {0xD1, "CMP"},
    {0xD5, "CMP"},
    {0xD6, "DEC"},
    {0xD8, "CLD"},
    {0xD9, "CMP"},
    {0xDD, "CMP"},
    {0xDE, "DEC"},
    {0xE0, "CPX"},
    {0xE1, "SBC"},
    {0xE4, "CPX"},
    {0xE5, "SBC"},
    {0xE6, "INC"},
    {0xE8, "INX"},
    {0xE9, "SBC"},
    {0xEA, "NOP"},
    {0xEC, "CPX"},
    {0xED, "SBC"},
    {0xEE, "INC"},
    {0xF0, "BEQ"},
    {0xF1, "SBC"},
    {0xF5, "SBC"},
    {0xF6, "INC"},
    {0xF8, "SED"},
    {0xF9, "SBC"},
    {0xFD, "SBC"},
    {0xFE, "INC"}
        };
};

#endif