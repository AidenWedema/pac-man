#pragma once
#include "SFML/Graphics.hpp"
#include "Vector2.hpp"

class Object
{
public:
	Object() {};
	~Object() {};

	virtual void Update() {};
	virtual void Draw(sf::RenderTarget& target) {};

	void SetPosition(Vector2 pos) { position = pos; }
	Vector2 GetPosition() { return position; }

	void SetDirection(Directions dir) { direction = dir; }
	Directions GetDirection() { return direction; }

protected:
	Vector2 position;
	Directions direction;
};
