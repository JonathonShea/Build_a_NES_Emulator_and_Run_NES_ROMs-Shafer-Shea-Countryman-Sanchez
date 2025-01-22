// Created by Abel Sanchez on 1/19/2025
// Last updated: 1/21/2025

#include "Bus.h"

Bus::Bus()
{	
	// Connect CPU to the bus
	cpu.connect_bus(this);
}

Bus::~Bus()
{
}

void Bus::write(uint16_t address, uint8_t data)
{
	// Ensures we only access the valid address range
	if (address >= 0x0000 && address <= 0xFFFF)  {
		ram[address] = data;
	}
}

void Bus::read(uint16_t address, bool readOnly)
{
	if (address >= 0x0000 && address <= 0xFFFF) {
		return ram[address];
	}

	// If we attempt to read outside the valid address range
	return 0x0000;
}
