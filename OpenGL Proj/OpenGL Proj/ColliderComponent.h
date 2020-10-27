#pragma once

#include "Component.h"

class ColliderComponent : public Component
{
public:
	Entity* entity;

	ColliderComponent(Entity* parent);
	~ColliderComponent();

	void Update();
};

