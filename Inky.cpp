#include "Inky.h"
#include "Game.h"

Inky::Inky(Blinky* blinky)
{
	this->blinky = blinky;
	
	color = sf::Color::Color(0, 255, 255, 255);

	scatterTarget = Vector2(27, 30); // placeholder

	animationController.loadAnimation("assets/sprites/inky/UP", "UP", 10);
	animationController.loadAnimation("assets/sprites/inky/DOWN", "DOWN", 10);
	animationController.loadAnimation("assets/sprites/inky/LEFT", "LEFT", 10);
	animationController.loadAnimation("assets/sprites/inky/RIGHT", "RIGHT", 10);

	state = HOME;
	homeTimer = 420; // 7 seconds
	direction = UP;
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
