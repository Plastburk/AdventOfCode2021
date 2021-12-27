#include "Day23.h"
#include "Utilities/Utilities.h"
#include "Utilities/RobinHood.h"

#include <queue>

void Day23::ReadInput(std::ifstream& stream)
{
	char buffer[8192];
	int i = 0;
	bool innerRoom = false;

	while (true)
	{
		stream.read(buffer, sizeof(buffer));
		char* c = buffer;

		std::streamsize bytes = stream.gcount();
		if (bytes == 0)
			break;

		while (bytes > 0)
		{
			if (*c == '.')
				Input[i++] = 0;
			else if (*c >= 'A' && *c <= 'D')
			{
				Input[i] = *c - 'A' + 1;
				if (i == 17)
					i = 12;
				else
					i += 2;
			}

			c++;
			bytes--;
		}
	}
}

template <int RoomSize>
using RoomsState = std::array<uint8_t, 11 + (4 * RoomSize)>;

template<>
struct std::hash<RoomsState<2>>
{
	size_t operator()(const RoomsState<2>& value) const
	{
		size_t hash = 0;
		for (int i = 0; i < 19; i++)
			hash = hash * 5 + value[i];
		return hash;
	}
};

template<>
struct std::hash<RoomsState<4>>
{
	size_t operator()(const RoomsState<4>& value) const
	{
		size_t hash = 0;
		for (int i = 0; i < 27; i++)
			hash = hash * 5 + value[i];
		return hash;
	}
};

// TODO Remove
inline char UnitToChar(uint8_t unit)
{
	if (unit == 0)
		return '.';
	else
		return 'A' + (unit - 1);
}

// TODO Remove
template <int RoomSize>
inline void PrintState(const RoomsState<RoomSize>& state)
{
	printf("\n");
	printf("#############\n");
	printf("#");
	for (int i = 0; i < 11; i++)
		printf("%c", UnitToChar(state[i]));
	printf("#\n");
	if (RoomSize == 2)
	{
		printf("###%c#%c#%c#%c###\n", UnitToChar(state[11]), UnitToChar(state[13]), UnitToChar(state[15]), UnitToChar(state[17]));
		printf("  #%c#%c#%c#%c#  \n", UnitToChar(state[12]), UnitToChar(state[14]), UnitToChar(state[16]), UnitToChar(state[18]));
	}
	else
	{

#pragma warning(push)
#pragma warning(disable:28020)
		printf("###%c#%c#%c#%c###\n", UnitToChar(state[11]), UnitToChar(state[15]), UnitToChar(state[19]), UnitToChar(state[23]));
		printf("  #%c#%c#%c#%c#  \n", UnitToChar(state[12]), UnitToChar(state[16]), UnitToChar(state[20]), UnitToChar(state[24]));
		printf("  #%c#%c#%c#%c#  \n", UnitToChar(state[13]), UnitToChar(state[17]), UnitToChar(state[21]), UnitToChar(state[25]));
		printf("  #%c#%c#%c#%c#  \n", UnitToChar(state[14]), UnitToChar(state[18]), UnitToChar(state[22]), UnitToChar(state[26]));
#pragma warning(pop)
	}
	printf("  #########  \n");
}

template <int RoomSize>
struct State
{
	int GCost;
	int FCost;
	RoomsState<RoomSize> Slots;
};

template<int RoomSize>
struct std::greater<State<RoomSize>>
{
	bool operator()(const State<RoomSize>& a, const State<RoomSize>& b) const
	{
		return a.FCost > b.FCost;
	}
};

template <int RoomSize>
using MinQueue = std::priority_queue<State<RoomSize>, std::vector<State<RoomSize>>, std::greater<State<RoomSize>>>;

template <int RoomSize>
using VisitedMap = robin_hood::unordered_map<RoomsState<RoomSize>, int>;

template <int RoomSize>
inline bool IsPathFree(const RoomsState<RoomSize>& state, int from, int to)
{
	int leftSlot = std::min(from, to);
	int rightSlot = std::max(from, to);

	if (leftSlot == from)
		leftSlot++;

	if (rightSlot == from)
		rightSlot--;

	for (int i = leftSlot; i <= rightSlot; i++)
		if (state[i] != 0)
			return false;

	return true;
}

