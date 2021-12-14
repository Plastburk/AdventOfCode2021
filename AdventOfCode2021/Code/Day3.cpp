#include "Day3.h"
#include "Utilities/Utilities.h"

void Day3::ReadInput(std::ifstream& stream)
{
	Input.reserve(2000);
	ReadBitsFromStream(stream, Input, Bits);
}

int Day3::RunA()
{
	std::vector<int> count(Bits, 0);
	for (int value : Input)
	{
		for (int i = 0; i < Bits; i++)
			count[i] += (value & (0x1 << i)) > 0;
	}

	size_t halfSize = Input.size() / 2;
	int gamma = 0;
	for (int i = 0; i < Bits; i++)
		gamma |= ((count[i] > halfSize ? 1 : 0) << i);

	uint32_t epsilon = ~(gamma | (0xFFFFFFFF << Bits));

	return gamma * epsilon;
}

inline int RunLoop(const std::vector<uint32_t>& Input, int bits, bool invertKeepOnes)
{
	std::vector<uint32_t> list(Input);

	for (int i = bits - 1; i >= 0; i--)
	{
		int count = 0;
		for (int value : list)
			count += (value & (0x1 << i)) > 0;

		bool keepOnes = ((count * 10) >= (((int)list.size() * 10) / 2)) ^ invertKeepOnes;

		for (auto it = list.begin(); it != list.end();)
		{
			if (((*it >> i) & 0x1) != keepOnes)
				it = list.erase(it);
			else
				it++;
		}

		if (list.size() == 1)
			return list[0];
	}

	return 0;
}

int Day3::RunB()
{
	int oxygen = RunLoop(Input, Bits, false);
	int co2 = RunLoop(Input, Bits, true);

	return oxygen * co2;
}