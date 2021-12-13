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
	char input[30000];
#pragma warning(pop)
	int inputSize = 0;
	int width = 0;
	int height = 0;
};
