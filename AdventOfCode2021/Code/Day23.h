#pragma once

#include <array>
#include <fstream>

class Day23
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	std::array<uint8_t, 19> Input;
};
