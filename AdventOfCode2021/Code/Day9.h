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
#pragma warning(push)
#pragma warning(disable:26495)
	char Input[30000];
#pragma warning(pop)
	int InputSize = 0;
	int Width = 0;
	int Height = 0;
};
