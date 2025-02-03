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
	Cartridge(std::vector<uint8_t> &romData);
	~Cartridge();
	inline int prgRomEnd() const { return prgRomStart + prgRomSize; }
	inline int chrRomEnd() const { return chrRomStart + chrRomSize; }
	inline bool getMirroring() const { return flag6 & mirroringMask; }
	inline bool hasBattery() const { return flag6 & batteryMask; }
	inline bool hasTrainer() const { return flag6 & trainerMask; }

private:
	// Constants for data sizes and offsets
	static constexpr int prgRomMultiplier = 0x4000;
	static constexpr int chrRomMultiplier = 0x2000;
	static constexpr int headerSize = 16;
	static constexpr int trainerSize = 512;
	static constexpr int mirroringMask = 0x01;
	static constexpr int batteryMask = 0x02;
	static constexpr int trainerMask = 0x04;


	int prgRomStart;
	int chrRomStart;
	int prgRomSize;
	int chrRomSize;
	uint8_t flag6;
	uint8_t flag7;
	uint8_t flag8;
	uint8_t flag9;
	uint8_t flag10;
	std::vector<uint8_t> trainer;
	std::vector<uint8_t> prgRom;
	std::vector<uint8_t> chrRom;
	memoryMapper::MemoryMapper mapper;
};

