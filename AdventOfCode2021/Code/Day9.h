#pragma once

#include <vector>
#include <fstream>

class Day9
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	std::vector<uint8_t> input;
};
