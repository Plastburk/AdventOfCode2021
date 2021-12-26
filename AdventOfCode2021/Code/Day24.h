#pragma once

#include <vector>
#include <fstream>

class Day24
{
public:
	void ReadInput(std::ifstream& stream);

	uint64_t RunA();
	uint64_t RunB();

private:
	std::vector<std::tuple<int, int, int>> Input;
};
