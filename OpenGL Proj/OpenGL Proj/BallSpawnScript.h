#pragma once
#include "Script.h"
class BallSpawnScript : public Script
{
public:

	bool set = false;
	float delay = 1.f;
	float time = delay;

	BallSpawnScript();
	~BallSpawnScript();

	void Update(Entity* parent);

};

