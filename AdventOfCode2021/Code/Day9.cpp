#include "Day9.h"
#include "Utilities/Utilities.h"

#include <set>

void Day9::ReadInput(std::ifstream& stream)
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

struct Vec2D
{
	int X;
	int Y;

	Vec2D(int x, int y) :
		X(x), Y(y)
	{	}
};

inline char GetValue(const char* input, int x, int y, int width)
{
	int i = y * (width + 1) + x;
	return input[i];
}

int Day9::RunA()
{
	int total = 0;

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			char self = GetValue(input, x, y, width);

			if ((y == 0 || GetValue(input, x, y - 1, width) > self) &&
				(x == 0 || GetValue(input, x - 1, y, width) > self) &&
				(x == width - 1 || GetValue(input, x + 1, y, width) > self) &&
				(y == height - 1 || GetValue(input, x, y + 1, width) > self))
				total += (self - '0') + 1;
		}

	return total;
}

inline int TraverseBasin(char* input, size_t i, size_t width, size_t height)
{
	std::vector<Vec2D> stack;
	stack.push_back(Vec2D(int(i % (width + 1)), int(i / (width + 1))));

	int basinSize = 0;
	while (!stack.empty())
	{
		Vec2D pos = stack.back();
		stack.pop_back();

		size_t i = pos.Y * (width + 1) + pos.X;
		if (input[i] == '9')
			continue;

		basinSize++;
		input[i] = '9';

		if (pos.Y > 0)
			stack.push_back(Vec2D(pos.X, pos.Y - 1));
		if (pos.X > 0)
			stack.push_back(Vec2D(pos.X - 1, pos.Y));
		if (pos.X < width - 1)
			stack.push_back(Vec2D(pos.X + 1, pos.Y));
		if (pos.Y < height - 1)
			stack.push_back(Vec2D(pos.X, pos.Y + 1));
	}

	return basinSize;
}

int Day9::RunB()
{
	std::multiset<int> largest;

	for (size_t i = 0; i < inputSize; i++)
	{
		if (input[i] < '9' && input[i] != '\n')
		{
			int size = TraverseBasin(input, i, width, height);
			if (largest.size() < 3)
				largest.insert(size);
			else if (size > *largest.begin())
			{
				largest.erase(largest.begin());
				largest.insert(size);
			}
		}
	}

	auto first = largest.cbegin();
	int total = *first++ * *(first++) * *(first);
	return total;
}