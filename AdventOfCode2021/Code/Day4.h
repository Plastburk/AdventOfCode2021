#pragma once

#include <vector>
#include <fstream>

class Day4
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	std::vector<int> Input1;
	std::vector<int> Input2;
};
