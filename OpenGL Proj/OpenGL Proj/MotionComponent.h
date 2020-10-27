#pragma once

#include "Component.h"
#include "Transform.h"
#include <vector>

struct LinearForce
{
	glm::vec3 linear_force;
	double time_remaining = -1.0;
	bool constant = false;

	LinearForce(glm::vec3 force_init, double time_remaining_init = -1, bool constant_init = false);
};

struct AngularForce
{
	glm::vec3 pivot;
	glm::vec3 rotational_force;
	double time_remaining = -1.0;
	bool constant = false;

	AngularForce(glm::vec3 force_init, glm::vec3 pivot_init, double time_remaining_init = -1, bool constant_init = false);
};

class MotionComponent : public Component
{
public:
	float mass = 1.0f;
	float inertia = 1.0f;
	float local_dampening = 0.0f;
	float regional_dampening = 0.0f;
	glm::vec3 centre_of_mass = glm::vec3(0, 0, 0);

	glm::vec3 linear_velocity = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 linear_acceleration = glm::vec3(0.0, 0.0, 0.0);

	glm::vec3 rotational_velocity = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 rotational_acceleration = glm::vec3(0.0, 0.0, 0.0);

	glm::vec3 resultant_linear_force = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 resultant_rotational_force = glm::vec3(0.0, 0.0, 0.0);

	std::vector<LinearForce> linear_forces;
	std::vector<AngularForce> rotational_forces;

	Entity* entity;

	MotionComponent(Entity* parent);

	void CalculateTotalForce(float delta_time, Transform* read_transform, Transform* write_transform);

	void Update() {};

	void Update(float delta_time, Transform* read_transform, Transform* write_transform);
};

