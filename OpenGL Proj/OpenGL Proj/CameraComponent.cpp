#include "CameraComponent.h"
#include "SystemManager.h"

CameraComponent::CameraComponent(Entity* parent)
{
	entity = parent;
	up_direction = glm::vec3(0.0f, 1.0f, 0.0f);
	entity->system->GetEntityGroup(entity, eCameraEntities);
}

CameraComponent::CameraComponent(Entity* parent, glm::vec3 up, Entity* focus)
{
	entity = parent;
	up_direction = up;
	focus_entity = focus;
	entity->system->GetEntityGroup(entity, eCameraEntities);
}

CameraComponent::CameraComponent(Entity* parent, glm::vec3 up, glm::vec3 focus)
{
	entity = parent;
	up_direction = up;
	focus_position = focus;
	entity->system->GetEntityGroup(entity, eCameraEntities);
}

CameraComponent::~CameraComponent()
{
	entity->system->DeleteEntityGroup(entity, eCameraEntities);
}

glm::mat4 CameraComponent::GetCameraMatrix()
{
	
	glm::vec3 entity_focus_position = (*entity->system->buffer->render_buffer)[focus_entity]->position;
	glm::vec3 entity_rotation = (*entity->system->buffer->render_buffer)[focus_entity]->rotation;
	glm::vec3 direction = glm::rotate(offset_direction, glm::radians(entity_rotation.x), glm::vec3(1, 0, 0));
	direction = glm::rotate(direction, glm::radians(entity_rotation.y), glm::vec3(0, 1, 0));
	direction = glm::rotate(direction, glm::radians(entity_rotation.z), glm::vec3(0, 0, 1));

	/*if (focus_on_entity)
	{
		glm::vec3 entity_focus_position = (*entity->system->buffer->render_buffer)[focus_entity]->position;
		if (follow_entity)
		{
			
			if (relative)
			{
				
				//glm::vec3 result_position = entity_focus_position + glm::vec3(glm::vec4(offset_position.x, offset_position.y, offset_position.z, 1.0f) * glm::yawPitchRoll(glm::radians(-(*entity->system->buffer->render_buffer)[focus_entity]->rotation.y), glm::radians(-(*entity->system->buffer->render_buffer)[focus_entity]->rotation.x), glm::radians(-(*entity->system->buffer->render_buffer)[focus_entity]->rotation.z)));
				
				glm::vec3 entity_rotation = (*entity->system->buffer->render_buffer)[focus_entity]->rotation;

				glm::vec3 direction = glm::rotate(offset_direction, glm::radians(entity_rotation.x), glm::vec3(1, 0, 0));
				direction = glm::rotate(direction, glm::radians(entity_rotation.y), glm::vec3(0, 1, 0));
				direction = glm::rotate(direction, glm::radians(entity_rotation.z), glm::vec3(0, 0, 1));
				//glm::mat4 rotationMatrix = glm::orientate4(glm::radians((*entity->system->buffer->render_buffer)[entity]->rotation));
				//glm::vec3 result_velocity = magnitude * glm::vec3(glm::vec4(velocity_value.x, velocity_value.y, velocity_value.z, 1.0f) * rotationMatrix);
				glm::vec3 result_position = (*entity->system->buffer->render_buffer)[focus_entity]->position + ((offset_magnitude) * direction);

				return glm::lookAt(result_position, entity_focus_position, up_direction);
			}
			return glm::lookAt((*entity->system->buffer->render_buffer)[entity]->position + (offset_magnitude), entity_focus_position, up_direction);
		}
		return glm::lookAt((*entity->system->buffer->render_buffer)[entity]->position + (offset_magnitude * offset_direction), (entity_focus_position), up_direction);
	}
	else
	{
		return glm::lookAt((*entity->system->buffer->render_buffer)[entity]->position + (offset_magnitude * offset_direction), focus_position, up_direction);
	}
	*/
	return glm::lookAt((*entity->system->buffer->render_buffer)[((follow_entity && focus_entity != nullptr)? focus_entity : entity)]->position + (offset_magnitude * (relative ? direction : offset_direction)), (focus_on_entity ? entity_focus_position : focus_position), up_direction);


}

glm::vec4 CameraComponent::GetCameraPosition(bool _relative)
{
	if (focus_on_entity && follow_entity && relative && _relative)
	{
		glm::vec3 entity_rotation = (*entity->system->buffer->render_buffer)[focus_entity]->rotation;
		glm::vec3 direction = glm::rotate(offset_direction, glm::radians(entity_rotation.x), glm::vec3(1, 0, 0));
		direction = glm::rotate(direction, glm::radians(entity_rotation.y), glm::vec3(0, 1, 0));
		direction = glm::rotate(direction, glm::radians(entity_rotation.z), glm::vec3(0, 0, 1));
		return glm::vec4((*entity->system->buffer->render_buffer)[focus_entity]->position + ((offset_magnitude) * direction), 1.0f);
	}
	return glm::vec4((*entity->system->buffer->render_buffer)[entity]->position + (offset_magnitude * offset_direction), 1.0f);
}
