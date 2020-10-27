#pragma once
#include "Command.h"
#include "MotionComponent.h"

class SetMainCameraCommand : public Command
{
public:

	Entity* camera;

	SetMainCameraCommand(Entity* camera_pointer);

	void Execute(Entity* entity);
	
};

