#include "Day6.h"
#include "Utilities/Utilities.h"

#include <array>
#include <numeric>

void Day6::ReadInput(std::ifstream& stream)
{
	Input.reserve(600);
	ReadFuncFromStream<ReadUInt8sT>(stream,
		Input, ReadUInt8s, { 0, ',' }
	);
}

inline uint64_t Run(const std::vector<uint8_t>& Input, int days)
{
	std::array<uint64_t, 9> fishPerDueDate{};

	for (int i = 0; i < Input.size(); i++)
		fishPerDueDate[Input[i]]++;

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
	return Run(Input, 80);
}

uint64_t Day6::RunB()
{
	return Run(Input, 256);
}