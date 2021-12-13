#pragma once

#include <vector>
#include <fstream>
#include <string>

enum class PathNodeType : uint8_t
{
	Start,
	End,
	BigRoom,
	SmallRoom
};

struct PathNode
{
#if _DEBUG
	std::string Name;
#endif
	uint8_t Id;
	PathNodeType Type;
	uint8_t Visited;
	std::vector<PathNode*> Neighbors;
};

class Day12
{
public:
	void ReadInput(std::ifstream& stream);

	int RunA();
	int RunB();

private:
	std::vector<PathNode> input;
};
