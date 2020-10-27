#pragma once
#include "Command.h"
#include "MotionComponent.h"

class ForceCommand : public Command
{
public:
	glm::vec3 force_value;
	glm::vec3 pivot_value;
	float time;
	bool constant;
	bool linear;
	bool relative;

	ForceCommand(glm::vec3 force_init, float time_remaining_init = -1, bool constant_init = false, bool relation = true);
	ForceCommand(glm::vec3 force_init, glm::vec3 pivot_init, float time_remaining_init = -1, bool constant_init = false, bool relation = false);

	void Execute(Entity* entity);
};

