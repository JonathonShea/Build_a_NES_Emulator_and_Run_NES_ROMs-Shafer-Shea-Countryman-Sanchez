#pragma once

#include <cstdint>
#include <chrono>
using namespace std::chrono_literals;

class Clock {
public:
	Clock();
	~Clock();
	bool tick();
	uint64_t getTicks() const;

private:
	static constexpr std::chrono::microseconds clockInterval = 1us;
	uint64_t ticks;
};