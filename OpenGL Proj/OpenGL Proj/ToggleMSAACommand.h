#pragma once
#include "Command.h"
class ToggleMSAACommand : public Command
{
public:
	bool toggled = false;

	ToggleMSAACommand(bool _toggle = true);

	void Execute(Entity* entity);
};

