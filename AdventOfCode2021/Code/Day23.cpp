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

template <int RoomSize>
inline uint64_t StateToHash(const RoomsState<RoomSize>& state)
{
	int length = 11 + (RoomSize * 4);

	uint64_t hash = 0;
	for (int i = 0; i < length; i++)
		hash = hash * 5 + state[i];
	return hash;
}

template <int RoomSize>
inline RoomsState<RoomSize> HashToState(uint64_t hash)
{
	int length = 11 + (RoomSize * 4);

	RoomsState<RoomSize> state;
	for (int i = length - 1; i >= 0; i--)
	{
		state[i] = hash % 5;
		hash /= 5;
	}
	return state;
}

template <int RoomSize>
struct State
{
	int GCost;
	int FCost;
	uint64_t Slots;
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
using VisitedMap = robin_hood::unordered_map<uint64_t, int>;

constexpr std::array<uint8_t, 7> ValidHallwayPositions = { 0, 1, 3, 5, 7, 9, 10 };
constexpr std::array<int, 4> CostPerUnitType = { 1, 10, 100, 1000 };

template <int RoomSize>
inline bool IsPathFree(const RoomsState<RoomSize>& state, int from, int to)
{
	int leftSlot = std::min(from, to);
	int rightSlot = std::max(from, to);

	for (int i = leftSlot; i <= rightSlot; i++)
		if (i != from && state[i] != 0)
			return false;

	return true;
}

template <int RoomSize>
inline int CalculateHeuristic(const RoomsState<RoomSize>& state)
{
	int heuristic = 0;

	// Calculate cost for all units that still need to leave their rooms
	// (This calculates cost to leave their room and go to the hallway slot right above their target room)
	for (int room = 0; room < 4; room++)
	{
		int firstRoomSlot = 11 + (room * RoomSize);
		int lastRoomSlot = firstRoomSlot + (RoomSize - 1);

		// Check if any units in this room needs to move
		bool hasAnyWrongUnits = false;
		int firstUnitSlot = -1;
		int lastWrongUnitSlot = -1;
		for (int i = firstRoomSlot; i <= lastRoomSlot; i++)
		{
			if (state[i] != 0 && state[i] != room + 1)
				lastWrongUnitSlot = i;
			if (firstUnitSlot == -1 && state[i] != 0)
				firstUnitSlot = i;
		}

		if (lastWrongUnitSlot != -1)
		{
			for (int i = firstUnitSlot; i <= lastWrongUnitSlot; i++)
			{
#pragma warning(push)
#pragma warning(disable:28020)
				uint8_t unitType = state[i];
#pragma warning(pop)

				// Distance to leave room and go into hallway
				int distance = (firstUnitSlot - firstRoomSlot + 1);

				// Distance to go to correct hallway slot
				int targetRoom = unitType - 1;

				int hallwayOutsideRoomSlot = 2 + (room * 2);
				int hallwayOutsideTargetRoomSlot = 2 + (targetRoom * 2);

				distance += std::abs(hallwayOutsideTargetRoomSlot - hallwayOutsideRoomSlot);

				// Add cost
				heuristic += CostPerUnitType[unitType - 1] * distance;
			}

			// Calculate cost to enter the correct room for all units that are not in the correct room
			{
				int unitsThatNeedToMoveIn = lastWrongUnitSlot - firstRoomSlot + 1;
				heuristic = CostPerUnitType[room] * (((unitsThatNeedToMoveIn * unitsThatNeedToMoveIn) + unitsThatNeedToMoveIn) / 2);
			}
		}
	}

	// Calculate cost for all units that are in the hallway
	// (This calculates cost to go to the hallway slot right above their target room)
	for (int i = 0; i < 11; i++)
	{
#pragma warning(push)
#pragma warning(disable:28020)
		uint8_t unitType = state[i];
#pragma warning(pop)

		if (unitType == 0)
			continue;

		// Distance to go to correct hallway slot
		int targetRoom = unitType - 1;
		int hallwayOutsideTargetRoomSlot = 2 + (targetRoom * 2);

		int distance = std::abs(hallwayOutsideTargetRoomSlot - i);

		// Add cost
		heuristic += CostPerUnitType[unitType - 1] * distance;
	}

	return heuristic;
}

template <int RoomSize>
inline void AddAllValidMoves(const State<RoomSize>& state, VisitedMap<RoomSize>& visited, MinQueue<RoomSize>& openList)
{
	const auto& slots = HashToState<RoomSize>(state.Slots);
	for (int pos : ValidHallwayPositions)
	{
		if (slots[pos] == 0)
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
					if (slots[i] != 0 && slots[i] != room + 1)
						hasAnyWrongUnits = true;
					if (firstUnitSlot == -1 && slots[i] != 0)
						firstUnitSlot = i;
				}

				if (!hasAnyWrongUnits)
					continue;

				// Check if there is any units in the way from this room to the target slot
				int hallwayOutsideRoomSlot = 2 + (room * 2);
				int targetHallwaySlot = pos;

				if (!IsPathFree<RoomSize>(slots, hallwayOutsideRoomSlot, targetHallwaySlot))
					continue;

				// Generate new state
				auto newState = slots;
#pragma warning(push)
#pragma warning(disable:28020)
				uint8_t unitType = slots[firstUnitSlot];
#pragma warning(pop)
				newState[targetHallwaySlot] = unitType;
				newState[firstUnitSlot] = 0;
				uint64_t newStateHash = StateToHash<RoomSize>(newState);

				// Add to openList & visited, but only if current cost in visited is higher than new cost, or if visited doesn't contain this node
				int distance = std::abs(targetHallwaySlot - hallwayOutsideRoomSlot) + (firstUnitSlot - firstRoomSlot + 1);
				int newCost = state.GCost + distance * CostPerUnitType[unitType - 1];
				auto visit = visited.find(newStateHash);
				if (visit == visited.end() || visit->second > newCost)
				{
					visited[newStateHash] = newCost;
					int heuristic = CalculateHeuristic<RoomSize>(newState);
					openList.push({ newCost, newCost + heuristic, newStateHash });
				}
			}
		}
		else
		{
			// Generate all valid moves from this hallway slot to final room slot
			uint8_t unitType = slots[pos];
			int room = unitType - 1;
			int firstRoomSlot = 11 + (room * RoomSize);
			int lastRoomSlot = firstRoomSlot + (RoomSize - 1);

			// Check if final room is either empty, or only contains correct units
			bool hasWrongUnit = false;
			int firstEmptyRoomSlot = -1;
			for (int i = firstRoomSlot; i <= lastRoomSlot; i++)
			{
				if (firstEmptyRoomSlot == -1 && slots[i] == 0)
					firstEmptyRoomSlot = i;
				if (slots[i] != 0 && slots[i] != unitType)
				{
					hasWrongUnit = true;
					break;
				}
			}

			if (hasWrongUnit)
				continue;

			// Check if path to room is free
			int hallwayOutsideRoomSlot = 2 + (room * 2);
			if (!IsPathFree<RoomSize>(slots, pos, hallwayOutsideRoomSlot))
				continue;

			// Generate new state
			auto newState = slots;
#pragma warning(push)
#pragma warning(disable:28020)
			newState[firstEmptyRoomSlot] = unitType;
#pragma warning(pop)
			newState[pos] = 0;
			uint64_t newStateHash = StateToHash<RoomSize>(newState);

			// Add to openList & visited, but only if current cost in visited is higher than new cost, or if visited doesn't contain this node
			int distance = std::abs(hallwayOutsideRoomSlot - pos) + (firstEmptyRoomSlot - firstRoomSlot + 1);
			int newCost = state.GCost + distance * CostPerUnitType[unitType - 1];
			auto visit = visited.find(newStateHash);
			if (visit == visited.end() || visit->second > newCost)
			{
				visited[newStateHash] = newCost;
				int heuristic = CalculateHeuristic<RoomSize>(newState);
				openList.push({ newCost, newCost + heuristic, newStateHash });
			}
		}
	}
}

template<int RoomSize>
inline int Run(const RoomsState<RoomSize>& initialState, const RoomsState<RoomSize>& endState)
{
	// Setup
	VisitedMap<RoomSize> visited;
	visited.reserve(100000);

	std::vector<State<RoomSize>> queueVector;
	queueVector.reserve(20000);
	MinQueue<RoomSize> openList(std::greater<State<RoomSize>>(), queueVector);

	// Add start node to openlist
	uint64_t initialStateHash = StateToHash<RoomSize>(initialState);
	visited[initialStateHash] = 0;
	openList.push({ 0, CalculateHeuristic<RoomSize>(initialState), initialStateHash });

	// Prepare end state
	uint64_t endStateHash = StateToHash<RoomSize>(endState);

	bool foundPath = false;
	while (true)
	{
		// Find state with lowest cost
		auto currentState = openList.top();
		openList.pop();

		// If we've already visited this node with a better cost, skip it
		if (visited[currentState.Slots] < currentState.GCost)
			continue;

		// If the node we just closed was the end node, we're done
		if (currentState.Slots == endStateHash)
			return currentState.GCost;

		// Add all valid moves
		AddAllValidMoves<RoomSize>(currentState, visited, openList);
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