#include "Day9.h"
#include "Utilities/Utilities.h"

#include <stack>
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

inline uint8_t GetValue(const std::vector<uint8_t>& input, Vec2D pos, Vec2D size)
{
	if (pos.X < 0 || pos.X >= size.X || pos.Y < 0 || pos.Y >= size.Y)
		return 10;

	int i = pos.Y * size.X + pos.X;
	return input[i];
}

int Day9::RunA()
{
	int total = 0;

	int inputSize = (int)input.size() - 1;
	int width = input[inputSize];
	int height = inputSize / width;
	Vec2D size(width, inputSize / width);

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			int top = GetValue(input, Vec2D(x, y - 1), size);
			int left = GetValue(input, Vec2D(x - 1, y), size);
			int self = GetValue(input, Vec2D(x, y), size);
			int right = GetValue(input, Vec2D(x + 1, y), size);
			int bottom = GetValue(input, Vec2D(x, y + 1), size);

			if (self < top && self < left && self < right && self < bottom)
				total += self + 1;
		}

	return total;
}

inline int PosToIndex(size_t x, size_t y, size_t width)
{
	return (int)(y * width + x);
}

inline int TraverseBasin(std::vector<uint8_t>& input, size_t i, size_t width, size_t height, size_t inputSize)
{
	std::stack<Vec2D> stack;
	stack.push(Vec2D(int(i % width), int(i / width)));

	int basinSize = 0;
	while (!stack.empty())
	{
		Vec2D pos = stack.top();
		stack.pop();

		size_t i = pos.Y * width + pos.X;
		if (pos.X < 0 || pos.X >= width || pos.Y < 0 || pos.Y >= height || input[i] >= 9)
			continue;

		basinSize++;
		input[i] = 9;

		stack.push(Vec2D(pos.X, pos.Y - 1));
		stack.push(Vec2D(pos.X - 1, pos.Y));
		stack.push(Vec2D(pos.X + 1, pos.Y));
		stack.push(Vec2D(pos.X, pos.Y + 1));
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
			int size = TraverseBasin(input, i, width, height, inputSize);
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