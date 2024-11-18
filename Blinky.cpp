#include "Blinky.h"
#include "Game.h"

Blinky::Blinky()
{
	texture = sf::Texture();
	if (!texture.loadFromFile("assets/sprites/blinky/0.png"))
	{
		std::cout << "Failed to load the sprite" << std::endl;
		return;
	}
	sprite.setTexture(texture);
	sprite.setScale(Maze::GetInstance()->GetResolution() / texture.getSize().x, Maze::GetInstance()->GetResolution() / texture.getSize().y);
	sprite.setPosition(x, y);

	color = sf::Color::Color(255, 0, 0, 255);
}

Blinky::~Blinky()
{
}

void Blinky::CalculateTarget()
{
	target = Game::GetInstance()->debugGhostTarget; // placeholder, change to pacman position later
}
