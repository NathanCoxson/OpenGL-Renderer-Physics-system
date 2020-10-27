#pragma once

#include "Component.h"

class CameraComponent : public Component
{
public:

	Entity* entity;

	bool focus_on_entity = false;
	Entity* focus_entity = nullptr;
	glm::vec3 focus_position = glm::vec3(0.0f);
	float offset_magnitude = 0;
	glm::vec3 offset_direction = glm::vec3(0.0f);
	bool relative = true;
	bool follow_entity = true;
	glm::vec3 up_direction = glm::vec3(0.0f, 1.0f, 0.0f);

	CameraComponent(Entity* parent);
	CameraComponent(Entity* parent, glm::vec3 up, Entity* focus);
	CameraComponent(Entity* parent, glm::vec3 up, glm::vec3 focus);
	~CameraComponent();

	glm::mat4 GetCameraMatrix();
	glm::vec4 GetCameraPosition(bool _relative = true);
};

