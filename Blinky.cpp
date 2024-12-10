#include "Blinky.h"
#include "Game.h"

Blinky::Blinky(Vector2 pos) : Ghost(pos)
{
	color = sf::Color::Color(255, 0, 0, 255);

	scatterTarget = Vector2(27, 3); // placeholder

	animationController.loadAnimation("assets/sprites/blinky/UP", "UP", 10);
	animationController.loadAnimation("assets/sprites/blinky/DOWN", "DOWN", 10);
	animationController.loadAnimation("assets/sprites/blinky/LEFT", "LEFT", 10);
	animationController.loadAnimation("assets/sprites/blinky/RIGHT", "RIGHT", 10);
}

Blinky::~Blinky()
{
}

void Blinky::CalculateTarget()
{
	target = Game::GetInstance()->debugGhostTarget; // placeholder, change to pacman position later
}
