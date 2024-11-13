#pragma once
#include "Vector2.hpp"

class Game
{
public:
	static Game* instance;
	static Game* GetInstance();

	Vector2 debugGhostTarget = Vector2(28, 24);
	Directions debugGhostDirection = Directions::RIGHT;

private:
	Game() {};
	~Game() {};
};