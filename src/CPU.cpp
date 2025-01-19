// Created by Abel Sanchez om 11/17/2024
// Last updated: 12/1/2024

#include "CPU.h"
#include "Bus.h"
#include <iostream>

void CPU::respTest()
{
    std::cout << "The CPU says hi! Say hi back or else..." << std::endl;
}

uint8_t CPU::read(uint16_t address) {
	return bus->read(address, false);
}

void CPU::write(uint16_t address, uint8_t data) {
	bus->write(address, data);
}