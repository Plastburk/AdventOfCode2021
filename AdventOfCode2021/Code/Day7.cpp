#include "Day7.h"
#include "Utilities/Utilities.h"

void Day7::ReadInput(std::ifstream& stream)
{
	//input.reserve(600);
	ReadPartialsFromStream<ReadUInt16sT>(stream,
		input, ReadUInt16s, { 0, ',' }
	);
}

int Day7::RunA()
{
	uint16_t max = *std::max_element(input.begin(), input.end());

	int lowestCost = INT_MAX;
	size_t lowestCostPos = -1;
	for (size_t i = 0; i < max; i++)
	{
		int cost = 0;
		for (size_t crab = 0; crab < input.size(); crab++)
		{
			cost += std::abs(input[crab] - (uint16_t)i);
		}

		if (cost < lowestCost)
		{
			lowestCost = cost;
			lowestCostPos = i;
		}
	}

	return lowestCost;
}

int Day7::RunB()
{
	uint16_t max = *std::max_element(input.begin(), input.end());

	int lowestCost = INT_MAX;
	size_t lowestCostPos = -1;
	for (size_t i = 0; i < max; i++)
	{
		int cost = 0;
		for (size_t crab = 0; crab < input.size(); crab++)
		{
			int distance = std::abs(input[crab] - (uint16_t)i);
			cost += ((distance * distance) + distance) / 2;
		}

		if (cost < lowestCost)
		{
			lowestCost = cost;
			lowestCostPos = i;
		}
	}

	return lowestCost;
}