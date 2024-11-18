#include "Ghost.h"
#include "Game.h"

void Ghost::Update()
{
#ifdef _DEBUG // Draw path to target
	DrawGhostPath();
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
	sprite.setPosition(x, y);
	target.draw(sprite);

	sf::RectangleShape rect;
	rect.setPosition(position.x * Maze::GetInstance()->GetResolution(), position.y * Maze::GetInstance()->GetResolution());
	rect.setSize(sf::Vector2f(Maze::GetInstance()->GetResolution() * 0.8f, Maze::GetInstance()->GetResolution() * 0.8f));
	rect.setFillColor(color);
	target.draw(rect);
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
	if (x % Maze::GetInstance()->GetResolution() == 0 && y % Maze::GetInstance()->GetResolution() == 0)
		direction = GetShortestDirection();

	// move the ghost
	Vector2 dir = Vector2::DirectionToVector(direction);
	x += dir.x * speed;
	y += dir.y * speed;

	// update the ghost's position
	position.x = static_cast<int>(x / Maze::GetInstance()->GetResolution());
	position.y = static_cast<int>(y / Maze::GetInstance()->GetResolution());
}

void Ghost::RandomMove()
{
}

std::vector<Directions> Ghost::GetMoveableDirections()
{
	std::vector<Directions> moveableDirections;

	// get the directions left and right from the ghost's perspective
	Directions r = (Directions)(direction - 1 == -1 ? 3 : direction - 1);
	Directions l = (Directions)((direction + 1) % 4);
	// get the tile in front, left and right of the ghost
	int front = Maze::GetInstance()->GetTileAtPosition(position + direction);
	int left = Maze::GetInstance()->GetTileAtPosition(position + l);
	int right = Maze::GetInstance()->GetTileAtPosition(position + r);

	// if the tile is 0 it is a blank tile and can be moved on
	if (front == 0)
		moveableDirections.push_back(direction);
	if (left == 0)
		moveableDirections.push_back(l);
	if (right == 0)
		moveableDirections.push_back(r);

	//sort the directions so the order is Up, Left, Down, Right
	std::sort(moveableDirections.begin(), moveableDirections.end());
	return moveableDirections;
}

Directions Ghost::GetShortestDirection()
{
	// get the direction that minimizes distance to the target
	Directions dir = direction;
	std::vector<Directions> moveableDirections = GetMoveableDirections();
	float shortestdistance = FLT_MAX;
	for (int i = 0; i < moveableDirections.size(); i++)
	{
		Vector2 point = position + moveableDirections[i];
		float distance = point.Distance(target);
		if (distance < shortestdistance)
		{
			shortestdistance = distance;
			dir = moveableDirections[i];
		}
	}
	return dir;
}

void Ghost::DrawGhostPath()
{
	sf::CircleShape circle;
	sf::Color c = color;
	c.a = 128;

	// Mark the target with a circle
	circle.setPosition(target.x * Maze::GetInstance()->GetResolution(), target.y * Maze::GetInstance()->GetResolution());
	circle.setRadius(4);
	circle.setFillColor(c);
	Game::GetInstance()->window.draw(circle);

	std::vector<Vector2> path;

	// Use save the current position and direction variables
	Vector2 curPos = position;
	Directions curDir = direction;
	int curX = x;
	int curY = y;

	for (int i = 0; i < 1000; i++)
	{
		switch (state)
		{
		case CHASE:
			Chase();
			break;
		case SCATTER:
			Scatter();
			break;
		case FRIGHTEND:
			return;
		case EATEN:
			Eaten();
			break;
		}

		path.push_back(position);

		if (position == target)
			break;
	}

	// Draw the simulated path
	sf::VertexArray line(sf::LineStrip, path.size());
	for (int i = 0; i < path.size(); i++)
	{
		int res = Maze::GetInstance()->GetResolution();
		line[i] = sf::Vertex(sf::Vector2f(path[i].x * res, path[i].y * res), c);
	}
	Game::GetInstance()->window.draw(line);

	// Restore the current position and direction
	position = curPos;
	direction = curDir;
	x = curX;
	y = curY;
}
