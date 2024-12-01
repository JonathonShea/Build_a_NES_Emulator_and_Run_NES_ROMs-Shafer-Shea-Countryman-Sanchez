#pragma once
#include <array>
#include <cstdint>

class NesRam {
public:
	NesRam();
	~NesRam();
	bool write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr);

private:
	static constexpr uint16_t ramSize = 0x0800;
	static constexpr bool validateAddr(uint16_t addr) { return addr < ramSize; }
	std::array<uint8_t, ramSize> ram;
};