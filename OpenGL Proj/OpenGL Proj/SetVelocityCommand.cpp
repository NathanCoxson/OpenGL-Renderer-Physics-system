#include "SetVelocityCommand.h"
#include "Entity.h"
#include "SystemManager.h"


SetVelocityCommand::SetVelocityCommand(float magnitude_init, glm::vec3 values, bool linear_init, bool relative_init)
{
	magnitude = magnitude_init;
	velocity_value = values;
	linear = linear_init;
	relative = relative_init;
}

void SetVelocityCommand::Execute(Entity* entity)
{
	glm::vec3 entity_rotation = (*entity->system->buffer->render_buffer)[entity]->rotation;

	glm::vec3 direction = glm::rotate(velocity_value, glm::radians(entity_rotation.x), glm::vec3(1, 0, 0));
	direction = glm::rotate(direction, glm::radians(entity_rotation.y), glm::vec3(0, 1, 0));
	direction = glm::rotate(direction, glm::radians(entity_rotation.z), glm::vec3(0, 0, 1));

	glm::vec3 result_velocity = magnitude * direction;

	if (relative)
	{
		if (linear) ((MotionComponent*)((*entity)[eMotion]))->linear_velocity = result_velocity;
		else ((MotionComponent*)((*entity)[eMotion]))->rotational_velocity = result_velocity;
	}
	else
	{
		if (linear) ((MotionComponent*)((*entity)[eMotion]))->linear_velocity = magnitude * velocity_value;
		else ((MotionComponent*)((*entity)[eMotion]))->rotational_velocity = magnitude * velocity_value;
	}

}