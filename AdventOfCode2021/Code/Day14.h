#pragma once

#include <vector>
#include <fstream>

class Day14
{
public:
	void ReadInput(std::ifstream& stream);

	uint64_t RunA();
	uint64_t RunB();

private:
	std::vector<char> Input1;
	std::vector<char> Input2;
};
