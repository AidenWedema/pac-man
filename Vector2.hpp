#pragma once
#include <iostream>
#include <cmath>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846 // pi
#endif

enum Directions
{
	LEFT,
	RIGHT,
	DOWN,
	UP
};

class Vector2
{
public:
	Vector2() : x(0), y(0) {}
	Vector2(int8_t x, int8_t y) : x(x), y(y) {}
	Vector2(Directions direction) { Vector2 dir = Vector2::DirectionToVector(direction); x = dir.x; y = dir.y; }
	~Vector2() {}

	int8_t x;
	int8_t y;

	static Vector2 Left() { return Vector2(-1, 0); }
	static Vector2 Right() { return Vector2(1, 0); }
	static Vector2 Down() { return Vector2(0, 1); }
	static Vector2 Up() { return Vector2(0, -1); }

	Vector2 operator+(const Vector2& other) {
		Vector2 result;
		result.x = x + other.x;
		result.y = y + other.y;
		// simulate overflow of y into x from original arcade game
		if (y < 0 || other.y < 0)
			result.x -= 1;
		return result;
	}

	Vector2 operator+(const Directions& other) {
		return *this + DirectionToVector(other);
	}

	Vector2 operator*(const uint8_t& other)	{
		Vector2 result;
		result.x = x * other;
		result.y = y * other;
		// simulate overflow of y into x from original arcade game
		if (y < 0)
			result.x -= std::abs(other) - 1;
		return result;
	}

	bool operator==(const Vector2& other) const {
		return x == other.x && y == other.y;
	}

	Vector2 operator=(Directions direction)	{
		switch (direction)
		{
		case LEFT:
			return Left();
		case RIGHT:
			return Right();
		case DOWN:
			return Down();
		case UP:
			return Up();
		}
	}

	Directions operator=(Vector2 direction)	{
		if (direction.x < 0)
			return LEFT;
		if (direction.x > 0)
			return RIGHT;
		if (direction.y < 0)
			return UP;
		if (direction.y > 0)
			return DOWN;
	}

	static Vector2 DirectionToVector(Directions direction) {
		switch (direction)
		{
		case LEFT:
			return Left();
		case RIGHT:
			return Right();
		case DOWN:
			return Down();
		case UP:
			return Up();
		}
	}

	Vector2 VectorDistance(const Vector2& other) const {
		return Vector2(x - other.x, y - other.y);
	}

	static Vector2 VectorDistance(const Vector2& v1, const Vector2& v2) {
		return Vector2(v1.x - v2.x, v1.y - v2.y);
	}

	float Distance(const Vector2& other) const {
		return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
	}

	static float Distance(const Vector2& v1, const Vector2& v2) {
		return std::sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
	}

	Vector2 RotateAround(const Vector2& center, float degrees) const
	{
		// Convert degrees to radians
		float radians = degrees * (M_PI / 180.0f);

		// Translate the point to rotate around the center (make center the origin)
		float translatedX = x - center.x;
		float translatedY = y - center.y;

		// Apply rotation formulas
		float rotatedX = translatedX * std::cos(radians) - translatedY * std::sin(radians);
		float rotatedY = translatedX * std::sin(radians) + translatedY * std::cos(radians);

		// Translate the point back
		return Vector2(rotatedX + center.x, rotatedY + center.y);
	}

	std::string toString()
	{
		return std::to_string(x) + ", " + std::to_string(y);
	}
};

// Define a custom hash function for Vector2 by specializing std::hash
namespace std {
	template <>
	struct hash<Vector2> {
		std::size_t operator()(const Vector2& v) const {
			// Combine x and y into a single hash value
			// A common way is to use bitwise operations or a hashing technique
			return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
		}
	};
}