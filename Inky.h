#pragma once
#include "Ghost.h"
#include "Blinky.h"

class Inky : public Ghost
{
public:
	Inky(Vector2 position, Blinky* blinky);
	~Inky();

private:
	Blinky* blinky;
	void CalculateTarget() override;
};