#pragma once

#include <vector>
#include <fstream>

class Day21
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	uint64_t RunB();

private:
	int Player1Start = 0;
	int Player2Start = 0;
};
