#include "ForceCommand.h"
#include "Entity.h"
#include "SystemManager.h"

ForceCommand::ForceCommand(glm::vec3 force_init, float time_remaining_init, bool constant_init, bool relation)
{
	force_value = force_init;
	time = time_remaining_init;
	constant = constant_init;
	linear = true;
	relative = relation;
}

ForceCommand::ForceCommand(glm::vec3 force_init, glm::vec3 pivot_init, float time_remaining_init, bool constant_init, bool relation)
{
	force_value = force_init;
	pivot_value = pivot_init;
	time = time_remaining_init;
	constant = constant_init;
	linear = false;
	relative = relation;
}

void ForceCommand::Execute(Entity* entity)
{
	if (relative)
	{
		glm::vec3 entity_rotation = (*entity->system->buffer->render_buffer)[entity]->rotation;

		glm::vec3 direction = glm::rotate(force_value, glm::radians(entity_rotation.x), glm::vec3(1, 0, 0));
		direction = glm::rotate(direction, glm::radians(entity_rotation.y), glm::vec3(0, 1, 0));
		glm::vec3 result_force = glm::rotate(direction, glm::radians(entity_rotation.z), glm::vec3(0, 0, 1));

		if (linear) ((MotionComponent*)((*entity)[eMotion]))->linear_forces.push_back(LinearForce(result_force, time, constant));
		else ((MotionComponent*)((*entity)[eMotion]))->rotational_forces.push_back(AngularForce(result_force, pivot_value, time, constant));
	}
	else
	{
		if (linear) ((MotionComponent*)((*entity)[eMotion]))->linear_forces.push_back(LinearForce(force_value, time, constant));
		else ((MotionComponent*)((*entity)[eMotion]))->rotational_forces.push_back(AngularForce(force_value, pivot_value, time, constant));
	}
}
