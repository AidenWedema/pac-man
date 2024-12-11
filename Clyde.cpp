#include "Clyde.h"
#include "Game.h"

Clyde::Clyde(Vector2 pos) : Ghost(pos)
{
	color = sf::Color::Color(255, 183, 81, 255);

	scatterTarget = Vector2(0, 33); // placeholder

	animationController.loadAnimation("assets/sprites/clyde/UP", "UP", 10);
	animationController.loadAnimation("assets/sprites/clyde/DOWN", "DOWN", 10);
	animationController.loadAnimation("assets/sprites/clyde/LEFT", "LEFT", 10);
	animationController.loadAnimation("assets/sprites/clyde/RIGHT", "RIGHT", 10);

	state = HOME;
	homeTimer = 720; // 12 seconds
	direction = LEFT;
	animationController.setAnimation("UP");
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
#ifdef _DEBUG
	sf::Color c = color;
	c.a = 128;
	sf::CircleShape radius;
	float r = 4 * Maze::GetInstance()->GetResolution();
	radius.setPosition(Game::GetInstance()->debugGhostTarget.x * Maze::GetInstance()->GetResolution() - r / 2, Game::GetInstance()->debugGhostTarget.y * Maze::GetInstance()->GetResolution() - r / 2);
	radius.setRadius(r);
	radius.setFillColor(sf::Color::Transparent);
	radius.setOutlineColor(c);
	radius.setOutlineThickness(2);
	Game::GetInstance()->window.draw(radius);
#endif // _DEBUG
}
