#pragma once

class Entity;

class Script
{
public:
	Script();
	~Script();

	virtual void Update(Entity* parent);
};

