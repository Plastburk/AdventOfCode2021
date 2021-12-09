#pragma once

#include "Utilities/Utilities.h"

#include <vector>
#include <array>
#include <fstream>

class Day8
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	std::vector<CharString<7>> input;
};
