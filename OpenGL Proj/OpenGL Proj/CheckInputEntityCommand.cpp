#include "CheckInputEntityCommand.h"
#include "Entity.h"
#include "SystemManager.h"

CheckInputEntityCommand::CheckInputEntityCommand(Entity* _check_entity, Command* _command)
{
	check_entity = _check_entity;
	stored_command = _command;
}


CheckInputEntityCommand::~CheckInputEntityCommand()
{
	delete stored_command;
}

void CheckInputEntityCommand::Execute(Entity* entity)
{

	if (entity == check_entity) stored_command->Execute(entity);
}
