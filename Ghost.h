#pragma once
#include <vector>
#include "Object.h"
#include "RNG.h"
class Ghost : public Object
{
public:
	Ghost() { state = CHASE; speed = 1.0f; };
	~Ghost() {};

	enum States
	{
		CHASE,
		SCATTER,
		FRIGHTEND,
		EATEN
	};

	void Update() override;
	void Draw(sf::RenderTarget& target) override;

	void SetState(States state);
	States GetState() { return state; }

	void SetTarget(Vector2 target) { this->target = target; }
	Vector2 GetTarget() { return target; }

protected:
	States state;
	Vector2 target;
	Vector2 scatterTarget;
	float speed;
	sf::Color color;
	sf::Sprite sprite;
	sf::Texture texture;

	virtual void CalculateTarget() {};

	void Chase();
	void Scatter();
	void Frightend();
	void Eaten();
	void Move();
	void RandomMove();
	std::vector<Directions> GetMoveableDirections();
	Directions GetShortestDirection();
	void DrawGhostPath();
};