#pragma once

#include "Utilities.h"

class Entity;

class Command
{
public:
	virtual ~Command() {}
	virtual void Execute(Entity* entity) = 0;
};

