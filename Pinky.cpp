#include "Pinky.h"
#include "Game.h"

Pinky::Pinky(Vector2 pos) : Ghost(pos)
{
	color = sf::Color::Color(255, 183, 255, 255);

	scatterTarget = Vector2(0, 3); // placeholder

	animationController.loadAnimation("assets/sprites/pinky/UP", "UP", 10);
	animationController.loadAnimation("assets/sprites/pinky/DOWN", "DOWN", 10);
	animationController.loadAnimation("assets/sprites/pinky/LEFT", "LEFT", 10);
	animationController.loadAnimation("assets/sprites/pinky/RIGHT", "RIGHT", 10);

	state = HOME;
	homeTimer = 120; // 2 seconds
	direction = UP;
}

Pinky::~Pinky()
{
}

void Pinky::CalculateTarget()
{
	Vector2 dir = Game::GetInstance()->debugGhostDirection;
	target = Game::GetInstance()->debugGhostTarget + dir * 4; // placeholder, change to pacman position and direction later
}
