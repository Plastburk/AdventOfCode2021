#include "Day2.h"
#include "Utilities/Utilities.h"

void Day2::ReadInput(std::ifstream& stream)
{
	Input.reserve(2000);
	ReadTypeAndIntFromStream(stream, Input);
}

int Day2::RunA()
{
	int horizontal = 0;
	int depth = 0;
	for (const auto& line : Input)
	{
		auto &[type, value] = line;

		if (type == 'f')
			horizontal += value;
		else if (type == 'd')
			depth += value;
		else if (type == 'u')
			depth -= value;
	}

	return horizontal * depth;
}

int Day2::RunB()
{
	int horizontal = 0;
	int depth = 0;
	int aim = 0;
	for (const auto& line : Input)
	{
		auto &[type, value] = line;

		if (type == 'f')
		{
			horizontal += value;
			depth += aim * value;
		}
		else if (type == 'd')
			aim += value;
		else if (type == 'u')
			aim -= value;
	}

	return horizontal * depth;
}