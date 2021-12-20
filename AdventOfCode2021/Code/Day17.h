#pragma once

#include <vector>
#include <fstream>
#include "Utilities/Utilities.h"

class Day17
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	Vec2D Min;
	Vec2D Max;
};
