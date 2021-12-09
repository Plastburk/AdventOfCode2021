#include "Day8.h"
#include "Utilities/Utilities.h"

#include <array>
#include <unordered_map>
#include <unordered_set>

void Day8::ReadInput(std::ifstream& stream)
{
	//input.reserve(2000);
	ReadPartialsFromStream<ReadCharArrayT(7)>(stream,
		input, ReadCharArray(7), { ' ', '\n' }
	);
}

int Day8::RunA()
{
	size_t sequences = input.size() / 14;

	int count = 0;
	for (size_t i = 0; i < sequences; i++)
	{
		for (size_t j = 10; j < 14; j++)
		{
			const auto& value = input[i * 14 + j];
			if (value.size == 2 || value.size == 3 || value.size == 4 || value.size == 7)
				count++;
		}
	}

	return count;
}

int Day8::RunB()
{
	int total = 0;

	size_t sequences = input.size() / 14;
	for (size_t i = 0; i < sequences; i++)
	{
		std::unordered_map<int, const CharString<7>*> known;

		// Find 1, 4, 7, 8
		for (size_t j = 0; j < 10; j++)
		{
			const auto& value = input[i * 14 + j];

			if (value.size == 2)
				known[1] = &value;
			else if (value.size == 4)
				known[4] = &value;
			else if (value.size == 3)
				known[7] = &value;
			else if (value.size == 7)
				known[8] = &value;

			if (known.size() == 4)
				break;
		}

		// Find 0, 6, 9
		for (size_t j = 0; j < 10; j++)
		{
			const auto& value = input[i * 14 + j];

			if (value.size != 6)
				continue;

			// TODO Can probably optimize the Contains checks
			const auto& known1 = known[1]->data;
			const auto& known4 = known[4]->data;
			if (value.Contains(known4[0]) && value.Contains(known4[1]) && value.Contains(known4[2]) && value.Contains(known4[3]))
				known[9] = &value;
			else if (value.Contains(known1[0]) && value.Contains(known1[1]))
				known[0] = &value;
			else
				known[6] = &value;
		}

		// Find 2, 3, 5
		for (size_t j = 0; j < 10; j++)
		{
			const auto& value = input[i * 14 + j];

			if (value.size != 5)
				continue;

			const auto& known1 = known[1]->data;
			if (value.Contains(known1[0]) && value.Contains(known1[1]))
				known[3] = &value;
			else if (known[6]->Contains(value.data[0]) && known[6]->Contains(value.data[1]) && known[6]->Contains(value.data[2]) && known[6]->Contains(value.data[3]) && known[6]->Contains(value.data[4]))
				known[5] = &value;
			else
				known[2] = &value;
		}

		// Find actual number
		int number = 0;
		for (size_t j = 10; j < 14; j++)
		{
			const auto& value = input[i * 14 + j];

			std::unordered_set<char> vSet;
			for (int x = 0; x < value.size; x++)
				vSet.insert(value.data[x]);

			// TODO Optimize
			for (int n = 0; n < 10; n++)
			{
				std::unordered_set<char> nSet;
				for (int x = 0; x < known[n]->size; x++)
					nSet.insert(known[n]->data[x]);

				if (vSet == nSet)
				{
					number *= 10;
					number += n;
					break;
				}
			}
		}

		total += number;
	}

	return total;
}