#include "Day15.h"
#include "Utilities/Utilities.h"

#include <queue>

void Day15::ReadInput(std::ifstream& stream)
{
	stream.read(Input, sizeof(Input));
	InputSize = (int)stream.gcount();

	for (int i = 0; i < InputSize; i++)
		if (Input[i] == '\n')
		{
			Width = i;
			break;
		}

#pragma warning(push)
#pragma warning(disable:6385)
	if (Input[InputSize - 1] == '\n')
		Height = InputSize / (Width + 1);
	else
		Height = InputSize / (Width);
#pragma warning(pop)
}

class PathNode
{
public:
	PathNode(uint16_t x, uint16_t y, uint16_t cost) : X(x), Y(y), Cost(cost) {}

	uint16_t X;
	uint16_t Y;
	uint16_t Cost;
};

template<>
struct std::greater<PathNode>
{
	bool operator()(const PathNode& a, const PathNode& b) const
	{
		return a.Cost > b.Cost;
	}
};

typedef std::priority_queue<PathNode, std::vector<PathNode>, std::greater<PathNode>> MinQueue;

inline void CheckAndAddNode(uint16_t width, uint16_t x, uint16_t y, std::vector<bool>& visited, MinQueue& openList, const std::vector<uint8_t>& costs, const PathNode& parent)
{
	int index = y * width + x;

	if (!visited[index])
	{
		visited[index] = true;
		openList.emplace(x, y, parent.Cost + costs[index]);
	}
}

inline int Run(const Vec2D& mapSize, const std::vector<uint8_t>& mapCosts, const Vec2D& start, const Vec2D& end)
{
	// Setup
	std::vector<bool> visited;
	visited.resize(mapSize.X * mapSize.Y, false);

	std::vector<PathNode> queueVector;
	queueVector.reserve(mapSize.X * mapSize.Y);
	MinQueue openList(std::greater<PathNode>(), queueVector);

	// Add start node to openlist
	visited[start.Y * mapSize.X + start.X] = true;
	openList.emplace((uint16_t)start.X, (uint16_t)start.Y, 0);

	while (true)
	{
		// Find node with lowest cost
		PathNode currentNode = openList.top();
		openList.pop();

		// If the node we just closed was the end node, we're done
		if (currentNode.X == end.X && currentNode.Y == end.Y)
			return currentNode.Cost;

		// Check adjacent nodes
		int cX = currentNode.X;
		int cY = currentNode.Y;
		if (cY > 0)
			CheckAndAddNode(mapSize.X, cX, cY - 1, visited, openList, mapCosts, currentNode);
		if (cX > 0)
			CheckAndAddNode(mapSize.X, cX - 1, cY, visited, openList, mapCosts, currentNode);
		if (cX < mapSize.X - 1)
			CheckAndAddNode(mapSize.X, cX + 1, cY, visited, openList, mapCosts, currentNode);
		if (cY < mapSize.Y - 1)
			CheckAndAddNode(mapSize.X, cX, cY + 1, visited, openList, mapCosts, currentNode);
	}
}

int Day15::RunA()
{
	std::vector<uint8_t> map(Width * Height);
	for (int y = 0; y < Height; y++)
		for (int x = 0; x < Width; x++)
			map[y * Width + x] = Input[y * (Width + 1) + x] - '0';

	return Run({ Width, Height }, map, { 0, 0 }, { Width - 1, Height - 1 });
}

int Day15::RunB()
{
	int newWidth = Width * 5;
	int newHeight = Height * 5;

	std::vector<uint8_t> map(newWidth * newHeight);
	for (int y = 0; y < newHeight; y++)
		for (int x = 0; x < newWidth; x++)
		{
			int yy = y % Height;
			int yRest = y / Height;
			int xx = x % Width;
			int xRest = x / Width;

			int value = ((Input[yy * (Width + 1) + xx] + xRest + yRest) - '0');
			if (value > 9)
				value -= 9;

			map[y * newWidth + x] = value;
		}

	return Run({ newWidth, newHeight }, map, { 0, 0 }, { newWidth - 1, newHeight - 1 });
}