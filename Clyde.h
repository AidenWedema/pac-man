#pragma once
#include "Ghost.h"
class Clyde : public Ghost
{
public:
	Clyde();
	~Clyde();

private:
	void CalculateTarget() override;
};