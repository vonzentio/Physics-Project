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
	this->startVelocity = vel;
	this->currentVelocity = startVelocity;
	this->startPos = startPos;

}

void Ball::update(float dt, sf::Vector2i cursorPos )
{
	static int dotDelay = 0;
	static float timeSinceStart = 0.f;

	if (started)
	{
		
		this->launchAngle = launchAngle * PI / 180.0;
		dotDelay++;
		

		float X = this->currentPos.x + this->startVelocity.x * cos(launchAngle) * dt;
		float Y = this->currentPos.y + this->currentVelocity.y * sin(launchAngle) * dt - ((GRAVITY * pow(dt, 2)) / 2.0f); ;
		

		this->currentPos = sf::Vector2f(X, Y);
		this->shape->setPosition(this->currentPos);

		this->currentVelocity.y = currentVelocity.y + startVelocity.y * sin(this->launchAngle) - GRAVITY * dt;

		vec2_t temp(currentVelocity.x, currentVelocity.y);
		temp.normalize();
		this->currentVelocity = sf::Vector2f(temp.x, temp.y);

		this->launchAngle = atan(this->currentVelocity.y / currentVelocity.x);


		if (dotDelay == dotDensity)
		{
			if (dotIndex == dotBufferCount)
				dotIndex = 0;

			sf::CircleShape dot(dotRadius);
			dot.setFillColor(sf::Color::Green);
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
