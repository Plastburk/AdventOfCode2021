#include "Day10.h"
#include "Utilities/Utilities.h"

#include <algorithm>

void Day10::ReadInput(std::ifstream& stream)
{
	//input.reserve(12000);
	/*ReadPartialsFromStream<Read2DMapT>(stream,
		input, Read2DMap, {}
	);*/
	ReadStringsFromStream(stream, input);
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
	int total = 0;
	for (int i = 0; i < input.size(); i++)
	{
		std::vector<char> stack;
		const auto& str = input[i];
		for (int j = 0; j < str.length(); j++)
		{
			char c = str[j];
			if (c == '{' || c == '(' || c == '[' || c == '<')
				stack.push_back(str[j]);
			else
			{
				char cc = RightToLeft(c);
				if (stack.back() != RightToLeft(c))
				{
					total += ScoreForA(c);
					break;
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
	for (int i = 0; i < input.size(); i++)
	{
		std::vector<char> stack;
		const auto& str = input[i];
		bool corrupted = false;
		for (int j = 0; j < str.length(); j++)
		{
			char c = str[j];
			if (c == '{' || c == '(' || c == '[' || c == '<')
				stack.push_back(str[j]);
			else
			{
				char cc = RightToLeft(c);
				if (stack.back() != RightToLeft(c))
				{
					corrupted = true;
					break;
				}
				else
					stack.pop_back();
			}
		}

		if (corrupted)
			continue;

		uint64_t score = 0;
		while (!stack.empty())
		{
			score = score * 5 + ScoreForB(stack.back());
			stack.pop_back();
		}

		scores.push_back(score);
	}

	std::sort(scores.begin(), scores.end());

	return scores[scores.size() / 2];
}