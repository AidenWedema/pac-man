#include "Blinky.h"
#include "Game.h"

Blinky::Blinky()
{
}

Blinky::~Blinky()
{
}

void Blinky::CalculateTarget()
{
	target = Game::GetInstance()->debugGhostTarget; // placeholder, change to pacman position later
}
