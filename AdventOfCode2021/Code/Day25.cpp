#include "Day25.h"
#include "Utilities/Utilities.h"

void Day25::ReadInput(std::ifstream& stream)
{
	stream.read(Input, sizeof(Input));
	int inputSize = (int)stream.gcount();

	for (int i = 0; i < inputSize; i++)
		if (Input[i] == '\n')
		{
			Width = i;
			break;
		}

#pragma warning(push)
#pragma warning(disable:6385)
	if (Input[inputSize - 1] == '\n')
		Height = inputSize / (Width + 1);
	else
		Height = inputSize / (Width);
#pragma warning(pop)
}

template<bool IsSouth>
inline int ToIndex(int a, int b, int mapWidth)
{
	if (IsSouth)
		return a * (mapWidth + 1) + b;
	else
		return b * (mapWidth + 1) + a;
}

template<bool IsSouth>
inline bool MoveHerd(char* map, int aDim, int bDim, int mapWidth)
{
	constexpr char c = IsSouth ? 'v' : '>';
	bool anyHasMoved = false;

	for (int b = 0; b < bDim; b++)
	{
		// Check if unit should loop, but don't actually perform it yet, as it needs to be the last move in this row/column
		bool willLoop = map[ToIndex<IsSouth>(0, b, mapWidth)] == '.' && map[ToIndex<IsSouth>(aDim - 1, b, mapWidth)] == c;

		// Move from start of row/column to end (order is important to prevent double moves when one should be blocked)
		for (int a = 0; a < aDim - 1; a++)
			if (map[ToIndex<IsSouth>(a + 1, b, mapWidth)] == '.' && map[ToIndex<IsSouth>(a, b, mapWidth)] == c)
			{
				anyHasMoved = true;
				map[ToIndex<IsSouth>(a + 1, b, mapWidth)] = c;
				map[ToIndex<IsSouth>(a++, b, mapWidth)] = '.';
			}

		// Perform loop, if one were supposed to happen
		if (willLoop)
		{
			anyHasMoved = true;
			map[ToIndex<IsSouth>(0, b, mapWidth)] = c;
			map[ToIndex<IsSouth>(aDim - 1, b, mapWidth)] = '.';
		}
	}

	return anyHasMoved;
}

int Day25::RunA()
{
	for (int steps = 0;; steps++)
	{
		bool anyHasMoved = false;

		if (MoveHerd<false>(Input, Width, Height, Width))
			anyHasMoved = true;
		if (MoveHerd<true>(Input, Height, Width, Width))
			anyHasMoved = true;

		if (!anyHasMoved)
			return steps + 1;
	};
}