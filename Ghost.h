#pragma once
#include <vector>
#include "Object.h"
#include "RNG.h"
#include "Maze.h"
#include "AnimationController.hpp"
#include "Time.h"
class Ghost : public Object
{
public:
	Ghost(Vector2 position);
	~Ghost() { delete sprite; };

	enum States
	{
		CHASE,
		SCATTER,
		FRIGHTEND,
		EATEN,
		HOME
	};

	void Update() override;
	void Draw(sf::RenderTarget& target) override;

	void BOO();

	void SetState(States state);
	States GetState() { return state; }

	void SetTarget(Vector2 target) { this->target = target; }
	Vector2 GetTarget() { return target; }

protected:
	States state;
	Vector2 moveTarget;		// position to move towards
	Vector2 target;			// target to get to
	Vector2 scatterTarget;	// target to get to in scatter state
	float speed;
	sf::Color color;
	sf::Sprite* sprite;
	std::string name;
	AnimationController animationController;
	int homeTimer;
	int scaredTimer;

	virtual void CalculateTarget() {};

	void Chase();
	void Scatter();
	void Frightend();
	void Eaten();
	void Move();
	void RandomMove();
	std::vector<Directions> GetMoveableDirections();
	Directions GetShortestDirection();
	void CheckPacmanDistance();
	void SetAnimation();
	void DrawGhostPath();
};