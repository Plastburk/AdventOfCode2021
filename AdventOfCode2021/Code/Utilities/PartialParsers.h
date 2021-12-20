#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <functional>

// Boilerplate

template <class ListT, class DataT, class ParseParamT>
using ParseFunc = std::function<bool(char*&, std::streamsize&, std::vector<ListT>&, DataT&, const ParseParamT&)>;

template <class ListT, class DataT>
using ParseFuncEnd = std::function<void(std::vector<ListT>&, DataT&)>;

template<class ListT, class DataT, class ParseParamT>
struct ParseFuncContainer
{
	ParseFunc<ListT, DataT, ParseParamT> Main;
	ParseFuncEnd<ListT, DataT> End;
};

template<class ListT1, class DataT1, class ParseParamT1, class ListT2, class DataT2, class ParseParamT2>
inline void ReadFuncFromStream(std::ifstream& stream, std::vector<ListT1>& list1, ParseFuncContainer<ListT1, DataT1, ParseParamT1> partial1, const ParseParamT1& parseParam1, std::vector<ListT2>& list2, ParseFuncContainer<ListT2, DataT2, ParseParamT2> partial2, const ParseParamT2& parseParam2)
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
			if (partial1.Main(c, bytes, list1, data1, parseParam1))
			{
				partial1.End(list1, data1);
				running1 = false;
			}
		}

		if (!running1)
		{
			if (partial2.Main(c, bytes, list2, data2, parseParam2))
				break;
		}
	}

	if (running1)
		partial1.End(list1, data1);
	else
		partial2.End(list2, data2);
}

template<class ListT1, class DataT1, class ParseParamT1>
inline void ReadFuncFromStream(std::ifstream& stream, std::vector<ListT1>& list1, ParseFuncContainer<ListT1, DataT1, ParseParamT1> partial1, const ParseParamT1& parseParam1)
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

		if (partial1.Main(c, bytes, list1, data1, parseParam1))
			break;
	}

	partial1.End(list1, data1);
}

// ReadInts

template<class IntT>
struct ReadInts_Data
{
	bool HasNumber;
	IntT Number;
};

struct ReadInts_Params
{
	char Until = 0;
	char Char1 = 0;
	char Char2 = 0;
	char Char3 = 0;
};

