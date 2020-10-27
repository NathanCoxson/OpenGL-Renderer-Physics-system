#include "SphereCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"
#include "SystemManager.h"

#define PI 3.14159265358979323846

#include <cmath>

SphereCollider::SphereCollider(Entity* parent, glm::vec3 offset_pos, float sphere_radius)
{
	entity = parent;
	position = offset_pos;
	radius = sphere_radius;

	glUseProgram(entity->system->renderer->debug_program_id);
	glGenVertexArrays(1, &(vao));
	glBindVertexArray(vao);
	glGenBuffers(1, &(vbo));

}

CollisionData SphereCollider::CheckCollision(ColliderMesh* other)
{
	return other->CheckCollision(this);
}

CollisionData SphereCollider::CheckCollision(SphereCollider* other)
{
	//std::cout << "SPHERE WITH SPHERE" << std::endl;
	glm::vec3 this_position = (*entity->system->buffer->render_buffer)[this->entity]->position + position;
	glm::vec3 other_position = (*entity->system->buffer->render_buffer)[other->entity]->position + other->position;

	float distance = glm::distance(this_position, other_position);
	if (distance < other->radius + radius)
	{
		return CollisionData(true, this_position + glm::normalize(other_position - this_position) * (radius / other->radius), glm::normalize(other_position - this_position), glm::vec3(((other->radius + radius) - distance)/2) , this->entity, other->entity);
	}
	else
	{
		return CollisionData();
	}
}

CollisionData SphereCollider::CheckCollision(AABBCollider* other)
{
	//std::cout << "SPHERE WITH AABB" << std::endl;
	glm::vec3 this_position = (*entity->system->buffer->render_buffer)[this->entity]->position + position;
	glm::vec3 other_position = (*entity->system->buffer->render_buffer)[other->entity]->position + other->position;

	float x = fmax(((*entity->system->buffer->render_buffer)[other->entity]->position.x + other->position.x - other->half_values.x), fmin((*entity->system->buffer->render_buffer)[entity]->position.x + position.x, ((*entity->system->buffer->render_buffer)[other->entity]->position.x + other->position.x + other->half_values.x)));
	float y = fmax(((*entity->system->buffer->render_buffer)[other->entity]->position.y + other->position.y - other->half_values.y), fmin((*entity->system->buffer->render_buffer)[entity]->position.y + position.y, ((*entity->system->buffer->render_buffer)[other->entity]->position.y + other->position.y + other->half_values.y)));
	float z = fmax(((*entity->system->buffer->render_buffer)[other->entity]->position.z + other->position.z - other->half_values.z), fmin((*entity->system->buffer->render_buffer)[entity]->position.z + position.z, ((*entity->system->buffer->render_buffer)[other->entity]->position.z + other->position.z + other->half_values.z)));

	if (glm::distance(glm::vec3(x, y, z), this_position) < radius)
	{
		glm::vec3 collision_normal = glm::normalize(glm::vec3(x, y, z) - this_position);
		float collision_depth = radius - glm::distance(glm::vec3(x, y, z), this_position);
		glm::vec3 collision_point = this_position + collision_normal * ((collision_depth / 2) + glm::distance(glm::vec3(x, y, z), this_position));
		return CollisionData(true, collision_point, collision_normal, glm::vec3(collision_depth), this->entity, other->entity);
	}
	else
	{
		return CollisionData();
	}
}

