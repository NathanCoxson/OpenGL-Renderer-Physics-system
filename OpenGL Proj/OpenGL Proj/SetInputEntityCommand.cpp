#include "SetInputEntityCommand.h"
#include "Entity.h"
#include "SystemManager.h"


SetInputEntityCommand::SetInputEntityCommand(Entity* _entity)
{
	input_entity = _entity;
}

SetInputEntityCommand::~SetInputEntityCommand()
{
}

void SetInputEntityCommand::Execute(Entity* entity)
{
	entity->system->input_focus_entity = input_entity;
}
