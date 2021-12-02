#pragma once
#include <string>
#include <vector>

class Day1
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	std::vector<int> input;
};
