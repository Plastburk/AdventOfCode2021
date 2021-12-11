#include "Day11.h"
#include "Utilities/Utilities.h"

void Day11::ReadInput(std::ifstream& stream)
{
	stream.read(input, sizeof(input));
	inputSize = (int)stream.gcount();

	for (int i = 0; i < inputSize; i++)
		if (input[i] == '\n')
		{
			width = i;
			break;
		}

#pragma warning(push)
#pragma warning(disable:6385)
	if (input[inputSize - 1] == '\n')
		height = inputSize / (width + 1);
	else
		height = inputSize / (width);
#pragma warning(pop)
}

inline void IncreaseValue(std::vector<char>& map, std::vector<int>& stack, int x, int y, int width)
{
	int i = y * width + x;
	map[i]++;
	if (map[i] == '9' + 1)
		stack.push_back(i);
}

inline int PerformStep(std::vector<char>& map, int width, int height)
{
	int flashes = 0;
	std::vector<int> stack;
	for (int i = 0; i < map.size(); i++)
	{
		map[i]++;
		if (map[i] == '9' + 1)
			stack.push_back(i);
	}

	while (!stack.empty())
	{
		int i = stack.back();
		stack.pop_back();
		flashes++;
		int x = i % width;
		int y = i / width;

		bool isNotLeft = x > 0;
		bool isNotTop = y > 0;
		bool isNotRight = x < width - 1;
		bool isNotBottom = y < height - 1;

		if (isNotLeft)
			IncreaseValue(map, stack, x - 1, y, width);
		if (isNotLeft && isNotTop)
			IncreaseValue(map, stack, x - 1, y - 1, width);
		if (isNotLeft && isNotBottom)
			IncreaseValue(map, stack, x - 1, y + 1, width);

		if (isNotRight)
			IncreaseValue(map, stack, x + 1, y, width);
		if (isNotRight && isNotTop)
			IncreaseValue(map, stack, x + 1, y - 1, width);
		if (isNotRight && isNotBottom)
			IncreaseValue(map, stack, x + 1, y + 1, width);

		if (isNotTop)
			IncreaseValue(map, stack, x, y - 1, width);
		if (isNotBottom)
			IncreaseValue(map, stack, x, y + 1, width);
	}

	for (size_t i = 0; i < map.size(); i++)
	{
		if (map[i] > '9')
			map[i] = '0';
	}

	return flashes;
}

int Day11::RunA()
{
	std::vector<char> map;
	int totalSize = width * height;
	map.resize(totalSize);
	for (int y = 0; y < height; y++)
		memcpy(map.data() + ((size_t)y * width), input + (y * ((size_t)width + 1)), width * sizeof(char));

	int flashes = 0;
	for (int step = 0; step < 100; step++)
		flashes += PerformStep(map, width, height);

	return flashes;
}

int Day11::RunB()
{
	std::vector<char> map;
	int totalSize = width * height;
	map.resize(totalSize);
	for (int y = 0; y < height; y++)
		memcpy(map.data() + ((size_t)y * width), input + (y * ((size_t)width + 1)), width * sizeof(char));

	for (int step = 0;; step++)
	{
		int flashes = PerformStep(map, width, height);
		if (flashes == totalSize)
			return step + 1;
	}

	return 0;
}