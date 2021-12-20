#pragma once

#include <functional>

struct Vec2D
{
	int X;
	int Y;

	Vec2D() :
		X(0), Y(0)
	{	}

	Vec2D(int x, int y) :
		X(x), Y(y)
	{	}

	inline bool operator==(const Vec2D& other) const
	{
		return X == other.X && Y == other.Y;
	}

	inline Vec2D& operator+=(const Vec2D& other)
	{
		X += other.X;
		Y += other.Y;
		return *this;
	}
};

inline bool operator<(const Vec2D& a, const Vec2D& b)
{
	return a.Y == b.Y ? a.X < b.X : a.Y < b.Y;
}

template<>
struct std::hash<Vec2D>
{
  size_t operator()(const Vec2D& value) const
  {
		return ((size_t)value.X) << 32 | (size_t)value.Y;
  }
};