#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <chrono>
#include "Vector2.hpp"
#include "Maze.h"

class Game
{
public:
	static Game* instance;
	static Game* GetInstance();

	sf::RenderWindow window;

	Vector2 debugGhostTarget = Vector2(28, 24);
	Directions debugGhostDirection = Directions::RIGHT;

	void Start();
	void Run();

private:
	Game() {};
	~Game() {};
};