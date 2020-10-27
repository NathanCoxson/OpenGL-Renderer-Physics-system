#include "SystemBuffer.h"
#include "SystemManager.h"

SystemBuffer::SystemBuffer(SystemManager* sys)
{
	system = sys;
	render_buffer = new std::map<Entity*, Transform*>;
	physics_buffer = new std::map<Entity*, Transform*>;
}

SystemBuffer::~SystemBuffer()
{
	delete render_buffer;
	delete physics_buffer;
}

bool SystemBuffer::AllocateEntityBuffer(Entity* entity)
{
	buffer_add_queue.emplace_back(entity);
	return true;
}

bool SystemBuffer::DeleteEntityBuffer(Entity* entity)
{
	buffer_delete_queue.emplace_back(entity);
	return true;
}

bool SystemBuffer::AllocateEntityTransform(Entity* entity)
{
	transform_add_queue.emplace_back(entity);
	return true;
}

bool SystemBuffer::DeleteEntityTransform(Entity* entity)
{
	transform_delete_queue.emplace_back(entity);
	return true;
}

bool SystemBuffer::AllocateLightLocations(Entity* entity)
{

	LightComponent* light = ((LightComponent*)entity->components[eLight]);
	light->index = light_locations.size();

	light_locations[entity] = std::vector<GLuint>(10);

	light_locations[entity][0] = glGetUniformLocation(system->renderer->program_id, ("lights[" + std::to_string(light->index) + "].type").c_str());
	light_locations[entity][1] = glGetUniformLocation(system->renderer->program_id, ("lights[" + std::to_string(light->index) + "].intensity").c_str());
	light_locations[entity][2] = glGetUniformLocation(system->renderer->program_id, ("lights[" + std::to_string(light->index) + "].ambient_colours").c_str());
	light_locations[entity][3] = glGetUniformLocation(system->renderer->program_id, ("lights[" + std::to_string(light->index) + "].diffuse_colours").c_str());
	light_locations[entity][4] = glGetUniformLocation(system->renderer->program_id, ("lights[" + std::to_string(light->index) + "].specular_colours").c_str());
	light_locations[entity][5] = glGetUniformLocation(system->renderer->program_id, ("lights[" + std::to_string(light->index) + "].position").c_str());
	light_locations[entity][6] = glGetUniformLocation(system->renderer->program_id, ("lights[" + std::to_string(light->index) + "].cone_direction").c_str());
	light_locations[entity][7] = glGetUniformLocation(system->renderer->program_id, ("lights[" + std::to_string(light->index) + "].attenuation").c_str());
	light_locations[entity][8] = glGetUniformLocation(system->renderer->program_id, ("lights[" + std::to_string(light->index) + "].ambient_coefficient").c_str());
	light_locations[entity][9] = glGetUniformLocation(system->renderer->program_id, ("lights[" + std::to_string(light->index) + "].cone_angle").c_str());
	return true;
}

bool SystemBuffer::SetEntityTransformPosition(Entity* entity, glm::vec3 position)
{
	transform_position_set[entity] = position;
	return true;
}

bool SystemBuffer::SetEntityTransformRotation(Entity* entity, glm::vec3 rotation)
{
	transform_rotation_set[entity] = rotation;
	return true;
}

bool SystemBuffer::SetEntityTransformScale(Entity* entity, glm::vec3 scale)
{
	transform_scale_set[entity] = scale;
	return true;
}

bool SystemBuffer::ProcessTransformQueue()
{
	for (auto entity_pointer : transform_add_queue)
	{
		(*render_buffer)[entity_pointer] = new Transform();
		(*physics_buffer)[entity_pointer] = new Transform();
	}

	transform_add_queue.clear();

	return true;
}

bool SystemBuffer::ProcessBufferQueue()
{
	for (auto entity_pointer : buffer_add_queue)
	{

		std::string entity_mesh = ((RenderComponent*)entity_pointer->components[eRender])->mesh;

		glUseProgram(system->renderer->program_id);

		glGenVertexArrays(1, &(vao_to_entity[entity_pointer]));
		glBindVertexArray(vao_to_entity[entity_pointer]);

		glGenBuffers(1, &(vbo_to_entity[entity_pointer]));
		glBindBuffer(GL_ARRAY_BUFFER, vbo_to_entity[entity_pointer]);
		glBufferData(GL_ARRAY_BUFFER, system->render_meshes[entity_mesh]->vertices.size() * sizeof(Vertex), &(system->render_meshes[entity_mesh]->vertices[0]), GL_STATIC_DRAW);

		system->renderer->texture_position[entity_pointer] = system->renderer->texture_position_count;
		system->renderer->texture_position_count++;

		glGenTextures(1, &(texture_to_entity[entity_pointer]));
		glActiveTexture(GL_TEXTURE0 + system->renderer->texture_position[entity_pointer]);
		glBindTexture(GL_TEXTURE_2D, texture_to_entity[entity_pointer]);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, (system->render_meshes[entity_mesh]->bitmap_texture->size_x), (system->render_meshes[entity_mesh]->bitmap_texture->size_y), 0, GL_RGBA, GL_UNSIGNED_BYTE, (system->render_meshes[entity_mesh]->bitmap_texture->data));
		glGenerateMipmap(GL_TEXTURE_2D);
		glUniform1i(system->renderer->texture_location, system->renderer->texture_position[entity_pointer]);
		//glUniform1i(system->renderer->texture_location, texture_to_entity[entity_pointer]);


		
		//This is disgusting. Basically, it takes the size of one vertex and multiplies it by how many verticies are in the vector.
		//This is because it expect an array and size vectors have more overhead they have more size and give the wrong info to opengl

		/*glGenBuffers(1, &(ibo_to_entity[entity_pointer]));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_to_entity[entity_pointer]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, system->render_meshes[entity_mesh]->indices.size() * sizeof(unsigned int), &(system->render_meshes[entity_mesh]->indices[0]), GL_STATIC_DRAW);*/

		//To use vectors, you must point to the start of the vector, similar to how arrays work. Won't accept a vector allocator

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(4 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(7 * sizeof(float)));
		glEnableVertexAttribArray(2);

	}

	buffer_add_queue.clear();

	return true;
}

bool SystemBuffer::ProcessTransformPRS()
{
	for (auto entity_position : transform_position_set)
	{

		(*physics_buffer)[entity_position.first]->position = entity_position.second;
		(*render_buffer)[entity_position.first]->position = entity_position.second;
		int a = 1;
	}

	for (auto entity_rotation : transform_rotation_set)
	{
		(*physics_buffer)[entity_rotation.first]->rotation = entity_rotation.second;
		(*render_buffer)[entity_rotation.first]->rotation = entity_rotation.second;

	}

	for (auto entity_scale : transform_scale_set)
	{
		(*physics_buffer)[entity_scale.first]->scale = entity_scale.second;
		(*render_buffer)[entity_scale.first]->scale = entity_scale.second;
	}

	transform_position_set.clear();
	transform_rotation_set.clear();
	transform_scale_set.clear();

	return true;
}

bool SystemBuffer::SwapSystemBuffers()
{
	std::map<Entity*, Transform*>* temp1 = render_buffer;
	std::map<Entity*, Transform*>* temp2 = physics_buffer;

	physics_buffer = temp1;
	render_buffer = temp2;

	return true;
}

bool SystemBuffer::Tick()
{
	ProcessBufferQueue();
	ProcessTransformQueue();
	ProcessTransformPRS();
	SwapSystemBuffers();
	return true;
}
