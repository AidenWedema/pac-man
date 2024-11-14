#include "Pinky.h"
#include "Game.h"

Pinky::Pinky()
{
}

Pinky::~Pinky()
{
}

void Pinky::CalculateTarget()
{
	Vector2 dir = Game::GetInstance()->debugGhostDirection;
	target = Game::GetInstance()->debugGhostTarget + dir * 4; // placeholder, change to pacman position and direction later
}
