// // Created by Abel Sanchez on 1/19/2025
// // Last updated: 1/21/2025

#include "Bus.h"
#include "PPU.h"
Bus::Bus()
{	
    // Initialize memory with 0x00
    memory.resize(0x10000, 0x00); // 64KB of memory
}

// Bus::~Bus()
// {
// }
void Bus::write(uint16_t address, uint8_t data) {
    //std::cout << "Bus WRITE: Addr=0x" << std::hex << address << " Data=0x" << std::hex << (int)data;

    if (address >= 0x2000 && address <= 0x3FFF) {
        // Mirroring PPU registers every 8 bytes
        uint16_t reg = 0x2000 + (address & 0x0007);
        std::cout << " (PPU Register Mirror: 0x" << std::hex << reg << ")";
        if (ppu) {
            ppu->cpuWrite(reg, data);
            std::cout << " (Memory Write)";
        }
    }
    else if (address <= 0xFFFF) {
        memory[address] = data;
    }

    std::cout << std::endl;
}

uint8_t Bus::read(uint16_t address) {
    if (address >= 0x2000 && address <= 0x2007) {
        if (ppu) {
            return ppu->cpuRead(address);
        }
    }
    else if(address <= 0xFFFF) {
        return memory[address];
    }
    else {
		return 0x00; // Return 0 if address is out of range
    }
    // Otherwise, read from main memory:
    return memory[address];
}
// 	// If we attempt to read outside the valid address range
// 	return 0x0000;
// }
