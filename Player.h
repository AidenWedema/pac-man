#pragma once
#include "Object.h"
#include "AnimationController.hpp"
#include "Maze.h"
#include "Scoreboard.hpp"

class Player : public Object
{
public:
	Player();
	~Player() {};	
	
	void Update() override;
	void Draw(sf::RenderTarget& target) override;

	float speed;
	Vector2 moveTarget;
	Directions bufferDirection;
	int palletCount;
	int lives;

	void Move();
	void Eat(Maze::Node* node);
	void Draw();

	AnimationController animations;
};

