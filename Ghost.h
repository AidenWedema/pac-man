#pragma once
#include <vector>
#include "Object.h"
class Ghost : public Object
{
public:
	Ghost() { state = CHASE; };
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

	void SetState(States state) { this->state = state; }
	States GetState() { return state; }

	void SetTarget(Vector2 target) { this->target = target; }
	Vector2 GetTarget() { return target; }

protected:
	States state;
	Vector2 target;
	const Vector2 scatterTarget;
	float speed;

	virtual void CalculateTarget() {};

	void Chase();
	void Scatter();
	void Frightend();
	void Eaten();
	void Move();
	void RandomMove();
	std::vector<Directions> GetMoveableDirections();
};