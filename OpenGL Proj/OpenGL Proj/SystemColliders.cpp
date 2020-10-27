#include "SystemColliders.h"
#include "SystemManager.h"


SystemColliders::SystemColliders(SystemManager* sys)
{
	system = sys;
	BroadPhaseComplete = true;
	main_oct_tree = new OctTreeBroadPhase(system, glm::vec3(0.f, 0.f, 0.f), glm::vec3(800.f, 200.f, 800.f), 8);
}


SystemColliders::~SystemColliders()
{
	delete main_oct_tree;
}

void SystemColliders::Tick()
{
	//std::cout << "Beginning Collider Tick" << std::endl;

	std::map<Entity*, Transform*> write_buffer = (*system->buffer->physics_buffer);
	std::map<Entity*, Transform*> read_buffer = (*system->buffer->render_buffer);

	positive_collisions.clear();
	negative_collisions.clear();

	//Basically broadphase here

	/*for (int i = 0; i < (system->entity_groups)[eCollisionEntities]->size()-1; i++)
	{
		for (int j = i + 1; j < (system->entity_groups)[eCollisionEntities]->size(); j++)
		{
			//std::cout << "i:" << i << " " << "j:" << j << std::endl;
			bool val = system->collider_meshes[(*(system->entity_groups)[eCollisionEntities])[i]]->CheckCollision(system->collider_meshes[(*(system->entity_groups)[eCollisionEntities])[j]]);

			std::cout << (*(system->entity_groups)[eCollisionEntities])[i] << " against " << (*(system->entity_groups)[eCollisionEntities])[j] << " is " << val << std::endl;
		}
	}*/

	/*for (auto oct_tree : all_oct_trees)
	{
		oct_tree->PreProcess();
		//std::cout << "POS (" << oct_tree->position.x << ", " << oct_tree->position.y << ", " << oct_tree->position.z << ")" << std::endl;
	}*/
	
	//std::cout << "TREES: " << all_oct_trees.size() << std::endl;

	BroadPhaseComplete = false;

	std::thread narrow_phase = CheckCollisionsThread();

	int count = 0;

	for (int i = 0; i < (system->entity_groups)[eCollisionEntities]->size(); i++)
	{
		//for (glm::vec3 vertex : system->collider_meshes[(*(system->entity_groups)[eCollisionEntities])[i]]->GetPoints())
		//{
		//	count += main_oct_tree->Process((*(system->entity_groups)[eCollisionEntities])[i], vertex, collision_areas);
		//}
		count += main_oct_tree->Process((*(system->entity_groups)[eCollisionEntities])[i], glm::vec3(0.f), collision_areas);
	}

	//std::cout << "FINAL COUNT: " << count << std::endl;
	//std::cout << "FINISHED BROADPHASE" << std::endl;

	BroadPhaseComplete = true;


	narrow_phase.join();

	//std::cout << "COL AREA: " << collision_areas.size() << std::endl;
	//for (auto section : collision_areas) { std::cout << section.second.size() << std::endl; }
	//std::cout << "POS COL: " << positive_collisions.size() << std::endl;
	//std::cout << "NEG COL: " << negative_collisions.size() << std::endl;
	//std::cout << "FINISHED NARROWPHASE" << std::endl;

	for (auto list : collision_areas)
	{
		list.second.clear();
	}
	collision_areas.clear();
}

void SystemColliders::CheckCollisions()
{
	bool final_check = false;

	

	while (true)
	{
		auto local_store = collision_areas;

		for (auto section : local_store)
		{
			for (auto entity_val_1 : section.second)
			{
				for (auto entity_val_2 : section.second)
				{
					if (entity_val_1 == entity_val_2) continue;

					if ((positive_collisions.count(std::pair<Entity*, Entity*>(entity_val_1, entity_val_2)) || positive_collisions.count(std::pair<Entity*, Entity*>(entity_val_2, entity_val_1))) || (negative_collisions.count(std::pair<Entity*, Entity*>(entity_val_1, entity_val_2)) || negative_collisions.count(std::pair<Entity*, Entity*>(entity_val_2, entity_val_1)))) continue;

					CollisionData data = system->collider_meshes[entity_val_1]->CheckCollision(system->collider_meshes[entity_val_2]);

					if (data.collided)
					{
						positive_collisions.insert(std::pair<Entity*, Entity*>(entity_val_1, entity_val_2));
						collision_data[std::pair<Entity*, Entity*>(entity_val_1, entity_val_2)] = data;
					}
					else negative_collisions.insert(std::pair<Entity*, Entity*>(entity_val_1, entity_val_2));
				}
			}
		}

		if (final_check) return;
		if (BroadPhaseComplete) final_check = true;
	}
}

