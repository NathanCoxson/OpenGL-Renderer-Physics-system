#include "SetEntityVelocityCommand.h"
#include "Entity.h"
#include "SystemManager.h"


SetEntityVelocityCommand::SetEntityVelocityCommand(Entity* entity, float magnitude_init, glm::vec3 values, bool linear_init, bool relative_init)
{
	set = entity;
	magnitude = magnitude_init;
	velocity_value = values;
	linear = linear_init;
	relative = relative_init;
}

void SetEntityVelocityCommand::Execute(Entity* entity)
{
	glm::vec3 entity_rotation = (*set->system->buffer->render_buffer)[set]->rotation;

	glm::vec3 direction = glm::rotate(velocity_value, glm::radians(entity_rotation.x), glm::vec3(1, 0, 0));
	direction = glm::rotate(direction, glm::radians(entity_rotation.y), glm::vec3(0, 1, 0));
	direction = glm::rotate(direction, glm::radians(entity_rotation.z), glm::vec3(0, 0, 1));

	glm::vec3 result_velocity = magnitude * direction;

	if (relative)
	{
		if (linear) ((MotionComponent*)((*set)[eMotion]))->linear_velocity = result_velocity;
		else ((MotionComponent*)((*set)[eMotion]))->rotational_velocity = result_velocity;
	}
	else
	{
		if (linear) ((MotionComponent*)((*set)[eMotion]))->linear_velocity = magnitude * velocity_value;
		else ((MotionComponent*)((*set)[eMotion]))->rotational_velocity = magnitude * velocity_value;
	}

}