#include "ScriptComponent.h"
#include "SystemManager.h"

ScriptComponent::ScriptComponent(Entity* parent)
{
	entity = parent;
	entity->system->GetEntityGroup(entity, eScriptEntities);
}


ScriptComponent::~ScriptComponent()
{
	delete script;
	entity->system->DeleteEntityGroup(entity, eScriptEntities);
}

void ScriptComponent::Update()
{
	script->Update(entity);
}
