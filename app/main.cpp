#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL2/SDL.h>
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
#include "input.h"

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
	InputHandler inputHandler; // Create an InputHandler instance

	if (!inputHandler.initialize()) {
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow("NES Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}

	bool running = true;
	SDL_Event event;

	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
			inputHandler.processEvent(event); // Use the InputHandler class
		}
		SDL_Delay(16); // Add a small delay to prevent CPU overuse
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
	while (true) {
		clock.tick();
		int test =  cpu.read(0xfffc);
		std::cout << test << std::endl;
	}
	

  return 0;
}


