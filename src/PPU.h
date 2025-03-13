/*
    Created by Ryan Countryman on 11/17/2024
    Last Updated: 3/3/2025
*/
#ifndef PPU_H
#define PPU_H

#include <vector>
#include <cstdint>
#include "Cartridge.h"
#include "Utilities.h"

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
    
	std::vector<std::vector<uint8_t>> patternTables; //Vector chosen over arrray for modern adapation of PatternTables

	PPU();

	void respTest();
	void loadPatternTable(const std::vector<uint8_t>& chrROM);
	void printPatternTables();

};

#endif
