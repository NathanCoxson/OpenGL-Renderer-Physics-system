#version 430 core

#define MAX_NO_LIGHTS 16

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct Material
{
	vec4 ambient_reflection;
	vec4 diffuse_reflection;
	vec4 specular_reflection;
	vec4 emission;
	float shine;
	bool lighting;
};

struct Light
{
	int type;
	float intensity;
	vec4 ambient_colours;
	vec4 diffuse_colours;
	vec4 specular_colours;
	vec4 position;
	vec4 cone_direction;
	float attenuation;
	float ambient_coefficient;
	float cone_angle;
};

in vec2 fragment_texture_coords;
in vec3 fragment_normal;
in vec4 fragment_vertex;uniform int number_of_lights;
uniform mat3 normal_matrix;

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

uniform vec4 camera_position;

uniform vec4 global_ambient;

uniform Material fragment_material;

uniform Light lights[MAX_NO_LIGHTS];

uniform sampler2D image_texture;

out vec4 colours_out;

vec3 CalculateLightValues(Light light, vec4 texture_colour, vec3 normal, vec4 fragment_position, vec3 camera_direction)
{
	vec3 light_direction = vec3(0);
	float attenuation = 1.0;

	if (light.type == DIRECTIONAL_LIGHT)
	{
		light_direction = normalize(vec3(light.position));
	}
	else if (light.type == POINT_LIGHT)
	{
		light_direction = normalize(vec3(light.position) - vec3(fragment_position));
		float distance_to_light = length(vec3(light.position) - vec3(fragment_position));
		attenuation = 1.0 / (1.0 + light.attenuation * pow(distance_to_light, 2));
	}
	else if (light.type == SPOT_LIGHT)
	{
		light_direction = normalize(vec3(light.position) - vec3(fragment_position));
		float distance_to_light = length(vec3(light.position) - vec3(fragment_position));
		attenuation = 1.0 / (1.0 + light.attenuation * pow(distance_to_light, 2));

		float light_to_fragment_angle = degrees(acos(dot(-light_direction, normalize(vec3(light.cone_direction)))));
		if (light_to_fragment_angle > light.cone_angle)
		{
			attenuation = 0.0;
		}
	}
	else
	{
		return vec3(0);
	}


	float diffuse_coefficient = max(0.0, dot(light_direction, normal));

	vec3 halfway_direction = normalize(light_direction + vec3(normalize(-fragment_position)));

	float specular_coefficient = 0.0;
	if (diffuse_coefficient > 0.0) specular_coefficient = pow(max(0.0, dot(normal, halfway_direction)), fragment_material.shine);

	vec3 fragment_ambient = vec3(light.ambient_coefficient * texture_colour * light.ambient_colours * light.intensity);
	vec3 fragment_diffuse = vec3(diffuse_coefficient * texture_colour * fragment_material.diffuse_reflection * light.diffuse_colours * light.intensity);
	vec3 fragment_specular = vec3(specular_coefficient * fragment_material.specular_reflection * light.specular_colours * light.intensity);

	return fragment_ambient + attenuation * (fragment_diffuse + fragment_specular);

}

void main(void)
{
	vec4 texture_colour = texture(image_texture, fragment_texture_coords);
	
	if (!fragment_material.lighting)
	{
		colours_out = vec4(1.0, 1.0, 1.0, 1.0) * texture_colour;
		return;
	}

	vec3 normal = normalize(normal_matrix * fragment_normal);

	normal = (gl_FrontFacing ? normal : -normal);

	vec4 fragment_position = model_matrix * fragment_vertex;
	
	vec3 camera_direction = normalize(vec3(camera_position - fragment_position));

	vec3 linear_colour = vec3(0);
	for (int i = 0; i < number_of_lights; i++)
	{
		linear_colour += CalculateLightValues(lights[i], texture_colour, normal, fragment_position, camera_direction);
	}

	//vec3(1.0/2.2) for gamma correction

	colours_out = vec4(pow(linear_colour.rgb, vec3(1.0/2.2)), 1.0);

}
