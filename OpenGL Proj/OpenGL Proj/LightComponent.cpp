#include "LightComponent.h"
#include "SystemManager.h"


LightComponent::LightComponent(Entity* parent)
{
	entity = parent;
	entity->system->GetEntityGroup(entity, eLightEntities);
}


LightComponent::~LightComponent()
{
}

void LightComponent::Setup(ELightType _type, float _intensity, float _attenuation, float _ambient_coefficient, float _cone_angle, glm::vec4 _cone_direction, glm::vec4 _ambient_colours, glm::vec4 _diffuse_colours, glm::vec4 _specular_colours, glm::vec4 _offset_position)
{

	type = _type;
	intensity = _intensity;
	ambient_colours = _ambient_colours;
	diffuse_colours = _diffuse_colours;
	specular_colours = _specular_colours;
	offset_position = _offset_position;
	cone_direction = _cone_direction;
	attenuation = _attenuation;
	ambient_coefficient = _ambient_coefficient;
	cone_angle = _cone_angle;
	rotated_cone_direction = _cone_direction;

	entity->system->buffer->AllocateLightLocations(entity);

}

void LightComponent::GetRotatedCone()
{
	glm::vec3 entity_rotation = (*entity->system->buffer->render_buffer)[entity]->rotation;
	rotated_cone_direction = cone_direction * glm::inverse(glm::yawPitchRoll(glm::radians(entity_rotation.y), glm::radians(entity_rotation.x), glm::radians(entity_rotation.z)));
}

void LightComponent::Update()
{
}
