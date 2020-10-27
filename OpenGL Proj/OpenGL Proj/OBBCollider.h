#pragma once
#include "ColliderMesh.h"

class AABBCollider;
class SphereCollider;

class OBBCollider : public ColliderMesh
{
public:

	EColliderType type = eOBBCollider;

	glm::vec3 half_values = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f);
	Entity* entity;

	OBBCollider(Entity* parent, glm::vec3 offset_pos, glm::vec3 offset_rot, glm::vec3 offset_half_values);

	CollisionData CheckCollision(ColliderMesh* other);
	CollisionData CheckCollision(SphereCollider* other);
	CollisionData CheckCollision(AABBCollider* other);
	CollisionData CheckCollision(OBBCollider* other);

	std::vector<glm::vec3> GetPoints();

	bool InArea(glm::vec3 point, glm::vec3 extents);
};

