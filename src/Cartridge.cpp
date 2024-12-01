#include "Cartridge.h"
#include <fstream>

Cartridge::Cartridge(std::filesystem::path rom) {
	// Load the ROM into the memory mapper
	std::ifstream romData;
	romData.open(rom, std::ios::binary);
}