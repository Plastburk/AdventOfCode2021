#include "Day12.h"
#include "Utilities/Utilities.h"

inline int CreateNode(std::vector<PathNode>& Input, std::unordered_map<uint16_t, uint8_t>& nameToId, uint8_t& nextId, char* nameBuffer, int nameLength)
{
	if (nameLength == 5) // Start
		return 0;
	else if (nameLength == 3) // End
		return 1;
	else
	{
		uint16_t name = (nameBuffer[0] << 8) + nameBuffer[1];
		auto result = nameToId.try_emplace(name, nextId);
		uint8_t id = (*result.first).second;

		PathNodeType type = nameBuffer[0] < 'a' ? PathNodeType::BigRoom : PathNodeType::SmallRoom;

		if (result.second)
		{
#if _DEBUG
			Input.push_back({ std::string(nameBuffer, nameLength), id, type, 0 });
#else
			input.push_back({ id, type, 0 });
#endif
			nextId++;
		}
		return id;
	}
}

void Day12::ReadInput(std::ifstream& stream)
{
	Input.reserve(100);

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
	Input.push_back({ "start", 0, PathNodeType::Start, 0 });
	Input.push_back({ "end", 1, PathNodeType::End, 0 });
	std::string left;
#else
	input.push_back({ 0, PathNodeType::Start, 0 });
	input.push_back({ 1, PathNodeType::End, 0 });
#endif

	while (bytes > 0)
	{
		if ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z'))
		{
			nameBuffer[nameLength++] = *c;
		}
		else if (*c == '-')
		{
			leftNodeId = CreateNode(Input, nameToId, nextId, nameBuffer, nameLength);
			nameLength = 0;
			nameBuffer[0] = 0;
			nameBuffer[1] = 0;
		}
		else if (*c == '\n')
		{
			rightNodeId = CreateNode(Input, nameToId, nextId, nameBuffer, nameLength);
			nameLength = 0;
			nameBuffer[0] = 0;
			nameBuffer[1] = 0;

			Input[leftNodeId].Neighbors.push_back(&Input[rightNodeId]);
			Input[rightNodeId].Neighbors.push_back(&Input[leftNodeId]);
		}

		c++;
		bytes--;
	}

	if (nameLength > 0)
	{
		rightNodeId = CreateNode(Input, nameToId, nextId, nameBuffer, nameLength);
		Input[leftNodeId].Neighbors.push_back(&Input[rightNodeId]);
		Input[rightNodeId].Neighbors.push_back(&Input[leftNodeId]);
	}
}

typedef std::unordered_map<uint64_t, int> CacheMap;

inline uint64_t CreateCacheKey(uint8_t currentNode, uint32_t visited, bool smallVisitedMultipleTimes)
{
	return ((uint64_t)smallVisitedMultipleTimes << (8 + 32)) | ((uint64_t)visited << 8) | currentNode;
}

inline void AddToVisited(uint32_t& visited, uint8_t index)
{
	visited |= (0x1 << index);
}

inline int RecursivelyTraverseA(PathNode* pathNode)
{
	if (pathNode->Type == PathNodeType::End)
		return 1;

	pathNode->Visited++;

	int total = 0;
	for (auto neighbor : pathNode->Neighbors)
	{
		if (neighbor->Type != PathNodeType::BigRoom && neighbor->Visited >= 1)
			continue;

		total += RecursivelyTraverseA(neighbor);
	}

	pathNode->Visited--;
	return total;
}

int Day12::RunA()
{
	// No cache on A, as the overhead of the cache seems to cost more than we gain.
	return RecursivelyTraverseA(&Input[0]);
}

inline int RecursivelyTraverseB(PathNode* pathNode, bool smallVisitedMultipleTimes, uint32_t visited, CacheMap& cache)
{
	if (pathNode->Type == PathNodeType::End)
		return 1;

	uint64_t cacheKey = CreateCacheKey(pathNode->Id, visited, smallVisitedMultipleTimes);
	auto it = cache.find(cacheKey);
	if (it != cache.end())
	{
		return (*it).second;
	}

	AddToVisited(visited, pathNode->Id);
	pathNode->Visited++;

	int total = 0;
	for (auto neighbor : pathNode->Neighbors)
	{
		if (neighbor->Type != PathNodeType::BigRoom && neighbor->Visited >= 1)
		{
			if (smallVisitedMultipleTimes || neighbor->Type == PathNodeType::Start)
				continue;

			total += RecursivelyTraverseB(neighbor, true, visited, cache);
		}
		else
			total += RecursivelyTraverseB(neighbor, smallVisitedMultipleTimes, visited, cache);
	}

	pathNode->Visited--;
	cache.insert({ cacheKey, total });
	return total;
}

int Day12::RunB()
{
	CacheMap cache;
	return RecursivelyTraverseB(&Input[0], false, 0, cache);
}