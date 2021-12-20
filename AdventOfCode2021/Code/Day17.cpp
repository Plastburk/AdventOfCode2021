#include "Day17.h"
#include "Utilities/Utilities.h"

void Day17::ReadInput(std::ifstream& stream)
{
	std::vector<int> ints;
	ReadFuncFromStream<ReadSignedIntsT>(stream,
		ints, ReadSignedInts, { '.', ',', '\n' }
	);

	Min.X = ints[0];
	Max.X = ints[1];
	Min.Y = ints[2];
	Max.Y = ints[3];
}

inline bool TryVelocity(Vec2D velocity, const Vec2D& min, const Vec2D& max)
{
	Vec2D pos;
	while (true)
	{
		pos += velocity;

		if (pos.X > max.X || pos.Y < min.Y)
			return false;

		if (pos.X >= min.X && pos.Y <= max.Y)
			return true;

		velocity.X = std::max(velocity.X - 1, 0);
		velocity.Y--;
	}
}

int Day17::RunA()
{
	int initialYVel = -Min.Y - 1;
	return ((initialYVel * initialYVel) + initialYVel) / 2;
}

int Day17::RunB()
{
	int amount = 0;

	for (int velX = (int)std::sqrt(2 * Min.X); velX <= Max.X; velX++)
		for (int velY = Min.Y; velY < -Min.Y; velY++)
			if (TryVelocity({ velX, velY }, Min, Max))
				amount++;

	return amount;
}