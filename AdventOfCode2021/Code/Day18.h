#pragma once

#include <vector>
#include <fstream>

struct SnailfishNumber
{
	uint8_t Number;
	uint8_t Depth;

	SnailfishNumber(uint8_t number, uint8_t depth) : Number(number), Depth(depth) {}
};

class Day18
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	std::vector<std::vector<SnailfishNumber>> Input;
};
