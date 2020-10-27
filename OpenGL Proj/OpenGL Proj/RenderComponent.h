#pragma once

#include "Component.h"
#include "RenderMesh.h"

class RenderComponent : public Component
{
public:

	std::string mesh;

	Entity* entity;

	RenderComponent(Entity* parent, std::string init_mesh = "cube");

	~RenderComponent();

	void Update();
};

