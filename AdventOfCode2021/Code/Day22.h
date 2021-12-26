#pragma once

#include "Utilities/Types.h"

#include <vector>
#include <fstream>

struct Cube
{
	Vec3D Min;
	Vec3D Max;
};

struct Instruction
{
	bool TurnOn;
	Cube Cube;
};

class Day22
{
public:
	void ReadInput(std::ifstream& stream);

	uint64_t RunA();
	uint64_t RunB();

private:
	std::vector<Instruction> Input;
};
