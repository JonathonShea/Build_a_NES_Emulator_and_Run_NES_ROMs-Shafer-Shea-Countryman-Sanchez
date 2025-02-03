#include "NesRam.h"

NesRam::NesRam() {
	// Initialize the RAM with 0x00
	std::fill(std::begin(ram), std::end(ram), 0x00);
}

NesRam::~NesRam() {
	// Nothing to do here
}

bool NesRam::write(uint16_t addr, uint8_t data) {
	// Write data to the RAM
	if (validateAddr(addr)){
		ram[addr] = data;
		return true;
	}
	return false;

}

uint8_t NesRam::read(uint16_t addr) {
	// Read data from the RAM
	return ram[addr];
}



