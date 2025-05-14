// // Created by Abel Sanchez on 1/19/2025
// // Last updated: 1/21/2025

#ifndef BUS_H
#define BUS_H
#include <cstdint>
#include <memory>
#include <vector>

class Bus
{
public:
	Bus();
	~Bus() = default;

public: 
	void write(uint16_t address, uint8_t data);
	uint8_t read(uint16_t address);
    std::vector<uint8_t> memory;
	bool nmi = false; // CPU and PPU set this.
private:
	

// public: 

};
#endif // BUS_H

