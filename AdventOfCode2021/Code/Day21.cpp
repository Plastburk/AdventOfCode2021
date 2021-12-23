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

struct PlayState
{
	uint8_t Player1Pos;
	uint8_t Player2Pos;
	uint8_t Player1Score;
	uint8_t Player2Score;

	inline bool operator==(const PlayState& other) const
	{
		return Player1Pos == other.Player1Pos && Player2Pos == other.Player2Pos && Player1Score == other.Player1Score && Player2Score == other.Player2Score;
	}
};

template<>
struct std::hash<PlayState>
{
	size_t operator()(const PlayState& value) const
	{
		return ((size_t)value.Player2Score) << 48 | ((size_t)value.Player1Score) << 32 | ((size_t)value.Player2Pos) << 16 | (size_t)value.Player1Pos;
	}
};

uint64_t Day21::RunB()
{
	robin_hood::unordered_map<PlayState, uint64_t> universesPerState[2];
	robin_hood::unordered_map<PlayState, uint64_t>* currentUniversesPerState = &universesPerState[0];
	robin_hood::unordered_map<PlayState, uint64_t>* nextUniversesPerState = &universesPerState[1];

	PlayState state{ (uint8_t)(Player1Start - 1), (uint8_t)(Player2Start - 1), 0, 0 };
	(*universesPerState)[state] = 1;

	int dieOutcomes[] = { 1, 3, 6, 7, 6, 3, 1 };

	uint64_t player1Wins = 0;
	uint64_t player2Wins = 0;
	do
	{
		for (const auto& pair : *currentUniversesPerState)
		{
			const auto& state = pair.first;
			uint64_t universes = pair.second;

			// TODO Could I optimize this with an array/map containing all outcomes for both dice?
			for (auto die1 = 3; die1 < 10; die1++)
				for (auto die2 = 3; die2 < 10; die2++)
				{
					PlayState newState{};
					newState.Player1Pos = (state.Player1Pos + die1) % 10;
					newState.Player2Pos = (state.Player2Pos + die2) % 10;

					newState.Player1Score = state.Player1Score + (newState.Player1Pos + 1);
					newState.Player2Score = state.Player2Score + (newState.Player2Pos + 1);

					uint64_t newUniverses = dieOutcomes[die1 - 3] * dieOutcomes[die2 - 3] * universes;
					if (newState.Player1Score >= 21)
					{
						player1Wins += dieOutcomes[die1 - 3] * universes;
						break;
					}
					else if (newState.Player2Score >= 21)
						player2Wins += newUniverses;
					else
						(* nextUniversesPerState)[newState] += newUniverses;
				}
		}

		std::swap(currentUniversesPerState, nextUniversesPerState);
		nextUniversesPerState->clear();
	} while ((*currentUniversesPerState).size() > 0);

	return std::max(player1Wins, player2Wins);
}