#include "MultiCommand.h"

MultiCommand::MultiCommand(std::vector<Command*> &_commands)
{
	commands = _commands;
}

MultiCommand::~MultiCommand()
{
	for (int i = 0; i < commands.size(); i++)
	{
		delete commands[i];
	}
}

void MultiCommand::Execute(Entity* entity)
{
	for (int i = 0; i < commands.size(); i++)
	{
		commands[i]->Execute(entity);
	}
}
