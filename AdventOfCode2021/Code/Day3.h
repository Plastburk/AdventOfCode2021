#pragma once

#include <vector>
#include <fstream>

class Day3
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	std::vector<uint32_t> input;
	int bits = 0;
};
