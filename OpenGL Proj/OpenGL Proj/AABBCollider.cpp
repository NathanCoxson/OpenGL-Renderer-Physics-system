#include "SphereCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"
#include "SystemManager.h"

AABBCollider::AABBCollider(Entity* parent, glm::vec3 offset_pos, glm::vec3 offset_half_values, bool _scale_to_mesh)
{
	entity = parent;
	position = offset_pos;
	half_values = offset_half_values;
	scale_to_mesh = _scale_to_mesh;

	if (scale_to_mesh)
	{
		std::string render_mesh = ((RenderComponent*)entity->components[eRender])->mesh;
		glm::vec3 positive_extents;
		glm::vec3 negative_extents;
		for (auto vectors : entity->system->render_meshes[render_mesh]->vertices)
		{
			auto value = glm::vec4(vectors.coordinates[0], vectors.coordinates[1], vectors.coordinates[2], vectors.coordinates[3]);
			positive_extents.x = (positive_extents.x > value.x ? positive_extents.x : value.x);
			positive_extents.y = (positive_extents.y > value.y ? positive_extents.y : value.y);
			positive_extents.z = (positive_extents.z > value.z ? positive_extents.z : value.z);

			negative_extents.x = (negative_extents.x < value.x ? negative_extents.x : value.x);
			negative_extents.y = (negative_extents.y < value.y ? negative_extents.y : value.y);
			negative_extents.z = (negative_extents.z < value.z ? negative_extents.z : value.z);
		}

		basic_vertices = {
			{positive_extents.x, positive_extents.y, positive_extents.z},
			{negative_extents.x, positive_extents.y, positive_extents.z},
			{positive_extents.x, negative_extents.y, positive_extents.z},
			{negative_extents.x, negative_extents.y, positive_extents.z},
			{positive_extents.x, positive_extents.y, negative_extents.z},
			{negative_extents.x, positive_extents.y, negative_extents.z},
			{positive_extents.x, negative_extents.y, negative_extents.z},
			{negative_extents.x, negative_extents.y, negative_extents.z}
		};
	}

	glUseProgram(entity->system->renderer->debug_program_id);
	glGenVertexArrays(1, &(vao));
	glBindVertexArray(vao);
	glGenBuffers(1, &(vbo));
}


CollisionData AABBCollider::CheckCollision(ColliderMesh* other)
{
	return other->CheckCollision(this);
}

CollisionData AABBCollider::CheckCollision(SphereCollider* other)
{
	return other->CheckCollision(this);
}

