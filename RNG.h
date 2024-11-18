#pragma once
class RNG
{
public:
	static int seed;
	static int GetRandom()
	{
		seed = (seed * 5 + 1) % 8192;
		return seed;
	}
};