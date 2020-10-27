#include "ToggleCullOrientationCommand.h"

ToggleCullOrientationCommand::ToggleCullOrientationCommand(bool _toggle)
{
	toggled = _toggle;
	(toggled ? glCullFace(GL_FRONT) : glCullFace(GL_BACK));
}

void ToggleCullOrientationCommand::Execute(Entity* entity)
{
	toggled = !toggled;
	(toggled ? glCullFace(GL_FRONT) : glCullFace(GL_BACK));
}