template <int RoomSize>
inline void AddAllValidMoves(const State<RoomSize>& state, VisitedMap<RoomSize>& visited, MinQueue<RoomSize>& openList)
{
	constexpr std::array<uint8_t, 7> ValidHallwayPositions = { 0, 1, 3, 5, 7, 9, 10 };
	constexpr std::array<int, 4> CostPerUnitType = { 1, 10, 100, 1000 };

	for (int pos : ValidHallwayPositions)
	{
		if (state.Slots[pos] == 0)
		{
			// Generate all valid moves from an initial room slot to this hallway slot
			for (int room = 0; room < 4; room++)
			{
				int firstRoomSlot = 11 + (room * RoomSize);
				int lastRoomSlot = firstRoomSlot + (RoomSize - 1);

				// Check if any units in this room needs to move
				bool hasAnyWrongUnits = false;
				int firstUnitSlot = -1;
				for (int i = firstRoomSlot; i <= lastRoomSlot; i++)
				{
					if (state.Slots[i] != 0 && state.Slots[i] != room + 1)
						hasAnyWrongUnits = true;
					if (firstUnitSlot == -1 && state.Slots[i] != 0)
						firstUnitSlot = i;
				}

				if (!hasAnyWrongUnits)
					continue;

				// Check if there is any units in the way from this room to the target slot
				int hallwayOutsideRoomSlot = 2 + (room * 2);
				int targetHallwaySlot = pos;
				
				if (!IsPathFree<RoomSize>(state.Slots, hallwayOutsideRoomSlot, targetHallwaySlot))
					continue;

				// Generate new state
				auto newState = state.Slots;
#pragma warning(push)
#pragma warning(disable:28020)
				uint8_t unitType = state.Slots[firstUnitSlot];
#pragma warning(pop)
				newState[targetHallwaySlot] = unitType;
				newState[firstUnitSlot] = 0;

				// Add to openList & visited, but only if current cost in visited is higher than new cost, or if visited doesn't contain this node
				int distance = std::abs(targetHallwaySlot - hallwayOutsideRoomSlot) + (firstUnitSlot - firstRoomSlot + 1);
				int newCost = state.GCost + distance * CostPerUnitType[unitType - 1];
				auto visit = visited.find(newState);
				if (visit == visited.end() || visit->second > newCost)
				{
					visited[newState] = newCost;
					int heuristic = 0; // TODO Add Heuristic
					openList.push({ newCost, newCost + heuristic, newState });

					//PrintState(newState);
				}
			}
		}
		else
		{
			// Generate all valid moves from this hallway slot to final room slot
			uint8_t unitType = state.Slots[pos];
			int room = unitType - 1;
			int firstRoomSlot = 11 + (room * RoomSize);
			int lastRoomSlot = firstRoomSlot + (RoomSize - 1);

			// Check if final room is either empty, or only contains correct units
			bool hasWrongUnit = false;
			int firstEmptyRoomSlot = -1;
			for (int i = firstRoomSlot; i <= lastRoomSlot; i++)
			{
				if (firstEmptyRoomSlot == -1 && state.Slots[i] == 0)
					firstEmptyRoomSlot = i;
				if (state.Slots[i] != 0 && state.Slots[i] != unitType)
				{
					hasWrongUnit = true;
					break;
				}
			}

			if (hasWrongUnit)
				continue;

			// Check if path to room is free
			int hallwayOutsideRoomSlot = 2 + (room * 2);
			if (!IsPathFree<RoomSize>(state.Slots, pos, hallwayOutsideRoomSlot))
				continue;

			// Generate new state
			auto newState = state.Slots;
#pragma warning(push)
#pragma warning(disable:28020)
			newState[firstEmptyRoomSlot] = unitType;
#pragma warning(pop)
			newState[pos] = 0;

			// Add to openList & visited, but only if current cost in visited is higher than new cost, or if visited doesn't contain this node
			int distance = std::abs(hallwayOutsideRoomSlot - pos) + (firstEmptyRoomSlot - firstRoomSlot + 1);
			int newCost = state.GCost + distance * CostPerUnitType[unitType - 1];
			auto visit = visited.find(newState);
			if (visit == visited.end() || visit->second > newCost)
			{
				visited[newState] = newCost;
				int heuristic = 0; // TODO Add Heuristic
				openList.push({ newCost, newCost + heuristic, newState });

				//PrintState(newState);
			}
		}
	}
}

template<int RoomSize>
inline int Run(const RoomsState<RoomSize>& initialState, const RoomsState<RoomSize>& endState)
{
	// Setup
	VisitedMap<RoomSize> visited;
	visited.reserve(1000); // TODO What is a good value here?

	std::vector<State<RoomSize>> queueVector;
	queueVector.reserve(1000); // TODO What is a good value here?
	MinQueue<RoomSize> openList(std::greater<State<RoomSize>>(), queueVector);

	// Add start node to openlist
	visited[initialState] = 0;
	openList.push({ 0, 0, initialState }); // TODO Add Heuristic

	//PrintState<RoomSize>(initialState);

	bool foundPath = false;
	while (true)
	{
		// Find state with lowest cost
		auto currentState = openList.top();
		openList.pop();

		// TODO
		//PrintState(currentState.Slots);

		// If we've already visited this node with a better cost, skip it
		if (visited[currentState.Slots] < currentState.GCost)
			continue;

		// If the node we just closed was the end node, we're done
		if (currentState.Slots == endState)
			return currentState.GCost;

		// Add all valid moves
		AddAllValidMoves<RoomSize>(currentState, visited, openList);

		// TODO
		//printf("\n\n----------\n\n");
	}
}

int Day23::RunA()
{
	RoomsState<2> endState = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4 };
	return Run<2>(Input, endState);
}

int Day23::RunB()
{
	RoomsState<4> input{};
	for (int i = 0; i < 11; i++)
		input[i] = Input[i];

	input[11] = Input[11];
	input[12] = 4;
	input[13] = 4;
	input[14] = Input[12];

	input[15] = Input[13];
	input[16] = 3;
	input[17] = 2;
	input[18] = Input[14];

	input[19] = Input[15];
	input[20] = 2;
	input[21] = 1;
	input[22] = Input[16];

	input[23] = Input[17];
	input[24] = 1;
	input[25] = 3;
	input[26] = Input[18];


	RoomsState<4> endState = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4 };

	return Run<4>(input, endState);
}