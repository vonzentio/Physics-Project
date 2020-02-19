#pragma once
#include "SFML\Graphics.hpp"
#include "Vector2.h"

#define DEFAULT_GRAVITY 9.82

class Ball : public sf::Drawable
{
private:
	vec2_t m_velocity;
	vec2_t m_position;

	//some variables for our sphere
	float m_radius; //in meters
	float m_mass; //in kilograms

	//this is just the physical representation of said sphere (in 2d)
	//maybe we should have a sprite instead, so we can see it rotate?
	//it won't be as easy to link the graphic part of the simulation
	//to the mathematical then though.
	sf::CircleShape m_sphere;

	float m_gravity;

public:

	Ball();
	Ball(vec2_t position, vec2_t velocity, float radius, float mass);
	~Ball();

	void setPosition(vec2_t position);
	void setVelocity(vec2_t position);

	//handle all updates and physics calculations directly in this class?
	void update(float deltaTime);

	vec2_t getPosition();
	vec2_t getVelocity();

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};