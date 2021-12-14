#include "Day11.h"
#include "Utilities/Utilities.h"

void Day11::ReadInput(std::ifstream& stream)
{
	stream.read(Input, sizeof(Input));
	InputSize = (int)stream.gcount();

	for (int i = 0; i < InputSize; i++)
		if (Input[i] == '\n')
		{
			Width = i;
			break;
		}

#pragma warning(push)
#pragma warning(disable:6385)
	if (Input[InputSize - 1] == '\n')
		Height = InputSize / (Width + 1);
	else
		Height = InputSize / (Width);
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
	int totalSize = Width * Height;
	map.resize(totalSize);
	for (int y = 0; y < Height; y++)
		memcpy(map.data() + ((size_t)y * Width), Input + (y * ((size_t)Width + 1)), Width * sizeof(char));

	int flashes = 0;
	for (int step = 0; step < 100; step++)
		flashes += PerformStep(map, Width, Height);

	return flashes;
}

int Day11::RunB()
{
	std::vector<char> map;
	int totalSize = Width * Height;
	map.resize(totalSize);
	for (int y = 0; y < Height; y++)
		memcpy(map.data() + ((size_t)y * Width), Input + (y * ((size_t)Width + 1)), Width * sizeof(char));

	for (int step = 0;; step++)
	{
		int flashes = PerformStep(map, Width, Height);
		if (flashes == totalSize)
			return step + 1;
	}

	return 0;
}