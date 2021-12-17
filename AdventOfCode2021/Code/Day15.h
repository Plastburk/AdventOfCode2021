#pragma once

#include <vector>
#include <fstream>

class Day15
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	char Input[15000]{};
	int InputSize = 0;
	int Width = 0;
	int Height = 0;
};
