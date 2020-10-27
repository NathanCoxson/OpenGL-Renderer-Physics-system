#pragma once
#include "Utilities.h"
#include "OctTreeBroadPhase.h"
#include "ColliderMesh.h"

#include <utility>

class SystemManager;


class SystemColliders
{
public:

	SystemManager* system;

	OctTreeBroadPhase* main_oct_tree;

	std::vector<OctTreeBroadPhase*> all_oct_trees;

	std::map<OctTreeBroadPhase*, std::set<Entity*>> collision_areas;

	std::set<std::pair<Entity*, Entity*>> positive_collisions;

	std::set<std::pair<Entity*, Entity*>> negative_collisions;

	std::map<std::pair<Entity*, Entity*>, CollisionData> collision_data;

	bool BroadPhaseComplete;

	SystemColliders(SystemManager* sys);
	~SystemColliders();

	void Tick();

	void CheckCollisions();

	void GenerateCollisionResponses();

	std::thread CheckCollisionsThread();

	std::thread TickThread();
};

