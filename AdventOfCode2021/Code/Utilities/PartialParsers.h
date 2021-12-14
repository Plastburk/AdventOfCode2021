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

// ReadCharsAsBits

template<class IntT>
struct ReadCharsAsBits_Data
{
	bool hasNumber;
	IntT number;
};

struct ReadCharsAsBits_Params
{
	char char1 = 0;
	char char2 = 0;
	char char3 = 0;
};

template<class IntT>
inline bool ReadCharsAsBits_Main(char*& c, std::streamsize& bytes, std::vector<IntT>& list, ReadCharsAsBits_Data<IntT>& data, const ReadCharsAsBits_Params& parseParam)
{
	while (bytes > 0)
	{
		if (*c >= 'a' && *c <= 'z')
		{
			data.hasNumber = true;
			data.number |= 1 << (*c - 'a');
		}
		else if (*c == parseParam.char1 || *c == parseParam.char2 || *c == parseParam.char3)
		{
			if (data.hasNumber)
				list.push_back(data.number);

			data.number = 0;
			data.hasNumber = false;
		}

		c++;
		bytes--;
	}

	return false;
}

template<class IntT>
inline void ReadCharsAsBits_End(std::vector<IntT>& list, ReadCharsAsBits_Data<IntT>& data)
{
	if (data.hasNumber)
		list.push_back(data.number);
}

#define ReadCharsAs8Bits { ReadCharsAsBits_Main<uint8_t>, ReadCharsAsBits_End<uint8_t> }
#define ReadCharsAs8BitsT uint8_t, ReadCharsAsBits_Data<uint8_t>, ReadCharsAsBits_Params

// ReadVec2D

struct ReadVec2D_Data
{
	bool hasNumber;
	int number;

	bool hasX;
	Vec2D vector;
};

struct ReadVec2D_Params
{
	char until = 0;
	char char1 = 0;
	char char2 = 0;
	char char3 = 0;
};

inline bool ReadVec2D_Main(char*& c, std::streamsize& bytes, std::vector<Vec2D>& list, ReadVec2D_Data& data, const ReadVec2D_Params& parseParam)
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
			{
				if (!data.hasX)
				{
					data.vector.X = data.number;
					data.hasX = true;
				}
				else
				{
					data.vector.Y = data.number;
					data.hasX = false;
					list.push_back(data.vector);
				}
			}

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

inline void ReadVec2D_End(std::vector<Vec2D>& list, ReadVec2D_Data& data)
{
	if (data.hasNumber && data.hasX)
	{
		data.vector.Y = data.number;
		data.hasX = false;
		list.push_back(data.vector);
	}
}

#define ReadVec2D { ReadVec2D_Main, ReadVec2D_End }
#define ReadVec2DT Vec2D, ReadVec2D_Data, ReadVec2D_Params

// ReadTypeAndInt

template<class IntT>
struct ReadTypeAndInt_Data
{
	bool hasType;
	bool hasNumber;
	char type;
	IntT number;
};

struct ReadTypeAndInt_Params
{
	char firstValidType = 0;
	char lastValidType = 0;
};

template<class IntT>
inline bool ReadTypeAndInt_Main(char*& c, std::streamsize& bytes, std::vector<std::tuple<char, IntT>>& list, ReadTypeAndInt_Data<IntT>& data, const ReadTypeAndInt_Params& parseParam)
{
	while (bytes > 0)
	{
		if (*c >= '0' && *c <= '9')
		{
			data.hasNumber = true;
			data.number *= 10;
			data.number += *c - '0';
		}
		else if (*c >= parseParam.firstValidType && *c <= parseParam.lastValidType)
		{
			if (!data.hasType)
			{
				data.type = *c;
				data.hasType = true;
			}
		}
		else if (*c == '\n')
		{
			if (data.hasType && data.hasNumber)
				list.emplace_back(data.type, data.number);

			data.number = 0;
			data.hasType = false;
			data.hasNumber = false;
		}

		c++;
		bytes--;
	}

	return false;
}

template<class IntT>
inline void ReadTypeAndInt_End(std::vector<std::tuple<char, IntT>>& list, ReadTypeAndInt_Data<IntT>& data)
{
	if (data.hasType && data.hasNumber)
		list.emplace_back(data.type, data.number);
}

#define ReadTypeAndInt { ReadTypeAndInt_Main<int>, ReadTypeAndInt_End<int> }
#define ReadTypeAndIntT std::tuple<char, int>, ReadTypeAndInt_Data<int>, ReadTypeAndInt_Params