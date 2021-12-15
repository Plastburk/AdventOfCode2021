#include "Day14.h"
#include "Utilities/Utilities.h"

#include <functional>

struct PolymerPair
{
	char A;
	char B;

	PolymerPair() :
		A(0), B(0)
	{	}

	PolymerPair(char a, char b) :
		A(a), B(b)
	{	}

	bool operator==(const PolymerPair& other) const
	{
		return A == other.A && B == other.B;
	}
};

template<>
struct std::hash<PolymerPair>
{
	size_t operator()(const PolymerPair& value) const
	{
		return ((size_t)value.A) << 32 | (size_t)value.B;
	}
};

struct PolymerOutcome
{
	PolymerPair A;
	PolymerPair B;

	PolymerOutcome() :
		A(), B()
	{	}

	PolymerOutcome(const PolymerPair& a, const PolymerPair& b) :
		A(a), B(b)
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

uint64_t Run(const std::vector<char>& input1, const std::vector<char>& input2, int iterations)
{
	std::unordered_map<PolymerPair, uint64_t> pairs;
	std::unordered_map<PolymerPair, PolymerOutcome> outcomes;

	for (size_t i = 0; i < input2.size(); i += 3)
	{
		char c1 = input2[i];
		char c2 = input2[i + 1];
		char c3 = input2[i + 2];
		PolymerPair pair(c1, c2);
		outcomes.emplace(pair, PolymerOutcome(PolymerPair(c1, c3), PolymerPair(c3, c2)));
	}

	std::unordered_map<char, uint64_t> elementCount;
	for (size_t i = 0; i < input1.size() - 1; i++)
	{
		PolymerPair pair(input1[i], input1[i + 1]);
		pairs[pair]++;

		elementCount[input1[i]]++;
	}
	elementCount[input1[input1.size() - 1]]++;

	for (int i = 0; i < iterations; i++)
	{
		std::unordered_map<PolymerPair, uint64_t> prePairs(pairs);
		for (auto& pair : prePairs)
		{
			if (pair.second == 0)
				continue;
			uint64_t amount = prePairs[pair.first];
			pairs[pair.first] -= amount;

			const auto& outcome = outcomes[pair.first];
			pairs[outcome.A] += amount;
			pairs[outcome.B] += amount;
			elementCount[outcome.A.B] += amount;
		}
	}

	uint64_t min = UINT64_MAX;
	uint64_t max = 0;
	for (auto& element : elementCount)
	{
		uint64_t value = element.second;
		if (value < min)
			min = value;
		if (value > max)
			max = value;
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