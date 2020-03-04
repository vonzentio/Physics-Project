#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <math.h>
#include <iostream>

constexpr float GRAVITY = 9.82f;

#ifndef M_PI
#define M_PI 3.14159265
#endif


/*
	Once the base works, add different calcs depending on starting height for the ball. 
	We can use an enum to determine which type of motion we want when we shoot the ball.

	Or we just say "fuck it" to that and focus on the regular one first.

	TODO:
	Add output of velocity, distance, and such to a textfile, in order to speed up the information gathering for the report.	
*/

enum REALISM {
	ALL,
	ONLY_GRAVITY,
	AIR_RESISTANCE,
	MAGNUS_EFFECT
};

class Ball:public sf::Drawable
{
public:
	struct output_info {
		std::string realismMode = "Projectile Motion - Full Realism";
		std::string timeSinceLauch;
		std::string posX;
		std::string posY;
		std::string velX;
		std::string velY;
	};

private:
	std::vector<output_info> projectile_info;


	sf::CircleShape* shape;
	sf::CircleShape* dots;

	unsigned int dotBufferCount = 128;
	float dotRadius = 1.f;
	unsigned int dotDensity = 5;
	int dotIndex;

	float m_frameTime;
	float m_totalTime;

	float m_mass;
	float m_airForce;
	float m_magnusForce;

	float m_totalForce;

	float radius;
	float angle;
	float deltaAngle;
	float accelerationX;
	float accelerationY;
	float velocityX;
	float velocityY;
	float xPos;
	float yPos;
	float resultingVelocity;



	bool started = false;
	bool done = false;

	bool airResistance;
	bool magnus;

	sf::Vector2f startVelocity;
	sf::Vector2f currentPos;

public:
	Ball(float radius, sf::Color color = sf::Color::Red);

	void shoot(sf::Vector2f startPos, float vel, float angle);

	void update(float dt, sf::Vector2f cursorPos);
	void doAir();
	void doMagnus();

	void setFinished();

	std::vector<output_info> getProjectileInfo() {
		return this->projectile_info;
	}

	bool hasFinished() {
		return this->done;
	}

	bool hasStarted() {
		return this->started;
	}

	sf::FloatRect getBoundingBox() {
		return this->shape->getGlobalBounds();
	}

	sf::Vector2f getPosition() {
		return this->currentPos;
	}

	sf::Vector2f getVelocity() {
		return sf::Vector2f(this->velocityX, this->velocityY);
	}

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


	~Ball();
};

