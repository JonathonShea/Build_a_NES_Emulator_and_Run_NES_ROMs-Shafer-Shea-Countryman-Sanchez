#pragma once
#include <string>
#include <cstdint>
#include <chrono>
#include "event/IEvent.h"
using namespace std::chrono_literals;

class Clock : IEvent{
public:
	Clock(uint64_t prescaler, const char* name);
	~Clock();
	bool tick();
	uint64_t getTicks() const;
	virtual const std::string& getName() const override { return this->mName; }

private:
	std::string mName;
	uint64_t mTicks;
	uint64_t mClockDivider;
	static constexpr auto frequency = 21.477272 * 1e6; // MHz
	static constexpr uint32_t periodt = (1 / frequency) * 1e9;
	static constexpr std::chrono::nanoseconds clockInterval = static_cast<std::chrono::nanoseconds>(periodt);
	
};