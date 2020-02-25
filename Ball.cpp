#include "Ball.h"

Ball::Ball(float radius, sf::Color color)
	:radius(radius)
{
	this->shape = new sf::CircleShape(radius);
	this->shape->setFillColor(color);
	this->shape->setRadius(radius);
	this->shape->setOrigin(sf::Vector2f(radius, radius));
}

void Ball::shoot(sf::Vector2i startPos, sf::Vector2f vel)
{
	this->started = true;
	this->startPos = vec2_t(startPos.x, startPos.y);
	this->xPos = 0;
	this->yPos = 0;
	this->startVelocity = vec2_t(vel.x, vel.y);
	this->resultingVelocity = this->resultingVelocity = sqrt(pow(startVelocity.x, 2) + pow(startVelocity.y, 2));;
	this->angle = atanf(this->startVelocity.y / this->startVelocity.x);
	this->velocityX = 0.f;
	this->velocityY = 0.f;
}

void Ball::update(float dt, sf::Vector2i cursorPos )
{
	static int dotDelay = 0;


	if (started)
	{
		dotDelay++;

		/*
			Calculate: 
			
			Acceleration. Until we add 

			The velocity based on previous frame
			New position

			save: 
			currentPos = newPos;
			currentVelocity = newVelocity;
		*/

		float time = dt / 100;

		sf::Vector2f position;

		this->angle = atanf(this->startVelocity.y / this->startVelocity.x);
		this->startVelocity.x = this->resultingVelocity * cos(angle);
		this->startVelocity.y = this->resultingVelocity * sin(angle);

		this->velocityX = startVelocity.x;
		this->velocityY = startVelocity.y;

		this->xPos = this->velocityX * time;
		this->yPos = this->velocityY - ((GRAVITY * time * time)/2.f);

		position = sf::Vector2f(startPos.x + xPos,  startPos.y + yPos);
		this->shape->setPosition(sf::Vector2f(position.x, position.y));
		this->startPos = vec2_t(position.x, position.y);

		this->resultingVelocity = sqrt(pow(velocityX, 2) + pow(velocityY, 2));

		if (dotDelay == dotDensity)
		{
			if (dotIndex == dotBufferCount)
				dotIndex = 0;

			sf::CircleShape dot(dotRadius);
			dot.setFillColor(sf::Color::Green);
			dot.setPosition(this->startPos.x, startPos.y);

			this->dots[dotIndex++] = dot;
			
			dotDelay = 0;
		}

	}
	else		//Follows cursor Position
	{
		this->shape->setPosition((float)cursorPos.x, (float)cursorPos.y);
	}


}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	target.draw(*this->shape);
	for (int i = 0; i < dotBufferCount; i++)
		target.draw(dots[i]);


}

Ball::~Ball()
{
	delete this->shape;
	delete[] this->dots;
}
