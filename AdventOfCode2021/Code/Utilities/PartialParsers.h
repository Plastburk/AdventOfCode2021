#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <functional>

// Boilerplate

template <class ListT, class DataT, class ParseParamT>
using PartialFunc = std::function<bool(char*&, std::streamsize&, std::vector<ListT>&, DataT&, const ParseParamT&)>;

template <class ListT, class DataT>
using PartialFuncEnd = std::function<void(std::vector<ListT>&, DataT&)>;

template<class ListT, class DataT, class ParseParamT>
struct PartialFuncContainer
{
	PartialFunc<ListT, DataT, ParseParamT> main;
	PartialFuncEnd<ListT, DataT> end;
};

template<class ListT1, class DataT1, class ParseParamT1, class ListT2, class DataT2, class ParseParamT2>
inline void ReadPartialsFromStream(std::ifstream& stream, std::vector<ListT1>& list1, PartialFuncContainer<ListT1, DataT1, ParseParamT1> partial1, const ParseParamT1& parseParam1, std::vector<ListT2>& list2, PartialFuncContainer<ListT2, DataT2, ParseParamT2> partial2, const ParseParamT2& parseParam2)
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
			if (partial1.main(c, bytes, list1, data1, parseParam1))
			{
				partial1.end(list1, data1);
				running1 = false;
			}
		}

		if (!running1)
		{
			if (partial2.main(c, bytes, list2, data2, parseParam2))
				break;
		}
	}

	if (running1)
		partial1.end(list1, data1);
	else
		partial2.end(list2, data2);
}


template<class ListT1, class DataT1, class ParseParamT1>
inline void ReadPartialsFromStream(std::ifstream& stream, std::vector<ListT1>& list1, PartialFuncContainer<ListT1, DataT1, ParseParamT1> partial1, const ParseParamT1& parseParam1)
{
	char buffer[8192];

	DataT1 data1{};

	while (true)
	{
		stream.read(buffer, sizeof(buffer));
		char* c = buffer;

		std::streamsize bytes = stream.gcount();
		if (bytes == 0)
			break;

		if (partial1.main(c, bytes, list1, data1, parseParam1))
			break;
	}

	partial1.end(list1, data1);
}

// ReadInts

template<class IntT>
struct ReadInts_Data
{
	bool hasNumber;
	IntT number;
};

struct ReadInts_Params
{
	char until = 0;
	char char1 = 0;
	char char2 = 0;
	char char3 = 0;
};

template<class IntT>
inline bool ReadInts_Main(char*& c, std::streamsize& bytes, std::vector<IntT>& list, ReadInts_Data<IntT>& data, const ReadInts_Params& parseParam)
{
	while (bytes > 0)
	{
		if (*c >= '0' && *c <= '9')
		{
			data.hasNumber = true;
			data.number *= 10;
			data.number += *c - '0';
		}
		else if (*c == parseParam.char1 || *c == parseParam.char2 || *c == parseParam.char3)
		{
			if (data.hasNumber)
				list.push_back(data.number);

			data.number = 0;
			data.hasNumber = false;
		}
		else if (*c == parseParam.until)
		{
			return true;
		}

		c++;
		bytes--;
	}

	return false;
}

template<class IntT>
inline void ReadInts_End(std::vector<IntT>& list, ReadInts_Data<IntT>& data)
{
	if (data.hasNumber)
		list.push_back(data.number);
}

#define ReadInts { ReadInts_Main<int>, ReadInts_End<int> }
#define ReadIntsT int, ReadInts_Data<int>, ReadInts_Params

#define ReadUInt16s { ReadInts_Main<uint16_t>, ReadInts_End<uint16_t> }
#define ReadUInt16sT uint16_t, ReadInts_Data<uint16_t>, ReadInts_Params

#define ReadUInt8s { ReadInts_Main<uint8_t>, ReadInts_End<uint8_t> }
#define ReadUInt8sT uint8_t, ReadInts_Data<uint8_t>, ReadInts_Params
