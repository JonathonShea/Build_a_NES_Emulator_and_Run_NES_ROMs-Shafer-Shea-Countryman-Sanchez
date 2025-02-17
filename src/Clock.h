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
	static constexpr auto frequency = 21.477272 * 1e6; // MHz
	static constexpr uint32_t periodt = (1 / frequency) * 1e9;
	static constexpr std::chrono::nanoseconds clockInterval = static_cast<std::chrono::nanoseconds>(periodt);
	uint64_t ticks;
};