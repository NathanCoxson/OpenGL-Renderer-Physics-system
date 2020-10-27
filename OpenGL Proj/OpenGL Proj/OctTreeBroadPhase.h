#pragma once

#include "BroadPhaseCollider.h"
#include "Transform.h"
#include "Utilities.h"
#include "Entity.h"

#include <set>
#include <vector>

class SystemManager;

struct BasicVertex
{
	float coordinates[4];
};

class OctTreeBroadPhase : public BroadPhaseCollider
{
public:
	SystemManager* system;

	bool subdivided = false;

	float subdivide_threshold;
	int threshold_multiplication;
	int threshold_delay;
	int reform_delay_count;
	int subdivide_delay_count;
	int object_count;
	int depth;

	GLuint vao;
	GLuint vbo;
	
	glm::vec3 position;
	glm::vec3 extents;

	std::vector<BasicVertex> vertices;

	std::vector<OctTreeBroadPhase*> subdivisions;

	OctTreeBroadPhase(SystemManager* _system, glm::vec3 _position, glm::vec3 _extents, int _threshold = 8, int _depth = 0, int _threshold_delay = 60, int _threshold_multiplication = 1);
	~OctTreeBroadPhase();

	bool PreProcess();

	int Process(Entity* entity, glm::vec3 point, std::map<OctTreeBroadPhase*, std::set<Entity*>> &collision_map);

	bool PostProcess();

	bool Subdivide();

	bool Reform();

	std::vector<OctTreeBroadPhase*> GetAllOctTrees();

};

