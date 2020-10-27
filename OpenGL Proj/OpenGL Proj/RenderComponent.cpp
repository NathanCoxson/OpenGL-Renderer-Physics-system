#include "RenderComponent.h"
#include "SystemManager.h"


RenderComponent::RenderComponent(Entity* parent, std::string init_mesh)
{
	mesh = init_mesh;

	entity = parent;

	entity->system->buffer->AllocateEntityBuffer(entity);
	entity->system->GetEntityGroup(entity, eRenderEntities);
}

RenderComponent::~RenderComponent()
{
	entity->system->buffer->DeleteEntityBuffer(entity);
	entity->system->DeleteEntityGroup(entity, eRenderEntities);
}

void RenderComponent::Update()
{
	//Not sure what the renderer will do compared to the render component
}
