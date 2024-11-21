#include "Inky.h"
#include "Game.h"

Inky::Inky(Blinky* blinky)
{
	this->blinky = blinky;

	texture = sf::Texture();
	if (!texture.loadFromFile("assets/sprites/inky/0.png"))
	{
		std::cout << "Failed to load the sprite" << std::endl;
		return;
	}
	sprite.setTexture(texture);
	sprite.setScale(Maze::GetInstance()->GetResolution() / texture.getSize().x, Maze::GetInstance()->GetResolution() / texture.getSize().y);
	sprite.setPosition(x, y);

	color = sf::Color::Color(0, 255, 255, 255);

	scatterTarget = Vector2(27, 30); // placeholder
}

Inky::~Inky()
{
}

void Inky::CalculateTarget()
{
	Vector2 dir = Game::GetInstance()->debugGhostDirection;
	Vector2 pivot = Game::GetInstance()->debugGhostTarget + dir * 2; // placeholder, change to pacman position and direction later
	target = blinky->GetPosition().RotateAround(pivot, 180);
#ifdef _DEBUG
	sf::Color c = sf::Color::Color(255, 255, 255, 128);
	sf::VertexArray line1(sf::LinesStrip, 2);
	sf::VertexArray line2(sf::LinesStrip, 2);
	sf::CircleShape p;
	// make line1 from blinky to pivot
	line1[0] = sf::Vertex(sf::Vector2f(blinky->GetPosition().x * Maze::GetInstance()->GetResolution(), blinky->GetPosition().y * Maze::GetInstance()->GetResolution()), c);
	line1[1] = sf::Vertex(sf::Vector2f(pivot.x * Maze::GetInstance()->GetResolution(), pivot.y * Maze::GetInstance()->GetResolution()), c);
	// make line2 from pivot to target
	line2[0] = sf::Vertex(sf::Vector2f(pivot.x * Maze::GetInstance()->GetResolution(), pivot.y * Maze::GetInstance()->GetResolution()), c);
	line2[1] = sf::Vertex(sf::Vector2f(target.x * Maze::GetInstance()->GetResolution(), target.y * Maze::GetInstance()->GetResolution()), c);
	// make circle at pivot
	p.setPosition(pivot.x * Maze::GetInstance()->GetResolution(), pivot.y * Maze::GetInstance()->GetResolution());
	p.setRadius(4);
	p.setFillColor(c);
	// draw everything
	Game::GetInstance()->window.draw(p);
	Game::GetInstance()->window.draw(line1);
	Game::GetInstance()->window.draw(line2);
#endif // _DEBUG
}
