#pragma once
#include "Ghost.h"
class Clyde : public Ghost
{
public:
	Clyde(Vector2 position);
	~Clyde();

private:
	void CalculateTarget() override;
};