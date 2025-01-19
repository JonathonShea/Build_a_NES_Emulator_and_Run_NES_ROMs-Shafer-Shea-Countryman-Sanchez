#include <cstdint>
#include "CPU.h"
#ifndef Bus_H
#define Bus_H

class Bus
{
public:
	Bus();
	~BUS();

public: // Devices that can connect to the bus
	// CPU
	CPU cpu;

	// RAM
	std::array<uint8, 64 * 1024> ram;

public:
	void write(uint16_t address, uint8_t data);
	uint8_t read(uint16_t address);
};

