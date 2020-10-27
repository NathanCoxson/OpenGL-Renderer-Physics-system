#pragma once
#include "Command.h"
#include "MotionComponent.h"

class SetVelocityCommand : public Command
{
public:

	
	glm::vec3 velocity_value;
	bool linear;
	bool relative;
	float magnitude;

	SetVelocityCommand(float magnitude_init, glm::vec3 values = glm::vec3(0.0f), bool linear = true, bool relative_init = true);

	void Execute(Entity* entity);
};

