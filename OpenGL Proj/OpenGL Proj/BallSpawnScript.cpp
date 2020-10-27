#include "BallSpawnScript.h"
#include "SystemManager.h"


BallSpawnScript::BallSpawnScript()
{

}


BallSpawnScript::~BallSpawnScript()
{
}

void BallSpawnScript::Update(Entity* parent)
{
	if (set && time <=  0.f)
	{
		Entity* store = parent->system->CreateEntity();
		store->AddComponent(eRender);
		store->AddComponent(eMotion);
		store->AddComponent(eCollider);

		((RenderComponent*)store->components[eRender])->mesh = "sphere";
		parent->system->collider_meshes[store] = new SphereCollider(store, glm::vec3(0.f, 0.f, 0.f), 5.1f);
		((MotionComponent*)store->components[eMotion])->linear_forces.push_back(LinearForce(glm::vec3(-0.0f, -20.f, 0.0f), -1.0f, true)); 
		parent->system->buffer->SetEntityTransformPosition(store, (*parent->system->buffer->render_buffer)[parent]->position - glm::vec3(10.f, 0.f, 10.f) + glm::vec3(rand() % 20 + 1, 0.f, rand() % 20 + 1));
		parent->system->buffer->SetEntityTransformScale(store, glm::vec3(5.f, 5.f, 5.f));

		time = delay;
	}
	else if (set)
	{
		time -= parent->system->delta_time;
	}
}
