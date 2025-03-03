#include "Cartridge.h"
#include <fstream>

Cartridge::Cartridge(std::vector<uint8_t>& romData){
	// TODO: Reset vector needs to be based on mapper.
	resetVector = 0xFFFC;
	// Load the ROM into the memory mapper
	prgRomSize = romData[4] * prgRomMultiplier;
	chrRomSize = romData[5] * chrRomMultiplier;
	flag6 = romData[6];
	flag7 = romData[7];
	flag8 = romData[8];
	flag9 = romData[9];
	flag10 = romData[10];
	prgRomStart = headerSize + (HasTrainer() ? trainerSize : 0);
	chrRomStart = prgRomStart + prgRomSize;
	if (HasTrainer()) {
		trainer = std::vector<uint8_t>(romData.begin() + headerSize, romData.begin() + trainerSize);
	}
	prgRom = std::vector<uint8_t>(romData.begin() + prgRomStart, romData.begin() + PrgRomEnd());
	if (prgRomSize == 0x4000) {
		prgRom.insert(prgRom.end(), romData.begin() + prgRomStart, romData.begin() + PrgRomEnd());
	}
	chrRom = std::vector<uint8_t>(romData.begin() + chrRomStart, romData.begin() + ChrRomEnd());
}


Cartridge::~Cartridge() {
	// Nothing to do here
}