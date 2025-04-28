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
#include "PPU.h"
#include <memory>
#include "event/EventDispatcher.h"
#include "input.h"
#include <OAM.h>
static constexpr std::array<uint8_t, 4> magicNumbers = { 0x4E, 0x45, 0x53, 0x1A }; // NES<EOF> magic numbers to identify a NES ROM file

SDL_Texture* LoadBMP(const std::string& filePath, SDL_Renderer* renderer) {
	SDL_Surface* imageSurface = SDL_LoadBMP(filePath.c_str());
	if (!imageSurface) {
		std::cerr << "Failed to load BMP: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
	SDL_FreeSurface(imageSurface); // Free surface after creating texture

	if (!texture) {
		std::cerr << "Failed to create texture from BMP: " << SDL_GetError() << std::endl;
	}

	return texture;
}

int main(int argc, const char* argv[]) {
	Clock clock(1, "CPU Clock");
	CPU cpu;
	PPU ppu; // Create PPU instance
	auto oam = std::make_shared<OAM>();
	cpu.SetOAM(oam); // Set OAM for the CPU
	ppu.SetOam(oam); // Set OAM for the PPU
	std::ifstream romFile;
	std::vector<uint8_t> romData;
	std::string filePath;
	if (argc > 1) {
		filePath = argv[1];
	}
	else {
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
	ppu.loadPatternTable(cart->getCHRROM()); // load the CHR ROM into PPU's pattern tables
	ppu.dumpPatternTablesToBitmap("output.bmp"); // dump the pattern tables to BMP
	InputHandler inputHandler; // Create an InputHandler instance

	if (!inputHandler.initialize()) {
		return -1;
	}

	// initialize window
	SDL_Window* window = SDL_CreateWindow("NES Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 512, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}

	// initialize the renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
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

		int randomX = std::rand() % 256;  // Random X coordinate
		int randomY = std::rand() % 128;   // Random Y coordinate
		ppu.writePixel(randomX, randomY, { 255,0,0 }, "output.bmp");


		SDL_Texture* texture = LoadBMP("output.bmp", renderer);
		if (!texture) {
			std::cerr << "Failed to load texture from BMP!" << std::endl;
			continue;
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
		SDL_DestroyTexture(texture);

		int cycles = cpu.execute();
		while (cycles > 0) {
			clock.tick();
			cycles--;
			ppu.step(); // Step the PPU
			ppu.step(); // There are 3 PPU steps per cycle
			ppu.step(); // Calling this three times as a "catch-up" method of keeping them in sync
		}
		SDL_Delay(16); // Add a small delay to prevent CPU overuse
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

