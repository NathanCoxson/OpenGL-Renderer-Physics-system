#pragma once
#include "ColliderMesh.h"

class SphereCollider;
class OBBCollider;

class AABBCollider : public ColliderMesh
{
public:

	EColliderType type = eAABBCollider;

	std::vector<glm::vec3> basic_vertices;
	glm::vec3 half_values = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
	bool scale_to_mesh;
	Entity* entity;

	AABBCollider(Entity* parent, glm::vec3 offset_pos, glm::vec3 offset_half_values, bool _scale_to_mesh = false);

	CollisionData CheckCollision(ColliderMesh* other);
	CollisionData CheckCollision(SphereCollider* other);
	CollisionData CheckCollision(AABBCollider* other);
	CollisionData CheckCollision(OBBCollider* other);

	std::vector<glm::vec3> GetPoints();
	
	bool InArea(glm::vec3 point, glm::vec3 extents);
};

