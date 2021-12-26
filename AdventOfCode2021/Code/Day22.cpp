#include "Day22.h"
#include "Utilities/Utilities.h"

void Day22::ReadInput(std::ifstream& stream)
{
	Input.reserve(500);

	char buffer[8192];
	int values[6]{};
	int i = 0;
	bool negative = false;
	bool hasValue = false;
	bool turnOn = false;

	while (true)
	{
		stream.read(buffer, sizeof(buffer));
		char* c = buffer;

		std::streamsize bytes = stream.gcount();
		if (bytes == 0)
			break;

		while (bytes > 0)
		{
			if (*c == 'n')
			{
				turnOn = true;
			}
			else if (*c == '.' || *c == ',')
			{
				if (hasValue)
				{
					if (negative)
						values[i] *= -1;
					i++;
					hasValue = false;
					negative = false;
				}
			}
			else if (*c == '-')
				negative = true;
			else if (*c >= '0' && *c <= '9')
			{
				values[i] *= 10;
				values[i] += *c - '0';
				hasValue = true;
			}
			else if (*c == '\n')
			{
				if (hasValue)
				{
					if (negative)
						values[i] *= -1;
					Input.push_back(Instruction{ turnOn,
							Vec3D(values[0], values[2], values[4]),
							Vec3D(values[1] + 1, values[3] + 1, values[5] + 1)
						});

					values[0] = 0;
					values[1] = 0;
					values[2] = 0;
					values[3] = 0;
					values[4] = 0;
					values[5] = 0;
					negative = false;
					hasValue = false;
					turnOn = false;
					i = 0;
				}
			}

			c++;
			bytes--;
		}
	}

	if (hasValue)
	{
		if (negative)
			values[i] *= -1;
		Input.push_back(Instruction{ turnOn,
				Vec3D(values[0], values[2], values[4]),
				Vec3D(values[1] + 1, values[3] + 1, values[5] + 1)
			});
	}
}

inline bool Intersects(const Cube& a, const Cube& b)
{
	return
		a.Max.X > b.Min.X && b.Max.X > a.Min.X &&
		a.Max.Y > b.Min.Y && b.Max.Y > a.Min.Y &&
		a.Max.Z > b.Min.Z && b.Max.Z > a.Min.Z;
}

inline void Subtract(const Cube& a, const Cube& b, std::vector<Cube>& cubes)
{
	if (!Intersects(a, b))
		cubes.push_back(a);
	else
	{
		Cube bb{
			{std::max(a.Min.X, b.Min.X), std::max(a.Min.Y, b.Min.Y), std::max(a.Min.Z, b.Min.Z) },
			{std::min(a.Max.X, b.Max.X), std::min(a.Max.Y, b.Max.Y), std::min(a.Max.Z, b.Max.Z) }
		};

		if (bb.Min.X > a.Min.X)
			cubes.push_back({ a.Min, { bb.Min.X, a.Max.Y, a.Max.Z } });

		if (bb.Max.X < a.Max.X)
			cubes.push_back({ { bb.Max.X, a.Min.Y, a.Min.Z }, a.Max });

		if (bb.Max.Y < a.Max.Y)
			cubes.push_back({ { bb.Min.X, bb.Max.Y, a.Min.Z }, { bb.Max.X, a.Max.Y, a.Max.Z } });

		if (bb.Min.Y > a.Min.Y)
			cubes.push_back({ { bb.Min.X, a.Min.Y, a.Min.Z }, { bb.Max.X, bb.Min.Y, a.Max.Z } });

		if (bb.Min.Z > a.Min.Z)
			cubes.push_back({ { bb.Min.X, bb.Min.Y, a.Min.Z }, {bb.Max.X, bb.Max.Y, bb.Min.Z } });

		if (bb.Max.Z < a.Max.Z)
			cubes.push_back({ { bb.Min.X, bb.Min.Y, bb.Max.Z }, { bb.Max.X, bb.Max.Y, a.Max.Z } });
	}
}

inline void FindCubesIntersectingWith(const Cube& cube, const std::vector<Cube>& cubes, std::vector<Cube>& overlaps)
{
	overlaps.clear();

	for (const auto& other : cubes)
		if (Intersects(cube, other))
			overlaps.push_back(other);
}

inline bool IsInside5050(const Cube& cube)
{
	return ((cube.Min.X >= -50 && cube.Max.X <= 50) ||
		(cube.Min.Y >= -50 && cube.Max.Y <= 50) ||
		(cube.Min.Z >= -50 && cube.Max.Z <= 50));
}

inline bool Contains5050(const Cube& cube)
{
	return (cube.Min.X <= 50 && cube.Max.X >= -50 &&
		cube.Min.Y <= 50 && cube.Max.Y >= -50 &&
		cube.Min.Z <= 50 && cube.Max.Z >= -50);
}

template <bool IsPart1>
uint64_t Run(std::vector<Instruction>& input)
{
	std::vector<Cube> seenCubes;
	seenCubes.reserve(500);

	uint64_t volume = 0;

	// Loop backwards
	// That way, we can simply subtract all cubes after the current cube from the current cube, and sum the remaining volume
	for (int i = (int)input.size() - 1; i >= 0; i--)
	{
		const Instruction& instruction = input[i];
		Cube cube = instruction.Cube;

		if (IsPart1)
		{
			if (!Contains5050(cube))
				continue;
			else if (!IsInside5050(cube))
			{
				cube.Min = { std::max(cube.Min.X, -50), std::max(cube.Min.Y, -50), std::max(cube.Min.Z, -50) };
				cube.Max = { std::min(cube.Max.X, 50), std::min(cube.Max.Y, 50), std::min(cube.Max.Z, 50) };
			}
		}

		if (instruction.TurnOn)
		{
			// 1) Find all cubes that intersects with our cube
			std::vector<Cube> overlaps;
			FindCubesIntersectingWith(cube, seenCubes, overlaps);

			// 2) Subtract the first overlap from the original cube, this leaves us with an array of multiple remaining cube pieces
			// For the second overlap, subtract from the overlap from *all* the new cubes
			// This will generate more and more cubes for each overlap we subtract
			// Once this is done, we'll have a list of remaining cubes
			std::vector<Cube> newCubes;
			newCubes.push_back(cube);
			for (const Cube& overlap : overlaps)
			{
				std::vector<Cube> temp;
				for (const Cube& newCube : newCubes)
					Subtract(newCube, overlap, temp);
				newCubes = temp;
			}

			// 3) Count their volume and sum it
			for (const Cube& newCube : newCubes)
			{
				volume += (uint64_t)(newCube.Max.X - newCube.Min.X) * (uint64_t)(newCube.Max.Y - newCube.Min.Y) * (uint64_t)(newCube.Max.Z - newCube.Min.Z);
			}
		}

		seenCubes.push_back(cube);
	}

	return volume;
}

uint64_t Day22::RunA()
{
	return Run<true>(Input);
}

uint64_t Day22::RunB()
{
	return Run<false>(Input);
}