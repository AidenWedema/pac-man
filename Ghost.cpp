#include "Ghost.h"
#include "Game.h"

void Ghost::Update()
{
#ifdef _DEBUG // Draw path to target
	sf::CircleShape circle;
	circle.setPosition(target.x, target.y);
	circle.setRadius(5);
	circle.setFillColor(sf::Color::White);
	Game::GetInstance()->window.draw(circle);
#endif // _DEBUG

	switch (state)
	{
	case CHASE:
		Chase();
		break;
	case SCATTER:
		Scatter();
		break;
	case FRIGHTEND:
		Frightend();
		break;
	case EATEN:
		Eaten();
		break;
	}
}

void Ghost::Draw(sf::RenderTarget& target)
{
}

void Ghost::Chase()
{
	CalculateTarget();
	Move();
}

void Ghost::Scatter()
{
	target = scatterTarget;
	Move();
}

void Ghost::Frightend()
{
	RandomMove();
}

void Ghost::Eaten()
{
	target = Vector2(24, 20); // placeholder
	Move();
}

void Ghost::Move()
{
}

void Ghost::RandomMove()
{
}

std::vector<Directions> Ghost::GetMoveableDirections()
{
	/*
	Get the maze
	look at the tiles 90 degrees left of the ghost in front of the ghost and 90 degrees right of the ghost
	if the tile is a wall, the direction is not moveable
	return the moveable direction(s)
	*/
	return {Directions::UP};
}