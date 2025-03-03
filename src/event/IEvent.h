#pragma once
#include <string>

class IEvent {
public:
	virtual const std::string& getName() const = 0;

protected:

private:
};