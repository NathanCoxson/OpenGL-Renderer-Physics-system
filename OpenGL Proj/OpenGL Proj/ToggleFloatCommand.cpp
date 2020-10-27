#include "ToggleFloatCommand.h"
#include "Entity.h"
#include "SystemManager.h"


ToggleFloatCommand::ToggleFloatCommand(float* _reference, float value1, float value2) : ref(_reference), val1(value1), val2(value2)
{
}


ToggleFloatCommand::~ToggleFloatCommand()
{
}

void ToggleFloatCommand::Execute(Entity* entity)
{
	if (*ref == val1) * ref = val2;
	else if (*ref == val2) * ref = val1;
}
