#include "OctTreeBroadPhase.h"
#include "SystemManager.h"

OctTreeBroadPhase::OctTreeBroadPhase(SystemManager* _system, glm::vec3 _position, glm::vec3 _extents, int _threshold, int _depth, int _threshold_delay, int _threshold_multiplication)
{
	object_count = 0;
	system = _system;
	position = _position;
	extents = _extents;
	subdivide_threshold = _threshold;
	depth = _depth;
	threshold_delay = _threshold_delay;
	threshold_multiplication = _threshold_multiplication;
	subdivide_delay_count = 0;
	reform_delay_count = 0;


	vertices =
	{
	{+extents.x, +extents.y, +extents.z, 1}, //1
	{-extents.x, +extents.y, +extents.z, 1}, //2
	{-extents.x, +extents.y, +extents.z, 1}, //2
	{-extents.x, -extents.y, +extents.z, 1}, //3
	{-extents.x, -extents.y, +extents.z, 1}, //3
	{+extents.x, -extents.y, +extents.z, 1}, //4
	{+extents.x, -extents.y, +extents.z, 1}, //4
	{+extents.x, +extents.y, +extents.z, 1}, //1
	{+extents.x, +extents.y, -extents.z, 1}, //5
	{-extents.x, +extents.y, -extents.z, 1}, //6
	{-extents.x, +extents.y, -extents.z, 1}, //6
	{-extents.x, -extents.y, -extents.z, 1}, //7
	{-extents.x, -extents.y, -extents.z, 1}, //7
	{+extents.x, -extents.y, -extents.z, 1}, //8
	{+extents.x, -extents.y, -extents.z, 1}, //8
	{+extents.x, +extents.y, -extents.z, 1}, //5
	{+extents.x, +extents.y, +extents.z, 1}, //1
	{+extents.x, +extents.y, -extents.z, 1}, //5
	{-extents.x, +extents.y, +extents.z, 1}, //2
	{-extents.x, +extents.y, -extents.z, 1}, //6
	{+extents.x, -extents.y, +extents.z, 1}, //4
	{+extents.x, -extents.y, -extents.z, 1}, //8
	{-extents.x, -extents.y, +extents.z, 1}, //3
	{-extents.x, -extents.y, -extents.z, 1}, //7
	};
	
	glUseProgram(system->renderer->debug_program_id);

	glGenVertexArrays(1, &(vao));
	glBindVertexArray(vao);

	glGenBuffers(1, &(vbo));
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(BasicVertex), &(vertices[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), 0);
	glEnableVertexAttribArray(0);

}

OctTreeBroadPhase::~OctTreeBroadPhase()
{
	for (int i = 0; i < subdivisions.size(); i++)
	{
		delete subdivisions[i];
	}
	subdivisions.clear();

	glUseProgram(system->renderer->debug_program_id);
	glDeleteVertexArrays(1, &(vao));
	glDeleteBuffers(1, &vbo);
}

bool OctTreeBroadPhase::PreProcess()
{
	object_count = 0;
	return true;
}

int OctTreeBroadPhase::Process(Entity* entity, glm::vec3 point, std::map<OctTreeBroadPhase*, std::set<Entity*>> &collision_map)
{

	//std::cout << this << " at depth " << depth << " processing " << entity << " against point (" << point.x << ", " << point.y << ", " << point.z << ")" << std::endl;
	//std::cout << (position.x - extents.x) << " <= " << point.x << " <= " << (position.x + extents.x) << std::endl;
	//std::cout << (position.y - extents.y) << " <= " << point.y << " <= " << (position.y + extents.y) << std::endl;
	//std::cout << (position.z - extents.z) << " <= " << point.z << " <= " << (position.z + extents.z) << std::endl;
	//std::cout << "IS " << ((((position.x - extents.x) <= point.x && point.x <= (position.x + extents.x)) && ((position.y - extents.y) <= point.y && point.y <= (position.y + extents.y)) && ((position.z - extents.z) <= point.z && point.z <= (position.z + extents.z))) ? "TRUE" : "FALSE") << std::endl;
	//std::cout << "STAGE 1" << std::endl;
	if (collision_map[this].count(entity)) return 0;
	//std::cout << "STAGE 2" << std::endl;
	//if (((position.x - extents.x) <= point.x && point.x <= (position.x + extents.x)) && ((position.y - extents.y) <= point.y && point.y <= (position.y + extents.y)) && ((position.z - extents.z) <= point.z && point.z <= (position.z + extents.z)))
	
	if (entity->system->collider_meshes[entity]->InArea(position, extents))
	{
		int count = 0;
		if (subdivided)
		{
			//std::cout << "STAGE 3A" << std::endl;
			for (int i = 0; i < subdivisions.size(); i++)
			{
				count += subdivisions[i]->Process(entity, point, collision_map);
			}
			object_count += (count > 0) ? 1 : 0;
			return (count > 0) ? 1 : 0;
		}
		else
		{
			//std::cout << "STAGE 3B" << std::endl;
			collision_map[this].insert(entity);
			count++;
			object_count += count;
			return count;
			


		}
	}
	
	//std::cout << "STAGE 3C" << std::endl;
	return 0;

}

