#include "Game.h"


Game::Game(unsigned int width, unsigned int height, const char* title)
{
	this->window = new sf::RenderWindow(sf::VideoMode(width, height), title);
	this->window->setFramerateLimit(this->FPS);
	this->window_height = height;
	this->window_width = width;

	this->radius = 5.f;
	this->velocity = 40.f;
	this->angle = 30.f;

	tgui::Theme::setDefault(&this->theme);

	//Default level is G only.
	this->levelOfRealism = REALISM::ONLY_GRAVITY;

	auto view = this->window->getView();
	view.setCenter(sf::Vector2f(50, 50));
	view.zoom(0.1);
	m_view = view;
	this->window->setView(view);

	Ball *b = new Ball(radius);
	this->balls.push_back(b);


	//Don't forget, these widgets are positioned relative to the *Window*, not the view. 
	gui.setTarget(*this->window);
	this->angleSlider = tgui::Slider::create(0.f, 90.f);
	this->angleSlider->setPosition(view.getCenter().x - (view.getSize().x / 2) + this->angleSlider->getSize().x / 2, view.getCenter().y - (view.getSize().y / 2) + 10.f);
	this->gui.add(this->angleSlider);
	this->angleSlider->setChangeValueOnScroll(true);
	this->angleSlider->setValue(30.f);
	this->angleSlider->connect("ValueChanged", [&] {this->angle = angleSlider->getValue(); });
	this->angleSlider->setToolTip(tgui::Label::create("Changes the angle of the motion."));

	this->velSlider = tgui::Slider::create(0.f, 100.f);
	this->velSlider->setPosition(view.getCenter().x - (view.getSize().x / 2) + this->velSlider->getSize().x / 2, view.getCenter().y - (view.getSize().y / 2) + 40.f);
	this->gui.add(this->velSlider);
	this->velSlider->setValue(40.f);
	this->velSlider->connect("ValueChanged", [&] {this->velocity = velSlider->getValue(); });
	this->velSlider->setToolTip(tgui::Label::create("Changes the velocity of the ball"));

	this->m_list = tgui::ListBox::create();
	this->m_list->addItem("Gravity Only");
	this->m_list->addItem("Air Resistance");
	this->m_list->addItem("Magnus Effect");
	this->m_list->addItem("The Full Experience");
	this->m_list->setPosition(1400.f, 0.f);
	this->gui.add(this->m_list);

	this->m_list->connect("ItemSelected", [&] {this->levelOfRealism = REALISM::ALL; });

	this->font.loadFromFile("c:/windows/fonts/arial.ttf");

	this->m_output.open("info.txt", 'rw');
	this->m_output.seekg(std::ios_base::end);

	sf::Text text;


	//init ground
	
	this->ground = new Ground(view.getCenter().x - (view.getSize().x / 2), view.getCenter().y + (view.getSize().y/2) -5);



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
	this->background[0] = sf::Vertex(sf::Vector2f(view.getCenter().x - (view.getSize().x / 2), view.getCenter().y - (view.getSize().y / 2)), sf::Color::Blue);
	this->background[1] = sf::Vertex(sf::Vector2f(view.getCenter().x + (view.getSize().x / 2), view.getCenter().y - (view.getSize().y / 2)), sf::Color::Blue);
	this->background[2] = sf::Vertex(sf::Vector2f(view.getCenter().x + (view.getSize().x / 2), view.getCenter().y + (view.getSize().y / 2)), sf::Color::White);
	this->background[3] = sf::Vertex(sf::Vector2f(view.getCenter().x - (view.getSize().x / 2), view.getCenter().y + (view.getSize().y / 2)), sf::Color::White);
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

			//if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
			//{
			//	sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
			//	sf::Vector2f worldPos = window->mapPixelToCoords(mousePos);

			//	if (!this->balls.back()->hasStarted())
			//	{
			//		this->balls.back()->shoot(worldPos, 40.f, 30.f);
			//	}
			//	else
			//	{
			//		Ball* ball = new Ball(5.0f);
			//		this->balls.push_back(ball);
			//		this->balls.back()->shoot(worldPos, velocity, angle);
			//	}

			//}

			if (e.type == sf::Event::KeyReleased && e.key.code == sf::Keyboard::Space)
			{
				//sf::Vector2f worldPos = window->mapPixelToCoords(mousePos);

				sf::Vector2f worldPos = sf::Vector2f(this->m_view.getCenter().x - (this->m_view.getSize().x / 2) + radius,
					this->m_view.getCenter().y + (this->m_view.getSize().y / 2) - 10);

				if (!this->balls.back()->hasStarted() && !this->balls.empty())
				{
					this->balls.back()->shoot(worldPos, 40.f, 30.f, this->levelOfRealism);
				}
				else
				{
					Ball* ball = new Ball(5.0f);
					this->balls.push_back(ball);
					this->balls.back()->shoot(worldPos, velocity, angle, this->levelOfRealism);
				}
			}

			this->gui.handleEvent(e);
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
				balls.at(i)->update(elapsed.count());//, worldPos);



			//Update text. It's borked right now, though.
			if (this->balls.at(currentBall)->hasStarted())
			{
				this->m_text[0].setString("X: " + std::to_string(this->balls.at(currentBall)->getPosition().x) + "\nY: " + std::to_string(this->balls.at(currentBall)->getPosition().y));
				this->m_text[1].setString("VelX: " + std::to_string(this->balls.at(currentBall)->getVelocity().x) + "\nVelY: " + std::to_string(this->balls.at(currentBall)->getVelocity().y));
			}

			this->ground->update(this->balls, this->file_output);

			fpsTimer = std::chrono::high_resolution_clock::now();
		}

		this->window->clear(sf::Color::Blue);
		this->window->draw(this->background);
		this->window->draw(*this->ground);
		this->gui.draw();


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
	
	for (int i = 0; i < this->balls.size(); i++)
	{
		if (balls[i] != nullptr)
		{
			auto output = balls.at(i)->getProjectileInfo();
			for (auto o : output)
				file_output.push_back(o);
		}

	}

	outFile << "| Time since lauch | " << " | Position X | " << " | Position Y | " << "| Velocity X |" << " | Velocity Y | " << std::endl;
	for (auto i : file_output)
	{
		outFile << "|	" << i.timeSinceLauch << "  |	|" << i.posX << "| |" << i.posY << "| |" << i.velX << "| |" << i.velY << "  |" << std::endl;
	}

	outFile.close();
}

void Game::itemSelected(sf::String & item)
{
	if (item == "Gravity Only")
	{
		this->levelOfRealism = REALISM::ONLY_GRAVITY;
	}
	else if (item == "Air Resistance")
	{
		this->levelOfRealism = REALISM::AIR_RESISTANCE;
	}
	else if (item == "Magnus Effect")
	{
		this->levelOfRealism = REALISM::MAGNUS_EFFECT;
	}
	else if (item == "The Full Experience")
	{
		this->levelOfRealism = REALISM::ALL;
	}
}

Game::~Game()
{
	for (auto b : balls)
		delete b;

	delete this->window;
}
