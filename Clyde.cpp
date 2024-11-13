#include "Clyde.h"
#include "Game.h"

Clyde::Clyde()
{
}

Clyde::~Clyde()
{
}

void Clyde::CalculateTarget()
{
	if (position.Distance(target) < 4)
		target = scatterTarget;
	else
		target = Game::GetInstance()->debugGhostTarget; // placeholder, change to pacman position later
}
