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

struct RGB {
	uint8_t r, g, b;
};


struct ShiftRegister{
	std::array<bool,16> reg;
	int index = 0;
	void Insert(bool val){
		reg[index] = val;
		index++ % 16;
	}
};

struct InternalRegister{
	uint16_t v; // current VRAM address
	uint16_t t = 0; // temp VRAM address
	uint8_t x = 0; // fine x scroll
	bool w = false; // 1st or 2nd write toggle
	void yIncrement(){
		if ((v & 0x7000) != 0x7000)        // if fine Y < 7
		v += 0x1000;                      // increment fine Y
	else
		v &= ~0x7000;                     // fine Y = 0
	int y = (v & 0x03E0) >> 5;        // let y = coarse Y
	if (y == 29){
		y = 0;                          // coarse Y = 0
		v ^= 0x0800;                    // switch vertical nametable
	}
	else if (y == 31)
		y = 0;                         // coarse Y = 0, nametable not switched
	else
		y += 1;                         // increment coarse Y
	v = (v & ~0x03E0) | (y << 5);     // put coarse Y back into v
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
uint8_t cycle;
	static const RGB nes_color_palette[64]; //Global NES palette
	std::vector<std::vector<uint8_t>> patternTables; //Vector chosen over arrray for modern adapation of PatternTables
	std::vector<uint8_t> paletteMemory; // Palette Memory
	NameTable nameTables[2]; //2 Physical NameTable + Attribute Tables

	uint8_t tilePlaneLow[2][256][8] = {}; //Storage for incomplete Pattern Table tiles
	uint8_t tilePlaneHigh[2][256][8] = {};

	std::vector<uint8_t> chrRam = std::vector<uint8_t>(0x2000, 0);


	PPU();

	void cpuWrite(uint16_t address, uint8_t data);
	void write(uint16_t address, uint8_t data);
	void loadPatternTable(const std::vector<uint8_t>& chrROM);
	void step();
	void SetOam(std::shared_ptr<OAM> oam) { m_oam = oam; }
	void setPixel(std::vector<uint8_t>& pixelBuffer, int x, int y, const RGB& color, int imageWidth, int imageHeight);
	void dumpPatternTablesToBitmap(const std::string& filename);
	void writePixel(int x, int y, const RGB& color, const std::string& filename);
	void writeScanline(int scanline, const std::vector<RGB>& colors, const std::string& filename);
  
	std::array<uint8_t, 64> getPatternTile(int tableIndex, int tileIndex) const;

	// Local + Test Functions
	void printPatternTables();
	void SetCartridge(std::shared_ptr<Cartridge> cart) { m_cart = cart; } 

//private:
	static constexpr uint8_t vBlankMask = 0x80;
	static constexpr uint8_t maxCycles = 341; // Maximum cycles per scanline
		std::shared_ptr<OAM> m_oam;
	std::shared_ptr<Cartridge> m_cart; // Pointer to the cartridge
	int Read(uint16_t addr) const; // Read from the PPU memory
	void stepScanline();
	void RenderScanline();
	void setNMI();
	void setVBlank() {PPUSTATUS |= vBlankMask;}
	void clearVBlank() {PPUSTATUS &= ~vBlankMask; internalRegs.v = 0;}
	bool RenderingEnabled() { return PPUMASK &  0x0008;} // Background rendering
	ShiftRegister shift1;
	ShiftRegister shift2;
	InternalRegister internalRegs;

	// Palette memory functions
	uint8_t readPaletteMemory(uint16_t address);
	void writePaletteMemory(uint16_t address, uint8_t data);
	RGB getColor(uint8_t paletteIndex) const;
	void writeBMP(const std::vector<uint8_t>& pixelBuffer, int imageWidth, int imageHeight, const std::string& filename);
	bool readBMP(const std::string& filename, std::vector<uint8_t>& pixelBuffer, int& imageWidth, int& imageHeight);

	//NameTable Functions
	void writeNameTable(uint16_t address, uint8_t data);
	uint8_t readNameTable(uint16_t address) const;
	int getTableIndex(uint16_t address) const;

	//Pattern Table Functions
	void writePatternTable(uint16_t address, uint8_t data);


};

#endif