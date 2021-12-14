#include "Day4.h"
#include "Utilities/Utilities.h"

#include <unordered_set>
#include <algorithm>

void Day4::ReadInput(std::ifstream& stream)
{
	Input1.reserve(200);
	Input2.reserve(4000);

	ReadFuncFromStream<ReadIntsT,ReadIntsT>(stream,
		Input1, ReadInts, { '\n',  ',' },
		Input2, ReadInts, { 0, ' ', '\n' }
	);

	// Remap boards so each number is the turn that number will be called instead
	std::unordered_map<int, int> valueToTurn;
	for (int i = 0; i < Input1.size(); i++)
		valueToTurn[Input1[i]] = i;

	for (int i = 0; i < Input2.size(); i++)
		Input2[i] = valueToTurn[Input2[i]];
}

inline int FindWinningTurn(std::vector<int>::iterator startOfBoard)
{
	// Find first horizontal win
	int winningTurn = INT_MAX;
	for (size_t row = 0; row < 5; row++)
	{
		int max = *std::max_element(startOfBoard + (row * 5), startOfBoard + (row * 5) + 5);
		if (max < winningTurn)
			winningTurn = max;
	}

	// Find first vertical win
	for (int column = 0; column < 5; column++)
	{
		bool win = true;
		int max = -1;
		for (size_t row = 0; row < 5; row++)
		{
			int value = *(startOfBoard + (row * 5) + column);
			if (value > max)
				max = value;
		}

		if (max < winningTurn)
			winningTurn = max;
	}

	return winningTurn;
}

inline int SumUnmarkedNumbers(int* startOfBoard, const std::vector<int>& drawnNumbers, int winningTurn)
{
	int sum = 0;

	for (int i = 0; i < 5 * 5; i++)
	{
		int turn = *(startOfBoard + i);
		if (turn > winningTurn)
			sum += drawnNumbers[turn];
	}

	return sum;
}

int Day4::RunA()
{
	const size_t numbersPerBoard = 5 * 5;
	const size_t boards = Input2.size() / numbersPerBoard;

	size_t winningBoard = -1;
	int winningTurn = INT_MAX;
	for (size_t board = 0; board < boards; board++)
	{
		int turn = FindWinningTurn(Input2.begin() + numbersPerBoard * board);
		if (turn < winningTurn)
		{
			winningTurn = turn;
			winningBoard = board;
		}
	}

	int sum = SumUnmarkedNumbers(&Input2[numbersPerBoard * winningBoard], Input1, winningTurn);
	return sum * Input1[winningTurn];
}

int Day4::RunB()
{
	const size_t numbersPerBoard = 5 * 5;
	const size_t boards = Input2.size() / numbersPerBoard;

	size_t losingBoard = -1;
	int losingTurn = -1;
	for (size_t board = 0; board < boards; board++)
	{
		int turn = FindWinningTurn(Input2.begin() + numbersPerBoard * board);
		if (turn > losingTurn)
		{
			losingTurn = turn;
			losingBoard = board;
		}
	}

	int sum = SumUnmarkedNumbers(&Input2[numbersPerBoard * losingBoard], Input1, losingTurn);
	return sum * Input1[losingTurn];
}