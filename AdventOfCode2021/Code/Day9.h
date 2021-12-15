#pragma once

#include <vector>
#include <fstream>

class Day9
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	char Input[30000]{};
	int InputSize = 0;
	int Width = 0;
	int Height = 0;
};
