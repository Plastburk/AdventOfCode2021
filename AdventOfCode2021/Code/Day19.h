#pragma once

#include <vector>
#include <fstream>

#include "Utilities/Types.h"

class Day19
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	std::vector<std::vector<Vec3D>> Input;
};
