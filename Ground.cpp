#include "Ground.h"

Ground::Ground(int xPos, int yPos,  sf::Color c)
{
	this->ground = new sf::RectangleShape;
	ground->setSize(sf::Vector2f(500, 5));
	ground->setPosition(xPos, yPos);
	ground->setFillColor(c);
}

void Ground::update(std::vector<Ball*>& balls, std::vector<Ball::output_info>& info)
{
	for (unsigned int i = 0; i < balls.size() ; i++)
	{
		if (!balls.at(i)->hasFinished())
		{
			if (this->ground->getGlobalBounds().intersects(balls.at(i)->getBoundingBox()) && balls.at(i)->hasStarted())
			{
				balls.at(i)->setFinished();
			}
		}

	}
	

}

void Ground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	
	target.draw(*this->ground);
}
