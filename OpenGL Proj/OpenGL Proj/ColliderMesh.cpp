#include "ColliderMesh.h"
#include "SphereCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"


ColliderMesh::ColliderMesh()
{
}

ColliderMesh::~ColliderMesh()
{
}

CollisionData ColliderMesh::CheckCollision(ColliderMesh* other)
{
	return other->CheckCollision(this);
}

CollisionData ColliderMesh::CheckCollision(SphereCollider* other)
{
	return other->CheckCollision(this);
}

CollisionData ColliderMesh::CheckCollision(AABBCollider* other)
{
	return other->CheckCollision(this);
}

CollisionData ColliderMesh::CheckCollision(OBBCollider* other)
{
	return other->CheckCollision(this);
}

glm::vec3 ColliderMesh::GetClosestPoint(glm::vec3 point)
{
	return position;
}

bool ColliderMesh::GenerateCollisionResponse(ColliderMesh* other)
{
	return true;
}

std::vector<glm::vec3> ColliderMesh::GetPoints()
{
	std::vector<glm::vec3> store = {{ 0, 0, 0 }};
	return store;
}
