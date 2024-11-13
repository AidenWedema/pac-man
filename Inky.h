#pragma once
#include "Ghost.h"
#include "Blinky.h"

class Inky : public Ghost
{
public:
	Inky(Blinky* blinky);
	~Inky();

private:
	Blinky* blinky;
	void CalculateTarget() override;
};