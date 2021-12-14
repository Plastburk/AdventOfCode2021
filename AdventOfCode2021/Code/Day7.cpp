#include "Day7.h"
#include "Utilities/Utilities.h"

#include <algorithm>

void Day7::ReadInput(std::ifstream& stream)
{
	input.reserve(2000);
	ReadFuncFromStream<ReadUInt16sT>(stream,
		input, ReadUInt16s, { 0, ',' }
	);
}

inline int CalculateCostForPosA(const std::vector<uint16_t>& input, size_t pos)
{
	int cost = 0;
	for (size_t crab = 0; crab < input.size(); crab++)
		cost += std::abs(input[crab] - (uint16_t)pos);

	return cost;
}

inline int CalculateCostForPosB(const std::vector<uint16_t>& input, size_t pos)
{
	int cost = 0;
	for (size_t crab = 0; crab < input.size(); crab++)
	{
		int distance = std::abs(input[crab] - (uint16_t)pos);
		cost += ((distance * distance) + distance) / 2;
	}

	return cost;
}

int Day7::RunA()
{
	uint16_t max = *std::max_element(input.begin(), input.end());

	size_t leftEdgePos = 0;
	size_t rightEdgePos = max;
	size_t middlePos = rightEdgePos / 2;

	int leftCost = CalculateCostForPosA(input, leftEdgePos);
	int rightCost = CalculateCostForPosA(input, rightEdgePos);
	int middleCost = CalculateCostForPosA(input, middlePos);

	while (true)
	{
		if (leftCost < rightCost)
		{
			rightCost = middleCost;
			rightEdgePos = middlePos;
		}
		else
		{
			leftCost = middleCost;
			leftEdgePos = middlePos;
		}

		if (std::abs((long long)leftEdgePos - (long long)rightEdgePos) == 1)
		{
			leftCost = CalculateCostForPosA(input, leftEdgePos);
			rightCost = CalculateCostForPosA(input, rightEdgePos);

			return std::min(leftCost, rightCost);
		}

		middlePos = (leftEdgePos + rightEdgePos) / 2;
		middleCost = CalculateCostForPosA(input, middlePos);
	}

	return 0;
}

int Day7::RunB()
{
	uint16_t max = *std::max_element(input.begin(), input.end());

	size_t leftEdgePos = 0;
	size_t rightEdgePos = max;
	size_t middlePos = rightEdgePos / 2;

	int leftCost = CalculateCostForPosB(input, leftEdgePos);
	int rightCost = CalculateCostForPosB(input, rightEdgePos);
	int middleCost = CalculateCostForPosB(input, middlePos);

	while (true)
	{
		if (leftCost < rightCost)
		{
			rightCost = middleCost;
			rightEdgePos = middlePos;
		}
		else
		{
			leftCost = middleCost;
			leftEdgePos = middlePos;
		}

		if (std::abs((long long)leftEdgePos - (long long)rightEdgePos) == 1)
		{
			leftCost = CalculateCostForPosB(input, leftEdgePos);
			rightCost = CalculateCostForPosB(input, rightEdgePos);

			return std::min(leftCost, rightCost);
		}

		middlePos = (leftEdgePos + rightEdgePos) / 2;
		middleCost = CalculateCostForPosB(input, middlePos);
	}

	return 0;
}