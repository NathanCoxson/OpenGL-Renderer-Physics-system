#pragma once

#include "Utilities.h"

class SystemManager;

class SystemPhysics
{
public:

	SystemManager* system;

	float global_dampening = 0.1f;

	bool verlet_integration = false;

	SystemPhysics(SystemManager* sys);
	~SystemPhysics();

	void Tick();

	std::thread TickThread();
};

