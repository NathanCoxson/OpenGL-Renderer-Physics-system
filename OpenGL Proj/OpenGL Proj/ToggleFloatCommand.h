#pragma once
#include "Command.h"
#include "MotionComponent.h"

class ToggleFloatCommand : public Command
{
public:

	float* ref;
	float val1;
	float val2;

	ToggleFloatCommand(float* _reference, float value1, float value2);
	~ToggleFloatCommand();

	void Execute(Entity* entity);
};

