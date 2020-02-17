#pragma once
#include <math.h>

class Vector2
{
private:
	float x, y;
public:
	Vector2(float x, float y);

	Vector2 normalize();
	Vector2 crossProduct(Vector2 b);
	float length();
	float dotProduct(Vector2 b);

	inline Vector2 operator/(const float t);
	inline Vector2 operator+(Vector2 a);
	inline Vector2 operator-(Vector2 a);
	inline Vector2 operator*(const float t);
	inline Vector2 operator/=(const float t);
};