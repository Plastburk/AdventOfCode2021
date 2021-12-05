#include "Day4.h"
#include "Utilities/Utilities.h"

#include <unordered_set>

void Day4::ReadInput(std::ifstream& stream)
{
	input1.reserve(200);
	input2.reserve(4000);
	ReadPartialsFromStream<int, int, ReadIntsUntilEndline_Data, ReadInts_Data>(stream, input1, input2, ReadIntsUntilEndline, ReadInts);

	// Remap boards so each number is the turn that number will be called instead
	std::unordered_map<int, int> valueToTurn;
	for (int i = 0; i < input1.size(); i++)
		valueToTurn[input1[i]] = i;

	for (int i = 0; i < input2.size(); i++)
		input2[i] = valueToTurn[input2[i]];
}

inline int FindWinningTurn(std::vector<int>::iterator startOfBoard)
{
	// Find first horizontal win
	int winningTurn = INT_MAX;
	for (int row = 0; row < 5; row++)
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
		for (int row = 0; row < 5; row++)
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
	const int numbersPerBoard = 5 * 5;
	const size_t boards = input2.size() / numbersPerBoard;

	int winningBoard = -1;
	int winningTurn = INT_MAX;
	for (size_t board = 0; board < boards; board++)
	{
		int turn = FindWinningTurn(input2.begin() + numbersPerBoard * board);
		if (turn < winningTurn)
		{
			winningTurn = turn;
			winningBoard = board;
		}
	}

	int sum = SumUnmarkedNumbers(&input2[numbersPerBoard * winningBoard], input1, winningTurn);
	return sum * input1[winningTurn];
}

int Day4::RunB()
{
	const int numbersPerBoard = 5 * 5;
	const size_t boards = input2.size() / numbersPerBoard;

	int losingBoard = -1;
	int losingTurn = -1;
	for (size_t board = 0; board < boards; board++)
	{
		int turn = FindWinningTurn(input2.begin() + numbersPerBoard * board);
		if (turn > losingTurn)
		{
			losingTurn = turn;
			losingBoard = board;
		}
	}

	int sum = SumUnmarkedNumbers(&input2[numbersPerBoard * losingBoard], input1, losingTurn);
	return sum * input1[losingTurn];
}