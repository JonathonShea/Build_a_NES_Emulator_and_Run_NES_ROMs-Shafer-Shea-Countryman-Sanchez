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

struct RGB {
	uint8_t r, g, b;
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

	static const RGB nes_color_palette[64]; //Global NES palette
	std::vector<std::vector<uint8_t>> patternTables; //Vector chosen over arrray for modern adapation of PatternTables
	std::vector<uint8_t> paletteMemory; // Palette Memory
	NameTable nameTables[2]; //2 Physical NameTable + Attribute Tables

	PPU();

	void cpuWrite(uint16_t address, uint8_t data);
	void write(uint16_t address, uint8_t data);
	void loadPatternTable(const std::vector<uint8_t>& chrROM);
	void step();
	void SetOam(std::shared_ptr<OAM> oam) { m_oam = oam; }
	std::array<uint8_t, 64> getPatternTile(int tableIndex, int tileIndex) const;

	// Local + Test Functions
	void printPatternTables();

//private:
	std::shared_ptr<OAM> m_oam;

	// Palette memory functions
	uint8_t readPaletteMemory(uint16_t address);
	void writePaletteMemory(uint16_t address, uint8_t data);
	RGB getColor(uint8_t paletteIndex) const;

	//NameTable Functions
	void writeNameTable(uint16_t address, uint8_t data);
	uint8_t readNameTable(uint16_t address) const;
	int getTableIndex(uint16_t address) const;

};

#endif