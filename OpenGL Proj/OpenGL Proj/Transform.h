#pragma once
#include "Utilities.h"

class Transform
{
public:

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	Transform();
	~Transform();
};

