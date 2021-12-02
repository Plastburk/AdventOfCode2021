#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <fstream>

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

inline void ReadIntsFromStream(std::ifstream& stream, std::vector<int>& list)
{
	char buffer[8192];
	bool hasNumber = false;
	int value = 0;

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
					list.push_back(value);

				value = 0;
				c++;
				bytes--;
				hasNumber = false;
			}
			else if (*c >= '0' && *c <= '9')
			{
				hasNumber = true;
				value *= 10;
				value += *c - '0';
				c++;
				bytes--;
			}
			else
				exit(1); // Unexpected input, just exit
		}
	}

	if (hasNumber)
		list.push_back(value);
}