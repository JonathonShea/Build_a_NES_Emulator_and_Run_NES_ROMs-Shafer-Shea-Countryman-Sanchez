// Created by Abel Sanchez on 11/17/2024
// Last updated: 1/21/2025

#include "CPU.h"
#include "Bus.h"
#include <iostream>

uint8_t CPU::bus_read(uint16_t address) {
	return bus->read(address, false);
}

void CPU::bus_write(uint16_t address, uint8_t data) {
	bus->write(address, data);
}