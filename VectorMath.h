#pragma once
#include "SFML/Graphics.hpp"

sf::Vector2f crossProduct(sf::Vector2f a, sf::Vector2f b)
{
	return sf::Vector2f(a.x + b.x, a.y + b.y);
}

sf::Vector2f normalize(sf::Vector2f a)
{
	sf::Vector2f b = a;
	b /= magnitude(a);
	return b;
}

float magnitude(sf::Vector2f a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

float dotProduct(sf::Vector2f a, sf::Vector2f b)
{
	return (a.x * b.x + a.y * b.y);
}