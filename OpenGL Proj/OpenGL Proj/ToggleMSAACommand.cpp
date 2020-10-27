#include "ToggleMSAACommand.h"

ToggleMSAACommand::ToggleMSAACommand(bool _toggle)
{
	toggled = _toggle;
	(toggled ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE));
}

void ToggleMSAACommand::Execute(Entity* entity)
{
	toggled = !toggled;
	(toggled ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE));
}
