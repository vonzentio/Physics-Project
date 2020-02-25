#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "Vector2.h"
#include <math.h>

constexpr float GRAVITY = 9.82f;


class Ball:public sf::Drawable
{
private:
	sf::CircleShape* shape;

	unsigned int dotBufferCount = 15;
	float dotRadius = 5.f;
	unsigned int dotDensity = 30;
	sf::CircleShape* dots = new sf::CircleShape[dotBufferCount];
	int dotIndex = 0;


	float radius;
	float angle;
	float velocityX;
	float velocityY;
	float xPos;
	float yPos;
	float resultingVelocity;

	bool started = false;

	vec2_t startVelocity;
	vec2_t startPos;

	//sf::Vector2f startVelocity;
	//sf::Vector2f currentVelocity;

	//sf::Vector2f currentPos;
	//sf::Vector2i startPos;

	float timeSinceStart = 0.f;


public:
	Ball(float radius, sf::Color color = sf::Color::Red);

	void shoot(sf::Vector2i startPos, sf::Vector2f vel);

	void update(float dt, sf::Vector2i cursorPos);

	bool hasStared() {
		return this->started;
	}

	sf::Vector2f getPosition() {
		return this->startPos;
	}

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


	~Ball();
};

