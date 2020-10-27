#include "MotionComponent.h"
#include "SystemManager.h"

LinearForce::LinearForce(glm::vec3 force_init, double time_remaining_init, bool constant_init)
{
	linear_force = force_init;
	time_remaining = time_remaining_init;
	constant = constant_init;
}

AngularForce::AngularForce(glm::vec3 force_init, glm::vec3 pivot_init, double time_remaining_init, bool constant_init)
{
	rotational_force = force_init;
	pivot = pivot_init;
	time_remaining = time_remaining_init;
	constant = constant_init;
}

MotionComponent::MotionComponent(Entity* parent)
{
	entity = parent;
	resultant_linear_force = glm::vec3(0, 0, 0);
	entity->system->GetEntityGroup(entity, eMotionEntities);
}

void MotionComponent::CalculateTotalForce(float delta_time, Transform* read_transform, Transform* write_transform)
{
	resultant_linear_force = glm::vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < linear_forces.size(); i++)
	{
		resultant_linear_force += linear_forces[i].linear_force;
		if (!linear_forces[i].constant)
		{ 
			linear_forces[i].time_remaining -= delta_time;
			if (linear_forces[i].time_remaining <= 0.0)
			{
				linear_forces.erase(linear_forces.begin()+i);
				i -= 1;
			}
		}
	}
	
	resultant_rotational_force = glm::vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < rotational_forces.size(); i++)
	{
		resultant_rotational_force += glm::cross(rotational_forces[i].pivot, rotational_forces[i].rotational_force);
		if (!rotational_forces[i].constant)
		{
			rotational_forces[i].time_remaining -= delta_time;
			if (rotational_forces[i].time_remaining <= 0.0)
			{
				rotational_forces.erase(rotational_forces.begin() + i);
				i -= 1;
			}
		}
	}
	//cout << resultant_rotational_force.x << " " resultant_rotational_force.y << 
}

void MotionComponent::Update(float delta_time, Transform* read_transform, Transform* write_transform)
{
	CalculateTotalForce(delta_time, read_transform, write_transform);

	//std::cout << entity << std::endl;

	linear_acceleration = resultant_linear_force / mass;
	rotational_acceleration = resultant_rotational_force / inertia;



	//std::cout << "LINVELB " << linear_velocity.x << " " << linear_velocity.y << " " << linear_velocity.z << std::endl;
	//std::cout << "ROTVELB " << rotational_velocity.x << " " << rotational_velocity.y << " " << rotational_velocity.z << std::endl;
	//std::cout << "LINACCB " << linear_acceleration.x << " " << linear_acceleration.y << " " << linear_acceleration.z << std::endl;
	//std::cout << "ROTACCB " << rotational_acceleration.x << " " << rotational_acceleration.y << " " << rotational_acceleration.z << std::endl;


	if (!entity->system->physics->verlet_integration)
	{
		//Semi-implicit Euler Method - Order 1
		linear_velocity = linear_velocity + linear_acceleration * delta_time;
		rotational_velocity = rotational_velocity + rotational_acceleration * delta_time;

		write_transform->position = read_transform->position + linear_velocity * delta_time;
		write_transform->rotation = read_transform->rotation + rotational_velocity * delta_time;
	}
	else
	{
		
		//Verlet Velocity Method - Order 2
		glm::vec3 new_linear_acceleration = resultant_linear_force / mass;
		write_transform->position = read_transform->position + linear_velocity * delta_time + (linear_acceleration * 0.5f * (delta_time * delta_time));
		linear_velocity = linear_velocity + 0.5f * (new_linear_acceleration + linear_acceleration) * delta_time;
		linear_acceleration = new_linear_acceleration;

		glm::vec3 new_rotational_acceleration = resultant_rotational_force / inertia;
		write_transform->rotation = read_transform->rotation + rotational_velocity * delta_time + (rotational_acceleration * 0.5f * (delta_time * delta_time));
		rotational_velocity = rotational_velocity + 0.5f * (new_rotational_acceleration + rotational_acceleration) * delta_time;
		rotational_acceleration = new_rotational_acceleration;
		
	}

	//std::cout << "POS " << read_transform->position.x << " " << read_transform->position.y << " " << read_transform->position.z << std::endl;
	//std::cout << "ROTVELA " << rotational_velocity.x << " " << rotational_velocity.y << " " << rotational_velocity.z << std::endl;

	//std::cout << "DELTIME " << delta_time << std::endl;

	//std::cout << entity << " LIN: " << linear_velocity.x << " " << linear_velocity.y << " " << linear_velocity.z << " DT: " << delta_time << std::endl;

	write_transform->rotation = glm::vec3(fmod(write_transform->rotation.x, 360.f), fmod(write_transform->rotation.y, 360.f), fmod(write_transform->rotation.z, 360.f));

	float dampening = (entity->system->physics->global_dampening + regional_dampening + local_dampening) * delta_time;
	glm::vec3 linear_normal = glm::normalize(linear_velocity);
	glm::vec3 rotational_normal = glm::normalize(rotational_velocity);

	if (linear_velocity.x > dampening);
	else if (linear_velocity.x < -dampening);
	else linear_velocity.x = 0;

	if (linear_velocity.y > dampening);
	else if (linear_velocity.y < -dampening);
	else linear_velocity.y = 0;

	if (linear_velocity.z > dampening);
	else if (linear_velocity.z < -dampening);
	else linear_velocity.z = 0;

	if (rotational_velocity.x > dampening);
	else if (rotational_velocity.x < -dampening);
	else rotational_velocity.x = 0;

	if (rotational_velocity.y > dampening);
	else if (rotational_velocity.y < -dampening);
	else rotational_velocity.y = 0;

	if (rotational_velocity.z > dampening);
	else if (rotational_velocity.z < -dampening);
	else rotational_velocity.z = 0;

	if (linear_velocity != glm::vec3(0.0f)) linear_velocity -= linear_normal * dampening;
	if (rotational_velocity != glm::vec3(0.0f)) rotational_velocity -= rotational_normal * dampening;
	
	regional_dampening = 0.0f;
}

