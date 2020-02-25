#pragma once

#include <chrono>

#include <iostream>

#include "Ball.h"

class Game
{
private:

	const double FPS = 60.0;
	
	Ball* b;

	sf::RenderWindow* window;

	unsigned int window_width, window_height;

	bool followBall = false;

public:
	Game(unsigned int width, unsigned int height, const char* title = "Fysik Projekt");

	void start();

	~Game();
};

