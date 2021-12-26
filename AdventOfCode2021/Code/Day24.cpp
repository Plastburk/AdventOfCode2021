#include "Day24.h"
#include "Utilities/Utilities.h"

void Day24::ReadInput(std::ifstream& stream)
{
	Input.reserve(14);

	char buffer[8192];
	bool hasValue[3]{};
	int values[3]{};
	int i = 0;
	bool negative = false;
	int linesBeforeNext = 4;

	while (true)
	{
		stream.read(buffer, sizeof(buffer));
		char* c = buffer;

		std::streamsize bytes = stream.gcount();
		if (bytes == 0)
			break;

		while (bytes > 0)
		{
			if (linesBeforeNext > 0)
			{
				if (*c == '\n')
					linesBeforeNext--;
			}
			else
			{
				if (*c == '-')
					negative = true;
				else if (*c >= '0' && *c <= '9')
				{
					values[i] *= 10;
					values[i] += *c - '0';
					hasValue[i] = true;
				}
				else if (*c == '\n')
				{
					if (hasValue[i])
					{
						if (negative)
							values[i] *= -1;

						negative = false;
						if (i == 0)
						{
							linesBeforeNext = 0;
							i = 1;
						}
						else if (i == 1)
						{
							linesBeforeNext = 9;
							i = 2;
						}
						else
						{
							Input.emplace_back(values[0], values[1], values[2]);
							values[0] = 0;
							values[1] = 0;
							values[2] = 0;
							hasValue[0] = false;
							hasValue[1] = false;
							hasValue[2] = false;
							linesBeforeNext = 6;
							i = 0;
						}
					}
				}
			}

			c++;
			bytes--;
		}
	}
}

template<int Init>
uint64_t Run(std::vector<std::tuple<int, int, int>>& input)
{
	// We essentially run the validator here, and adjust the values as we need to keep it valid
	std::vector<int> result(14, Init);
	std::vector<std::tuple<int, int>> stack;
	for (int i = 0; i < 14; i++)
	{
		const auto& [a, b, c] = input[i];

		if (a == 26)
		{
			const auto& [lastIndex, lastValue] = stack.back();
			stack.pop_back();

			// Adjust the value so that it fits the validation
			result[i] = result[lastIndex] + lastValue + b;

			// If this ends up too small or too large (i.e. smaller than 1 or larger than 9), we need to adjust the value previously pushed to the stack
			if (result[i] > 9)
			{
				result[lastIndex] -= (result[i] - 9);
				result[i] = 9;
			}
			else if (result[i] < 1)
			{
				result[lastIndex] += (1 - result[i]);
				result[i] = 1;
			}
		}
		else
			stack.push_back({ i, c });
	}

	uint64_t value = 0;
	for (int i = 0; i < 14; i++)
	{
		value = value * 10 + result[i];
	}

	return value;
}

uint64_t Day24::RunA()
{
	return Run<9>(Input);
}

uint64_t Day24::RunB()
{
	return Run<1>(Input);
}