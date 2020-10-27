#pragma once
#include "Command.h"
#include "MotionComponent.h"

class SetInputEntityCommand : public Command
{
public:

	Entity* input_entity;

	SetInputEntityCommand(Entity* _entity);
	~SetInputEntityCommand();

	void Execute(Entity* entity);
};

