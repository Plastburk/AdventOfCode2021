#pragma once

#include <functional>
#include <array>

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

struct Vec3D
{
	int X;
	int Y;
	int Z;

	Vec3D() :
		X(0), Y(0), Z(0)
	{	}

	Vec3D(int x, int y, int z) :
		X(x), Y(y), Z(z)
	{	}

	inline bool operator==(const Vec3D& other) const
	{
		return X == other.X && Y == other.Y && Z == other.Z;
	}

	inline Vec3D operator+(const Vec3D& other) const
	{
		return Vec3D(X + other.X, Y + other.Y, Z + other.Z);
	}

	inline Vec3D operator-(const Vec3D& other) const
	{
		return Vec3D(X - other.X, Y - other.Y, Z - other.Z);
	}

	inline Vec3D operator-() const
	{
		return Vec3D(-X, -Y, -Z);
	}

	static int DistanceSquared(const Vec3D& a, const Vec3D& b)
	{
		return (b.X - a.X) * (b.X - a.X) + (b.Y - a.Y) * (b.Y - a.Y) + (b.Z - a.Z) * (b.Z - a.Z);
	}
};

template<>
struct std::hash<Vec3D>
{
	size_t operator()(const Vec3D& value) const
	{
		return ((size_t)value.X & 0x1FFFFF) << 42 | ((size_t)value.Y & 0x1FFFFF) << 21 | ((size_t)value.Z & 0x1FFFFF);
	}
};

typedef std::array<int, 9> Matrix;

inline Vec3D operator*(const Vec3D& vec, const Matrix& matrix)
{
	return Vec3D(
		vec.X * matrix[0] + vec.Y * matrix[1] + vec.Z * matrix[2],
		vec.X * matrix[3] + vec.Y * matrix[4] + vec.Z * matrix[5],
		vec.X * matrix[6] + vec.Y * matrix[7] + vec.Z * matrix[8]
	);
}

inline Vec3D operator*(const Matrix& matrix, const Vec3D& vec)
{
	return vec * matrix;
}