CollisionData AABBCollider::CheckCollision(AABBCollider* other)
{
	//std::cout << "AABB WITH AABB" << std::endl;

	if (scale_to_mesh)
	{
		glm::vec3 entity_position = (*entity->system->buffer->render_buffer)[entity]->position + position;
		glm::vec3 entity_rotation = (*entity->system->buffer->render_buffer)[entity]->rotation + rotation;
		glm::vec3 entity_scale = (*entity->system->buffer->render_buffer)[entity]->scale;
		glm::mat4 rotationMatrix = glm::yawPitchRoll(glm::radians(entity_rotation.y), glm::radians(entity_rotation.x), glm::radians(entity_rotation.z));

		glm::vec3 positive_extents = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 negative_extents = glm::vec3(0.f, 0.f, 0.f);
		std::string render_mesh = ((RenderComponent*)entity->components[eRender])->mesh;
		for (auto vector : basic_vertices)
		{
			auto rotation_values = glm::vec4(vector, 1) * rotationMatrix;
			positive_extents.x = (positive_extents.x > rotation_values.x ? positive_extents.x : rotation_values.x);
			positive_extents.y = (positive_extents.y > rotation_values.y ? positive_extents.y : rotation_values.y);
			positive_extents.z = (positive_extents.z > rotation_values.z ? positive_extents.z : rotation_values.z);

			negative_extents.x = (negative_extents.x < rotation_values.x ? negative_extents.x : rotation_values.x);
			negative_extents.y = (negative_extents.y < rotation_values.y ? negative_extents.y : rotation_values.y);
			negative_extents.z = (negative_extents.z < rotation_values.z ? negative_extents.z : rotation_values.z);
		}

		positive_extents *= entity_scale;
		negative_extents *= entity_scale;

		half_values = glm::vec3((positive_extents.x > negative_extents.x) ? positive_extents.x : abs(negative_extents.x), (positive_extents.y > negative_extents.y) ? positive_extents.y : abs(negative_extents.y), (positive_extents.z > negative_extents.z) ? positive_extents.z : abs(negative_extents.z));


	}

	glm::vec3 this_position = (*entity->system->buffer->render_buffer)[entity]->position + position;
	glm::vec3 other_position = (*entity->system->buffer->render_buffer)[other->entity]->position + other->position;

	if (!(this_position.x - half_values.x <= other_position.x + other->half_values.x)) return CollisionData();
	if (!(this_position.x + half_values.x >= other_position.x - other->half_values.x)) return CollisionData();
	if (!(this_position.y - half_values.y <= other_position.y + other->half_values.y)) return CollisionData();
	if (!(this_position.y + half_values.y >= other_position.y - other->half_values.y)) return CollisionData();
	if (!(this_position.z - half_values.z <= other_position.z + other->half_values.z)) return CollisionData();
	if (!(this_position.z + half_values.z >= other_position.z - other->half_values.z)) return CollisionData();
	
	glm::vec3 collision_point = (glm::vec3(glm::max(this_position.x - half_values.x, other_position.x - other->half_values.x), glm::max(this_position.y - half_values.y, other_position.y - other->half_values.y), glm::max(this_position.z - half_values.z, other_position.z - other->half_values.z)) + glm::vec3(glm::min(this_position.x + half_values.x, other_position.x + other->half_values.x), glm::min(this_position.y + half_values.y, other_position.y + other->half_values.y), glm::min(this_position.z + half_values.z, other_position.z + other->half_values.z))) / 2.f;
	glm::vec3 collision_normal = glm::normalize(this_position - collision_point);
	glm::vec3 collision_depth = glm::vec3(glm::distance(this_position + (glm::normalize(collision_point - this_position) * half_values), collision_point));
	return CollisionData(true, collision_point, collision_normal, collision_depth, this->entity, other->entity);
}

