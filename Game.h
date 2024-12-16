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
#include "Time.h"

class Game
{
public:
	~Game() { delete pacman; };

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

	Player* GetPacman() { return pacman; }

private:
	Game() {};

	GameState gameState = MENU;
	Player* pacman;
	bool running;
};