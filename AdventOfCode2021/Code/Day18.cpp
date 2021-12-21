#include "Day18.h"
#include "Utilities/Utilities.h"
#include "Utilities/Mutex.h"

#include <ppl.h>

void Day18::ReadInput(std::ifstream& stream)
{
	Input.reserve(200);

	char buffer[8192];
	int depth = 0;
	std::vector<SnailfishNumber>* line = &Input.emplace_back();

	while (true)
	{
		stream.read(buffer, sizeof(buffer));
		char* c = buffer;

		std::streamsize bytes = stream.gcount();
		if (bytes == 0)
			break;

		while (bytes > 0)
		{
			if (*c == '\n')
			{
				line = &Input.emplace_back();
				depth = 0;
			}
			else if (*c == '[')
				depth++;
			else if (*c == ']')
				depth--;
			else if (*c >= '0' && *c <= '9')
				line->emplace_back(*c - '0', depth);

			c++;
			bytes--;
		}
	}

	if (Input.back().size() == 0)
		Input.pop_back();
}

inline bool ExplodePairs(std::vector<SnailfishNumber>& number)
{
	bool anyExploded = false;
	int firstValid = 0;
	while (true)
	{
		int j = -1;
		const size_t size = number.size();
		for (int i = firstValid; i < size - 1; i++)
		{
			uint8_t depth1 = number[i].Depth;
			uint8_t depth2 = number[i + 1].Depth;
			if (depth1 > 4 && depth1 == depth2)
			{
				j = i;
				break;
			}
		}
		if (j < 0)
			break;

		if (j > 0)
			number[j - 1].Number += number[j].Number;

		if (j < size - 2)
			number[j + 2].Number += number[j + 1].Number;

		number[j].Number = 0;
		number[j].Depth--;

		number.erase(number.begin() + j + 1);
		firstValid = j;
		anyExploded = true;
	}
	return anyExploded;
}

inline bool TrySplit(std::vector<SnailfishNumber>& number)
{
	int j = -1;
	const size_t size = number.size();
	for (int i = 0; i < size; i++)
	{
		if (number[i].Number >= 10)
		{
			j = i;
			break;
		}
	}

	if (j < 0)
		return false;

	uint8_t orig = number[j].Number;
	uint8_t a = orig / 2;
	uint8_t b = (a * 2) == orig ? a : a + 1;

	number[j].Number = a;
	number[j].Depth++;
	number.emplace(number.begin() + j + 1, b, number[j].Depth);

	return true;
}

inline void ReduceNumber(std::vector<SnailfishNumber>& number)
{
	while (true)
	{
		bool anyExploded = ExplodePairs(number);
		bool splitNumber = TrySplit(number);

		if (!anyExploded && !splitNumber)
			break;
	}
}

inline void AddNumbers(std::vector<SnailfishNumber>& number1, const std::vector<SnailfishNumber>& number2)
{
	number1.insert(number1.end(), number2.begin(), number2.end());

	for (int i = 0; i < number1.size(); i++)
		number1[i].Depth++;
}

inline int CalculateMagnitude(const std::vector<SnailfishNumber>& number, int& i, int depth)
{
	int a;

	if (number[i].Depth == depth)
		a = number[i++].Number * 3;
	else
		a = CalculateMagnitude(number, i, depth + 1) * 3;

	if (number[i].Depth == depth)
		return a + (number[i++].Number * 2);
	else
		return a + (CalculateMagnitude(number, i, depth + 1) * 2);
}

int Day18::RunA()
{
	std::vector<SnailfishNumber> number = Input[0];

	for (int i = 1; i < Input.size(); i++)
	{
		AddNumbers(number, Input[i]);
		ReduceNumber(number);
	}

	int j = 0;
	return CalculateMagnitude(number, j, 1);
}

int Day18::RunB()
{
	Mutex mutex;

	int max = 0;
	concurrency::parallel_for(0, (int)Input.size() - 1, [&](int i)
		{
			for (int j = i + 1; j < Input.size(); j++)
			{
				std::vector<SnailfishNumber> a = Input[i];
				std::vector<SnailfishNumber> b = Input[j];
				AddNumbers(a, Input[j]);
				AddNumbers(b, Input[i]);

				ReduceNumber(a);
				ReduceNumber(b);

				int aa = 0;
				int bb = 0;
				int aMag = CalculateMagnitude(a, aa, 1);
				int bMag = CalculateMagnitude(b, bb, 1);

				{
					Lock lock(mutex);

					if (aMag > max)
						max = aMag;
					if (bMag > max)
						max = bMag;
				}
			}
		});

	return max;
}