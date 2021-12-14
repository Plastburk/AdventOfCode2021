#pragma once

#include <vector>
#include <fstream>

class Day2
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	std::vector<std::tuple<char, int>> Input;
};
