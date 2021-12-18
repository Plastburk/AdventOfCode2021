#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <bitset>

inline void ReadStringsFromStream(std::ifstream& stream, std::vector<std::string>& list)
{
	std::string line;
	while (std::getline(stream, line))
	{
		list.push_back(line);
	}
}

inline void ReadTypeAndIntFromStream(std::ifstream& stream, std::vector<std::tuple<char, int>>& list)
{
	char buffer[8192];
	bool hasType = false;
	bool hasNumber = false;
	char type = 0;
	int number = 0;

	while (true)
	{
		stream.read(buffer, sizeof(buffer));
		char* c = buffer;

		std::streamsize bytes = stream.gcount();
		if (bytes == 0)
			break;

		while (bytes > 0)
		{
			if (*c == '\n')
			{
				if (hasType && hasNumber)
					list.emplace_back(type, number);

				number = 0;
				hasType = false;
				hasNumber = false;
			}
			else if (*c == ' ')
			{
				// Do nothing
			}
			else if (*c >= '0' && *c <= '9')
			{
				hasNumber = true;
				number *= 10;
				number += *c - '0';
			}
			else if (*c >= 'a' && *c <= 'z')
			{
				if (!hasType)
				{
					type = *c;
					hasType = true;
				}
			}
			else
				exit(1); // Unexpected input, just exit

			c++;
			bytes--;
		}
	}

	if (hasType && hasNumber)
		list.emplace_back(type, number);
}

inline void ReadIntsFromStream(std::ifstream& stream, std::vector<int>& list)
{
	char buffer[8192];
	bool hasNumber = false;
	int number = 0;

	while (true)
	{
		stream.read(buffer, sizeof(buffer));
		char* c = buffer;

		std::streamsize bytes = stream.gcount();
		if (bytes == 0)
			break;

		while (bytes > 0)
		{
			if (*c == '\n')
			{
				if (hasNumber)
					list.push_back(number);

				number = 0;
				hasNumber = false;
			}
			else if (*c >= '0' && *c <= '9')
			{
				hasNumber = true;
				number *= 10;
				number += *c - '0';
			}
			else
				exit(1); // Unexpected input, just exit

			c++;
			bytes--;
		}
	}

	if (hasNumber)
		list.push_back(number);
}

inline void ReadBitsFromStream(std::ifstream& stream, std::vector<uint32_t>& list, int& bits)
{
	char buffer[8192];
	bool hasNumber = false;
	int number = 0;
	bits = 0;

	while (true)
	{
		stream.read(buffer, sizeof(buffer));
		char* c = buffer;

		std::streamsize bytes = stream.gcount();
		if (bytes == 0)
			break;

		while (bytes > 0)
		{
			if (*c == '\n')
			{
				if (hasNumber)
					list.push_back(number);

				number = 0;
				hasNumber = false;
			}
			else if (*c == '0' || *c == '1')
			{
				hasNumber = true;
				number <<= 1;
				number |= (*c - '0');
				bits++;
			}
			else
				exit(1); // Unexpected input, just exit

			c++;
			bytes--;
		}
	}

	if (hasNumber)
		list.push_back(number);

	bits /= (int)list.size();
}

template <size_t Bits>
inline void ReadHexAsBitsFromStream(std::ifstream& stream, std::bitset<Bits>& list)
{
	char buffer[8192];

	int i = 0;
	while (true)
	{
		stream.read(buffer, sizeof(buffer));
		char* c = buffer;

		std::streamsize bytes = stream.gcount();
		if (bytes == 0)
			break;

		while (bytes > 0)
		{
			if (*c >= '0' && *c <= '9')
			{
				uint8_t value = *c - '0';
				list.set(i, value >> 3 & 0x1);
				list.set(i+1, value >> 2 & 0x1);
				list.set(i+2, value >> 1 & 0x1);
				list.set(i+3, value & 0x1);
				i += 4;
			}
			else if (*c >= 'A' && *c <= 'F')
			{
				uint8_t value = *c - 'A' + 10;
				list.set(i, value >> 3 & 0x1);
				list.set(i + 1, value >> 2 & 0x1);
				list.set(i + 2, value >> 1 & 0x1);
				list.set(i + 3, value & 0x1);
				i += 4;
			}

			c++;
			bytes--;
		}
	}
}
