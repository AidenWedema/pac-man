#pragma once
#include "Ghost.h"
class Pinky : public Ghost
{
public:
	Pinky();
	~Pinky();

private:
	void CalculateTarget() override;
};