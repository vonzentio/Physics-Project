#pragma once

#include <SFML/Graphics.hpp>
#include "Ball.h"
#include <vector>

class Ground :public sf::Drawable
{

private:
	sf::RectangleShape *ground = nullptr;



public:
	Ground(int xPos, int yPos,  sf::Color c = sf::Color::Green);


	void update(std::vector<Ball*> & balls);

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


	~Ground() {
		delete this->ground;
	}
};

