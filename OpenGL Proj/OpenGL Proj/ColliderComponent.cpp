#include "ColliderComponent.h"
#include "SystemManager.h"

ColliderComponent::ColliderComponent(Entity* parent)
{
	entity = parent;
	entity->system->GetEntityGroup(entity, eCollisionEntities);
}

ColliderComponent::~ColliderComponent()
{
	entity->system->DeleteEntityGroup(entity, eCollisionEntities);
}

void ColliderComponent::Update()
{

}