#pragma once

#include <vector>
#include <array>
#include <fstream>

class Day20
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	std::array<char, 512> Input1;
	std::array<bool, 200 * 200> Input2{};
	int InitialDimension;
};
