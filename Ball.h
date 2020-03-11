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
		std::string realismMode = " ";
		std::string timeSinceLauch;
		std::string posX;
		std::string posY;
		std::string velX;
		std::string velY;
	};

private:
	std::vector<output_info> projectile_info;


	sf::CircleShape* shape;
	sf::Sprite* sprite;
	sf::Texture* texture;
	sf::CircleShape* dots;
	sf::Color dotColor = sf::Color::Green;

	unsigned int dotBufferCount = 128;
	float dotRadius = 0.7f;
	unsigned int dotDensity = 5;
	int dotIndex;
	int dotDelay;

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
	float m_area;

	float speen;

	bool started = false;
	bool done = false;

	bool airResistance;
	bool magnus;

	sf::Vector2f startVelocity;
	sf::Vector2f currentPos;

	REALISM m_mode;

public:
	Ball(float radius, sf::Color color = sf::Color::Red);

	void shoot(sf::Vector2f startPos, float vel, float angle, REALISM mode);

	void update(float dt);
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

