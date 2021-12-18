#pragma once

#include <vector>
#include <fstream>
#include <bitset>

class Day16
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	uint64_t RunB();

private:
	std::bitset<8192> Input;
};
