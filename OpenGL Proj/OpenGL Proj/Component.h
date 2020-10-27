#pragma once

#include "Utilities.h"

class Entity;

enum EComponents
{
	eMotion,
	eRender,
	eCollider,
	eCamera,
	eLight,
	eScript
};

class Component
{
public:
	Entity* entity;

	Component();

	~Component();

	virtual void Update() {};
};

