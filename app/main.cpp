#include <iostream>

#include "NesRam.h"
#include "Clock.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <Cartridge.h>
#include <Utilities.h>
#include "CPU.h"
#include <memory>
#include "event/EventDispatcher.h"

static constexpr std::array<uint8_t, 4> magicNumbers = { 0x4E, 0x45, 0x53, 0x1A }; // NES<EOF> magic numbers to identify a NES ROM file

int main(int argc, const char * argv[]){
	Clock clock(3000, "CPU Clock");
	CPU cpu;
	std::ifstream romFile; 
	std::vector<uint8_t> romData;
	std::string filePath;
    if (argc > 1) {
        filePath = argv[1];
    }
    else{
        filePath = Utilities::OpenFileDialog();
    }
	romFile.open(filePath, std::ios::binary);
	if (romFile.is_open()) {
		std::cout << "Opened file: " << filePath << std::endl;
		romData = std::vector<uint8_t>(std::istreambuf_iterator<char>(romFile), std::istreambuf_iterator<char>());
		romFile.close();
	}
	else {
		std::cout << "Failed to open file: " << filePath << std::endl;
		return(0);
	}
	// Check for valid NES ROM file header
	for (int i = 0; i < magicNumbers.size(); i++) {
		if (romData[i] != magicNumbers[i]) {
			std::cout << "Invalid NES ROM file" << std::endl;
			return(0);
		}
	}
	auto cart = std::make_shared<Cartridge>(romData);
	cpu.SetCartridge(cart);

	while (true) {
		int cycles = cpu.execute();
		while (cycles > 0) {
			clock.tick();
			cycles--;
		}

	}
	

  return 0;
}


