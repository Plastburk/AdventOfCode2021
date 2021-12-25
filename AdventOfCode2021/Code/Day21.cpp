#include "Day21.h"
#include "Utilities/Utilities.h"

#include "Utilities/RobinHood.h"

void Day21::ReadInput(std::ifstream& stream)
{
	std::vector<int> input;
	ReadFuncFromStream<ReadIntsT>(stream,
		input, ReadInts, { 0, ' ', '\n' }
	);

	if (input[0] == 1)
	{
		Player1Start = input[1];
		Player2Start = input[3];
	}
	else
	{
		Player1Start = input[3];
		Player2Start = input[1];
	}
}

inline int RollDie(int& die)
{
	return ((++die) % 100) + 1;
}

int Day21::RunA()
{
	int die = -1;
	int playerPos[2] = { Player1Start - 1, Player2Start - 1 };
	int playerPoints[2] = { 0, 0 };
	int rolls = 0;

	for (int i = 0;; i++)
	{
		for (int p = 0; p < 2; p++)
		{
			int roll = RollDie(die) + RollDie(die) + RollDie(die);
			rolls += 3;

			playerPos[p] = (playerPos[p] + roll) % 10;
			playerPoints[p] += playerPos[p] + 1;

			if (playerPoints[p] >= 1000)
			{
				return playerPoints[(p + 1) % 2] * rolls;
			}
		}
	}
}

static const int DieOutcomes[] = { 1, 3, 6, 7, 6, 3, 1 };

struct PlayState
{
	uint8_t CurrentPlayerPos;
	uint8_t OtherPlayerPos;
	uint8_t CurrentPlayerScore;
	uint8_t OtherPlayerScore;
};

typedef robin_hood::unordered_map<uint32_t, std::tuple<uint64_t, uint64_t>> CacheMap;

inline uint32_t CreateCacheKey(const PlayState& value)
{
	return ((uint32_t)value.OtherPlayerScore) << 24 | ((uint32_t)value.CurrentPlayerScore) << 16 | ((uint32_t)value.OtherPlayerPos) << 8 | (uint32_t)value.CurrentPlayerPos;
}

inline void RecursivelyRunTurn(const PlayState& state, uint64_t& thisPlayerWins, uint64_t& otherPlayerWins, CacheMap& cachedOutcomes)
{
	uint32_t cacheKey = CreateCacheKey(state);
	auto cache = cachedOutcomes.find(cacheKey);
	if (cache != cachedOutcomes.end())
	{
		auto& [thisWins, otherWins] = cache->second;
		thisPlayerWins = thisWins;
		otherPlayerWins = otherWins;
		return;
	}

	thisPlayerWins = 0;
	otherPlayerWins = 0;
	for (auto die = 3; die < 10; die++)
	{
		PlayState newState{};
		newState.CurrentPlayerPos = (state.CurrentPlayerPos + die) % 10;
		newState.OtherPlayerPos = state.OtherPlayerPos;

		newState.CurrentPlayerScore = state.CurrentPlayerScore + (newState.CurrentPlayerPos + 1);
		newState.OtherPlayerScore = state.OtherPlayerScore;

		if (newState.CurrentPlayerScore >= 21)
			thisPlayerWins += DieOutcomes[die - 3];
		else
		{
			// Flip which player is the current player between turns
			std::swap(newState.CurrentPlayerPos, newState.OtherPlayerPos);
			std::swap(newState.CurrentPlayerScore, newState.OtherPlayerScore);

			uint64_t newThisPlayerWins;
			uint64_t newOtherPlayerWins;
			RecursivelyRunTurn(newState, newOtherPlayerWins, newThisPlayerWins, cachedOutcomes);

			thisPlayerWins += newThisPlayerWins * DieOutcomes[die - 3];
			otherPlayerWins += newOtherPlayerWins * DieOutcomes[die - 3];
		}
	}

	cachedOutcomes[cacheKey] = { thisPlayerWins, otherPlayerWins };
}

uint64_t Day21::RunB()
{
	CacheMap cachedOutcomes;
	cachedOutcomes.reserve(20000);

	PlayState state{ (uint8_t)(Player1Start - 1), (uint8_t)(Player2Start - 1), 0, 0 };
	uint64_t player1Wins = 0;
	uint64_t player2Wins = 0;
	RecursivelyRunTurn(state, player1Wins, player2Wins, cachedOutcomes);

	return std::max(player1Wins, player2Wins);
}