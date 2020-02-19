#include "Ball.h"

Ball::Ball()
	:m_velocity(0.f, 0.f), m_position(0.f, 0.f), m_radius(1.f), m_mass(1.f)
{
}

Ball::Ball(vec2_t position, vec2_t velocity, float radius, float mass)
	:m_position(position), m_velocity(velocity), m_radius(radius), m_mass(mass), 
	m_gravity(DEFAULT_GRAVITY)
{
	this->m_sphere.setRadius(radius);
	this->m_sphere.setOrigin(sf::Vector2f(radius, radius));
	this->m_sphere.setPosition(sf::Vector2f(position.x, position.y));
}

Ball::~Ball()
{
}

void Ball::setPosition(vec2_t position)
{
}

void Ball::setVelocity(vec2_t position)
{
}

void Ball::update(float deltaTime)
{

}

vec2_t Ball::getPosition()
{
	return this->m_position;
}

vec2_t Ball::getVelocity()
{
	return this->m_velocity;
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->m_sphere);
}
