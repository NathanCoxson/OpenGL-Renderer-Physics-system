#include "SystemRenderer.h"
#include "SystemManager.h"

SystemRenderer* SystemRenderer::myself;

SystemRenderer::SystemRenderer(SystemManager* sys) 
{
	system = sys;

	vertex_shader = ParseFile("VertexShader.glsl");
	vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_id, 1, (const char**)& vertex_shader, NULL);
	glCompileShader(vertex_shader_id);
	std::cout << "VERTEX SHADER" << std::endl;
	shaderCompileTest(vertex_shader_id);

	fragment_shader = ParseFile("FragmentShader.glsl");
	fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_id, 1, (const char**)& fragment_shader, NULL);
	glCompileShader(fragment_shader_id);
	std::cout << "FRAGMENT SHADER" << std::endl;
	shaderCompileTest(fragment_shader_id);

	line_vertex_shader = ParseFile("LineVertexShader.glsl");
	line_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(line_vertex_shader_id, 1, (const char**)& line_vertex_shader, NULL);
	glCompileShader(line_vertex_shader_id);
	std::cout << "LINE VERTEX SHADER" << std::endl;
	shaderCompileTest(line_vertex_shader_id);

	line_fragment_shader = ParseFile("LineFragmentShader.glsl");
	line_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(line_fragment_shader_id, 1, (const char**)& line_fragment_shader, NULL);
	glCompileShader(line_fragment_shader_id);
	std::cout << "LINE FRAGMENT SHADER" << std::endl;
	shaderCompileTest(line_fragment_shader_id);

	debug_program_id = glCreateProgram();
	glAttachShader(debug_program_id, line_vertex_shader_id);
	glAttachShader(debug_program_id, line_fragment_shader_id);
	glLinkProgram(debug_program_id);
	glUseProgram(debug_program_id);

	line_projection_matrix = glm::perspective(90.f, 1920.f / 1080.f, 0.01f, 10000.0f);
	line_projection_matrix_location = glGetUniformLocation(debug_program_id, "line_projection_matrix");
	glUniformMatrix4fv(line_projection_matrix_location, 1, GL_FALSE, glm::value_ptr(line_projection_matrix));

	line_model_matrix = glm::mat4(1.0f);
	line_model_matrix_location = glGetUniformLocation(debug_program_id, "line_model_matrix");
	glUniformMatrix4fv(line_model_matrix_location, 1, GL_FALSE, glm::value_ptr(line_model_matrix));

	line_view_matrix = glm::lookAt(glm::vec3(0, 20, 40), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	line_view_matrix_location = glGetUniformLocation(debug_program_id, "line_view_matrix");
	glUniformMatrix4fv(line_view_matrix_location, 1, GL_FALSE, glm::value_ptr(line_view_matrix));

	program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);
	glUseProgram(program_id);
	
	projection_matrix = glm::perspective(90.f, 1920.f / 1080.f, 0.01f, 10000.0f);
	projection_matrix_location = glGetUniformLocation(program_id, "projection_matrix");
	glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, glm::value_ptr(projection_matrix)); 

	model_matrix = glm::mat4(1.0f);
	model_matrix_location = glGetUniformLocation(program_id, "model_matrix");
	glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, glm::value_ptr(model_matrix));

	view_matrix = glm::lookAt(glm::vec3(0, 20, 40), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	view_matrix_location = glGetUniformLocation(program_id, "view_matrix");
	glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, glm::value_ptr(view_matrix));

	normal_matrix = glm::mat3(1.0f);
	normal_matrix_location = glGetUniformLocation(program_id, "normal_matrix");
	glUniformMatrix3fv(normal_matrix_location, 1, GL_FALSE, glm::value_ptr(normal_matrix));

	global_ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	global_ambient_location = glGetUniformLocation(program_id, "global_ambient");
	glUniform4fv(global_ambient_location, 1, glm::value_ptr(global_ambient));

	camera_position = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	camera_position_location = glGetUniformLocation(program_id, "camera_position");
	glUniform4fv(camera_position_location, 1, glm::value_ptr(camera_position));

	vertex_material_ambient_reflection_location = glGetUniformLocation(program_id, "fragment_material.ambient_reflection");
	vertex_material_diffuse_reflection_location = glGetUniformLocation(program_id, "fragment_material.diffuse_reflection");
	vertex_material_specular_reflection_location = glGetUniformLocation(program_id, "fragment_material.specular_reflection");
	vertex_material_emission_location = glGetUniformLocation(program_id, "fragment_material.emission");
	vertex_material_shine_location = glGetUniformLocation(program_id, "fragment_material.shine");
	vertex_material_lighting_location = glGetUniformLocation(program_id, "fragment_material.lighting");

	light_count_location = glGetUniformLocation(program_id, "number_of_lights");

	/*light_locations[0] = glGetUniformLocation(program_id, "light.type");
	light_locations[1] = glGetUniformLocation(program_id, "light.intensity");
	light_locations[2] = glGetUniformLocation(program_id, "light.ambient_colours");
	light_locations[3] = glGetUniformLocation(program_id, "light.diffuse_colours");
	light_locations[4] = glGetUniformLocation(program_id, "light.specular_colours");
	light_locations[5] = glGetUniformLocation(program_id, "light.position");
	light_locations[6] = glGetUniformLocation(program_id, "light.cone_direction");
	light_locations[7] = glGetUniformLocation(program_id, "light.attenuation");
	light_locations[8] = glGetUniformLocation(program_id, "light.ambient_coefficient");
	light_locations[9] = glGetUniformLocation(program_id, "light.cone_angle");*/

	/*glUniform4fv(glGetUniformLocation(program_id, "light.ambient_colours"), 1, &main_light.ambient_colours[0]);
	glUniform4fv(glGetUniformLocation(program_id, "light.diffuse_colours"), 1, &main_light.diffuse_colours[0]);
	glUniform4fv(glGetUniformLocation(program_id, "light.specular_colours"), 1, &main_light.specular_colours[0]);
	glUniform4fv(glGetUniformLocation(program_id, "light.coords"), 1, &main_light.coords[0]);
	glUniform1fv(glGetUniformLocation(program_id, "light.attenuation"), 1, &main_light.attenuation);
	glUniform1fv(glGetUniformLocation(program_id, "light.ambient_coefficient"), 1, &main_light.ambient_coefficient);*/

	texture_location = glGetUniformLocation(program_id, "image_texture");
}


