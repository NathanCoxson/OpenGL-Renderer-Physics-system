#pragma once
#include "Command.h"
#include "MotionComponent.h"

class ToggleBoolCommand : public Command
{
public:

	bool &toggle;

	ToggleBoolCommand(bool &_toggle);
	
	void Execute(Entity* entity);
};

