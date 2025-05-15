/*
    Created by Ryan Countryman on 11/17/2024
    Last Updated: 3/23/2025
*/
#ifndef PPU_H
#define PPU_H

#include <vector>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include "Cartridge.h"
#include "Utilities.h"
#include "OAM.h"
#include <memory>
#include <array>
#include "Bus.h"

#define PPU_WIDTH 256
#define PPU_HEIGHT 240

struct RGB {
	uint8_t r, g, b;
};


struct ShiftRegister{
	std::array<bool,16> reg;
	int index = 0;
	void Insert(uint8_t val){
		for (int i = 0; i < 8; ++i) {
			reg[index] = (val >> (7 - i)) & 1;
			index = index++ % 16;
		}
	}
};


struct NameTable {
	std::vector<uint8_t> tiles;
	std::vector<uint8_t> attributes;

	NameTable() {
		tiles.resize(960, 0);
		attributes.resize(64, 0);
	}
};


class PPU
{
public:
	uint8_t PPUCTRL = 0x00; //General Controller (Write)
	uint8_t PPUMASK = 0x00; //Rendering Controller (Write)
	uint8_t PPUSTATUS = 0x00; //Rendering Status (Read)
	uint8_t OAMADDR = 0x00; //Sprite RAM Address (Write)
	uint8_t OAMDATA = 0x00; //Sprite Data(Read / Write)
	uint8_t PPUSCROLL = 0x00; //X-Y Coords for Scroll (Write) WRITE AFTER PPUADDR
	uint8_t PPUADDR = 0x00; //VRAM Address (Write) Communication with CPU
	uint8_t PPUDATA = 0x00; //VRAM Data (Read / Write) 
	uint8_t OAMDMA = 0x00; //Sprite DMA (Write) Suspend CPU to begin DMA
	uint32_t dot; // These are the "pixels" in the scanline. Goes up to 340 then wraps around
	static const RGB nes_color_palette[64]; //Global NES palette
	std::vector<std::vector<uint8_t>> patternTables; //Vector chosen over arrray for modern adapation of PatternTables
	std::vector<uint8_t> paletteMemory; // Palette Memory
	NameTable nameTables[2]; //2 Physical NameTable + Attribute Tables
	bool toggle = false;
	bool toggle2 = false;
	uint8_t tilePlaneLow[2][256][8] = {}; //Storage for incomplete Pattern Table tiles
	uint8_t tilePlaneHigh[2][256][8] = {};

	std::vector<uint8_t> chrRam = std::vector<uint8_t>(0x2000, 0);

	// For PPUSCROLL
	bool scroll_latch = false; // *
	uint8_t scroll_x = 0; // *
	uint8_t scroll_y = 0; // *

	// For PPUADDR
	bool addr_latch = false; // *
	uint8_t addr_high = 0; // *
	uint8_t addr_low = 0; // *
	uint16_t vram_address = 0; // *

	uint8_t fetched_nametable_byte = 0;
	uint8_t fetched_attribute_byte = 0;
	uint8_t fetched_pattern_low = 0;
	uint8_t fetched_pattern_high = 0;

	ShiftRegister shift1;
	ShiftRegister shift2;
	static constexpr uint8_t vBlankMask = 0x80;
	static constexpr uint16_t maxCycles = 341; // Maximum cycles per scanline
	std::shared_ptr<OAM> m_oam;
	std::shared_ptr<Cartridge> m_cart; // Pointer to the cartridge
	std::shared_ptr<Bus> m_bus; // Pointer to the bus

	PPU(std::shared_ptr<Bus> bus, std::shared_ptr<Cartridge> cart, std::shared_ptr<OAM> oam);
	PPU() = default;

	void cpuWrite(uint16_t address, uint8_t data);
	void write(uint16_t address, uint8_t data);
	void loadPatternTable(const std::vector<uint8_t>& chrROM);
	void step();
	void SetOam(std::shared_ptr<OAM> oam) { m_oam = oam; }
	const uint32_t* getFrameBuffer();
	void writeToFrameBuffer(int scanline, const std::vector<RGB>& colors);
  
	std::array<uint8_t, 64> getPatternTile(int tableIndex, int tileIndex) const;

	// Local + Test Functions
	void printPatternTables();
	void SetCartridge(std::shared_ptr<Cartridge> cart) { m_cart = cart; } 

//private:

	int Read(uint16_t addr) const; // Read from the PPU memory
	void RenderScanline();
	void stepScanline();
	void setNMI();
	uint16_t scanline = 0; // Current scanline. Goes up to 261 then wraps around
	void setVBlank() {PPUSTATUS |= vBlankMask;}
	void clearVBlank() {PPUSTATUS &= ~vBlankMask; vram_address = 0;}
	bool RenderingEnabled() { return PPUMASK &  0x0008;} // Background rendering

	const char* framebufferFilename = "output.bmp"; // Filename for the BMP file

	// Palette memory functions
	uint8_t readPaletteMemory(uint16_t address);
	void writePaletteMemory(uint16_t address, uint8_t data);
	RGB getColor(uint8_t paletteIndex) const;

	//NameTable Functions
	void writeNameTable(uint16_t address, uint8_t data);
	uint8_t readNameTable(uint16_t address) const;
	int getTableIndex(uint16_t address) const;

	//Pattern Table Functions
	void writePatternTable(uint16_t address, uint8_t data);


};

#endif