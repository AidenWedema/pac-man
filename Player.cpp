#include "Player.h"

Player::Player() 
{
	speed = 1.0f;
	direction = RIGHT;
	bufferDirection = RIGHT;
	
	position = Maze::GetInstance()->GetSpawn()->position;
	moveTarget = position + direction;
	x = position.x * Maze::GetInstance()->GetResolution();
	y = position.y * Maze::GetInstance()->GetResolution();

	animations.loadAnimation("assets/test animations/Die", "die", 12);
	animations.loadAnimation("assets/test animations/UP", "UP", 12, true);
	animations.loadAnimation("assets/test animations/DOWN", "DOWN", 12, true);
	animations.loadAnimation("assets/test animations/LEFT", "LEFT", 12, true);
	animations.loadAnimation("assets/test animations/RIGHT", "RIGHT", 12, true);

	animations.setAnimation("RIGHT");
}

void Player::Update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		bufferDirection = LEFT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		bufferDirection = RIGHT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		bufferDirection = UP;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		bufferDirection = DOWN;
	}

	animations.Update();

	std::string dirStr = Vector2::DirectionToString(direction);
	if (animations.getCurrent()->name != dirStr)
		animations.setAnimationNoReset(dirStr);
	
	Move();
}

void Player::Move()
{
	// Check if the player is at the current target
	float resolution = Maze::GetInstance()->GetResolution();

	if (x == moveTarget.x * resolution && y == moveTarget.y * resolution) {
		position = moveTarget;  // Update tile position
		Maze::Node* node = Maze::GetInstance()->GetNode(position);
		if (node->pellet)
			Eat(node);

		if (node->connections.find(bufferDirection) != node->connections.end())
			direction = bufferDirection;
		// Check if we can move in the current direction
		if (node->connections.find(direction) != node->connections.end()) {
			moveTarget = node->connections[direction]->position;  // Set new target

			// Handle warp cases
			if (node->warp) {
				position = moveTarget;  // Warp immediately
				moveTarget = node->connections[direction]->connections[direction]->position;
				x = position.x * resolution;
				y = position.y * resolution;
			}
		}
		else {
			// If we can't move, stay on the current tile
			node = nullptr;
			delete node; // deletes nodes from maze, making 
			return;
		}
		node = nullptr;
		delete node;
	}

	// Move towards the target
	Vector2 dirVec = direction;
	float spd = resolution / 8;
	x += dirVec.x * speed * spd;
	y += dirVec.y * speed * spd;
}

void Player::Eat(Maze::Node* node)
{
	node->pellet = false;
	palletCount++;
	Scoreboard::GetInstance()->AddScore(10);
	if (node->super) 
	{
		Scoreboard::GetInstance()->AddScore(40);
	}

	// Update the tile to a blank tile
	Maze::GetInstance()->UpdateTile(node->position, 0);

	if (palletCount == Maze::GetInstance()->GetPelletCounter())
	{
		std::cout << "YOU WIN" << std::endl; // placeholder
	}
}

void Player::Draw(sf::RenderTarget& target)
{

	sf::Sprite* sprite = animations.getSprite();
	int resolution = Maze::GetInstance()->GetResolution();
	float scale = resolution / 8;
	if (scale < 1.0f)
		scale = 1.0f;
	sprite->setScale(scale, scale);
	sprite->setPosition(x, y);
	sprite->setOrigin(sprite->getLocalBounds().width / 4, sprite->getLocalBounds().height / 4);
	target.draw(*sprite);

	sprite = nullptr;
	delete sprite;
#ifdef _DEBUG
	sf::RectangleShape rect;
	rect.setPosition(position.x * Maze::GetInstance()->GetResolution(), position.y * Maze::GetInstance()->GetResolution());
	rect.setSize(sf::Vector2f(Maze::GetInstance()->GetResolution() * 0.8f, Maze::GetInstance()->GetResolution() * 0.8f));
	rect.setFillColor(sf::Color::Yellow);
	target.draw(rect);
#endif // _DEBUG
}
