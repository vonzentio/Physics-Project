#include "Game.h"


Game::Game(unsigned int width, unsigned int height, const char* title)
{
	this->window = new sf::RenderWindow(sf::VideoMode(width, height), title);
	this->window->setFramerateLimit(this->FPS);
	this->window_height = height;
	this->window_width = width;

	//
	auto view = this->window->getView();
//	view.setSize(sf::Vector2f(160, 90));
	view.zoom(0.2);
	view.setCenter(sf::Vector2f(50, 50));
	sf::FloatRect v = view.getViewport();
	this->window->setView(view);

	Ball *b = new Ball(5.f);
	Ball* b1 = new Ball(5.0f);
	Ball* b2 = new Ball(5.0f);
	this->balls.push_back(b);
	this->balls.push_back(b1);
	this->balls.push_back(b2);


	this->font.loadFromFile("c:/windows/fonts/arial.ttf");

	this->m_output.open("info.txt", 'rw');
	this->m_output.seekg(std::ios_base::end);

	sf::Text text;


	//init ground
	
	this->ground = new Ground(view.getCenter().x - (view.getSize().x / 2) + 1  ,view.getCenter().y + (view.getSize().y/2) - 10);



	//figure out a good way to work out the coordinates to place text + UI.
	this->m_text.push_back(text);
	this->m_text.back().setFont(font);
	this->m_text.back().setPosition(view.getCenter().x - (view.getSize().x/2) + 10.f, view.getCenter().y - (view.getSize().y/2) + 10.f);
	this->m_text.back().setScale(0.1, 0.1);

	sf::Text velocity;
	this->m_text.push_back(velocity);
	this->m_text.back().setFont(font);
	this->m_text.back().setPosition(view.getCenter().x - (view.getSize().x / 2) + 10.f, view.getCenter().y - (view.getSize().y / 2) + 20.f);
	this->m_text.back().setScale(0.1, 0.1);

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
				if (!this->balls.at(currentBall)->hasStarted())
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
					sf::Vector2f worldPos = window->mapPixelToCoords(mousePos);

					this->balls.at(currentBall)->shoot(worldPos, 40.f, 30.f);
					if (currentBall != this->balls.size()-1)
						currentBall++;
				}

			}

		}

		if (elapsed.count() >= 1.0 / FPS)
		{
			//Update text
			this->m_text.back().setString("X: " + std::to_string(this->balls.at(currentBall)->getPosition().x) + "\nY: " + std::to_string(this->balls.at(currentBall)->getPosition().y));
			this->m_text.back().setString("VelX: " + std::to_string(this->balls.at(currentBall)->getVelocity().x) + "\nVelY: " + std::to_string(this->balls.at(currentBall)->getVelocity().y));

			std::cout << elapsed.count() << std::endl;

			sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
			sf::Vector2f worldPos = window->mapPixelToCoords(mousePos);
			

			for (int i = 0; i < this->balls.size() && balls.at(i)->hasFinished() == false; i++)
				balls.at(i)->update(elapsed.count(), worldPos);



			//Update text. It's borked right now, though.
			if (this->balls.at(currentBall)->hasStarted())
			{
				this->m_text[0].setString("X: " + std::to_string(this->balls.at(currentBall)->getPosition().x) + "\nY: " + std::to_string(this->balls.at(currentBall)->getPosition().y));
				this->m_text[1].setString("VelX: " + std::to_string(this->balls.at(currentBall)->getVelocity().x) + "\nVelY: " + std::to_string(this->balls.at(currentBall)->getVelocity().y));
			}

			this->ground->update(this->balls);

			fpsTimer = std::chrono::high_resolution_clock::now();
		}

		this->window->clear(sf::Color::Blue);
		this->window->draw(this->background);
		this->window->draw(*this->ground);

		
		for (auto b : balls)
		{
			if (b != nullptr)
				this->window->draw(*b);
		}

		for (int i = 0; i <this->m_text.size(); i++)
		{
			this->window->draw(this->m_text[i]);
		}

		this->window->display();

	}
}

void Game::output_info(const char* textfile)
{
	std::ofstream outFile(textfile);

	std::vector<Ball::output_info> file_output;
	
	for (int i = 0; i < this->balls.size(); i++)
	{
		auto output = balls.at(i)->getProjectileInfo();
		for (auto o : output)
			file_output.push_back(o);
	}

	outFile << "| Time since lauch | " << " | Position X | " << " | Position Y | " << "| Velocity X |" << " | Velocity Y | " << std::endl;
	for (auto i : file_output)
	{
		outFile << "|	" << i.timeSinceLauch << "  |	|" << i.posX << "| |" << i.posY << "| |" << i.velX << "| |" << i.velY << "  |" << std::endl;
	}

	outFile.close();
}

Game::~Game()
{
	for (auto b : balls)
		delete b;

	delete this->window;
}
