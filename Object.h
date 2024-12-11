#pragma once
#include "SFML/Graphics.hpp"
#include "Vector2.hpp"

class Object
{
public:
	Object() {direction = Directions::RIGHT; x = 0; y = 0;};
	~Object() {};

	virtual void Update() {};
	virtual void Draw(sf::RenderTarget& target) {};

	void SetPosition(Vector2 pos) { position = pos; }
	void SetPosition(int x, int y) { this->x = x; this->y = y; }
	Vector2 GetPosition() { return position; }

	void SetDirection(Directions dir) { direction = dir; }
	Directions GetDirection() { return direction; }

protected:
	Vector2 position;
	Directions direction;
	float x;
	float y;
};
