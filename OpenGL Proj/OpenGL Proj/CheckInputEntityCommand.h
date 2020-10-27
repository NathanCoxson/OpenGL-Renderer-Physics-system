#pragma once
#include "Command.h"
#include "MotionComponent.h"
class CheckInputEntityCommand : public Command
{
public:

	Entity* check_entity;
	Command* stored_command;

	CheckInputEntityCommand(Entity* _check_entity, Command* _command);
	~CheckInputEntityCommand();

	void Execute(Entity* entity);
};

