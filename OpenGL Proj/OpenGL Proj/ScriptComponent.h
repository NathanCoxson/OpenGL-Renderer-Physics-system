#pragma once

#include "Component.h"
#include "Script.h"

class ScriptComponent : public Component
{
public:

	Entity* entity;
	Script* script;

	ScriptComponent(Entity* parent);
	~ScriptComponent();

	void Update();

};

