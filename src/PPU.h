/*
    Created by Ryan Countryman on 11/17/2024
    Last Updated: 3/3/2025
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

struct RGB {
	uint8_t r, g, b;
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
    
	static const RGB nes_color_palette[64]; //Global NES palette
	std::vector<std::vector<uint8_t>> patternTables; //Vector chosen over arrray for modern adapation of PatternTables
	std::vector<uint8_t> paletteMemory; // Palette Memory

	PPU();
	
	// Local + Test Functions
	void loadPatternTable(const std::vector<uint8_t>& chrROM);
	void printPatternTables();
	void step();
	void SetOam(std::shared_ptr<OAM> oam) { m_oam = oam; }

private:
std::shared_ptr<OAM> m_oam;

	// Palette memory functions
	uint8_t readPaletteMemory(uint16_t address);
	void writePaletteMemory(uint16_t address, uint8_t data);
	RGB getColor(uint8_t paletteIndex) const;
};

#endif
