#include "Pinky.h"
#include "Game.h"

Pinky::Pinky()
{
	texture = sf::Texture();
	if (!texture.loadFromFile("assets/sprites/pinky/0.png"))
	{
		std::cout << "Failed to load the sprite" << std::endl;
		return;
	}
	sprite.setTexture(texture);
	sprite.setScale(Maze::GetInstance()->GetResolution() / texture.getSize().x, Maze::GetInstance()->GetResolution() / texture.getSize().y);
	sprite.setPosition(x, y);

	color = sf::Color::Color(255, 183, 255, 255);

	scatterTarget = Vector2(0, 3); // placeholder
}

Pinky::~Pinky()
{
}

void Pinky::CalculateTarget()
{
	Vector2 dir = Game::GetInstance()->debugGhostDirection;
	target = Game::GetInstance()->debugGhostTarget + dir * 4; // placeholder, change to pacman position and direction later
}
