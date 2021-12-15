#include "Day14.h"
#include "Utilities/Utilities.h"

struct PolymerOutcome
{
	int IndexA;
	int IndexB;
	int NewElementIndex;

	PolymerOutcome() :
		IndexA(), IndexB(), NewElementIndex()
	{	}

	PolymerOutcome(int indexA, int indexB, int newElementIndex) :
		IndexA(indexA), IndexB(indexB), NewElementIndex(newElementIndex)
	{	}
};

void Day14::ReadInput(std::ifstream& stream)
{
	Input1.reserve(30);
	Input2.reserve(500);

	ReadFuncFromStream<ReadCharsT, ReadCharsT>(stream,
		Input1, ReadChars, { '\n' },
		Input2, ReadChars, { 0 }
	);
}

inline int PairToIndex(char a, char b)
{
	return (a - 'A') * 26 + (b - 'A');
}

inline int ElementToIndex(char element)
{
	return element - 'A';
}

inline uint64_t Run(const std::vector<char>& input1, const std::vector<char>& input2, int iterations)
{
	const int maxAmountOfPairs = 26 * 26;
	uint64_t pairs[maxAmountOfPairs]{};
	PolymerOutcome outcomes[maxAmountOfPairs]{};

	for (size_t i = 0; i < input2.size(); i += 3)
	{
		char c1 = input2[i];
		char c2 = input2[i + 1];
		char c3 = input2[i + 2];
		outcomes[PairToIndex(c1, c2)] = PolymerOutcome(PairToIndex(c1, c3), PairToIndex(c3, c2), ElementToIndex(c3));
	}

	uint64_t elementCount[26]{};
	for (size_t i = 0; i < input1.size() - 1; i++)
	{
		pairs[PairToIndex(input1[i], input1[i + 1])]++;
		elementCount[ElementToIndex(input1[i])]++;
	}
	elementCount[ElementToIndex(input1[input1.size() - 1])]++;

	for (int i = 0; i < iterations; i++)
	{
		uint64_t previousPairs[maxAmountOfPairs];
		memcpy(previousPairs, pairs, sizeof(uint64_t) * maxAmountOfPairs);
		for (int j = 0; j < maxAmountOfPairs; j++)
		{
			uint64_t pairAmount = previousPairs[j];
			if (pairAmount == 0)
				continue;
			pairs[j] -= pairAmount;

			const auto& outcome = outcomes[j];
			pairs[outcome.IndexA] += pairAmount;
			pairs[outcome.IndexB] += pairAmount;
			elementCount[outcome.NewElementIndex] += pairAmount;
		}
	}

	uint64_t min = UINT64_MAX;
	uint64_t max = 0;
	for (uint64_t element : elementCount)
	{
		if (element == 0)
			continue;
		if (element < min)
			min = element;
		if (element > max)
			max = element;
	}

	return max - min;
}

uint64_t Day14::RunA()
{
	return Run(Input1, Input2, 10);
}

uint64_t Day14::RunB()
{
	return Run(Input1, Input2, 40);
}