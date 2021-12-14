#include "Day5.h"
#include "Utilities/Utilities.h"

void Day5::ReadInput(std::ifstream& stream)
{
	input.reserve(4000);
	ReadFuncFromStream<ReadUInt16sT>(stream,
		input, ReadUInt16s, { 0, ',', ' ', '\n' }
	);
}

int Day5::RunA()
{
	const int dimensions = 1000;
	std::vector<std::uint8_t> map(dimensions * dimensions, 0);

	int total = 0;
	for (size_t i = 0; i < input.size(); i += 4)
	{
		size_t x1 = input[i];
		size_t y1 = input[i + 1];
		size_t x2 = input[i + 2];
		size_t y2 = input[i + 3];

		if (x1 != x2 && y1 != y2)
			continue;

		int xDirection = (x2 < x1) ? -1 : 1;
		int yDirection = (y2 < y1) ? -1 : 1;

		if (x1 != x2)
		{
			// Horizontal
			size_t x = x1;
			size_t num = std::abs((long long)x2 - (long long)x1);

			for (size_t j = 0; j <= num; j++, x += xDirection)
				if (++map[y1 * dimensions + x] == 2)
					total++;
		}
		else
		{
			// Vertical
			size_t y = y1;
			size_t num = std::abs((long long)y2 - (long long)y1);

			for (size_t j = 0; j <= num; j++, y += yDirection)
				if (++map[y * dimensions + x1] == 2)
					total++;
		}
	}

	return total;
}

int Day5::RunB()
{
	const int dimensions = 1000;
	std::vector<uint8_t> map(dimensions * dimensions, 0);

	int total = 0;
	for (size_t i = 0; i < input.size(); i += 4)
	{
		size_t x1 = input[i];
		size_t y1 = input[i + 1];
		size_t x2 = input[i + 2];
		size_t y2 = input[i + 3];

		int xDirection = (x2 < x1) ? -1 : 1;
		int yDirection = (y2 < y1) ? -1 : 1;

		if (x1 != x2 && y1 == y2)
		{
			// Horizontal
			size_t x = x1;
			size_t num = std::abs((long long)x2 - (long long)x1);

			for (size_t j = 0; j <= num; j++, x += xDirection)
				if (++map[y1 * dimensions + x] == 2)
					total++;
		}
		else if (x1 == x2 && y1 != y2)
		{
			// Vertical
			size_t y = y1;
			size_t num = std::abs((long long)y2 - (long long)y1);

			for (size_t j = 0; j <= num; j++, y += yDirection)
				if (++map[y * dimensions + x1] == 2)
					total++;
		}
		else
		{
			// Diagonal
			size_t x = x1;
			size_t y = y1;
			size_t num = std::abs((long long)x2 - (long long)x1);
			for (int i = 0; i <= num; i++, x += xDirection, y += yDirection)
				if (++map[y * dimensions + x] == 2)
					total++;
		}
	}

	return total;
}