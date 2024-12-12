#pragma once
#include <iostream>
class Time
{
public:
	~Time() {};
	static Time* instance;
	static Time* GetInstance() {
		if (instance == nullptr)
			instance = new Time();
		return instance;
	}

	uint16_t fps;
	float minDelta;
	float deltaTime;
	uint32_t frameCount;

private:
	Time() {
		fps = 60;
		minDelta = 1 / fps;
		deltaTime = 0;
		frameCount = 0;
	};
};