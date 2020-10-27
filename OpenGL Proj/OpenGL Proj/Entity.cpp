#include "Entity.h"
#include "SystemManager.h"

Entity::Entity(SystemManager* sys)
{
	system = sys;
	label = "";
	system->buffer->AllocateEntityTransform(this);
}

Entity::Entity(SystemManager* sys, std::string entity_label)
{
	system = sys;
	label = entity_label;
	system->buffer->AllocateEntityTransform(this);
}

Entity::~Entity()
{
	system->buffer->DeleteEntityTransform(this);
}

Component* Entity::operator[] (EComponents index)
{

	return components[index];
}

Component* Entity::AddComponent(EComponents component)
{
	Component* new_component;

	if (component == eRender)
	{
		new_component = new RenderComponent(this);
		components[eRender] = new_component;
		return new_component;
	}
	else if (component == eMotion)
	{
		new_component = new MotionComponent(this);
		components[eMotion] = new_component;
		return new_component;
	}
	else if (component == eCollider)
	{
		new_component = new ColliderComponent(this);
		components[eCollider] = new_component;
		return new_component;
	}
	else if (component == eCamera)
	{
		new_component = new CameraComponent(this);
		components[eCamera] = new_component;
		return new_component;
	}
	else if (component == eLight)
	{
		new_component = new LightComponent(this);
		components[eLight] = new_component;
		return new_component;
	}
	else if (component == eScript)
	{
		new_component = new ScriptComponent(this);
		components[eScript] = new_component;
		return new_component;
	}

}

bool Entity::DeleteComponent(EComponents component)
{
	return false;
}