CollisionData AABBCollider::CheckCollision(OBBCollider* other)
{
	//std::cout << "AABB WITH OBB" << std::endl;

	if (scale_to_mesh)
	{
		glm::vec3 entity_position = (*entity->system->buffer->render_buffer)[entity]->position + position;
		glm::vec3 entity_rotation = (*entity->system->buffer->render_buffer)[entity]->rotation + rotation;
		glm::vec3 entity_scale = (*entity->system->buffer->render_buffer)[entity]->scale;
		glm::mat4 rotationMatrix = glm::yawPitchRoll(glm::radians(entity_rotation.y), glm::radians(entity_rotation.x), glm::radians(entity_rotation.z));

		glm::vec3 positive_extents = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 negative_extents = glm::vec3(0.f, 0.f, 0.f);
		std::string render_mesh = ((RenderComponent*)entity->components[eRender])->mesh;
		for (auto vector : basic_vertices)
		{
			auto rotation_values = glm::vec4(vector, 1) * rotationMatrix;
			positive_extents.x = (positive_extents.x > rotation_values.x ? positive_extents.x : rotation_values.x);
			positive_extents.y = (positive_extents.y > rotation_values.y ? positive_extents.y : rotation_values.y);
			positive_extents.z = (positive_extents.z > rotation_values.z ? positive_extents.z : rotation_values.z);

			negative_extents.x = (negative_extents.x < rotation_values.x ? negative_extents.x : rotation_values.x);
			negative_extents.y = (negative_extents.y < rotation_values.y ? negative_extents.y : rotation_values.y);
			negative_extents.z = (negative_extents.z < rotation_values.z ? negative_extents.z : rotation_values.z);
		}

		positive_extents *= entity_scale;
		negative_extents *= entity_scale;

		half_values = glm::vec3((positive_extents.x > negative_extents.x) ? positive_extents.x : abs(negative_extents.x), (positive_extents.y > negative_extents.y) ? positive_extents.y : abs(negative_extents.y), (positive_extents.z > negative_extents.z) ? positive_extents.z : abs(negative_extents.z));
	}

	glm::vec3 a_pos = (*entity->system->buffer->render_buffer)[entity]->position + position;
	glm::vec3 b_pos = (*entity->system->buffer->render_buffer)[other->entity]->position + other->position;

	glm::vec3 t = b_pos - a_pos;

	glm::vec3 entity_rotation2 = (*entity->system->buffer->render_buffer)[other->entity]->rotation + other->rotation;

	glm::mat4 rotation_matrix = glm::yawPitchRoll(glm::radians(0.f), glm::radians(0.f), glm::radians(0.f));
	glm::mat4 rotationMatrix2 = glm::yawPitchRoll(glm::radians(entity_rotation2.y), glm::radians(entity_rotation2.x), glm::radians(entity_rotation2.z));

	glm::vec4 rotX = glm::vec4(1, 0, 0, 1) * rotation_matrix;
	glm::vec4 rotY = glm::vec4(0, 1, 0, 1) * rotation_matrix;
	glm::vec4 rotZ = glm::vec4(0, 0, 1, 1) * rotation_matrix;

	glm::vec4 rot2X = glm::vec4(1, 0, 0, 1) * rotationMatrix2;
	glm::vec4 rot2Y = glm::vec4(0, 1, 0, 1) * rotationMatrix2;
	glm::vec4 rot2Z = glm::vec4(0, 0, 1, 1) * rotationMatrix2;

	glm::vec3 a_x = glm::vec3(rotX.x, rotX.y, rotX.z);
	glm::vec3 a_y = glm::vec3(rotY.x, rotY.y, rotY.z);
	glm::vec3 a_z = glm::vec3(rotZ.x, rotZ.y, rotZ.z);

	glm::vec3 b_x = glm::vec3(rot2X.x, rot2X.y, rot2X.z);
	glm::vec3 b_y = glm::vec3(rot2Y.x, rot2Y.y, rot2Y.z);
	glm::vec3 b_z = glm::vec3(rot2Z.x, rot2Z.y, rot2Z.z);

	float test_a;
	float test_b;

	float smallest_depth = 100;
	glm::vec3 axis = glm::vec3(0);

	//1
	test_a = abs(glm::dot(t, a_x));
	test_b = half_values.x + abs(other->half_values.x * glm::dot(a_x, b_x)) + abs(other->half_values.y * glm::dot(a_x, b_y)) + abs(other->half_values.z * glm::dot(a_x, b_z));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();

	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = a_x;
	}
	//2
	test_a = abs(glm::dot(t, a_y));
	test_b = half_values.y + abs(other->half_values.x * glm::dot(a_y, b_x)) + abs(other->half_values.y * glm::dot(a_y, b_y)) + abs(other->half_values.z * glm::dot(a_y, b_z));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();
	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = a_y;
	}
	//3
	test_a = abs(glm::dot(t, a_z));
	test_b = half_values.z + abs(other->half_values.x * glm::dot(a_z, b_x)) + abs(other->half_values.y * glm::dot(a_z, b_y)) + abs(other->half_values.z * glm::dot(a_z, b_z));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();
	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = a_z;
	}
	//4
	test_a = abs(glm::dot(t, b_x));
	test_b = abs(half_values.x * glm::dot(a_x, b_x)) + abs(half_values.y * glm::dot(a_y, b_x)) + abs(half_values.z * glm::dot(a_z, b_x)) + other->half_values.x;
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();
	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = b_x;
	}
	//5
	test_a = abs(glm::dot(t, b_y));
	test_b = abs(half_values.x * glm::dot(a_x, b_y)) + abs(half_values.y * glm::dot(a_y, b_y)) + abs(half_values.z * glm::dot(a_z, b_y)) + other->half_values.y;
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();
	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = b_y;
	}
	//6
	test_a = abs(glm::dot(t, b_z));
	test_b = abs(half_values.x * glm::dot(a_x, b_z)) + abs(half_values.y * glm::dot(a_y, b_z)) + abs(half_values.z * glm::dot(a_z, b_z)) + other->half_values.z;
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();
	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = b_z;
	}
	//7
	test_a = abs(glm::dot(t, a_z) * glm::dot(a_y, b_x) - glm::dot(t, a_y) * glm::dot(a_z, b_x));
	test_b = abs(half_values.y * glm::dot(a_z, b_x)) + abs(half_values.z * glm::dot(a_y, b_x)) + abs(other->half_values.y * glm::dot(a_x, b_z)) + abs(other->half_values.z * glm::dot(a_x, b_y));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();
	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = glm::cross(a_x, b_x);
	}
	//8
	test_a = abs(glm::dot(t, a_z) * glm::dot(a_y, b_y) - glm::dot(t, a_y) * glm::dot(a_z, b_y));
	test_b = abs(half_values.y * glm::dot(a_z, b_y)) + abs(half_values.z * glm::dot(a_y, b_y)) + abs(other->half_values.x * glm::dot(a_x, b_z)) + abs(other->half_values.z * glm::dot(a_x, b_x));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();
	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = glm::cross(a_x, b_y);
	}
	//9
	test_a = abs(glm::dot(t, a_z) * glm::dot(a_y, b_z) - glm::dot(t, a_y) * glm::dot(a_z, b_z));
	test_b = abs(half_values.y * glm::dot(a_z, b_z)) + abs(half_values.z * glm::dot(a_y, b_z)) + abs(other->half_values.x * glm::dot(a_x, b_y)) + abs(other->half_values.y * glm::dot(a_x, b_x));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();
	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = glm::cross(a_x, b_z);
	}
	//10
	test_a = abs(glm::dot(t, a_x) * glm::dot(a_z, b_x) - glm::dot(t, a_z) * glm::dot(a_x, b_x));
	test_b = abs(half_values.x * glm::dot(a_z, b_x)) + abs(half_values.z * glm::dot(a_x, b_x)) + abs(other->half_values.y * glm::dot(a_y, b_z)) + abs(other->half_values.z * glm::dot(a_y, b_y));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();
	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = glm::cross(a_y, b_x);
	}
	//11
	test_a = abs(glm::dot(t, a_x) * glm::dot(a_z, b_y) - glm::dot(t, a_z) * glm::dot(a_x, b_y));
	test_b = abs(half_values.x * glm::dot(a_z, b_y)) + abs(half_values.z * glm::dot(a_x, b_y)) + abs(other->half_values.x * glm::dot(a_y, b_z)) + abs(other->half_values.z * glm::dot(a_y, b_x));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();
	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = glm::cross(a_y, b_y);
	}
	//12
	test_a = abs(glm::dot(t, a_x) * glm::dot(a_z, b_z) - glm::dot(t, a_z) * glm::dot(a_x, b_z));
	test_b = abs(half_values.x * glm::dot(a_z, b_z)) + abs(half_values.z * glm::dot(a_x, b_z)) + abs(other->half_values.x * glm::dot(a_y, b_y)) + abs(other->half_values.y * glm::dot(a_y, b_x));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();
	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = glm::cross(a_y, b_z);
	}
	//13
	test_a = abs(glm::dot(t, a_y) * glm::dot(a_x, b_x) - glm::dot(t, a_x) * glm::dot(a_y, b_x));
	test_b = abs(half_values.x * glm::dot(a_y, b_x)) + abs(half_values.y * glm::dot(a_x, b_x)) + abs(other->half_values.y * glm::dot(a_z, b_z)) + abs(other->half_values.z * glm::dot(a_z, b_y));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();
	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = glm::cross(a_z, b_x);
	}
	//14
	test_a = abs(glm::dot(t, a_y) * glm::dot(a_x, b_y) - glm::dot(t, a_x) * glm::dot(a_y, b_y));
	test_b = abs(half_values.x * glm::dot(a_y, b_y)) + abs(half_values.y * glm::dot(a_x, b_y)) + abs(other->half_values.x * glm::dot(a_z, b_z)) + abs(other->half_values.z * glm::dot(a_z, b_x));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();
	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = glm::cross(a_z, b_y);
	}
	//15	
	test_a = abs(glm::dot(t, a_y) * glm::dot(a_x, b_z) - glm::dot(t, a_x) * glm::dot(a_y, b_z));
	test_b = abs(half_values.x * glm::dot(a_y, b_z)) + abs(half_values.y * glm::dot(a_x, b_z)) + abs(other->half_values.x * glm::dot(a_z, b_y)) + abs(other->half_values.y * glm::dot(a_z, b_x));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return CollisionData();
	if (test_b - test_a != 0 && test_b - test_a < smallest_depth)
	{
		smallest_depth = test_b - test_a;
		axis = glm::cross(a_z, b_z);
	}

	return CollisionData(true, glm::vec3((a_pos + b_pos)/2.0f), axis, glm::vec3(smallest_depth), this->entity, other->entity);
}