void SystemColliders::GenerateCollisionResponses()
{
	for (auto pair : positive_collisions)
	{
		//std::cout << pair.first << " " << pair.second << " " <<  std::endl;
		//std::cout << "STAGE 1A!" << std::endl;

		if (system->collider_meshes[pair.first]->response_type == eDampen && system->collider_meshes[pair.second]->response_type == eDampen) continue;

		CollisionData data = collision_data[pair];

		glm::vec3 first_linear_velocity = ((MotionComponent*)pair.first->components[eMotion])->linear_velocity;
		glm::vec3 second_linear_velocity = ((MotionComponent*)pair.second->components[eMotion])->linear_velocity;

		//std::cout << "STAGE 1B!" << std::endl;

		glm::vec3 first_position = (*system->buffer->render_buffer)[pair.first]->position;
		glm::vec3 second_position = (*system->buffer->render_buffer)[pair.second]->position;

		float first_mass = ((MotionComponent*)pair.first->components[eMotion])->mass;
		float second_mass = ((MotionComponent*)pair.second->components[eMotion])->mass;

		float first_inertia = ((MotionComponent*)pair.first->components[eMotion])->inertia;
		float second_inertia = ((MotionComponent*)pair.second->components[eMotion])->inertia;
		//std::cout << "STAGE 1C!" << std::endl;

		float first_restitution_coefficient = system->collider_meshes[pair.first]->restitution_coefficient;
		float second_restitution_coefficient = system->collider_meshes[pair.second]->restitution_coefficient;

		//std::cout << "STAGE 2A!" << std::endl;

		float first_linear_impulse = -(((glm::dot((first_linear_velocity - second_linear_velocity), data.collision_normal)) * (1 + first_restitution_coefficient)) / ((glm::dot(data.collision_normal, data.collision_normal)) * ((1 / first_mass) + (1 / second_mass))));
		//std::cout << "STAGE 2B! " << first_linear_impulse << std::endl;

		float second_linear_impulse = -(((glm::dot((first_linear_velocity - second_linear_velocity), data.collision_normal)) * (1 + second_restitution_coefficient)) / ((glm::dot(data.collision_normal, data.collision_normal)) * ((1 / first_mass) + (1 / second_mass))));

		//std::cout << "STAGE 3A! " << second_linear_impulse << std::endl;


		if (system->collider_meshes[pair.first]->response_type == eDampen || system->collider_meshes[pair.second]->response_type == eDampen)
		{ 
			if (system->collider_meshes[pair.first]->response_type == eDampen)
			{
				((MotionComponent*)pair.second->components[eMotion])->regional_dampening = system->collider_meshes[pair.first]->dampen;
			}
			else
			{
				((MotionComponent*)pair.first->components[eMotion])->regional_dampening = system->collider_meshes[pair.second]->dampen;
			}
		}
		else
		{

			switch (system->collider_meshes[pair.first]->response_type)
			{
			default:
				break;
			case eStatic:
				break;
			case eLinear:
				((MotionComponent*)pair.first->components[eMotion])->linear_velocity += (first_linear_impulse / first_mass) * data.collision_normal;
				(*system->buffer->physics_buffer)[pair.first]->position += (first_linear_impulse / first_mass) * data.collision_normal * system->delta_time * 2.0f;
				break;
			case eRotational:
				((MotionComponent*)pair.first->components[eMotion])->rotational_velocity -= (1 / first_inertia) * (glm::distance(data.collision_point, first_position) * (first_linear_impulse * data.collision_normal));
				(*system->buffer->physics_buffer)[pair.first]->rotation -= (1 / first_inertia) * (glm::distance(data.collision_point, first_position) * (first_linear_impulse * data.collision_normal)) * system->delta_time * 2.0f;
				break;
			case eComposition:
				((MotionComponent*)pair.first->components[eMotion])->linear_velocity += (first_linear_impulse / first_mass) * data.collision_normal;
				((MotionComponent*)pair.first->components[eMotion])->rotational_velocity -= (1 / first_inertia) * (glm::cross(glm::vec3(first_position - data.collision_point), (first_linear_impulse * data.collision_normal)));
				(*system->buffer->physics_buffer)[pair.first]->position += (first_linear_impulse / first_mass) * data.collision_normal * system->delta_time * 2.0f;
				(*system->buffer->physics_buffer)[pair.first]->rotation -= (1 / first_inertia) * (glm::distance(data.collision_point, first_position) * (first_linear_impulse * data.collision_normal)) * system->delta_time * 2.0f;
				break;
			}

			//std::cout << "STAGE 3B!" << std::endl;

			switch (system->collider_meshes[pair.second]->response_type)
			{
			default:
				break;
			case eStatic:
				break;
			case eLinear:
				((MotionComponent*)pair.second->components[eMotion])->linear_velocity -= (second_linear_impulse / second_mass) * data.collision_normal;
				(*system->buffer->physics_buffer)[pair.second]->position -= (second_linear_impulse / second_mass) * data.collision_normal * system->delta_time * 2.0f;
				break;
			case eRotational:
				((MotionComponent*)pair.second->components[eMotion])->rotational_velocity += (1 / second_inertia) * (glm::distance(data.collision_point, second_position) * (second_linear_impulse * data.collision_normal));
				(*system->buffer->physics_buffer)[pair.second]->rotation -= (1 / second_inertia) * (glm::distance(data.collision_point, second_position) * (second_linear_impulse * data.collision_normal)) * system->delta_time * 2.0f;
				break;
			case eComposition:
				((MotionComponent*)pair.second->components[eMotion])->linear_velocity -= (second_linear_impulse / second_mass) * data.collision_normal;
				((MotionComponent*)pair.second->components[eMotion])->rotational_velocity += (1 / second_inertia) * (glm::cross(glm::vec3(second_position - data.collision_point), (second_linear_impulse * data.collision_normal)));
				(*system->buffer->physics_buffer)[pair.second]->position -= (second_linear_impulse / second_mass) * data.collision_normal * system->delta_time * 2.0f;
				(*system->buffer->physics_buffer)[pair.second]->rotation -= (1 / second_inertia) * (glm::distance(data.collision_point, second_position) * (second_linear_impulse * data.collision_normal)) * system->delta_time * 2.0f;
				break;
			}
		}

		//std::cout << "STAGE 4!" << std::endl;

	}

	collision_data.clear();
}

std::thread SystemColliders::CheckCollisionsThread()
{
	return std::thread([=] {CheckCollisions();});
}

std::thread SystemColliders::TickThread()
{
	return std::thread([=] {Tick();});
}