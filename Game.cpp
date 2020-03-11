#include "Game.h"


Game::Game(unsigned int width, unsigned int height, const char* title)
{
	this->window = new sf::RenderWindow(sf::VideoMode(width, height), title);
	this->window->setFramerateLimit(this->FPS);
	
	this->window_height = height;
	this->window_width = width;

	this->radius = 11.f;
	this->velocity = 30.f;
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
	this->velSlider->setValue(this->velocity);
	this->velSlider->connect("ValueChanged", [&] {this->velocity = velSlider->getValue(); });
	this->velSlider->setToolTip(tgui::Label::create("Changes the velocity of the ball"));


	
	//Init buttons
	for (int i = 0; i < 4; i++)
		this->button[i] = tgui::Button::create();


	for(int i = 0; i < 4; i++)
		this->gui.add(this->button[i]);

	this->button[0]->setText("Gravity Only");
	this->button[0]->setPosition(1400.f, 0.f);
	this->button[0]->connect("pressed", [&] {this->levelOfRealism = REALISM::ONLY_GRAVITY; });
	

	this->button[1]->setText("Air Resistance");
	this->button[1]->setPosition(this->button[0]->getPosition().x, this->button[0]->getPosition().y + 20.0f);
	this->button[1]->connect("pressed", [&] {this->levelOfRealism = REALISM::AIR_RESISTANCE; });

	this->button[2]->setText("Magnus Effect");
	this->button[2]->setPosition(this->button[0]->getPosition().x, this->button[1]->getPosition().y + 20.0f);
	this->button[2]->connect("pressed", [&] {this->levelOfRealism = REALISM::MAGNUS_EFFECT; });


	this->button[3]->setText("The Full Experience");
	this->button[3]->setPosition(this->button[0]->getPosition().x, this->button[2]->getPosition().y + 20.0f);
	this->button[3]->connect("pressed", [&] {this->levelOfRealism = REALISM::ALL; });



	this->font.loadFromFile("c:/windows/fonts/arial.ttf");

	this->m_output.open("info.txt", 'rw');
	this->m_output.seekg(std::ios_base::end);

	sf::Text text;

	//Inti text for angle and velocity
	this->textBox->setPosition(this->angleSlider->getPosition().x - 60.f,
		this->angleSlider->getPosition().y );

	this->textBox->setSize(50, 50);
	this->textBox->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
	this->textBox->setInheritedOpacity(0.7f);

	this->textBox->setTextSize(14);

	this->gui.add(this->textBox);



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

	sf::Clock clock;

	while (this->window->isOpen())
	{
		


		sf::Event e;
		while (this->window->pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				this->window->close();

			//Update the text on the sliders
			//Can only happen during mouse mooved and mouse scrolled event
			if (e.type == sf::Event::MouseWheelScrolled || e.type == sf::Event::MouseMoved)
			{
				std::string angleString = std::to_string((int)this->angleSlider->getValue()) + "\n";
				this->textBox->setText(angleString);

				std::string velocityString = std::to_string((int)this->velSlider->getValue());
				this->textBox->addText(velocityString);
			}


			if (e.type == sf::Event::KeyReleased && e.key.code == sf::Keyboard::Space)
			{
				//sf::Vector2f worldPos = window->mapPixelToCoords(mousePos);

				sf::Vector2f worldPos = sf::Vector2f(this->m_view.getCenter().x - (this->m_view.getSize().x / 2) + radius,
					this->m_view.getCenter().y + (this->m_view.getSize().y / 2) - radius * 0.01f - this->ground->getSize().y);


				if (!this->balls.back()->hasStarted() && !this->balls.empty())
				{
					this->balls.back()->shoot(worldPos, velocity, angle, this->levelOfRealism);
				}
				else
				{
					
					Ball* ball = new Ball(radius);
					this->balls.push_back(ball);
					this->balls.back()->shoot(worldPos, velocity, angle, this->levelOfRealism);
					
				}
			}

			this->gui.handleEvent(e);
		}

		
		if (clock.getElapsedTime().asMilliseconds() >= 1.0 / this->FPS)
		{
			//Update text
			if (this->balls[currentBall]->hasStarted() && !this->balls[currentBall]->hasFinished())
			{
				this->m_text.back().setString("X: " + std::to_string(this->balls.at(currentBall)->getPosition().x) + "\nY: " + std::to_string(this->balls.at(currentBall)->getPosition().y));
				this->m_text.back().setString("VelX: " + std::to_string(this->balls.at(currentBall)->getVelocity().x) + "\nVelY: " + std::to_string(this->balls.at(currentBall)->getVelocity().y));
			}
			

			std::cout << clock.getElapsedTime().asMilliseconds() << std::endl;


			sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
			sf::Vector2f worldPos = window->mapPixelToCoords(mousePos);
			

			//Update the current if needed (Determines which ball to get velocity info from)
			if (balls[currentBall]->hasFinished() && currentBall + 1 < balls.size() )
				currentBall++;



			for (int i = 0; i < this->balls.size() && balls.at(currentBall)->hasFinished() == false; i++)
				balls.at(i)->update(clock.getElapsedTime().asMilliseconds() );//, worldPos);




			this->ground->update(this->balls, this->file_output);

			clock.restart();
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

	outFile << "| Tot. Time | " << " | Position X | " << " | Position Y | " << "| Velocity X |" << " | Velocity Y | " << " | Realism Level | " << std::endl;
	for (auto i : file_output)
	{
		outFile << "|	" << i.timeSinceLauch << "  |	|" << i.posX << "| |" << i.posY << "| |" << i.velX << "| |" << i.velY << " | ";
		if (i.realismMode != " ")
		{
			outFile << "| " << i.realismMode << " | ";
		}

		outFile << std::endl;
	}

	outFile.close();
}


Game::~Game()
{
	for (auto b : balls)
		delete b;

	delete this->window;
}
