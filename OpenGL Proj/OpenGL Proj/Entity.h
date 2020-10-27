#pragma once

#include <vector>
#include <map>
#include <string>

#include "Component.h"
#include "MotionComponent.h"
#include "RenderComponent.h"
#include "ColliderComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"
#include "ScriptComponent.h"

class SystemManager;

class Entity
{
public:

	std::string label;
	SystemManager* system;

	std::map<EComponents, Component*> components;

	Entity(SystemManager* sys);
	Entity(SystemManager* sys, std::string entity_label);

	~Entity();

	Component* operator [] (EComponents index);

	Component* AddComponent(EComponents component);

	bool DeleteComponent(EComponents component);
};

