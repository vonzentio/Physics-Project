#include "Vector2.h"

vec2_t::vec2_t(float x, float y)
	:x(x), y(y)
{

}

vec2_t vec2_t::normalize()
{
	vec2_t result = *this;
	return result /= this->length();
}

float vec2_t::length()
{
	return sqrt(pow(this->x, 2) + pow(this->y, 2));
}

inline vec2_t vec2_t::crossProduct(vec2_t b)
{
	return vec2_t(this->x * b.x, this->y * b.y);
}

float vec2_t::dotProduct(vec2_t b)
{
	return (this->x * b.x + this->y * b.y);
}

inline vec2_t vec2_t::operator/(const float t)
{
	vec2_t b = *this;
	b.x /= t;
	b.y /= t;
	return b;
}

inline vec2_t vec2_t::operator+(vec2_t a)
{
	return vec2_t(this->x + a.x, this->y + a.y);
}

inline vec2_t vec2_t::operator-(vec2_t a)
{
	return vec2_t(this->x - a.x, this->y - a.y);
}

inline vec2_t vec2_t::operator*(const float t)
{
	vec2_t b = *this;
	b.x *= t;
	b.y *= t;
	return b;
}

inline vec2_t vec2_t::operator/=(const float t)
{
	float d = 1.f / t;
	this->x * d;
	this->y * d;
	return *this;
}

vec2_t::operator sf::Vector2f()
{
	return sf::Vector2f(this->x, this->y);
}
