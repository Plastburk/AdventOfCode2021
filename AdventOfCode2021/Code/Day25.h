#pragma once

#include <vector>
#include <fstream>

class Day25
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();

private:
	char Input[20000]{};
	int Width = 0;
	int Height = 0;
};
