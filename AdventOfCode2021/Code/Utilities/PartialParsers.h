#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <functional>

// Boilerplate

template <class ListT, class DataT>
using PartialFunc = std::function<bool(char*&, std::streamsize&, std::vector<ListT>&, DataT&)>;

template <class ListT, class DataT>
using PartialFuncEnd = std::function<void(std::vector<ListT>&, DataT&)>;

template<class ListT, class DataT>
struct PartialFuncContainer
{
	PartialFunc<ListT, DataT> main;
	PartialFuncEnd<ListT, DataT> end;
};

template<class ListT1, class ListT2, class DataT1, class DataT2>
inline void ReadPartialsFromStream(std::ifstream& stream, std::vector<ListT1>& list1, std::vector<ListT2>& list2, PartialFuncContainer<ListT1, DataT1> partial1, PartialFuncContainer<ListT2, DataT2> partial2)
{
	char buffer[8192];

	DataT1 data1{};
	DataT2 data2{};

	bool running1 = true;

	while (true)
	{
		stream.read(buffer, sizeof(buffer));
		char* c = buffer;

		std::streamsize bytes = stream.gcount();
		if (bytes == 0)
			break;

		if (running1)
		{
			if (partial1.main(c, bytes, list1, data1))
			{
				partial1.end(list1, data1);
				running1 = false;
			}
		}

		if (!running1)
		{
			partial2.main(c, bytes, list2, data2);
		}
	}

	if (running1)
		partial1.end(list1, data1);
	else
		partial2.end(list2, data2);
}

// ReadIntsUntilEndline

struct ReadIntsUntilEndline_Data
{
	bool hasNumber;
	int number;
};

inline bool ReadIntsUntilEndline_Main(char*& c, std::streamsize& bytes, std::vector<int>& list, ReadIntsUntilEndline_Data& data)
{
	while (bytes > 0)
	{
		if (*c >= '0' && *c <= '9')
		{
			data.hasNumber = true;
			data.number *= 10;
			data.number += *c - '0';
		}
		else if (*c == ',')
		{
			if (data.hasNumber)
				list.push_back(data.number);

			data.number = 0;
			data.hasNumber = false;
		}
		else if (*c == '\n')
		{
			return true;
		}
		else
			exit(1); // Unexpected input, just exit

		c++;
		bytes--;
	}

	return false;
}

inline void ReadIntsUntilEndline_End(std::vector<int>& list, ReadIntsUntilEndline_Data& data)
{
	if (data.hasNumber)
		list.push_back(data.number);
}

#define ReadIntsUntilEndline { ReadIntsUntilEndline_Main, ReadIntsUntilEndline_End }

// ReadInts

struct ReadInts_Data
{
	bool hasNumber;
	int number;
};

inline bool ReadInts_Main(char*& c, std::streamsize& bytes, std::vector<int>& list, ReadInts_Data& data)
{
	while (bytes > 0)
	{
		if (*c >= '0' && *c <= '9')
		{
			data.hasNumber = true;
			data.number *= 10;
			data.number += *c - '0';
		}
		else if (*c == ' ' || *c == '\n')
		{
			if (data.hasNumber)
				list.push_back(data.number);

			data.number = 0;
			data.hasNumber = false;
		}
		else
			exit(1); // Unexpected input, just exit

		c++;
		bytes--;
	}

	return false;
}

inline void ReadInts_End(std::vector<int>& list, ReadInts_Data& data)
{
	if (data.hasNumber)
		list.push_back(data.number);
}

#define ReadInts { ReadInts_Main, ReadInts_End }
