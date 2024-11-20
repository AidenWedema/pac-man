#include "Ghost.h"
#include "Game.h"

Ghost::Ghost()
{
	state = CHASE;
	speed = 1.0f;
	moveTarget = Vector2(14, 14); // placeholder
}

void Ghost::Update()
{
#ifdef _DEBUG // Draw path to target
	//DrawGhostPath();
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

void Ghost::SetState(States s)
{
	switch (s)
	{
	case CHASE:
		direction = (Directions)((direction + 2) % 4);
		break;
	case SCATTER:
		direction = (Directions)((direction + 2) % 4);
		break;
	case FRIGHTEND:
		direction = (Directions)((direction + 2) % 4);
		break;
	default:
		break;
	}

	state = s;
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
	if (position == target)
		SetState(CHASE);
}

void Ghost::Move()
{
	Vector2 dir = Vector2::DirectionToVector(direction);
	x = x + dir.x * speed;
	y = y + dir.y * speed;

	if (x / Maze::GetInstance()->GetResolution() == moveTarget.x && y / Maze::GetInstance()->GetResolution() == moveTarget.y)
	{
		position = moveTarget;
		direction = GetShortestDirection();
		Maze::Node* node = Maze::GetInstance()->GetNode(position);
		moveTarget = node->connections[direction]->position;
		if (node->warp)
		{
			position = node->connections[direction]->position;
			moveTarget = node->connections[direction]->connections[direction]->position;
			x = position.x * Maze::GetInstance()->GetResolution();
			y = position.y * Maze::GetInstance()->GetResolution();
		}
	}
}

void Ghost::RandomMove()
{
	Vector2 dir = Vector2::DirectionToVector(direction);
	x = x + dir.x * speed;
	y = y + dir.y * speed;

	if (x / Maze::GetInstance()->GetResolution() == moveTarget.x && y / Maze::GetInstance()->GetResolution() == moveTarget.y)
	{
		position = moveTarget;
		std::vector<Directions> directions = GetMoveableDirections();
		int rng = RNG::GetRandom() % directions.size();
		direction = directions[rng];
		Maze::Node* node = Maze::GetInstance()->GetNode(position);
		moveTarget = node->connections[direction]->position;
		if (node->warp)
		{
			position = node->connections[direction]->position;
			moveTarget = node->connections[direction]->connections[direction]->position;
			x = position.x * Maze::GetInstance()->GetResolution();
			y = position.y * Maze::GetInstance()->GetResolution();
		}
	}
}

std::vector<Directions> Ghost::GetMoveableDirections()
{
	std::vector<Directions> directions = {UP, DOWN, LEFT, RIGHT};

	// Remove the direction opposite of the current direction. I.E. prevent the ghost from turning around 180 degrees
	Directions opposite = (Directions)((direction + 2) % 4);
	directions.erase(std::find(directions.begin(), directions.end(), opposite));

	Maze::Node* node = Maze::GetInstance()->GetNode(position);
	std::vector<Directions> forbidden;
	for (Directions d : directions)
	{
		if (node->connections.find(d) == node->connections.end())
			forbidden.push_back(d);
	}
	for (Directions d : forbidden)
		directions.erase(std::find(directions.begin(), directions.end(), d));

	return directions;
}

Directions Ghost::GetShortestDirection()
{
	std::vector<Directions> directions = GetMoveableDirections();
	float shortestDistance = FLT_MAX;
	Directions dir = direction;
	for (Directions d : directions)
	{
		float distance = Vector2::Distance(position + d, target);
		if (distance < shortestDistance)
		{
			shortestDistance = distance;
			dir = d;
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
	Vector2 curMov = moveTarget;
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
	moveTarget = curMov;
	direction = curDir;
	x = curX;
	y = curY;
}
