#include "Day18.h"
#include "Utilities/Utilities.h"

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

inline int FindPairToExplode(const std::vector<SnailfishNumber>& number)
{
	const size_t size = number.size();
	for (int i = 0; i < size - 1; i++)
	{
		uint8_t depth1 = number[i].Depth;
		uint8_t depth2 = number[i + 1].Depth;
		if (depth1 > 4 && depth1 == depth2)
			return i;
	}
	return -1;
}

inline void ExplodePair(std::vector<SnailfishNumber>& number, int i)
{
	if (i > 0)
		number[i - 1].Number += number[i].Number;

	if (i < number.size() - 2)
		number[i + 2].Number += number[i + 1].Number;

	number[i].Number = 0;
	number[i].Depth--;

	number.erase(number.begin() + i + 1);
}

inline int FindNumberToSplit(const std::vector<SnailfishNumber>& number)
{
	const size_t size = number.size();
	for (int i = 0; i < size; i++)
	{
		if (number[i].Number >= 10)
			return i;
	}
	return -1;
}

inline void SplitNumber(std::vector<SnailfishNumber>& number, int i)
{
	uint8_t orig = number[i].Number;
	uint8_t a = orig / 2;
	uint8_t b = (a * 2) == orig ? a : a + 1;

	number[i].Number = a;
	number[i].Depth++;
	number.emplace(number.begin() + i + 1, b, number[i].Depth);
}

inline void ReduceNumber(std::vector<SnailfishNumber>& number)
{
	while (true)
	{
		int i = FindPairToExplode(number);
		while (i >= 0)
		{
			ExplodePair(number, i);
			i = FindPairToExplode(number);
		}

		int j = FindNumberToSplit(number);
		if (j >= 0)
			SplitNumber(number, j);

		if (i < 0 && j < 0)
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
	int max = 0;
	for (int i = 0; i < Input.size() - 1; i++)
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

			if (aMag > max)
				max = aMag;
			if (bMag > max)
				max = bMag;
		}

	return max;
}