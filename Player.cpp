#include "Player.h"
#include "Maze.h"


Player::Player() 
{
	speed = 1.0f;
	direction = RIGHT;
	
	position = Vector2(13, 14);
	moveTarget = Vector2(14,14);
	x = position.x * Maze::GetInstance()->GetResolution();
	y = position.y * Maze::GetInstance()->GetResolution();
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
	
	Move();
}

void Player::Move()
{
	// Check if the player is at the current target
	float resolution = Maze::GetInstance()->GetResolution();

	if (x == moveTarget.x * resolution && y == moveTarget.y * resolution) {
		position = moveTarget;  // Update tile position
		Maze::Node* node = Maze::GetInstance()->GetNode(position);

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
	sf::RectangleShape rect;
	rect.setPosition(position.x * Maze::GetInstance()->GetResolution(), position.y * Maze::GetInstance()->GetResolution());
	rect.setSize(sf::Vector2f(Maze::GetInstance()->GetResolution() * 0.8f, Maze::GetInstance()->GetResolution() * 0.8f));
	rect.setFillColor(sf::Color::Yellow);
	target.draw(rect);
}
