#pragma once

#include <map>

#include "Entity.h"
#include "Transform.h"

class SystemManager;

class SystemBuffer
{
public:

	SystemManager* system;

	std::map<Entity*, GLuint> vao_to_entity;
	std::map<Entity*, GLuint> vbo_to_entity;
	std::map<Entity*, GLuint> texture_to_entity;

	std::map<Entity*, std::vector<GLuint>> light_locations;

	std::map<Entity*, Transform*>* render_buffer;
	std::map<Entity*, Transform*>* physics_buffer;

	std::vector<Entity*> transform_add_queue;
	std::vector<Entity*> transform_delete_queue;

	std::map<Entity*, glm::vec3> transform_position_set;
	std::map<Entity*, glm::vec3> transform_rotation_set;
	std::map<Entity*, glm::vec3> transform_scale_set;

	std::vector<Entity*> buffer_add_queue;
	std::vector<Entity*> buffer_delete_queue;

	SystemBuffer(SystemManager* sys);
	~SystemBuffer();

	bool AllocateEntityBuffer(Entity* entity);

	bool DeleteEntityBuffer(Entity* entity);

	bool AllocateEntityTransform(Entity* entity);

	bool DeleteEntityTransform(Entity* entity);

	bool AllocateLightLocations(Entity* entity);

	bool DeleteLightLocations(Entity* entity);

	bool SetEntityTransformPosition(Entity* entity, glm::vec3 position);

	bool SetEntityTransformRotation(Entity* entity, glm::vec3 rotation);

	bool SetEntityTransformScale(Entity* entity, glm::vec3 scale);

	bool ProcessTransformQueue();

	bool ProcessBufferQueue();

	bool ProcessTransformPRS();

	bool SwapSystemBuffers();

	bool Tick();
};

