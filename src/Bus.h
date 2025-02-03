// // Created by Abel Sanchez on 1/19/2025
// // Last updated: 1/21/2025

// #ifndef BUS_H
// #define BUS_H
// #include <cstdint>
// #include "CPU.h"
// #include <array>

// class Bus
// {
// public:
// 	Bus();
// 	~BUS();

// public: // Devices that are connected to the bus
// 	// CPU
// 	CPU cpu;

// 	// RAM
// 	std::array<uint8, 64 * 1024> ram;

// public: 
// 	void write(uint16_t address, uint8_t data);
// 	uint8_t read(uint16_t address, bool readOnly = false);
// };

