#include "Vector2.h"

Vector2::Vector2(float x, float y)
	:x(x), y(y)
{

}

Vector2 Vector2::normalize()
{
	Vector2 result = *this;
	return result /= this->length();
}

float Vector2::length()
{
	return sqrt(pow(this->x, 2) + pow(this->y, 2));
}

inline Vector2 Vector2::crossProduct(Vector2 b)
{
	return Vector2(this->x * b.x, this->y * b.y);
}

float Vector2::dotProduct(Vector2 b)
{
	return (this->x * b.x + this->y * b.y);
}

inline Vector2 Vector2::operator/(const float t)
{
	Vector2 b = *this;
	b.x /= t;
	b.y /= t;
	return b;
}

inline Vector2 Vector2::operator+(Vector2 a)
{
	return Vector2(this->x + a.x, this->y + a.y);
}

inline Vector2 Vector2::operator-(Vector2 a)
{
	return Vector2(this->x - a.x, this->y - a.y);
}

inline Vector2 Vector2::operator*(const float t)
{
	Vector2 b = *this;
	b.x *= t;
	b.y *= t;
	return b;
}

inline Vector2 Vector2::operator/=(const float t)
{
	float d = 1.f / t;
	this->x * d;
	this->y * d;
	return *this;
}
