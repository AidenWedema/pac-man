#pragma once
#include "Object.h"
#include "AnimationController.hpp"
#include "Maze.h"


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

	void Eat(Maze::Node*);
	int palletCount;

	void Move();
	void Draw();

	AnimationController animations;
};

