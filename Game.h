#pragma once

#include <chrono>

#include <iostream>
#include <fstream>

#include "Ball.h"
#include "Ground.h"

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


	//use to format output to text file. preferably something that can be coverted to matlab or excel diagrams quite easily.
	std::ifstream m_output;

	sf::RenderWindow* window;
	sf::VertexArray background;
	sf::Font font;
	std::vector<sf::Text> m_text;

	unsigned int window_width, window_height;


	bool followBall = false;
	bool airRes;
	bool magnus;
	bool realisticGravity;

public:
	Game(unsigned int width, unsigned int height, const char* title = "Fysik Projekt");

	void start();

	void output_info(const char* textfile);

	~Game();
};

