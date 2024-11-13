#pragma once
#include "Ghost.h"

class Blinky : public Ghost
{
public:
	Blinky();
	~Blinky();

private:
	void CalculateTarget() override;
};