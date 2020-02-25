#include "Game.h"


Game::Game(unsigned int width, unsigned int height, const char* title)
{
	this->window = new sf::RenderWindow(sf::VideoMode(width, height), title);
	this->window->setFramerateLimit(this->FPS);
	
	b = new Ball(25.f);
}

void Game::start()
{

	std::chrono::time_point<std::chrono::high_resolution_clock> fpsTimer(std::chrono::high_resolution_clock::now());

	while (this->window->isOpen())
	{
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - fpsTimer);

		sf::Event e;
		while (this->window->pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				this->window->close();

			if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
			{
				if (!b->hasStared())
					b->shoot(sf::Mouse::getPosition(*this->window), sf::Vector2f(0.1, 0.01));
			}

		}

		if (elapsed.count() >= 1.0 / FPS)
		{
			std::cout << elapsed.count() << std::endl;

			sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
			b->update(elapsed.count(), mousePos);

			


			//Make Camera folow the ball
			if (this->b->hasStared())
			{
				auto view = this->window->getView();
				view.setCenter(b->getPosition());
				this->window->setView(view);
			}



			fpsTimer = std::chrono::high_resolution_clock::now();
		}

		this->window->clear();

		this->window->draw(*b);


		this->window->display();

	}
}

Game::~Game()
{
	delete this->b;
	delete this->window;
}
