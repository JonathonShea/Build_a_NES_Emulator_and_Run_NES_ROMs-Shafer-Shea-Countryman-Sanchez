#include "Clock.h"
#include <thread>

Clock::Clock() {
	// Initialize the clock
	ticks = 0;
}

Clock::~Clock() {
	// Nothing to do here
}


uint64_t Clock::getTicks() const {
	// Return the number of ticks
	return ticks;
}

bool Clock::tick() {
	// Increment the number of ticks
	std::this_thread::sleep_for(Clock::clockInterval);
	ticks++;
	return true;
}

