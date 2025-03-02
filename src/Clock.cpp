#include "Clock.h"
#include <thread>

Clock::Clock(uint64_t prescaler, const char* name) : mClockDivider(prescaler), mTicks(0), mName(name){
	// Initialize the clock
}

Clock::~Clock() {
	// Nothing to do here
}


uint64_t Clock::getTicks() const {
	// Return the number of ticks
	return mTicks;
}

bool Clock::tick() {
	// Increment the number of ticks
	while (mTicks++ % mClockDivider != 0) {
		std::this_thread::sleep_for(Clock::clockInterval);
	}
	return true;
}

