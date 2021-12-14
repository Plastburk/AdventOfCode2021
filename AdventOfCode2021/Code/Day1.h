#pragma once

#include <vector>
#include <fstream>

class Day1
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	std::vector<int> Input;
};
