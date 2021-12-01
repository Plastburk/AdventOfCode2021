#include "Day1.h"
#include "Utilities.h"

int Day1::RunA(const std::vector<std::string>& input)
{
	int currentValue = 0;
	int increases = 0;
	for (const std::string& value : input)
	{
		int depth = ToInt(value);
		if (depth > currentValue)
		{
			increases++;
		}
		currentValue = depth;
	}
	return increases - 1;
}

int Day1::RunB(const std::vector<std::string>& input)
{
	int values[3] = { ToInt(input[0]), ToInt(input[1]), ToInt(input[2]) };
	int currentValue = values[0] + values[1] + values[2];
	int increases = 0;

	size_t end = input.size() - 2;
	for (size_t i = 1; i < end; i++)
	{
		values[0] = values[1];
		values[1] = values[2];
		values[2] = ToInt(input[i + 2]);
		int depth = values[0] + values[1] + values[2];
		if (depth > currentValue)
		{
			increases++;
		}
		currentValue = depth;
	}

	return increases;
}