bool OctTreeBroadPhase::PostProcess()
{
	if (object_count > subdivide_threshold && !subdivided)
	{
		subdivide_delay_count++;
		reform_delay_count = 0;
		if (subdivide_delay_count >= threshold_delay)
		{
			Subdivide();
			return true;
		}
	}
	else if (object_count <= subdivide_threshold && subdivided)
	{
		subdivide_delay_count = 0;
		reform_delay_count++;
		if (reform_delay_count >= threshold_delay)
		{
			Reform();
			return true;
		}
	}
	return false;
}

bool OctTreeBroadPhase::Subdivide()
{
	if (!subdivided)
	{
		//x left, y top, z front
		//x right, y top, z front
		//x left, y bottom, z front
		//x right, y bottom, z front
		//x left, y top, z back
		//x right, y top, z back
		//x left, y bottom, z back
		//x right, y bottom, z back
		subdivisions.push_back(new OctTreeBroadPhase(system, glm::vec3(position.x - (extents.x / 2), position.y + (extents.y / 2), position.z + (extents.z / 2)), extents * 0.5f, subdivide_threshold * threshold_multiplication, depth + 1, threshold_delay, threshold_multiplication));
		subdivisions.push_back(new OctTreeBroadPhase(system, glm::vec3(position.x + (extents.x / 2), position.y + (extents.y / 2), position.z + (extents.z / 2)), extents * 0.5f, subdivide_threshold * threshold_multiplication, depth + 1, threshold_delay, threshold_multiplication));
		subdivisions.push_back(new OctTreeBroadPhase(system, glm::vec3(position.x - (extents.x / 2), position.y - (extents.y / 2), position.z + (extents.z / 2)), extents * 0.5f, subdivide_threshold * threshold_multiplication, depth + 1, threshold_delay, threshold_multiplication));
		subdivisions.push_back(new OctTreeBroadPhase(system, glm::vec3(position.x + (extents.x / 2), position.y - (extents.y / 2), position.z + (extents.z / 2)), extents * 0.5f, subdivide_threshold * threshold_multiplication, depth + 1, threshold_delay, threshold_multiplication));
		subdivisions.push_back(new OctTreeBroadPhase(system, glm::vec3(position.x - (extents.x / 2), position.y + (extents.y / 2), position.z - (extents.z / 2)), extents * 0.5f, subdivide_threshold * threshold_multiplication, depth + 1, threshold_delay, threshold_multiplication));
		subdivisions.push_back(new OctTreeBroadPhase(system, glm::vec3(position.x + (extents.x / 2), position.y + (extents.y / 2), position.z - (extents.z / 2)), extents * 0.5f, subdivide_threshold * threshold_multiplication, depth + 1, threshold_delay, threshold_multiplication));
		subdivisions.push_back(new OctTreeBroadPhase(system, glm::vec3(position.x - (extents.x / 2), position.y - (extents.y / 2), position.z - (extents.z / 2)), extents * 0.5f, subdivide_threshold * threshold_multiplication, depth + 1, threshold_delay, threshold_multiplication));
		subdivisions.push_back(new OctTreeBroadPhase(system, glm::vec3(position.x + (extents.x / 2), position.y - (extents.y / 2), position.z - (extents.z / 2)), extents * 0.5f, subdivide_threshold * threshold_multiplication, depth + 1, threshold_delay, threshold_multiplication));
		subdivided = true;
		return true;
	}
	return false;
}

bool OctTreeBroadPhase::Reform()
{
	if (subdivided)
	{
		for (int i = 0; i < subdivisions.size(); i++)
		{
			subdivisions[i]->Reform();
		}
		for (int i = 0; i < subdivisions.size(); i++)
		{
			delete subdivisions[i];
		}
		subdivisions.clear();
		subdivided = false;
		std::cout << "REFORMING!" << std::endl;
		return true;
	}
	return false;
}

std::vector<OctTreeBroadPhase*> OctTreeBroadPhase::GetAllOctTrees()
{
	std::vector<OctTreeBroadPhase*> store;

	if (subdivided)
	{
		for (OctTreeBroadPhase* subdivision : subdivisions)
		{
			for (OctTreeBroadPhase* entry : subdivision->GetAllOctTrees())
			{
				store.push_back(entry);
			}
		}
	}

	store.push_back(this);

	return store;
}
