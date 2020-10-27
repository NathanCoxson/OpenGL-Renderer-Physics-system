#pragma once
#include "Command.h"
class ToggleCullOrientationCommand : public Command
{
public:
	bool toggled = false;

	ToggleCullOrientationCommand(bool _toggle = true);

	void Execute(Entity* entity);
};

