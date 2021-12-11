#pragma once

#include <vector>
#include <fstream>

class Day11
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	char input[300];
	int inputSize = 0;
	int width = 0;
	int height = 0;
};
