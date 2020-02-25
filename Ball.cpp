#include "Ball.h"

Ball::Ball(float radius, sf::Color color)
	:radius(radius)
{
	this->shape = new sf::CircleShape(radius);
	this->shape->setFillColor(color);

}

void Ball::shoot(sf::Vector2i startPos, sf::Vector2f vel)
{
	this->started = true;
	this->velocity = vel;
	this->startPos = startPos;

}

void Ball::update(float dt, sf::Vector2i cursorPos)
{
	static int dotDelay = 0;

	if (started)
	{
		dotDelay++;


		timeSinceStart += dt;



		float deltaX = this->velocity.x * timeSinceStart;
		float posY = velocity.y + (GRAVITY * timeSinceStart);

		this->currentPos = sf::Vector2f(startPos.x + deltaX, posY);
		this->shape->setPosition(this->currentPos);

		if (dotDelay == dotDensity)
		{
			if (dotIndex == dotBufferCount)
				dotIndex = 0;

			sf::CircleShape dot(dotRadius);
			dot.setFillColor(sf::Color::Red);
			dot.setPosition(this->currentPos.x + radius, currentPos.y + radius);

			this->dots[dotIndex++] = dot;
			
			dotDelay = 0;
		}

	}
	else		//Follows cursor Position
	{
		this->shape->setPosition((float)cursorPos.x - this->radius, (float)cursorPos.y -this->radius);
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
