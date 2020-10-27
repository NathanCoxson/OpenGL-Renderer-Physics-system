#pragma once
#include "Utilities.h"

#include <vector>

enum EColliderType
{
	eColliderMesh,
	eSphereCollider,
	eAABBCollider,
	eOBBCollider
};

enum ECollisionType
{
	eStatic,
	eLinear,
	eRotational,
	eComposition,
	eDampen
};

class Entity;
class SphereCollider;
class AABBCollider;
class OBBCollider;

struct CollisionData
{
	bool collided;
	glm::vec3 collision_point;
	glm::vec3 collision_normal;
	glm::vec3 collision_depth;
	Entity* first;
	Entity* second;
	CollisionData(bool _collided = false, glm::vec3 _collision_point = glm::vec3(0), glm::vec3 _collision_normal = glm::vec3(0), glm::vec3 _collision_depth = glm::vec3(0), Entity* _first = nullptr, Entity* _second = nullptr) : collided(_collided), collision_point(_collision_point), collision_normal(_collision_normal), collision_depth(_collision_depth), first(_first), second(_second) {}
};


class ColliderMesh
{
public:

	EColliderType type = eColliderMesh;
	ECollisionType response_type = eComposition;

	float restitution_coefficient = 1;
	float dampen = 0.0f;

	glm::vec3 half_values;
	glm::vec3 position;
	glm::vec3 rotation;
	float radius;
	Entity* entity;

	GLuint vao;
	GLuint vbo;

	ColliderMesh();
	~ColliderMesh();

	virtual CollisionData CheckCollision(ColliderMesh* other);
	virtual CollisionData CheckCollision(SphereCollider* other);
	virtual CollisionData CheckCollision(AABBCollider* other);
	virtual CollisionData CheckCollision(OBBCollider* other);

	virtual glm::vec3 GetClosestPoint(glm::vec3 point);

	virtual bool GenerateCollisionResponse(ColliderMesh* other);

	virtual std::vector<glm::vec3> GetPoints();

	virtual bool InArea(glm::vec3 point, glm::vec3 extents) { return false; };

};

