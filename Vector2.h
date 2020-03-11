#pragma once
#include <math.h>
#include "SFML/Graphics.hpp"

class vec2_t
{
public:
	float x, y;

	vec2_t() 
		:x(0.f), y(0.f) {};
	vec2_t(float x, float y);

	vec2_t normalize();
	vec2_t crossProduct(vec2_t b);
	float length();
	float dotProduct(vec2_t b);

	inline vec2_t operator/(const float t);
	inline vec2_t operator+(vec2_t a);
	inline vec2_t operator-(vec2_t a);
	inline vec2_t operator*(const float t);
	inline vec2_t operator/=(const float t);

	operator sf::Vector2f();
};