#pragma once
#include "ColliderMesh.h"

class AABBCollider;
class OBBCollider;

class SphereCollider : public ColliderMesh
{
public:

	EColliderType type = eSphereCollider;

	float radius;

	glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
	Entity* entity;

	SphereCollider(Entity* parent, glm::vec3 offset_pos, float sphere_radius);

	CollisionData CheckCollision(ColliderMesh* other);
	CollisionData CheckCollision(SphereCollider* other);
	CollisionData CheckCollision(AABBCollider* other);
	CollisionData CheckCollision(OBBCollider* other);

	std::vector<glm::vec3> GetPoints();

	bool InArea(glm::vec3 point, glm::vec3 extents);
};

