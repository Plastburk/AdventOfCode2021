#include "Day6.h"
#include "Utilities/Utilities.h"

#include <array>
#include <numeric>

void Day6::ReadInput(std::ifstream& stream)
{
	input.reserve(600);
	ReadPartialsFromStream<ReadUInt8sT>(stream,
		input, ReadUInt8s, { 0, ',' }
	);
}

inline uint64_t Run(const std::vector<uint8_t>& input, int days)
{
	std::array<uint64_t, 9> fishPerDueDate{};

	for (int i = 0; i < input.size(); i++)
		fishPerDueDate[input[i]]++;

	int currentDueIndex = 0;
	for (int i = 0; i < days; i++)
	{
		uint64_t fishDue = fishPerDueDate[currentDueIndex];
		fishPerDueDate[currentDueIndex] = 0;
		fishPerDueDate[(currentDueIndex + 9) % 9] += fishDue;
		fishPerDueDate[(currentDueIndex + 7) % 9] += fishDue;

		currentDueIndex = (currentDueIndex + 1) % 9;
	}

	return std::accumulate(fishPerDueDate.begin(), fishPerDueDate.end(), uint64_t(0));
}

uint64_t Day6::RunA()
{
	return Run(input, 80);
}

uint64_t Day6::RunB()
{
	return Run(input, 256);
}