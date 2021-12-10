#include "Day10.h"
#include "Utilities/Utilities.h"

#include <algorithm>

void Day10::ReadInput(std::ifstream& stream)
{
	stream.read(input, sizeof(input));
	inputSize = (int)stream.gcount();
}

inline char RightToLeft(char c)
{
	if (c == ')')
		return '(';
	return c - 2;
}

inline int ScoreForA(char c)
{
	switch (c)
	{
	case ')':
		return 3;
	case ']':
		return 57;
	case '}':
		return 1197;
	case '>':
		return 25137;
	}
	return 0;
}

int Day10::RunA()
{
	std::vector<char> stack;

	int total = 0;
	bool corrupted = false;
	for (int i = 0; i < inputSize; i++)
	{
		char c = input[i];
		if (c == '\n')
		{
			stack.clear();
			corrupted = false;
		}
		else if (!corrupted)
		{
			if (c == '{' || c == '(' || c == '[' || c == '<')
				stack.push_back(c);
			else
			{
				if (stack.back() != RightToLeft(c))
				{
					total += ScoreForA(c);
					corrupted = true;
				}
				else
					stack.pop_back();
			}
		}
	}

	return total;
}

inline int ScoreForB(char c)
{
	switch (c)
	{
	case '(':
		return 1;
	case '[':
		return 2;
	case '{':
		return 3;
	case '<':
		return 4;
	}
	return 0;
}

uint64_t Day10::RunB()
{
	std::vector<uint64_t> scores;
	std::vector<char> stack;

	bool corrupted = false;
	for (int i = 0; i < inputSize; i++)
	{
		char c = input[i];
		if (c == '\n')
		{
			if (!corrupted)
			{
				uint64_t score = 0;
				while (!stack.empty())
				{
					score = score * 5 + ScoreForB(stack.back());
					stack.pop_back();
				}

				scores.push_back(score);
			}

			stack.clear();
			corrupted = false;
		}
		else if (!corrupted)
		{
			if (c == '{' || c == '(' || c == '[' || c == '<')
				stack.push_back(c);
			else
			{
				if (stack.back() != RightToLeft(c))
					corrupted = true;
				else
					stack.pop_back();
			}
		}
	}

	std::sort(scores.begin(), scores.end());
	return scores[scores.size() / 2];
}