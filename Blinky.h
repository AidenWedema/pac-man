#pragma once
#include "Ghost.h"

class Blinky : public Ghost
{
public:
	Blinky(Vector2 position);
	~Blinky();

private:
	void CalculateTarget() override;
};