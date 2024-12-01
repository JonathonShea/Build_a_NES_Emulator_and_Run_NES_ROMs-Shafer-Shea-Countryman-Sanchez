#pragma once
#include <vector>
#include <cstdint>
#include "MemoryMapper.h"
#include <filesystem>

/**
 * @brief Class representing NES cartridge. Contains the memory mapper and the ROM data.
 */
class Cartridge
{
public:
	/**
	 * @brief Construct a new Cartridge object
	 *
	 * @param rom Path to the ROM file to load
	 */
	Cartridge(std::filesystem::path rom);
	~Cartridge();
	uint16_t read(uint16_t addr);
	bool write(uint16_t addr, uint8_t data);

private:
	static constexpr int prgRomMultiplier = 0x4000;
	uint8_t flag6;
	uint8_t flag7;
	uint8_t flag8;
	uint8_t flag9;
	uint8_t flag10;
	std::vector<uint8_t> prgRom;
	std::vector<uint8_t> chrRom;
	memoryMapper::MemoryMapper mapper;
};