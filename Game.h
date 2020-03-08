#pragma once

#include <chrono>

#include <iostream>
#include <fstream>

#include "Ball.h"
#include "Ground.h"

#include <TGUI\TGUI.hpp>

//make sure you can toggle things on/off, like air resistance, magnus, follow-ball-garbage.
//Also, implement a way to vary the delta-t to showcase how we can change it on the fly. 

//also, define local origin when we drop the ball so that we don't have to keep calculating
//proper coordinates.

class Game
{
private:

	const double FPS = 60.0;
	
	int currentBall = 0;
	std::vector<Ball*> balls;
	Ground* ground = nullptr;

	//GUI-stuff - add labels.
	tgui::Gui gui;
	tgui::Theme theme{"theme/Black.txt"};
	tgui::Slider::Ptr angleSlider;
	tgui::Slider::Ptr velSlider;

	tgui::ListBox::Ptr m_list;

	//use to format output to text file. preferably something that can be coverted to matlab or excel diagrams quite easily.
	std::ifstream m_output;

	sf::RenderWindow* window;
	sf::VertexArray background;
	sf::Font font;
	std::vector<sf::Text> m_text;

	sf::View m_view;

	//Change into a vector of vectors to store all info?
	//or make output_info into a struct of vectors`?
	std::vector<Ball::output_info> file_output;

	unsigned int window_width, window_height;
	REALISM levelOfRealism;

	float angle;
	float velocity;
	float radius;

	bool followBall = false;
	bool gravity;
	bool airRes;
	bool magnus;
	bool realisticGravity;

public:
	Game(unsigned int width, unsigned int height, const char* title = "Fysik Projekt");

	void start();

	void output_info(const char* textfile);

	void signal() {
		this->angle = this->angleSlider->getValue();
	}

	void itemSelected(sf::String & item);

	~Game();
};

