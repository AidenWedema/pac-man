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
	Vector2 dir = Game::GetInstance()->debugGhostDirection;
	Vector2 pivot = Game::GetInstance()->debugGhostTarget + dir * 2; // placeholder, change to pacman position and direction later
	target = blinky->GetPosition().RotateAround(pivot, 180);
}
