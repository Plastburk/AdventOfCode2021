#include "Day9.h"
#include "Utilities/Utilities.h"

#include <set>

void Day9::ReadInput(std::ifstream& stream)
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

inline char GetValue(const char* Input, int x, int y, int width)
{
	int i = y * (width + 1) + x;
	return Input[i];
}

int Day9::RunA()
{
	int total = 0;

	for (int y = 0; y < Height; y++)
		for (int x = 0; x < Width; x++)
		{
			char self = GetValue(Input, x, y, Width);

			if ((y == 0 || GetValue(Input, x, y - 1, Width) > self) &&
				(x == 0 || GetValue(Input, x - 1, y, Width) > self) &&
				(x == Width - 1 || GetValue(Input, x + 1, y, Width) > self) &&
				(y == Height - 1 || GetValue(Input, x, y + 1, Width) > self))
				total += (self - '0') + 1;
		}

	return total;
}

inline int TraverseBasin(char* Input, size_t i, size_t width, size_t height)
{
	std::vector<Vec2D> stack;
	stack.push_back(Vec2D(int(i % (width + 1)), int(i / (width + 1))));

	int basinSize = 0;
	while (!stack.empty())
	{
		Vec2D pos = stack.back();
		stack.pop_back();

		size_t j = pos.Y * (width + 1) + pos.X;
		if (Input[j] == '9')
			continue;

		basinSize++;
		Input[j] = '9';

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

	for (size_t i = 0; i < InputSize; i++)
	{
		if (Input[i] < '9' && Input[i] != '\n')
		{
			int size = TraverseBasin(Input, i, Width, Height);
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