#include "Day12.h"
#include "Utilities/Utilities.h"

inline int CreateNode(std::vector<PathNode>& input, std::unordered_map<uint16_t, uint8_t>& nameToId, uint8_t& nextId, char* nameBuffer, int nameLength)
{
	if (nameLength == 5) // Start
		return 0;
	else if (nameLength == 3) // End
		return 1;
	else
	{
		uint16_t name = (nameBuffer[0] << 8) + nameBuffer[1];
		auto result = nameToId.try_emplace(name, nextId);
		int id = (*result.first).second;

		bool isBigRoom = nameBuffer[0] < 'a';

		if (result.second)
		{
			nextId++;
#if _DEBUG
			input.push_back({ std::string(nameBuffer, nameLength), isBigRoom, false, false, 0 });
#else
			input.push_back({ isBigRoom, false, false, 0 });
#endif
		}
		return id;
	}
}

void Day12::ReadInput(std::ifstream& stream)
{
	input.reserve(100);

	char buffer[1024];
	stream.read(buffer, sizeof(buffer));
	auto bytes = stream.gcount();
	char* c = buffer;

	char nameBuffer[15]{};
	int nameLength = 0;

	std::unordered_map<uint16_t, uint8_t> nameToId;
	uint8_t nextId = 2;
	uint8_t leftNodeId = 0;
	uint8_t rightNodeId = 0;
#if _DEBUG
	input.push_back({ "start", false, true, false, 0 });
	input.push_back({ "end", false, false, true, 0 });
	std::string left;
#else
	input.push_back({ false, true, false, 0 });
	input.push_back({ false, false, true, 0 });
#endif

	while (bytes > 0)
	{
		if ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z'))
		{
			nameBuffer[nameLength++] = *c;
		}
		else if (*c == '-')
		{
			leftNodeId = CreateNode(input, nameToId, nextId, nameBuffer, nameLength);
			nameLength = 0;
			nameBuffer[0] = 0;
			nameBuffer[1] = 0;
		}
		else if (*c == '\n')
		{
			rightNodeId = CreateNode(input, nameToId, nextId, nameBuffer, nameLength);
			nameLength = 0;
			nameBuffer[0] = 0;
			nameBuffer[1] = 0;

			input[leftNodeId].Neighbors.push_back(&input[rightNodeId]);
			input[rightNodeId].Neighbors.push_back(&input[leftNodeId]);
		}

		c++;
		bytes--;
	}

	if (nameLength > 0)
	{
		rightNodeId = CreateNode(input, nameToId, nextId, nameBuffer, nameLength);
		input[leftNodeId].Neighbors.push_back(&input[rightNodeId]);
		input[rightNodeId].Neighbors.push_back(&input[leftNodeId]);
	}
}

inline int RecursivelyTraverseA(PathNode* pathNode)
{
	if (pathNode->IsEnd)
		return 1;

	int total = 0;
	pathNode->Visited++;
	for (auto neighbor : pathNode->Neighbors)
	{
		if (!neighbor->IsBigRoom && neighbor->Visited >= 1)
			continue;

		total += RecursivelyTraverseA(neighbor);
	}
	pathNode->Visited--;
	return total;
}

int Day12::RunA()
{
	return 0;
	return RecursivelyTraverseA(&input[0]);
}

inline int RecursivelyTraverseB(PathNode* pathNode, bool smallVisitedMultipleTimes)
{
	if (pathNode->IsEnd)
		return 1;

	int total = 0;
	pathNode->Visited++;
	for (auto neighbor : pathNode->Neighbors)
	{
		if (!neighbor->IsBigRoom && neighbor->Visited >= 1)
		{
			if (smallVisitedMultipleTimes || neighbor->IsStart)
				continue;

			total += RecursivelyTraverseB(neighbor, true);
		}
		else
			total += RecursivelyTraverseB(neighbor, smallVisitedMultipleTimes);
	}
	pathNode->Visited--;
	return total;
}

int Day12::RunB()
{
	//return 0;
	return RecursivelyTraverseB(&input[0], false);
}