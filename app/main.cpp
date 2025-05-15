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
#include <random>

#define CPU_CYCLES_PER_FRAME 29780

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

void runAnimationTests(CPU& cpu, PPU& ppu, int& scanlineOffset, int frameCount) {


    std::vector<RGB> colors(PPU_WIDTH, RGB{ 1, 1, 0 }); // Initialize colors to black
    ppu.writeToFrameBuffer(1, colors);
}

void present_frame(SDL_Texture* texture, SDL_Renderer* renderer, const uint32_t* framebuffer, int width, int height) {
    void* pixels;
    int pitch;
    SDL_LockTexture(texture, NULL, &pixels, &pitch);
    memcpy(pixels, framebuffer, width * height * sizeof(uint32_t));
    SDL_UnlockTexture(texture);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int main(int argc, const char* argv[]) {
	Clock clock(1, "CPU Clock");
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
	auto bus = std::make_shared<Bus>();
	auto oam = std::make_shared<OAM>();
	CPU cpu(bus, cart, oam); // Create CPU instance
	PPU ppu(bus, cart, oam); // Create PPU instance
	bus->ppu = &ppu; // PPU Pointer For cpuWrite functionality within BUS
	ppu.loadPatternTable(cart->getCHRROM()); // load the CHR ROM into PPU's pattern tables

	// ppu.dumpPatternTablesToBitmap("output.bmp"); // dump the pattern tables to BMP
	InputHandler inputHandler; // Create an InputHandler instance

	if (!inputHandler.initialize()) {
		return -1;
	}

	// initialize window
	SDL_Window* window = SDL_CreateWindow("NES Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256 * 3, 240 * 3, SDL_WINDOW_SHOWN);
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
	int scanline = 0;

	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 240);

	if (!texture) {
		std::cerr << "Failed to create texture! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	int frame = 0;

	while (running) {
		Uint32 frameStart = SDL_GetTicks(); // Start time
		Uint32 curTime;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
			inputHandler.processEvent(event); // Use the InputHandler class
		}

		//print time elapsed
		curTime = SDL_GetTicks();
		std::cout << "Time elapsed for input handling: " << (curTime - frameStart) << " ms" << std::endl;

		cpu.controller1_state = inputHandler.getControllerState();

		// print time elapsed
		curTime = SDL_GetTicks();
		std::cout << "Time elapsed for controller state: " << (curTime - frameStart) << " ms" << std::endl;

		int framecycles = 0;
		while (framecycles < CPU_CYCLES_PER_FRAME) {
			int cycles = cpu.execute(); // Executes one instruction
			framecycles += cycles;

			// Step the PPU for each CPU cycle (3 PPU steps per CPU cycle)
			for (int i = 0; i < cycles * 3; ++i) {
				ppu.step();
			}
		}
		//print time elapsed
		curTime = SDL_GetTicks();
		std::cout << "Time elapsed for CPU execution: " << (curTime - frameStart) << " ms" << std::endl;

		frame++;

		present_frame(texture, renderer, ppu.getFrameBuffer(), 256, 240);

		curTime = SDL_GetTicks();
		std::cout << "Time elapsed for render: " << (curTime - frameStart) << " ms" << std::endl;

		std::cout << "Frame: " << frame << " Framecycles: " << framecycles << std::endl;
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

