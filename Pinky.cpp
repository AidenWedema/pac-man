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
	target = Game::GetInstance()->debugGhostTarget + Game::GetInstance()->debugGhostDirection * 4; // placeholder, change to pacman position and direction later
}
