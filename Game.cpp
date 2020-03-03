#include "Game.h"


Game::Game(unsigned int width, unsigned int height, const char* title)
{
	this->window = new sf::RenderWindow(sf::VideoMode(width, height), title);
	this->window->setFramerateLimit(this->FPS);
	this->window_height = height;
	this->window_width = width;

	//
	auto view = this->window->getView();
	view.setSize(sf::Vector2f(160, 90));
	sf::FloatRect v = view.getViewport();
	this->window->setView(view);

	b = new Ball(5.f);

	this->font.loadFromFile("c:/windows/fonts/arial.ttf");

	this->m_output.open("info.txt", 'rw');
	this->m_output.seekg(std::ios_base::end);

	sf::Text text;

	//figure out a good way to work out the coordinates to place text + UI.
	this->m_text.push_back(text);
	this->m_text.back().setFont(font);
	this->m_text.back().setPosition(v.left + 10.f, v.top - v.height + 10.f);
	this->m_text.back().setString("X: " + std::to_string(this->b->getPosition().x) + "\nY: " + std::to_string(this->b->getPosition().y));
	this->m_text.back().setScale(0.8, 0.8);

	sf::Text velocity;
	this->m_text.push_back(velocity);
	this->m_text.back().setFont(font);
	this->m_text.back().setPosition(10.f, 50.f);
	this->m_text.back().setString("VelX: " + std::to_string(this->b->getVelocity().x) + "\nVelY: " + std::to_string(this->b->getVelocity().y));
	this->m_text.back().setScale(0.8, 0.8);

	//Background quad, scale and translate if we want to follow ball. Else just stay.
	this->background = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	this->background[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Color::Blue);
	this->background[1] = sf::Vertex(sf::Vector2f(width, 0), sf::Color::Blue);
	this->background[2] = sf::Vertex(sf::Vector2f(width, height), sf::Color::White);
	this->background[3] = sf::Vertex(sf::Vector2f(0, height), sf::Color::White);
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
				if (!b->hasStarted())
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
					sf::Vector2f worldPos = window->mapPixelToCoords(mousePos);

					b->shoot(worldPos, 40.f, 30.f);
				}

			}

		}

		if (elapsed.count() >= 1.0 / FPS)
		{
			std::cout << elapsed.count() << std::endl;

			sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
			sf::Vector2f worldPos = window->mapPixelToCoords(mousePos);
			b->update(elapsed.count(), worldPos);

			


			//Update text. It's borked right now, though.
			if (b->hasStarted())
			{
				this->m_text[0].setString("X: " + std::to_string(this->b->getPosition().x) + "\nY: " + std::to_string(this->b->getPosition().y));
				this->m_text[1].setString("VelX: " + std::to_string(this->b->getVelocity().x) + "\nVelY: " + std::to_string(this->b->getVelocity().y));
			}



			fpsTimer = std::chrono::high_resolution_clock::now();
		}

		this->window->clear();
		this->window->draw(this->background);
		this->window->draw(*b);
		
		for (int i = 0; i <this->m_text.size(); i++)
		{
			this->window->draw(this->m_text[i]);
		}

		this->window->display();

	}
}

Game::~Game()
{
	delete this->b;
	this->b = nullptr;
	delete this->window;
}
