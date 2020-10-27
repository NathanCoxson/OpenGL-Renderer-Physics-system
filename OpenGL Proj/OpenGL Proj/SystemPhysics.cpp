#include "SystemPhysics.h"
#include "SystemManager.h"


SystemPhysics::SystemPhysics(SystemManager* sys)
{
	system = sys;
}


SystemPhysics::~SystemPhysics()
{

}

void SystemPhysics::Tick()
{

	std::map<Entity*, Transform*> write_buffer = (*system->buffer->physics_buffer);
	std::map<Entity*, Transform*> read_buffer = (*system->buffer->render_buffer);
	//std::cout << "DELTIME " << system->delta_time << std::endl;


	for (Entity* entity_pointer : *(system->entity_groups)[eMotionEntities])
	{
		((MotionComponent*)entity_pointer->components[eMotion])->Update(system->delta_time, read_buffer[entity_pointer], write_buffer[entity_pointer]);
	}

}

std::thread SystemPhysics::TickThread()
{
	return std::thread([=] {Tick(); });
}
