#pragma once

#include <vector>
#include <fstream>

class Day5
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	std::vector<uint16_t> input;
};
