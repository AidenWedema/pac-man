#include "Clyde.h"
#include "Game.h"

Clyde::Clyde()
{
	texture = sf::Texture();
	if (!texture.loadFromFile("assets/sprites/clyde/0.png"))
	{
		std::cout << "Failed to load the sprite" << std::endl;
		return;
	}
	sprite.setTexture(texture);
	sprite.setScale(Maze::GetInstance()->GetResolution() / texture.getSize().x, Maze::GetInstance()->GetResolution() / texture.getSize().y);
	sprite.setPosition(x, y);

	color = sf::Color::Color(255, 183, 81, 255);
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