std::vector<glm::vec3> AABBCollider::GetPoints()
{

	std::vector<glm::vec3> store;
	glm::vec3 entity_position = (*entity->system->buffer->render_buffer)[entity]->position + position;
	glm::vec3 entity_rotation = (*entity->system->buffer->render_buffer)[entity]->rotation + rotation;
	glm::vec3 entity_scale = (*entity->system->buffer->render_buffer)[entity]->scale;
	glm::mat4 rotationMatrix = glm::yawPitchRoll(glm::radians(entity_rotation.y), glm::radians(entity_rotation.x), glm::radians(entity_rotation.z));

	if (scale_to_mesh)
	{
		glm::vec3 positive_extents = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 negative_extents = glm::vec3(0.f, 0.f, 0.f);
		std::string render_mesh = ((RenderComponent*)entity->components[eRender])->mesh;
		for (auto vector : basic_vertices)
		{
			auto rotation_values = glm::vec4(vector, 1) * rotationMatrix;
			positive_extents.x = (positive_extents.x > rotation_values.x ? positive_extents.x : rotation_values.x);
			positive_extents.y = (positive_extents.y > rotation_values.y ? positive_extents.y : rotation_values.y);
			positive_extents.z = (positive_extents.z > rotation_values.z ? positive_extents.z : rotation_values.z);

			negative_extents.x = (negative_extents.x < rotation_values.x ? negative_extents.x : rotation_values.x);
			negative_extents.y = (negative_extents.y < rotation_values.y ? negative_extents.y : rotation_values.y);
			negative_extents.z = (negative_extents.z < rotation_values.z ? negative_extents.z : rotation_values.z);
		}

		positive_extents *= entity_scale;
		negative_extents *= entity_scale;

		half_values = glm::vec3((positive_extents.x > negative_extents.x) ? positive_extents.x : abs(negative_extents.x), (positive_extents.y > negative_extents.y) ? positive_extents.y : abs(negative_extents.y), (positive_extents.z > negative_extents.z) ? positive_extents.z : abs(negative_extents.z));

		store = {
			{entity_position.x + half_values.x, entity_position.y + half_values.y, entity_position.z + half_values.z},
			{entity_position.x - half_values.x, entity_position.y + half_values.y, entity_position.z + half_values.z},
			{entity_position.x + half_values.x, entity_position.y - half_values.y, entity_position.z + half_values.z},
			{entity_position.x - half_values.x, entity_position.y - half_values.y, entity_position.z + half_values.z},
			{entity_position.x + half_values.x, entity_position.y + half_values.y, entity_position.z - half_values.z},
			{entity_position.x - half_values.x, entity_position.y + half_values.y, entity_position.z - half_values.z},
			{entity_position.x + half_values.x, entity_position.y - half_values.y, entity_position.z - half_values.z},
			{entity_position.x - half_values.x, entity_position.y - half_values.y, entity_position.z - half_values.z}
		};

	}
	else
	{


		store = { 
			{entity_position.x + half_values.x, entity_position.y + half_values.y, entity_position.z + half_values.z},
			{entity_position.x - half_values.x, entity_position.y + half_values.y, entity_position.z + half_values.z},
			{entity_position.x + half_values.x, entity_position.y - half_values.y, entity_position.z + half_values.z},
			{entity_position.x - half_values.x, entity_position.y - half_values.y, entity_position.z + half_values.z},
			{entity_position.x + half_values.x, entity_position.y + half_values.y, entity_position.z - half_values.z},
			{entity_position.x - half_values.x, entity_position.y + half_values.y, entity_position.z - half_values.z},
			{entity_position.x + half_values.x, entity_position.y - half_values.y, entity_position.z - half_values.z},
			{entity_position.x - half_values.x, entity_position.y - half_values.y, entity_position.z - half_values.z}
		};
	}

	return store;

}