CollisionData SphereCollider::CheckCollision(OBBCollider* other)
{
	
	glm::vec3 this_position = (*entity->system->buffer->render_buffer)[this->entity]->position + position;
	glm::vec3 entity_position = (*entity->system->buffer->render_buffer)[other->entity]->position + other->position;
	glm::vec3 entity_rotation = (*entity->system->buffer->render_buffer)[other->entity]->rotation + other->rotation;
	glm::vec3 entity_scale = (*entity->system->buffer->render_buffer)[other->entity]->scale;

	glm::mat3 model_matrix = glm::mat3(entity->system->renderer->GetModelMatrix(other->entity));

	//glm::mat4 obb_model_matrix = glm::translate(glm::mat4(1.0f), entity_position);
	glm::mat4 obb_model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(entity_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	obb_model_matrix = glm::rotate(obb_model_matrix, glm::radians(entity_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	obb_model_matrix = glm::rotate(obb_model_matrix, glm::radians(entity_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	//obb_model_matrix = glm::scale(obb_model_matrix, entity_scale);

	glm::mat4 rotationMatrix = glm::yawPitchRoll(glm::radians(entity_rotation.y), glm::radians(entity_rotation.x), glm::radians(entity_rotation.z));
	glm::vec4 sphere_world_position = glm::vec4(((*entity->system->buffer->render_buffer)[entity]->position.x + position.x), ((*entity->system->buffer->render_buffer)[entity]->position.y + position.y), ((*entity->system->buffer->render_buffer)[entity]->position.z + position.z), 1.f);

	glm::mat4 sphere_local_position_matrix = glm::inverse(obb_model_matrix);
	glm::vec3 thingy = (glm::vec3(1.f/entity_scale.x, 1.f/entity_scale.y, 1.f/entity_scale.z))*((this_position - entity_position)*(glm::inverse(glm::quat_cast(rotationMatrix))));

	glm::vec3 sphere_local_position = glm::vec3(glm::vec4((this_position - entity_position), 1.0f) * obb_model_matrix);
	glm::vec4 obb_world_position = glm::vec4(((*entity->system->buffer->render_buffer)[other->entity]->position.x + other->position.x), ((*entity->system->buffer->render_buffer)[other->entity]->position.y + other->position.y), ((*entity->system->buffer->render_buffer)[other->entity]->position.z + other->position.z),1.f);
	glm::vec4 obb_local_position = obb_world_position * obb_model_matrix;
	glm::vec4 obb_extensions = glm::vec4((other->half_values), 1.f) * obb_model_matrix;

	float x = fmax((-other->half_values.x), fmin(sphere_local_position.x, (other->half_values.x)));
	float y = fmax((-other->half_values.y), fmin(sphere_local_position.y, (other->half_values.y)));
	float z = fmax((-other->half_values.z), fmin(sphere_local_position.z, (other->half_values.z)));

	glm::vec3 offset = (glm::vec3(sphere_local_position) - glm::vec3(x, y, z));

	glm::vec3 point = glm::vec3(glm::vec4(x, y, z, 1));


	if (radius > glm::distance(point, glm::vec3(sphere_local_position)))
	{
		glm::vec3 collision_normal = glm::normalize(entity_position - this_position);
		float collision_depth = radius - offset.length();
		glm::vec3 collision_point = glm::vec3(this_position) - collision_normal * ((collision_depth / 2) + glm::distance(point, glm::vec3(this_position)));
		return CollisionData(true, collision_point, collision_normal, glm::vec3(collision_depth), this->entity, other->entity);
	}
	else
	{
		return CollisionData();
	}
}

std::vector<glm::vec3> SphereCollider::GetPoints()
{
	std::vector<glm::vec3> store;

	glm::vec3 entity_position = (*entity->system->buffer->render_buffer)[entity]->position + position;

	float sector_count = 9, stack_count = 6;

	float sector_step = 2 * (PI / sector_count);
	float stack_step = PI / stack_count;

	for (int i = 0; i <= stack_count; ++i)
	{
		float stack_angle = PI / 2 - i * stack_step;
		float xz = radius * std::cosf(stack_angle);
		float y = radius * std::sinf(stack_angle);

		for (int j = 0; j <= sector_count; ++j)
		{
			float sector_angle = j * sector_step;
			float x = xz * std::cosf(sector_angle);
			float z = xz * std::sinf(sector_angle);

			store.push_back(glm::vec3(entity_position.x + x, entity_position.y + y, entity_position.z + z));
		}
	}

	return store;
}

bool SphereCollider::InArea(glm::vec3 point, glm::vec3 extents)
{
	glm::vec3 this_position = (*entity->system->buffer->render_buffer)[this->entity]->position + position;
	glm::vec3 other_position = point;

	float x = fmax((point.x - extents.x), fmin((*entity->system->buffer->render_buffer)[entity]->position.x + position.x, (point.x + extents.x)));
	float y = fmax((point.y - extents.y), fmin((*entity->system->buffer->render_buffer)[entity]->position.y + position.y, (point.y + extents.y)));
	float z = fmax((point.z - extents.z), fmin((*entity->system->buffer->render_buffer)[entity]->position.z + position.z, (point.z + extents.z)));

	return glm::distance(glm::vec3(x, y, z), this_position) < radius;
}
