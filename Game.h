#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <chrono>
#include "Vector2.hpp"
#include "Maze.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"

class Game
{
public:
	static Game* instance;
	static Game* GetInstance();

	sf::RenderWindow window;

	Vector2 debugGhostTarget = Vector2(8, 20);
	Directions debugGhostDirection = Directions::RIGHT;

	void Start();
	void Run();

private:
	Game() {};
	~Game() {};
};