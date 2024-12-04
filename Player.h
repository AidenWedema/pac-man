#pragma once
#include "Object.h"
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

	void Move();
	void Draw();
};

