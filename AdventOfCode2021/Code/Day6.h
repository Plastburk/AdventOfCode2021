#pragma once

#include <vector>
#include <fstream>

class Day6
{
public:
	void ReadInput(std::ifstream& stream);

	uint64_t RunA();
	uint64_t RunB();

private:
	std::vector<uint8_t> Input;
};
