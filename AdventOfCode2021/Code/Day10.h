#pragma once

#include <vector>
#include <fstream>

class Day10
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	uint64_t RunB();

private:
#pragma warning(push)
#pragma warning(disable:26495)
	char Input[30000];
#pragma warning(pop)
	int InputSize = 0;
};
