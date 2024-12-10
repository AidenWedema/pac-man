#pragma once
#include "Ghost.h"
class Pinky : public Ghost
{
public:
	Pinky(Vector2 position);
	~Pinky();

private:
	void CalculateTarget() override;
};