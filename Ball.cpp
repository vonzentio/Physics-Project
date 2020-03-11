#include "Ball.h"

Ball::Ball(float radius, sf::Color color)
	:radius(radius * 0.01), m_frameTime(0), m_airForce(0), m_mass(0),
	m_totalTime(0), m_magnusForce(0), m_totalForce(0), angularVelocity(6*M_PI), oldVel(0)
{
	this->shape = new sf::CircleShape(radius);
	this->shape->setFillColor(color);
	this->shape->setRadius(radius * 0.01);
	this->shape->setOrigin(sf::Vector2f(radius* 0.01, radius* 0.01));
	this->dots = new sf::CircleShape[dotBufferCount];
	this->started = false;
	this->m_mode = REALISM::ONLY_GRAVITY;
	this->m_area = this->radius * this->radius * M_PI;

	this->texture = new sf::Texture();
	this->texture->loadFromFile("wonk-ball.png");
	this->sprite = new sf::Sprite();
	this->sprite->setTexture(*texture);
	this->sprite->setOrigin(this->texture->getSize().x/2, this->texture->getSize().y / 2);
	this->sprite->setScale(0.1, 0.1);
}

void Ball::shoot(sf::Vector2f startPos, float vel, float angle, REALISM mode)
{
	this->started = true;
	this->currentPos = startPos;
	this->xPos = 0;
	this->yPos = 0;
	this->resultingVelocity = vel;
	this->angle = angle * (M_PI / 180);
	this->startVelocity.x = resultingVelocity * cosf(angle * (M_PI / 180));
	this->startVelocity.y = resultingVelocity * sinf(angle * (M_PI / 180));
	this->velocityX = startVelocity.x;
	this->velocityY = startVelocity.y;
	this->accelerationX = 0;
	this->accelerationY = 0;

	this->m_mode = mode;

	output_info info;
	info.posX = std::to_string(currentPos.x);
	info.posY = std::to_string(currentPos.y);
	info.velX = std::to_string(this->velocityX);
	info.velY = std::to_string(this->velocityY);
	info.timeSinceLauch = std::to_string(m_totalTime);

	sf::CircleShape dot(dotRadius);
	dot.setFillColor(this->dotColor);
	dot.setOrigin(sf::Vector2f(dotRadius, dotRadius));

	this->dots[dotIndex++] = dot;
	this->dotDelay = 0;

	this->m_mass = 0.45f;

	switch (mode)
	{
	case ALL:
		this->dotColor = sf::Color::Magenta;
		info.realismMode = "All";
		break;
	case ONLY_GRAVITY:
		info.realismMode = "Gravity";
		break;
	case AIR_RESISTANCE:
		this->dotColor = sf::Color(40, 108, 130);
		info.realismMode = "Air";
		break;
	case MAGNUS_EFFECT:
		this->dotColor = sf::Color::Yellow;
		info.realismMode = "Magnus";
		break;
	}
	this->projectile_info.push_back(info);
	dot.setPosition(this->currentPos.x, currentPos.y);
	this->shape->setPosition(this->currentPos);
	this->sprite->setPosition(this->currentPos);
}

void Ball::update(float dt)
{
	if (started && !done)
	{
		dotDelay++;

		this->m_frameTime = (dt / 1000);
		this->m_totalTime += m_frameTime;

		switch (this->m_mode)
		{
		case(ONLY_GRAVITY):
			break;
		case(AIR_RESISTANCE):
			this->doAir();
			break;
		case(MAGNUS_EFFECT):
			this->doMagnus();
			break;
		case(ALL):
			this->doAir();
			this->doMagnus();
			break;
		}

		this->m_totalForce = m_magnusForce + m_airForce;

		float forceX = m_totalForce * cosf(angle);
		this->accelerationX = forceX / m_mass;

		float forceY = m_totalForce * sinf(angle)- m_mass * GRAVITY;
		this->accelerationY = (forceY / m_mass);

		this->velocityX += this->accelerationX * this->m_frameTime;
		this->velocityY += this->accelerationY * this->m_frameTime;

		this->startVelocity = sf::Vector2f(this->velocityX, this->velocityY);
		this->oldVel = resultingVelocity;
		this->resultingVelocity = sqrt((this->startVelocity.x * this->startVelocity.x +
			this->startVelocity.y * this->startVelocity.y));

		this->deltaAngle = atanf(this->startVelocity.y / this->startVelocity.x) - angle;
		this->angle = atanf(this->startVelocity.y / this->startVelocity.x);

		this->currentPos.x = this->currentPos.x + this->velocityX * this->m_frameTime;
		this->currentPos.y = this->currentPos.y - this->velocityY * this->m_frameTime;

		this->shape->setPosition(this->currentPos);
		this->sprite->setPosition(this->currentPos);

		//Save current Values afor output;
		output_info info;
		info.posX = std::to_string(currentPos.x);
		info.posY = std::to_string(currentPos.y);
		info.velX = std::to_string(this->velocityX);
		info.velY = std::to_string(this->velocityY);
		info.timeSinceLauch = std::to_string(m_totalTime);
		this->projectile_info.push_back(info);




		if (dotDelay == dotDensity)
		{
			if (dotIndex == dotBufferCount)
				dotIndex = 0;

			sf::CircleShape dot(dotRadius);
			dot.setFillColor(this->dotColor);
			dot.setOrigin(sf::Vector2f(dotRadius, dotRadius));
			dot.setPosition(this->currentPos.x, currentPos.y);

			this->dots[dotIndex++] = dot;
			
			dotDelay = 0;
		}

	}
}

void Ball::doAir()
{
	this->m_airForce = -0.47 * ((1.293 * pow(this->radius, 2) * M_PI * pow(resultingVelocity, 2)) / 2);
}

void Ball::doMagnus()
{
	/*float torque = m_airForce * radius;
	torque += m_magnusForce * radius;
	float inertia = (2.f * m_mass * radius * radius/ 5.f);
	float angularAcceleration = torque / inertia;*/
	this->m_magnusForce = (2*M_PI* 1.293*resultingVelocity*pow(radius, 4)*angularVelocity) / (2*radius);
	this->sprite->rotate(angularVelocity);
	this->angularVelocity -= 0.1f;
}

void Ball::setFinished()
{
	this->shape->setFillColor(sf::Color(0, 0, 0, 0));
	this->done = true;
}



void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	target.draw(*this->shape);
	target.draw(*this->sprite);
	for (int i = 0; i < dotBufferCount; i++)
	{
		target.draw(dots[i]);
	}

}

Ball::~Ball()
{
	if (this->shape != nullptr)
	{
		delete this->shape;
		this->shape = nullptr;
	}
	if (this->dots != nullptr)
	{
		delete[] this->dots;
		this->dots = nullptr;
	}
}
