#pragma once

#include "Utilities.h"

#include <map>

class SystemManager;

class Entity;

struct Light
{
	glm::vec4 ambient_colours;
	glm::vec4 diffuse_colours;
	glm::vec4 specular_colours;
	glm::vec4 coords;
	float attenuation;
	float ambient_coefficient;
};
class SystemRenderer
{
public:

	SystemManager* system;

	static SystemRenderer* myself;

	//MAIN PROGRAM

	unsigned int program_id;

	char* vertex_shader;
	unsigned int vertex_shader_id;

	char* fragment_shader;
	unsigned int fragment_shader_id;

	glm::mat4 model_matrix = glm::mat4(1.0f);
	unsigned int model_matrix_location;

	glm::mat4 view_matrix = glm::mat4(1.0f);
	unsigned int view_matrix_location;

	glm::mat4 projection_matrix = glm::mat4(1.0f);
	unsigned int projection_matrix_location;

	glm::mat3 normal_matrix = glm::mat3(1.0f);
	unsigned int normal_matrix_location;

	glm::vec4 global_ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	unsigned int global_ambient_location;

	glm::vec4 camera_position = glm::vec4(1.0f);
	unsigned int camera_position_location;

	//DEBUG PROGRAM

	bool draw_debug = true;

	unsigned int debug_program_id;

	char* line_vertex_shader;
	unsigned int line_vertex_shader_id;

	char* line_fragment_shader;
	unsigned int line_fragment_shader_id;

	glm::mat4 line_model_matrix = glm::mat4(1.0f);
	unsigned int line_model_matrix_location;

	glm::mat4 line_view_matrix = glm::mat4(1.0f);
	unsigned int line_view_matrix_location;

	glm::mat4 line_projection_matrix = glm::mat4(1.0f);
	unsigned int line_projection_matrix_location;

	//OTHER

	std::map<Entity*, int> texture_position;
	int texture_position_count = 0;
	unsigned int texture_location;

	unsigned int vertex_material_ambient_reflection_location;
	unsigned int vertex_material_diffuse_reflection_location;
	unsigned int vertex_material_specular_reflection_location;
	unsigned int vertex_material_emission_location;
	unsigned int vertex_material_shine_location;
	unsigned int vertex_material_lighting_location;
	unsigned int light_count_location;

	Entity* skybox;


	SystemRenderer(SystemManager* sys);
	~SystemRenderer();

	glm::mat4 GetModelMatrix(Entity* entity);

	void Render();
	void static StaticRender();

	void ResizeWindow(int width, int height);
	void static StaticResizeWindow(int width, int height);

};

