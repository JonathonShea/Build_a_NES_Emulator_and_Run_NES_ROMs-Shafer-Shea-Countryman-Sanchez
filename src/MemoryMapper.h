//
// Created by sheaj on 11/15/2024.
//

#ifndef MEMORYMAPPER_H
#define MEMORYMAPPER_H

#include <vector>

namespace memoryMapper {

class MemoryMapper {
public:
	MemoryMapper();
	~MemoryMapper();

private:

	std::vector<std::vector<uint8_t>> prgRomBanks;
	std::vector<std::vector<uint8_t>> chrRomBanks;

};

} // memoryMapper

#endif //MEMORYMAPPER_H
