#include "Day16.h"
#include "Utilities/Utilities.h"

enum class PacketType : uint8_t
{
	Sum,
	Product,
	Minimum,
	Maximum,
	Literal,
	GreaterThan,
	LessThan,
	EqualTo
};

void Day16::ReadInput(std::ifstream& stream)
{
	ReadHexAsBitsFromStream(stream, Input);
}

inline uint32_t ReadBits(const std::bitset<8192>& input, int bits, int& offset)
{
	uint32_t value = 0;
	for (int i = 0; i < bits; i++)
		value |= input[offset++] << (bits - i - 1);
	return value;
}

inline void ReadPacketA(std::bitset<8192>& input, int& offset, int& versionSum)
{
	int version = ReadBits(input, 3, offset);
	PacketType typeId = (PacketType)ReadBits(input, 3, offset);

	versionSum += version;

	if (typeId == PacketType::Literal)
	{
		uint32_t value = 0;
		bool lastBlock = false;
		do
		{
			lastBlock = !ReadBits(input, 1, offset);
			value <<= 4;
			value |= ReadBits(input, 4, offset);
		} while (!lastBlock);
	}
	else
	{
		int lengthTypeId = ReadBits(input, 1, offset);
		if (lengthTypeId == 0)
		{
			int length = ReadBits(input, 15, offset);
			int offsetAfterPackets = offset + length;

			while (offset < offsetAfterPackets)
				ReadPacketA(input, offset, versionSum);
		}
		else
		{
			int subPackets = ReadBits(input, 11, offset);
			for (int i = 0; i < subPackets; i++)
				ReadPacketA(input, offset, versionSum);
		}
	}
}

int Day16::RunA()
{
	int versionSum = 0;
	int offset = 0;
	ReadPacketA(Input, offset, versionSum);

	return versionSum;
}

#define ReadOperatorPacket(Func) \
{ \
	int lengthTypeId = ReadBits(input, 1, offset); \
	if (lengthTypeId == 0) \
	{ \
		int length = ReadBits(input, 15, offset); \
		int offsetAfterPackets = offset + length; \
		while (offset < offsetAfterPackets) \
		{ \
			uint64_t packetValue = ReadPacketB(input, offset); \
			Func \
		} \
	} \
	else \
	{ \
		int subPackets = ReadBits(input, 11, offset); \
		for (int i = 0; i < subPackets; i++) \
		{ \
			uint64_t packetValue = ReadPacketB(input, offset); \
			Func \
		} \
	} \
}

#define ReadOperatorPacket2(Func) \
{ \
	int lengthTypeId = ReadBits(input, 1, offset); \
	if (lengthTypeId == 0) \
		ReadBits(input, 15, offset); \
	else \
		ReadBits(input, 11, offset); \
	uint64_t packet1Value = ReadPacketB(input, offset); \
	uint64_t packet2Value = ReadPacketB(input, offset); \
	Func \
}

inline uint64_t ReadPacketB(std::bitset<8192>& input, int& offset)
{
	int version = ReadBits(input, 3, offset);
	PacketType typeId = (PacketType)ReadBits(input, 3, offset);

	switch (typeId)
	{
	case PacketType::Sum:
	{
		uint64_t value = 0;
		ReadOperatorPacket(
			{
				value += packetValue;
			});

		return value;
	}
	case PacketType::Product:
	{
		uint64_t value = 1;
		ReadOperatorPacket(
			{
				value *= packetValue;
			});

		return value;
	}
	case PacketType::Minimum:
	{
		uint64_t value = INT_MAX;
		ReadOperatorPacket(
			{
				if (packetValue < value)
					value = packetValue;
			});

		return value;
	}
	case PacketType::Maximum:
	{
		uint64_t value = 0;
		ReadOperatorPacket(
			{
				if (packetValue > value)
					value = packetValue;
			});

		return value;
	}
	case PacketType::Literal:
	{
		uint64_t value = 0;
		bool lastBlock = false;
		do
		{
			lastBlock = !ReadBits(input, 1, offset);
			value <<= 4;
			value |= ReadBits(input, 4, offset);
		} while (!lastBlock);

		return value;
	}
	case PacketType::GreaterThan:
	{
		ReadOperatorPacket2(
			{
				return (packet1Value > packet2Value);
			});
	}
	case PacketType::LessThan:
	{
		ReadOperatorPacket2(
			{
				return (packet1Value < packet2Value);
			});
	}
	case PacketType::EqualTo:
	{
		ReadOperatorPacket2(
			{
				return (packet1Value == packet2Value);
			});
	}
	}

	return 0;
}

uint64_t Day16::RunB()
{
	int offset = 0;
	return ReadPacketB(Input, offset);
}