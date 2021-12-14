#include "Day7.h"
#include "Utilities/Utilities.h"

#include <algorithm>

void Day7::ReadInput(std::ifstream& stream)
{
	Input.reserve(2000);
	ReadFuncFromStream<ReadUInt16sT>(stream,
		Input, ReadUInt16s, { 0, ',' }
	);
}

inline int CalculateCostForPosA(const std::vector<uint16_t>& Input, size_t pos)
{
	int cost = 0;
	for (size_t crab = 0; crab < Input.size(); crab++)
		cost += std::abs(Input[crab] - (uint16_t)pos);

	return cost;
}

inline int CalculateCostForPosB(const std::vector<uint16_t>& Input, size_t pos)
{
	int cost = 0;
	for (size_t crab = 0; crab < Input.size(); crab++)
	{
		int distance = std::abs(Input[crab] - (uint16_t)pos);
		cost += ((distance * distance) + distance) / 2;
	}

	return cost;
}

int Day7::RunA()
{
	uint16_t max = *std::max_element(Input.begin(), Input.end());

	size_t leftEdgePos = 0;
	size_t rightEdgePos = max;
	size_t middlePos = rightEdgePos / 2;

	int leftCost = CalculateCostForPosA(Input, leftEdgePos);
	int rightCost = CalculateCostForPosA(Input, rightEdgePos);
	int middleCost = CalculateCostForPosA(Input, middlePos);

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
			leftCost = CalculateCostForPosA(Input, leftEdgePos);
			rightCost = CalculateCostForPosA(Input, rightEdgePos);

			return std::min(leftCost, rightCost);
		}

		middlePos = (leftEdgePos + rightEdgePos) / 2;
		middleCost = CalculateCostForPosA(Input, middlePos);
	}

	return 0;
}

int Day7::RunB()
{
	uint16_t max = *std::max_element(Input.begin(), Input.end());

	size_t leftEdgePos = 0;
	size_t rightEdgePos = max;
	size_t middlePos = rightEdgePos / 2;

	int leftCost = CalculateCostForPosB(Input, leftEdgePos);
	int rightCost = CalculateCostForPosB(Input, rightEdgePos);
	int middleCost = CalculateCostForPosB(Input, middlePos);

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
			leftCost = CalculateCostForPosB(Input, leftEdgePos);
			rightCost = CalculateCostForPosB(Input, rightEdgePos);

			return std::min(leftCost, rightCost);
		}

		middlePos = (leftEdgePos + rightEdgePos) / 2;
		middleCost = CalculateCostForPosB(Input, middlePos);
	}

	return 0;
}