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
#include "Music.h"
#include "Player.h"

class Game
{
public:
	static Game* instance;
	static Game* GetInstance();

	enum GameState { CLOSE, MENU, GAME };

	sf::RenderWindow window;

	Vector2 debugGhostTarget = Vector2(8, 20);
	Directions debugGhostDirection = Directions::RIGHT;

	void Start();
	void Menu();
	void Run();
	void GameOver();

	void SetGameState(GameState state) { gameState = state; }
	GameState GetGameState() const { return gameState; }

private:
	Game() {};
	~Game() {};

	GameState gameState = MENU;
};