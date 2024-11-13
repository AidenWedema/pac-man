#include "Inky.h"
#include "Game.h"

Inky::Inky(Blinky* blinky)
{
	this->blinky = blinky;
}

Inky::~Inky()
{
}

void Inky::CalculateTarget()
{
	Vector2 pivot = Game::GetInstance()->debugGhostTarget + Game::GetInstance()->debugGhostDirection * 2; // placeholder, change to pacman position and direction later
	target = blinky->GetPosition().RotateAround(pivot, 180);
}
