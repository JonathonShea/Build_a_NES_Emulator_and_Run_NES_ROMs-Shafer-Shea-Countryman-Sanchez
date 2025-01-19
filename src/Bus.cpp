#include "Bus.h"

Bus::Bus()
{	
	// Connect CPU to the bus
	cpu.ConnectBus(this);

	// Clear RAM
	for (auto& i : ram) {
		i = 0x00;
	}
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

void Bus::read(uint16_t address)
{
	if (address >= 0x0000 && address <= 0xFFFF) {
		return ram[address];
	}

	// If we attempt to read outside the valid address range
	return 0x0000;
}
