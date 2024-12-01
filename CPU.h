// Created by Abel Sanchez om 11/17/2024
// Last updated: 12/1/2024

#ifndef CPU_H
#define PPU_H

#include <cstdint>

class CPU
{
public:
	uint8_t accumulator = 0x00; // 8-bit arithmetic register
	uint8_t x = 0x00; // 8-bit general purpose register
	uint8_t y = 0x00; // 8-bit general purpose register
	uint8_t stack_pointer = 0x0000; // 8-bit register that contains lower 8 bits of stack
	uint16_t = program_counter = 0x00; // 16-bit register that contains a pointer to the next instruction
	uint8_t status = 0x00; // 8-bit register that contains status flags

	CPU(){
	}

	void respTest();
};

#endif