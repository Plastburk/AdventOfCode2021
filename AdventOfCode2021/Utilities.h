#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <fstream>
#include <tuple>

template <class In, class Out, class Func>
inline void Transform(const In in, Out out, Func func)
{
	std::transform(in.cbegin(), in.cend(), std::back_inserter(out), func);
}

inline int ToInt(const std::string& str)
{
	return atoi(str.c_str());
}

inline int ToInt(const char* str)
{
	return atoi(str);
}

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
			if (*c == '\n' || *c == ' ')
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