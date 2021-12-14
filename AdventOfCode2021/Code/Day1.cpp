#include "Day1.h"
#include "Utilities/Utilities.h"

void Day1::ReadInput(std::ifstream& stream)
{
	Input.reserve(3000);
	ReadIntsFromStream(stream, Input);
}

int Day1::RunA()
{
	int currentValue = 0;
	int increases = 0;
	for (int value : Input)
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
	int values[3] = { Input[0], Input[1], Input[2] };
	int currentValue = values[0] + values[1] + values[2];
	int increases = 0;

	size_t end = Input.size() - 2;
	for (size_t i = 1; i < end; i++)
	{
		values[0] = values[1];
		values[1] = values[2];
		values[2] = Input[i + 2];
		int depth = values[0] + values[1] + values[2];
		if (depth > currentValue)
		{
			increases++;
		}
		currentValue = depth;
	}

	return increases;
}