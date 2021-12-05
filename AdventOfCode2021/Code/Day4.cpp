#include "Day4.h"
#include "Utilities/Utilities.h"

#include <numeric>
#include <unordered_set>

void Day4::ReadInput(std::ifstream& stream)
{
	input1.reserve(200);
	input2.reserve(4000);
	ReadPartialsFromStream<int, int, ReadIntsUntilEndline_Data, ReadInts_Data>(stream, input1, input2, ReadIntsUntilEndline, ReadInts);
}

inline bool IsWinner(int* startOfBoard, const std::unordered_set<int>& drawnNumbers)
{
	// Check horizontal wins
	for (int row = 0; row < 5; row++)
	{
		bool win = true;
		for (int column = 0; column < 5; column++)
		{
			int value = *(startOfBoard + (row * 5) + column);
			if (drawnNumbers.find(value) == drawnNumbers.end())
			{
				win = false;
				break; // If any is not a hit, we didn't win
			}
		}

		if (win)
			return true;
	}

	// Check vertical wins
	for (int column = 0; column < 5; column++)
	{
		bool win = true;
		for (int row = 0; row < 5; row++)
		{
			int value = *(startOfBoard + (row * 5) + column);
			if (drawnNumbers.find(value) == drawnNumbers.end())
			{
				win = false;
				break; // If any is not a hit, we didn't win
			}
		}

		if (win)
			return true;
	}

	return false;
}

inline int SumUnmarkedNumbers(int* startOfBoard, const std::unordered_set<int>& drawnNumbers)
{
	int sum = 0;

	for (int i = 0; i < 5 * 5; i++)
	{
		int value = *(startOfBoard + i);
		if (drawnNumbers.find(value) == drawnNumbers.end())
			sum += value;
	}

	return sum;
}

int Day4::RunA()
{
	const int numbersPerBoard = 5 * 5;
	const size_t boards = input2.size() / numbersPerBoard;

	std::unordered_set<int> drawnNumbers(input1.cbegin(), input1.cbegin() + 5);

	for (size_t drawn = 5; drawn < input1.size(); drawn++)
	{
		drawnNumbers.insert(input1[drawn]);

		for (size_t board = 0; board < boards; board++)
		{
			if (IsWinner(&input2[numbersPerBoard * board], drawnNumbers))
			{
				int sum = SumUnmarkedNumbers(&input2[numbersPerBoard * board], drawnNumbers);
				return sum * input1[drawn];
			}
		}
	}

	return 0;
}
int Day4::RunB()
{
	const int numbersPerBoard = 5 * 5;
	const size_t boards = input2.size() / numbersPerBoard;

	std::vector<int> remainingBoards(boards);
	std::iota(remainingBoards.begin(), remainingBoards.end(), 0);

	std::unordered_set<int> drawnNumbers(input1.cbegin(), input1.cbegin() + 5);

	for (size_t drawn = 5; drawn < input1.size(); drawn++)
	{
		drawnNumbers.insert(input1[drawn]);

		for (auto it = remainingBoards.begin(); it != remainingBoards.end();)
		{
			size_t board = *it;

			if (IsWinner(&input2[numbersPerBoard * board], drawnNumbers))
			{
				it = remainingBoards.erase(it);

				if (remainingBoards.size() == 0)
				{
					int sum = SumUnmarkedNumbers(&input2[numbersPerBoard * board], drawnNumbers);
					return sum * input1[drawn];
				}
			}
			else
				it++;
		}
	}

	return 0;
}