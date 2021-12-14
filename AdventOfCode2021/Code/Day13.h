#pragma once

#include "Utilities/Types.h"

#include <vector>
#include <fstream>

class Day13
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	void RunB();

private:
	std::vector<Vec2D> Input1;
	std::vector<std::tuple<char, int>> Input2;
};
