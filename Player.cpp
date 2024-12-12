#include "Player.h"


Player::Player() 
{
	speed = 1.0f;
	direction = RIGHT;
	
	position = Vector2(13, 14);
	moveTarget = Vector2(14,14);
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		bufferDirection = LEFT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		bufferDirection = RIGHT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		bufferDirection = UP;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		bufferDirection = DOWN;
	}

	animations.Update();

	std::string dirStr = Vector2::DirectionToString(direction);
	if (animations.getCurrent()->name != dirStr)
		animations.setAnimation(dirStr);
	
	Move();
}

void Player::Eat(Maze::Node* node)
{
	node->pellet = false;
	palletCount++;
	std::cout << "You ate " << palletCount << "pellets!" << std::endl;

	if (palletCount == Maze::GetInstance()->GetPalletCounter())
	{
		std::cout << "YOU WIN" << std::endl; 
	}
}

void Player::Move()
{
	// Check if the player is at the current target
	float resolution = Maze::GetInstance()->GetResolution();

	if (x == moveTarget.x * resolution && y == moveTarget.y * resolution) {
		position = moveTarget;  // Update tile position
		Maze::Node* node = Maze::GetInstance()->GetNode(position);

		if (node->pellet)
		{
			Eat(node);
		}
		
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
			return;
		}
	}

	// Move towards the target
	Vector2 dirVec = direction;
	x += dirVec.x * speed;
	y += dirVec.y * speed;
}



void Player::Draw(sf::RenderTarget& target)
{

	sf::Sprite* sprite = animations.getSprite();
	sprite->setPosition(x, y);
	sprite->setOrigin(sprite->getLocalBounds().width / 2, sprite->getLocalBounds().height / 2);
	target.draw(*sprite);

	/*sf::RectangleShape rect;
	rect.setPosition(position.x * Maze::GetInstance()->GetResolution(), position.y * Maze::GetInstance()->GetResolution());
	rect.setSize(sf::Vector2f(Maze::GetInstance()->GetResolution() * 0.8f, Maze::GetInstance()->GetResolution() * 0.8f));
	rect.setFillColor(sf::Color::Yellow);
	target.draw(rect);*/
}
