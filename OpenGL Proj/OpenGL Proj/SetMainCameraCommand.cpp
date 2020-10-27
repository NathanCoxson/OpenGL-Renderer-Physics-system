#include "SetMainCameraCommand.h"
#include "Entity.h"
#include "SystemManager.h"


SetMainCameraCommand::SetMainCameraCommand(Entity* camera_pointer)
{
	camera = camera_pointer;
}

void SetMainCameraCommand::Execute(Entity* entity)
{
	entity->system->main_camera = camera;
}


