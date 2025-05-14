// // Created by Abel Sanchez on 1/19/2025
// // Last updated: 1/21/2025

#include "Bus.h"

Bus::Bus()
{	
    // Initialize memory with 0x00
    memory.resize(0x10000, 0x00); // 64KB of memory
}

// Bus::~Bus()
// {
// }

void Bus::write(uint16_t address, uint8_t data)
{
	// Ensures we only access the valid address range
	if (address >= 0x0000 && address <= 0xFFFF)  {
        memory[address] = data;
	}
}

uint8_t Bus::read(uint16_t address)
{
	if (address >= 0x0000 && address <= 0xFFFF) {
		return memory[address];
	}
    else {
        return 0x00; // Return 0 if address is out of range
    }
}

// 	// If we attempt to read outside the valid address range
// 	return 0x0000;
// }