bool AABBCollider::InArea(glm::vec3 point, glm::vec3 extents)
{
	if (scale_to_mesh)
	{
		glm::vec3 entity_position = (*entity->system->buffer->render_buffer)[entity]->position + position;
		glm::vec3 entity_rotation = (*entity->system->buffer->render_buffer)[entity]->rotation + rotation;
		glm::vec3 entity_scale = (*entity->system->buffer->render_buffer)[entity]->scale;
		glm::mat4 rotationMatrix = glm::yawPitchRoll(glm::radians(entity_rotation.y), glm::radians(entity_rotation.x), glm::radians(entity_rotation.z));

		glm::vec3 positive_extents = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 negative_extents = glm::vec3(0.f, 0.f, 0.f);
		std::string render_mesh = ((RenderComponent*)entity->components[eRender])->mesh;
		for (auto vector : basic_vertices)
		{
			auto rotation_values = glm::vec4(vector, 1) * rotationMatrix;
			positive_extents.x = (positive_extents.x > rotation_values.x ? positive_extents.x : rotation_values.x);
			positive_extents.y = (positive_extents.y > rotation_values.y ? positive_extents.y : rotation_values.y);
			positive_extents.z = (positive_extents.z > rotation_values.z ? positive_extents.z : rotation_values.z);

			negative_extents.x = (negative_extents.x < rotation_values.x ? negative_extents.x : rotation_values.x);
			negative_extents.y = (negative_extents.y < rotation_values.y ? negative_extents.y : rotation_values.y);
			negative_extents.z = (negative_extents.z < rotation_values.z ? negative_extents.z : rotation_values.z);
		}

		positive_extents *= entity_scale;
		negative_extents *= entity_scale;

		half_values = glm::vec3((positive_extents.x > negative_extents.x) ? positive_extents.x : abs(negative_extents.x), (positive_extents.y > negative_extents.y) ? positive_extents.y : abs(negative_extents.y), (positive_extents.z > negative_extents.z) ? positive_extents.z : abs(negative_extents.z));


	}

	glm::vec3 this_position = (*entity->system->buffer->render_buffer)[entity]->position + position;
	glm::vec3 other_position = point;

	if (!(this_position.x - half_values.x <= other_position.x + extents.x)) return false;
	if (!(this_position.x + half_values.x >= other_position.x - extents.x)) return false;
	if (!(this_position.y - half_values.y <= other_position.y + extents.y)) return false;
	if (!(this_position.y + half_values.y >= other_position.y - extents.y)) return false;
	if (!(this_position.z - half_values.z <= other_position.z + extents.z)) return false;
	if (!(this_position.z + half_values.z >= other_position.z - extents.z)) return false;

	return true;
}
