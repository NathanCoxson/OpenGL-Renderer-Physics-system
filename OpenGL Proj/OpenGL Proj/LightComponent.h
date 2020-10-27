#pragma once
#include "Component.h"
#include "Transform.h"
#include <vector>

enum ELightType
{
	eDirectionalLight,
	ePointLight,
	eSpotLight
};

class LightComponent : public Component
{
public:

	Entity* entity;

	int index;
	ELightType type;
	float intensity;
	glm::vec4 ambient_colours;
	glm::vec4 diffuse_colours;
	glm::vec4 specular_colours;
	glm::vec4 offset_position;
	glm::vec4 cone_direction;
	glm::vec4 rotated_cone_direction;
	float attenuation;
	float ambient_coefficient;
	float cone_angle;

	LightComponent(Entity* parent);
	~LightComponent();

	void Setup(ELightType _type, float _intensity, float _attenuation, float _ambient_coefficient, float _cone_angle, glm::vec4 _cone_direction, glm::vec4 _ambient_colours, glm::vec4 _diffuse_colours, glm::vec4 _specular_colours, glm::vec4 _offset_position);
	void GetRotatedCone();
	void Update();
};

