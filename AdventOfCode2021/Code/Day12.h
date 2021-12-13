#pragma once

#include <vector>
#include <fstream>
#include <string>

struct PathNode
{
#if _DEBUG
	std::string Name;
#endif
	bool IsBigRoom;
	bool IsStart;
	bool IsEnd;
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
