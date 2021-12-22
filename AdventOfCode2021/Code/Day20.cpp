#include "Day20.h"
#include "Utilities/Utilities.h"

#include <atomic>
#include <ppl.h>

void Day20::ReadInput(std::ifstream& stream)
{
	stream.read(Input1.data(), Input1.size());

	char c = 0;
	do
	{
		stream.read(&c, 1);
	} while (c != '#' && c != '.');

	stream.seekg(-1, stream.cur);

	char buffer[100 * 100 + 99];
	stream.read(buffer, sizeof(buffer));
	int readBytes = (int)stream.gcount();

	InitialDimension = 0;
	for (int i = 0; i < readBytes; i++)
		if (buffer[i] == '\n')
		{
			InitialDimension = i;
			break;
		}

	int origin = 100 - (InitialDimension / 2);
	for (int y = 0; y < InitialDimension; y++)
		for (int x = 0; x < InitialDimension; x++)
		{
			int i = y * (InitialDimension + 1) + x;

			int xx = x + origin;
			int yy = y + origin;
			int j = yy * 200 + xx;
			Input2[j] = buffer[i] == '#' ? true : false;
		}
}

inline int GetValue(const std::array<bool, 200 * 200>& map, int x, int y, int origin, int end, bool outOfBoundsValue)
{
	if (x >= origin && y >= origin && x <= end && y <= end)
		return (int)map[y * 200 + x];
	return (int)outOfBoundsValue;
}

int Run(const std::array<char, 512>& input1, const std::array<bool, 200 * 200>& input2, int initialDimension, int iterations)
{
	std::array<bool, 200 * 200> map1(input2);
	std::array<bool, 200 * 200> map2{};
	std::array<bool, 200 * 200>* map = &map1;
	std::array<bool, 200 * 200>* nextMap = &map2;

	int origin = 100 - (initialDimension / 2);
	int end = origin + initialDimension - 1;

	int outOfBounds[2]{ false, input1[0] == '#' };

	std::atomic<int> litPixels = 0;
	for (int i = 0; i < iterations; i++)
	{
		litPixels = 0;
		int prevOrigin = origin;
		int prevEnd = end;
		origin--;
		end++;

		concurrency::parallel_for(origin, end + 1, [&](int y)
			{
				int litPixelsThread = 0;
				for (int x = origin; x <= end; x++)
				{
#define Get(x, y) GetValue(*map, x, y, prevOrigin, prevEnd, outOfBounds[0])

					int enhanceIndex =
						(Get(x - 1, y - 1) << 8) | (Get(x, y - 1) << 7) | (Get(x + 1, y - 1) << 6) |
						(Get(x - 1, y) << 5) | (Get(x, y) << 4) | (Get(x + 1, y) << 3) |
						(Get(x - 1, y + 1) << 2) | (Get(x, y + 1) << 1) | Get(x + 1, y + 1);

					bool lit = input1[enhanceIndex] == '#';
					(*nextMap)[y * 200 + x] = lit;
					if (lit)
						litPixelsThread++;
				}

				litPixels += litPixelsThread;
			}
		);

		std::swap(map, nextMap);
		std::swap(outOfBounds[0], outOfBounds[1]);
	}

	return litPixels;
}

int Day20::RunA()
{
	return Run(Input1, Input2, InitialDimension, 2);
}

int Day20::RunB()
{
	return Run(Input1, Input2, InitialDimension, 50);
}