template<class IntT>
inline bool ReadInts_Main(char*& c, std::streamsize& bytes, std::vector<IntT>& list, ReadInts_Data<IntT>& data, const ReadInts_Params& parseParam)
{
	while (bytes > 0)
	{
		if (*c >= '0' && *c <= '9')
		{
			data.HasNumber = true;
			data.Number *= 10;
			data.Number += *c - '0';
		}
		else if (*c == parseParam.Char1 || *c == parseParam.Char2 || *c == parseParam.Char3)
		{
			if (data.HasNumber)
				list.push_back(data.Number);

			data.Number = 0;
			data.HasNumber = false;
		}
		else if (*c == parseParam.Until)
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
	if (data.HasNumber)
		list.push_back(data.Number);
}

#define ReadInts { ReadInts_Main<int>, ReadInts_End<int> }
#define ReadIntsT int, ReadInts_Data<int>, ReadInts_Params

#define ReadUInt16s { ReadInts_Main<uint16_t>, ReadInts_End<uint16_t> }
#define ReadUInt16sT uint16_t, ReadInts_Data<uint16_t>, ReadInts_Params

#define ReadUInt8s { ReadInts_Main<uint8_t>, ReadInts_End<uint8_t> }
#define ReadUInt8sT uint8_t, ReadInts_Data<uint8_t>, ReadInts_Params

// ReadSignedInts

struct ReadSignedInts_Data
{
	bool IsNegative;
	bool HasNumber;
	int Number;
};

struct ReadSignedInts_Params
{
	char Char1 = 0;
	char Char2 = 0;
	char Char3 = 0;
};

inline bool ReadSignedInts_Main(char*& c, std::streamsize& bytes, std::vector<int>& list, ReadSignedInts_Data& data, const ReadSignedInts_Params& parseParam)
{
	while (bytes > 0)
	{
		if (*c >= '0' && *c <= '9')
		{
			data.HasNumber = true;
			data.Number *= 10;
			data.Number += *c - '0';
		}
		if (*c == '-')
		{
			data.IsNegative = true;
		}
		else if (*c == parseParam.Char1 || *c == parseParam.Char2 || *c == parseParam.Char3)
		{
			if (data.HasNumber)
				list.push_back(data.Number * (data.IsNegative ? -1 : 1));

			data.Number = 0;
			data.HasNumber = false;
			data.IsNegative = false;
		}

		c++;
		bytes--;
	}

	return false;
}

inline void ReadSignedInts_End(std::vector<int>& list, ReadSignedInts_Data& data)
{
	if (data.HasNumber)
		list.push_back(data.Number * (data.IsNegative ? -1 : 1));
}

#define ReadSignedInts { ReadSignedInts_Main, ReadSignedInts_End }
#define ReadSignedIntsT int, ReadSignedInts_Data, ReadSignedInts_Params

// ReadChars

struct ReadChars_Data
{};

struct ReadChars_Params
{
	char Until = 0;
};

inline bool ReadChars_Main(char*& c, std::streamsize& bytes, std::vector<char>& list, ReadChars_Data& data, const ReadChars_Params& parseParam)
{
	while (bytes > 0)
	{
		if ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z'))
		{
			list.push_back(*c);
		}
		else if (*c == parseParam.Until)
		{
			return true;
		}

		c++;
		bytes--;
	}

	return false;
}

inline void ReadChars_End(std::vector<char>& list, ReadChars_Data& data)
{}

#define ReadChars { ReadChars_Main, ReadChars_End }
#define ReadCharsT char, ReadChars_Data, ReadChars_Params

// ReadCharsAsBits

template<class IntT>
struct ReadCharsAsBits_Data
{
	bool HasNumber;
	IntT Number;
};

struct ReadCharsAsBits_Params
{
	char Char1 = 0;
	char Char2 = 0;
	char Char3 = 0;
};

template<class IntT>
inline bool ReadCharsAsBits_Main(char*& c, std::streamsize& bytes, std::vector<IntT>& list, ReadCharsAsBits_Data<IntT>& data, const ReadCharsAsBits_Params& parseParam)
{
	while (bytes > 0)
	{
		if (*c >= 'a' && *c <= 'z')
		{
			data.HasNumber = true;
			data.Number |= 1 << (*c - 'a');
		}
		else if (*c == parseParam.Char1 || *c == parseParam.Char2 || *c == parseParam.Char3)
		{
			if (data.HasNumber)
				list.push_back(data.Number);

			data.Number = 0;
			data.HasNumber = false;
		}

		c++;
		bytes--;
	}

	return false;
}

template<class IntT>
inline void ReadCharsAsBits_End(std::vector<IntT>& list, ReadCharsAsBits_Data<IntT>& data)
{
	if (data.HasNumber)
		list.push_back(data.Number);
}

#define ReadCharsAs8Bits { ReadCharsAsBits_Main<uint8_t>, ReadCharsAsBits_End<uint8_t> }
#define ReadCharsAs8BitsT uint8_t, ReadCharsAsBits_Data<uint8_t>, ReadCharsAsBits_Params

// ReadVec2D

struct ReadVec2D_Data
{
	bool HasNumber;
	int Number;

	bool HasX;
	Vec2D Vector;
};

struct ReadVec2D_Params
{
	char Until = 0;
	char Char1 = 0;
	char Char2 = 0;
	char Char3 = 0;
};

inline bool ReadVec2D_Main(char*& c, std::streamsize& bytes, std::vector<Vec2D>& list, ReadVec2D_Data& data, const ReadVec2D_Params& parseParam)
{
	while (bytes > 0)
	{
		if (*c >= '0' && *c <= '9')
		{
			data.HasNumber = true;
			data.Number *= 10;
			data.Number += *c - '0';
		}
		else if (*c == parseParam.Char1 || *c == parseParam.Char2 || *c == parseParam.Char3)
		{
			if (data.HasNumber)
			{
				if (!data.HasX)
				{
					data.Vector.X = data.Number;
					data.HasX = true;
				}
				else
				{
					data.Vector.Y = data.Number;
					data.HasX = false;
					list.push_back(data.Vector);
				}
			}

			data.Number = 0;
			data.HasNumber = false;
		}
		else if (*c == parseParam.Until)
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
	if (data.HasNumber && data.HasX)
	{
		data.Vector.Y = data.Number;
		data.HasX = false;
		list.push_back(data.Vector);
	}
}

#define ReadVec2D { ReadVec2D_Main, ReadVec2D_End }
#define ReadVec2DT Vec2D, ReadVec2D_Data, ReadVec2D_Params

// ReadTypeAndInt

template<class IntT>
struct ReadTypeAndInt_Data
{
	bool HasType;
	bool HasNumber;
	char Type;
	IntT Number;
};

struct ReadTypeAndInt_Params
{
	char FirstValidType = 0;
	char LastValidType = 0;
};

template<class IntT>
inline bool ReadTypeAndInt_Main(char*& c, std::streamsize& bytes, std::vector<std::tuple<char, IntT>>& list, ReadTypeAndInt_Data<IntT>& data, const ReadTypeAndInt_Params& parseParam)
{
	while (bytes > 0)
	{
		if (*c >= '0' && *c <= '9')
		{
			data.HasNumber = true;
			data.Number *= 10;
			data.Number += *c - '0';
		}
		else if (*c >= parseParam.FirstValidType && *c <= parseParam.LastValidType)
		{
			if (!data.HasType)
			{
				data.Type = *c;
				data.HasType = true;
			}
		}
		else if (*c == '\n')
		{
			if (data.HasType && data.HasNumber)
				list.emplace_back(data.Type, data.Number);

			data.Number = 0;
			data.HasType = false;
			data.HasNumber = false;
		}

		c++;
		bytes--;
	}

	return false;
}

template<class IntT>
inline void ReadTypeAndInt_End(std::vector<std::tuple<char, IntT>>& list, ReadTypeAndInt_Data<IntT>& data)
{
	if (data.HasType && data.HasNumber)
		list.emplace_back(data.Type, data.Number);
}

#define ReadTypeAndInt { ReadTypeAndInt_Main<int>, ReadTypeAndInt_End<int> }
#define ReadTypeAndIntT std::tuple<char, int>, ReadTypeAndInt_Data<int>, ReadTypeAndInt_Params