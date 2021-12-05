#include "Day1.h"
#include "Utilities/Utilities.h"

void Day1::ReadInput(std::ifstream& stream)
{
	input.reserve(3000);
	ReadIntsFromStream(stream, input);
}

int Day1::RunA()
{
	int currentValue = 0;
	int increases = 0;
	for (int value : input)
	{
		int depth = value;
		if (depth > currentValue)
		{
			increases++;
		}
		currentValue = depth;
	}
	return increases - 1;
}

int Day1::RunB()
{
	int values[3] = { input[0], input[1], input[2] };
	int currentValue = values[0] + values[1] + values[2];
	int increases = 0;

	size_t end = input.size() - 2;
	for (size_t i = 1; i < end; i++)
	{
		values[0] = values[1];
		values[1] = values[2];
		values[2] = input[i + 2];
		int depth = values[0] + values[1] + values[2];
		if (depth > currentValue)
		{
			increases++;
		}
		currentValue = depth;
	}

	return increases;
}