#pragma once

#include <vector>
#include <fstream>

class Day10
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	uint64_t RunB();

private:
	std::vector<std::string> input;
};
