#include "ToggleBoolCommand.h"


ToggleBoolCommand::ToggleBoolCommand(bool& _toggle) : toggle(_toggle)
{
}

void ToggleBoolCommand::Execute(Entity* entity)
{
	toggle = !toggle;
}
