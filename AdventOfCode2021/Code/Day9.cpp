#include "Day9.h"
#include "Utilities/Utilities.h"

#include <set>

void Day9::ReadInput(std::ifstream& stream)
{
	input.reserve(12000);
	ReadPartialsFromStream<Read2DMapT>(stream,
		input, Read2DMap, {}
	);
}

struct Vec2D
{
	int X;
	int Y;

	Vec2D(int x, int y) :
		X(x), Y(y)
	{	}
};

inline uint8_t GetValue(const std::vector<uint8_t>& input, int x, int y, int width)
{
	int i = y * width + x;
	return input[i];
}

int Day9::RunA()
{
	int total = 0;

	int inputSize = (int)input.size() - 1;
	int width = input[inputSize];
	int height = inputSize / width;

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			int self = GetValue(input, x, y, width);

			if ((y == 0 || GetValue(input, x, y - 1, width) > self) &&
				(x == 0 || GetValue(input, x - 1, y, width) > self) &&
				(x == width - 1 || GetValue(input, x + 1, y, width) > self) &&
				(y == height - 1 || GetValue(input, x, y + 1, width) > self))
				total += self + 1;
		}

	return total;
}

inline int TraverseBasin(std::vector<uint8_t>& input, size_t i, size_t width, size_t height)
{
	std::vector<Vec2D> stack;
	stack.push_back(Vec2D(int(i % width), int(i / width)));

	int basinSize = 0;
	while (!stack.empty())
	{
		Vec2D pos = stack.back();
		stack.pop_back();

		size_t i = pos.Y * width + pos.X;
		if (input[i] == 9)
			continue;

		basinSize++;
		input[i] = 9;

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

	size_t inputSize = input.size() - 1;
	size_t width = input[inputSize];
	size_t height = inputSize / width;
	for (size_t i = 0; i < inputSize; i++)
	{
		if (input[i] < 9)
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