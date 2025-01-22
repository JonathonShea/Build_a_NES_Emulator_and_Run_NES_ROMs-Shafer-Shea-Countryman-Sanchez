// Created by Abel Sanchez om 11/17/2024
// Last updated: 1/21/2025

#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <vector>

class CPU
{
public:
	uint8_t accumulator = 0x00; // 8-bit arithmetic register
	uint8_t x = 0x00; // 8-bit general purpose register
	uint8_t y = 0x00; // 8-bit general purpose register
	uint8_t stack_pointer = 0x0000; // 8-bit register that contains lower 8 bits of stack
	uint16_t program_counter = 0x00; // 16-bit register that contains a pointer to the next instruction
	uint8_t status = 0x00; // 8-bit register that contains status flags

	CPU();

	void respTest();
	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);
	void ASL(uint16_t addr);
	void LSR(uint16_t addr);
	void ROL(uint16_t addr);
	void ROR(uint16_t addr);
	void ADC(uint16_t addr);
	void INC(uint16_t addr);

	bool getOverFlowFlag() const;
	bool getNegativeFlag() const;


private:
	// Masks for status register
	static constexpr uint8_t overflow_mask = 0x40;
	static constexpr uint8_t negative_mask = 0x80;
	static constexpr uint8_t carry_mask = 0x01;
	static constexpr uint8_t zero_mask = 0x02;
	std::vector<uint8_t> memory;

	void setCarryFlag(bool value);
    bool getCarryFlag();
    void setZeroFlag(bool value);
    void setInterruptDisableFlag(bool value);
    void setDecimalModeFlag(bool value);
    void setBreakCommandFlag(bool value);
    void setOverflowFlag(bool value);
    void setNegativeFlag(bool value);
};

#endif