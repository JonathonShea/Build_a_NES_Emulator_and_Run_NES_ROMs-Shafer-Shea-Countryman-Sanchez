// Created by Abel Sanchez om 11/17/2024
// Last updated: 1/21/2025

#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <vector>
#include <memory>
#include "Cartridge.h"
#include "Utilities.h"

class Bus;

class CPU
{
public:
	uint8_t accumulator = 0x00; // 8-bit arithmetic register
	uint8_t x = 0x00; // 8-bit general purpose register
	uint8_t y = 0x00; // 8-bit general purpose register
	uint8_t stack_pointer; // 8-bit register that contains lower 8 bits of stack
	uint16_t program_counter; // 16-bit register that contains a pointer to the next instruction
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

	void Execute();
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

	std::vector<uint8_t> getStackTESTING() const;
	void setStackBackTESTING(uint8_t value);

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
	void TXA(); // Transfer X to Accumulator
	void TYA(); // Transfer Y to Accumulator
	void TAX(); // Transfer Accumulator to X
	void TAY(); // Transfer Accumulator to Y


	// Stack OP codes
	void PHA();
	void PLA();
	void PHP();
	void PLP();
	void TXS();
	void TSX();

	// Jump opcodes
	// JMP can be an absolute address or indirect.
	void JMP_ABS(uint16_t addr);
	void JMP_IND(uint16_t addr);
	void JSR(uint16_t addr);
	void RTS(uint16_t addr);
	void BRK(); // Break (interrupt)

	// Branch Opcodes
	void BCC(int8_t offset); // Branch on carry clear
	void BCS(int8_t offset); // Branch on carry set
	void BMI(int8_t offset); // Branch on minus (negative set)
	void BPL(int8_t offset); // Branch on plus (negative clear)
	void BVC(int8_t offset); // Branch on overflow clear
	void BVS(int8_t offset); // Break on overflow set
	void RTI(); // Return from interrupt


	// Branch
	void BNE(uint16_t addr); // Branch on not equal (zero clear)
	void BEQ(uint16_t addr); // Branch on equal (zero set)


	void SetCartridge(std::shared_ptr<Cartridge> cartridge);


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

	std::vector<uint8_t> memory;
	std::vector<uint8_t> stack;
	std::shared_ptr<Cartridge> cart;
	Bus *bus = nullptr;

private: // Addressing Modes

private: // Opcodes
	void CLC(); // Clear carry
	void CLD(); // Clear decimal
	void CLI(); // Clear interrupt disable
	void CLV(); // Clear overflow
	void SEC(); // Set carry
	void SED(); // Set decimal
	void SEI(); // Set interrupt disable
};

#endif