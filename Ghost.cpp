#include "Ghost.h"
#include "Game.h"

Ghost::Ghost(Vector2 pos)
{
	homeTimer = 0;
	sprite = nullptr;
	state = SCATTER;
	speed = 1.0f;
	position = pos;
	x = position.x * Maze::GetInstance()->GetResolution();
	y = position.y * Maze::GetInstance()->GetResolution();
	moveTarget = position + direction;

	animationController.loadAnimation("assets/sprites/ghost_eaten/UP", "eaten_UP", 0);
	animationController.loadAnimation("assets/sprites/ghost_eaten/DOWN", "eaten_DOWN", 0);
	animationController.loadAnimation("assets/sprites/ghost_eaten/LEFT", "eaten_LEFT", 0);
	animationController.loadAnimation("assets/sprites/ghost_eaten/RIGHT", "eaten_RIGHT", 0);
	animationController.loadAnimation("assets/sprites/ghost_scared", "scared", 10);
}

void Ghost::Update()
{
#ifdef _DEBUG // Draw path to target
	//DrawGhostPath();
#endif // _DEBUG

	SetAnimation();

	switch (state)
	{
	case HOME:
		homeTimer--;
		if (homeTimer > 0)
			break;
		if (homeTimer == 0)
			moveTarget = position + direction;
		target = Maze::GetInstance()->GetHouse()->position;
		if (position == target + Vector2(0, 2))
		{
			direction = UP;
			moveTarget = target;
		}
		if (position == target)
			state = CHASE;
		Move();
		break;
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
	sprite = animationController.getSprite();

	int resolution = Maze::GetInstance()->GetResolution();
	float scale = resolution / 8;
	if (scale < 1.0f)
		scale = 1.0f;
	sprite->setScale(scale, scale);
	sprite->setOrigin(sprite->getLocalBounds().width / 4, sprite->getLocalBounds().height / 4);
	sprite->setPosition(x, y);
	target.draw(*sprite);

#ifdef _DEBUG
	sf::RectangleShape rect;
	rect.setPosition(position.x * Maze::GetInstance()->GetResolution(), position.y * Maze::GetInstance()->GetResolution());
	rect.setSize(sf::Vector2f(Maze::GetInstance()->GetResolution() * 0.8f, Maze::GetInstance()->GetResolution() * 0.8f));
	rect.setFillColor(color);
	target.draw(rect);
#endif // _DEBUG
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
	case EATEN:
		break;
	default:
		break;
	}

	moveTarget = position;
	state = s;
}

void Ghost::Chase()
{
	CalculateTarget();
	Move();
	CheckPacmanDistance();
}

void Ghost::Scatter()
{
	target = scatterTarget;
	Move();
	CheckPacmanDistance();
}

void Ghost::Frightend()
{
	RandomMove();
	CheckPacmanDistance();
}

void Ghost::Eaten()
{
	target = Vector2(14, 14); // placeholder
	Move();/*
	if (position == target)
		SetState(CHASE);*/
}

void Ghost::Move()
{
	int res = Maze::GetInstance()->GetResolution();

	Vector2 dir = direction;
	float spd = res / 8;
	x += dir.x * speed * spd;
	y += dir.y * speed * spd;

	if (moveTarget.x * res == x && moveTarget.y * res == y)
	{
		// Toggle between scatter and chase
		if (Time::GetInstance()->frameCount % 1200 > 420 && state == SCATTER)
			SetState(CHASE);
		else if (Time::GetInstance()->frameCount % 1200 < 420 && state == CHASE)
			SetState(SCATTER);

		position = moveTarget;
		direction = GetShortestDirection();
		Maze::Node* node = Maze::GetInstance()->GetNode(position);
		moveTarget = node->connections[direction]->position;
		if (node->warp)
		{
			position = node->connections[direction]->position;
			moveTarget = node->connections[direction]->connections[direction]->position;
			x = position.x * res;
			y = position.y * res;
		}
		node = nullptr;
		delete node;
	}
}

void Ghost::RandomMove()
{
	int res = Maze::GetInstance()->GetResolution();

	Vector2 dir = direction;
	float spd = res / 8;
	x += dir.x * speed * spd;
	y += dir.y * speed * spd;

	if (moveTarget.x * res == x && moveTarget.y * res == y)
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
			x = position.x * res;
			y = position.y * res;
		}
		node = nullptr;
		delete node;
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
	
	node = nullptr;
	delete node;
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

void Ghost::CheckPacmanDistance()
{
	// Check if pacman is close to the ghost via tile positions
	Player* pacman = Game::GetInstance()->GetPacman();
	Vector2 pacmanPosition = pacman->GetPosition();
	float distance = position.Distance(pacmanPosition);
	if (distance >= 1)
		return;
	// Check if pacman is close to the ghost via pixel positions
	int res = Maze::GetInstance()->GetResolution();
	int pacmanX = pacman->GetX();
	int pacmanY = pacman->GetY();
	res /= 1.5f;
	if (abs(x - pacmanX) > res || abs(y - pacmanY) > res)
		return;
	// If the ghost is frightened, pacman eats the ghost. Otherwise, pacman dies
	if (state == FRIGHTEND)
		SetState(EATEN);
	else
		Game::GetInstance()->GameOver();
}

void Ghost::SetAnimation()
{
	if (animationController.getCurrent() == nullptr)
		animationController.setAnimation("scared");

	animationController.Update();

	std::string dirStr = Vector2::DirectionToString(direction);
	switch (state)
	{
	case Ghost::CHASE:
	case Ghost::SCATTER:
		if (animationController.getCurrent()->name != dirStr)
			animationController.setAnimation(dirStr);
		break;
	case Ghost::FRIGHTEND:
		if (animationController.getCurrent()->name != "scared")
			animationController.setAnimation("scared");
		break;
	case Ghost::EATEN:
		if (animationController.getCurrent()->name != "eaten_" + dirStr)
			animationController.setAnimation("eaten_" + dirStr);
		break;
	default:
		break;
	}
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

		// check if position is already in the path
		if (std::find(path.begin(), path.end(), position) != path.end())
			continue;

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
