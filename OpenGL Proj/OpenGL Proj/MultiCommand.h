#pragma once
#include "Command.h"
#include "MotionComponent.h"

class MultiCommand : public Command
{
public:
	std::vector<Command*> commands;

	MultiCommand(std::vector<Command*> &_commands);
	~MultiCommand();

	void Execute(Entity* entity);
};

