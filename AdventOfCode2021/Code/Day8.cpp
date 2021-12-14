#include "Day8.h"
#include "Utilities/Utilities.h"

#include <unordered_map>
#include <atomic>
#include <ppl.h>

void Day8::ReadInput(std::ifstream& stream)
{
	input.reserve(3000);
	ReadFuncFromStream<ReadCharsAs8BitsT>(stream,
		input, ReadCharsAs8Bits, { ' ', '\n' }
	);
}

inline int AmountOfChars(uint8_t charSet)
{
	char amount = 0;
	while (charSet != 0)
	{
		amount += charSet & 0x1;
		charSet >>= 1;
	}

	return amount;
}

inline bool BIsSubsetOfA(uint8_t a, uint8_t b)
{
	return (b | a) == a;
}

int Day8::RunA()
{
	size_t sequences = input.size() / 14;

	int count = 0;
	for (size_t i = 0; i < sequences; i++)
	{
		for (size_t j = 10; j < 14; j++)
		{
			char amount = AmountOfChars(input[i * 14 + j]);

			if (amount == 2 || amount == 3 || amount == 4 || amount == 7)
				count++;
		}
	}

	return count;
}

int Day8::RunB()
{
	std::atomic<int> total = 0;

	size_t sequences = input.size() / 14;
	concurrency::parallel_for(size_t(0), sequences, [&](size_t i)
		{
			std::unordered_map<int, uint8_t> known;

			// Find 1, 4, 7, 8
			for (size_t j = 0; j < 10; j++)
			{
				const auto& value = input[i * 14 + j];
				int amount = AmountOfChars(value);

				if (amount == 2)
					known[1] = value;
				else if (amount == 4)
					known[4] = value;
				else if (amount == 3)
					known[7] = value;
				else if (amount == 7)
					known[8] = value;

				if (known.size() == 4)
					break;
			}

			// Find 0, 6, 9
			for (size_t j = 0; j < 10; j++)
			{
				const auto& value = input[i * 14 + j];
				int amount = AmountOfChars(value);

				if (amount != 6)
					continue;

				if (BIsSubsetOfA(value, known[4]))
					known[9] = value;
				else if (BIsSubsetOfA(value, known[1]))
					known[0] = value;
				else
					known[6] = value;
			}

			// Find 2, 3, 5
			for (size_t j = 0; j < 10; j++)
			{
				const auto& value = input[i * 14 + j];
				int amount = AmountOfChars(value);

				if (amount != 5)
					continue;

				if (BIsSubsetOfA(value, known[1]))
					known[3] = value;
				else if (BIsSubsetOfA(known[6], value))
					known[5] = value;
				else
					known[2] = value;
			}

			// Find actual number
			int number = 0;
			for (size_t j = 10; j < 14; j++)
			{
				const auto& value = input[i * 14 + j];

				for (int n = 0; n < 10; n++)
					if (value == known[n])
					{
						number *= 10;
						number += n;
						break;
					}
			}

			total.fetch_add(number);
		});

	return total;
}