SystemRenderer::~SystemRenderer()
{

}

glm::mat4 SystemRenderer::GetModelMatrix(Entity* entity)
{
	glm::vec3 entity_rotation = (*system->buffer->render_buffer)[entity]->rotation;

	glm::mat4 model_matrix_new = glm::translate(glm::mat4(1.0f), (*system->buffer->render_buffer)[entity]->position);
	model_matrix_new = glm::rotate(model_matrix_new, glm::radians(entity_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix_new = glm::rotate(model_matrix_new, glm::radians(entity_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model_matrix_new = glm::rotate(model_matrix_new, glm::radians(entity_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model_matrix_new = glm::scale(model_matrix_new, (*system->buffer->render_buffer)[entity]->scale);
	

	return model_matrix_new;
}


void SystemRenderer::Render()
{
	glm::mat4 view_matrix_new = (dynamic_cast<CameraComponent*>((*system->main_camera)[eCamera]))->GetCameraMatrix();
	glm::vec4 camera_position_new = (dynamic_cast<CameraComponent*>((*system->main_camera)[eCamera]))->GetCameraPosition();

	glUseProgram(program_id);

	glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, glm::value_ptr(view_matrix_new));
	glUniform4fv(camera_position_location, 1, glm::value_ptr(camera_position_new));

	glUniform1i(light_count_location, int((*(system->entity_groups)[eLightEntities]).size()));

	(*system->buffer->render_buffer)[system->renderer->skybox]->position = glm::vec3(((CameraComponent*)(system->main_camera->components[eCamera]))->GetCameraPosition());

	for (Entity* light_entity_pointer : *(system->entity_groups)[eLightEntities])
	{
		LightComponent* light = ((LightComponent*)light_entity_pointer->components[eLight]);

		glUniform1i(system->buffer->light_locations[light_entity_pointer][0], (light->type));
		glUniform1f(system->buffer->light_locations[light_entity_pointer][1], (light->intensity));
		glUniform4fv(system->buffer->light_locations[light_entity_pointer][2], 1, &(light->ambient_colours[0]));
		glUniform4fv(system->buffer->light_locations[light_entity_pointer][3], 1, &(light->diffuse_colours[0]));
		glUniform4fv(system->buffer->light_locations[light_entity_pointer][4], 1, &(light->specular_colours[0]));
		glUniform4fv(system->buffer->light_locations[light_entity_pointer][5], 1, &(glm::vec4((*system->buffer->render_buffer)[light_entity_pointer]->position, 1.0f) + light->offset_position)[0]);	
		light->GetRotatedCone();
		glUniform4fv(system->buffer->light_locations[light_entity_pointer][6], 1, &(light->rotated_cone_direction[0]));
		glUniform1f(system->buffer->light_locations[light_entity_pointer][7], (light->attenuation));
		glUniform1f(system->buffer->light_locations[light_entity_pointer][8], (light->ambient_coefficient));
		glUniform1f(system->buffer->light_locations[light_entity_pointer][9], (light->cone_angle));
	}

	for (Entity* entity_pointer : *(system->entity_groups)[eRenderEntities])
	{
		
		std::string entity_mesh = ((RenderComponent*)entity_pointer->components[eRender])->mesh;

		glm::mat4 model_matrix_new = GetModelMatrix(entity_pointer);

		glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, glm::value_ptr(model_matrix_new));
		glUniformMatrix3fv(normal_matrix_location, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(model_matrix_new)))));

		glUniform4fv(vertex_material_ambient_reflection_location, 1, glm::value_ptr(system->render_meshes[entity_mesh]->material.ambient_reflection));
		glUniform4fv(vertex_material_diffuse_reflection_location, 1, glm::value_ptr(system->render_meshes[entity_mesh]->material.diffuse_reflection));
		glUniform4fv(vertex_material_specular_reflection_location, 1, glm::value_ptr(system->render_meshes[entity_mesh]->material.specular_reflection));
		glUniform4fv(vertex_material_emission_location, 1, glm::value_ptr(system->render_meshes[entity_mesh]->material.emission));
		glUniform1f(vertex_material_shine_location, system->render_meshes[entity_mesh]->material.shine);
		glUniform1f(vertex_material_lighting_location, system->render_meshes[entity_mesh]->material.lighting);

		glActiveTexture(GL_TEXTURE0 + system->renderer->texture_position[entity_pointer]);
		glBindTexture(GL_TEXTURE_2D, system->buffer->texture_to_entity[entity_pointer]);
		glUniform1i(texture_location, system->renderer->texture_position[entity_pointer]);
		glBindVertexArray(system->buffer->vao_to_entity[entity_pointer]);

		glDrawArrays(GL_TRIANGLES, 0, system->render_meshes[entity_mesh]->vertices.size());
	}

	if (draw_debug)
	{
		glUseProgram(debug_program_id);
		glUniformMatrix4fv(line_view_matrix_location, 1, GL_FALSE, glm::value_ptr(view_matrix_new));

		for (OctTreeBroadPhase* oct_tree_instance : system->colliders->all_oct_trees)
		{
			glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), oct_tree_instance->position);
			glUniformMatrix4fv(line_model_matrix_location, 1, GL_FALSE, glm::value_ptr(model_matrix));
			glBindVertexArray(oct_tree_instance->vao);
			glDrawArrays(GL_LINES, 0, oct_tree_instance->vertices.size());
		}

		for (auto mesh : system->collider_meshes)
		{
			std::vector<glm::vec3> vertices = mesh.second->GetPoints();

			glUniformMatrix4fv(line_model_matrix_location, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
			glBindVertexArray(mesh.second->vao);

			glBindBuffer(GL_ARRAY_BUFFER, mesh.second->vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, &(vertices[0]), GL_DYNAMIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
			glEnableVertexAttribArray(0);
			glDrawArrays(GL_POINTS, 0, vertices.size());
		}
		
	}

}

void SystemRenderer::StaticRender()
{
	myself->Render();
}

void SystemRenderer::ResizeWindow(int width, int height)
{
	glViewport(0, 0, width, height);
}

void SystemRenderer::StaticResizeWindow(int width, int height)
{
	myself->ResizeWindow(width, height);
}




