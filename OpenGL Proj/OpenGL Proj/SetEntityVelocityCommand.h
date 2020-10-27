#pragma once
#include "Command.h"
class SetEntityVelocityCommand :
	public Command
{
public:
	glm::vec3 velocity_value;
	bool linear;
	bool relative;
	float magnitude;
	Entity* set;

	SetEntityVelocityCommand(Entity* entity, float magnitude_init, glm::vec3 values = glm::vec3(0.0f), bool linear = true, bool relative_init = true);

	void Execute(Entity* entity);
};

