#include "Day13.h"
#include "Utilities/Utilities.h"

#include <unordered_set>
#include <tuple>
#include <iostream>

void Day13::ReadInput(std::ifstream& stream)
{
	input1.reserve(1000);
	input2.reserve(30);

	ReadPartialsFromStream<ReadVec2DT, ReadTypeAndIntT>(stream,
		input1, ReadVec2D, { 'f',  ',', '\n' },
		input2, ReadTypeAndInt, { 'x', 'y' }
	);
}

int Day13::RunA()
{
	std::vector<Vec2D> points(input1);

	auto& [type, line] = input2[0];

	for (auto& point : points)
	{
		if (type == 'x')
			point.X = point.X < line ? point.X : (2 * line) - point.X;
		else
			point.Y = point.Y < line ? point.Y : (2 * line) - point.Y;
	}

	std::sort(points.begin(), points.end());
	return (int)(std::unique(points.begin(), points.end()) - points.begin());
}

void Day13::RunB()
{
	std::vector<Vec2D> points(input1);

	for (const auto& fold : input2)
	{
		auto& [type, line] = fold;

		for (auto& point : points)
		{
			if (type == 'x')
				point.X = point.X < line ? point.X : (2 * line) - point.X;
			else
				point.Y = point.Y < line ? point.Y : (2 * line) - point.Y;
		}
	}

	int width = 0;
	int height = 0;
	for (const auto& point : points)
	{
		auto& [x, y] = point;

		width = std::max(width, x + 1);
		height = std::max(height, y + 1);
	}

	std::vector<std::string> rows;
	rows.resize(height, std::string((size_t)width + 1, ' '));

	for (const auto& point : points)
		rows[point.Y].data()[point.X] = '#';

	printf("\n");

	for (auto& row : rows)
	{
		row.data()[width] = '\n';
		fwrite(row.data(), sizeof(char), row.length(), stdout);
	}
}