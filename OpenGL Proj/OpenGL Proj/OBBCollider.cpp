#include "SphereCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"
#include "SystemManager.h"



OBBCollider::OBBCollider(Entity* parent, glm::vec3 offset_pos, glm::vec3 offset_rot, glm::vec3 offset_half_values)
{
	entity = parent;
	position = offset_pos;
	rotation = offset_rot;
	half_values = offset_half_values;

	glUseProgram(entity->system->renderer->debug_program_id);
	glGenVertexArrays(1, &(vao));
	glBindVertexArray(vao);
	glGenBuffers(1, &(vbo));
}

CollisionData OBBCollider::CheckCollision(ColliderMesh* other)
{
	return other->CheckCollision(this);
}

CollisionData OBBCollider::CheckCollision(SphereCollider* other)
{
	return other->CheckCollision(this);
}

CollisionData OBBCollider::CheckCollision(AABBCollider* other)
{
	return other->CheckCollision(this);
}

CollisionData OBBCollider::CheckCollision(OBBCollider* other)
{
	//std::cout << "OBB WITH OBB" << std::endl;

	glm::vec3 a_pos = (*entity->system->buffer->render_buffer)[entity]->position + position;
	glm::vec3 b_pos = (*entity->system->buffer->render_buffer)[other->entity]->position + other->position;

	glm::vec3 t = b_pos - a_pos;

	glm::vec3 entity_rotation = (*entity->system->buffer->render_buffer)[entity]->rotation + rotation;
	glm::vec3 entity_rotation2 = (*entity->system->buffer->render_buffer)[other->entity]->rotation + other->rotation;

	glm::mat4 rotationMatrix = glm::yawPitchRoll(entity_rotation.y, entity_rotation.x, entity_rotation.z);
	glm::mat4 rotationMatrix2 = glm::yawPitchRoll(entity_rotation2.y, entity_rotation2.x, entity_rotation2.z);

	glm::vec4 rotX = glm::vec4(1, 0, 0, 1) * rotationMatrix;
	glm::vec4 rotY = glm::vec4(0, 1, 0, 1) * rotationMatrix;
	glm::vec4 rotZ = glm::vec4(0, 0, 1, 1) * rotationMatrix;

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

	float smallest_depth = 1000000;
	glm::vec3 axis = a_x;

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

std::vector<glm::vec3> OBBCollider::GetPoints()
{

	std::vector<glm::vec3> store;

	glm::vec3 entity_position = (*entity->system->buffer->render_buffer)[entity]->position + position;
	glm::vec3 entity_rotation = (*entity->system->buffer->render_buffer)[entity]->rotation + rotation;
	glm::mat4 rotationMatrix = glm::inverse(glm::yawPitchRoll(glm::radians(entity_rotation.y), glm::radians(entity_rotation.x), glm::radians(entity_rotation.z)));

	glm::vec3 a_x = glm::vec3(glm::vec4(1, 0, 0, 1) * rotationMatrix);
	glm::vec3 a_y = glm::vec3(glm::vec4(0, 1, 0, 1) * rotationMatrix);
	glm::vec3 a_z = glm::vec3(glm::vec4(0, 0, 1, 1) * rotationMatrix);

	store = {
			{entity_position + half_values.x * a_x + half_values.y * a_y + half_values.z * a_z},
			{entity_position - half_values.x * a_x + half_values.y * a_y + half_values.z * a_z},
			{entity_position + half_values.x * a_x - half_values.y * a_y + half_values.z * a_z},
			{entity_position - half_values.x * a_x - half_values.y * a_y + half_values.z * a_z},
			{entity_position + half_values.x * a_x + half_values.y * a_y - half_values.z * a_z},
			{entity_position - half_values.x * a_x + half_values.y * a_y - half_values.z * a_z},
			{entity_position + half_values.x * a_x - half_values.y * a_y - half_values.z * a_z},
			{entity_position - half_values.x * a_x - half_values.y * a_y - half_values.z * a_z}
	};

	return store;

}

bool OBBCollider::InArea(glm::vec3 point, glm::vec3 extents)
{
	
	glm::vec3 a_pos = point;
	glm::vec3 b_pos = (*entity->system->buffer->render_buffer)[entity]->position + position;

	glm::vec3 t = b_pos - a_pos;

	glm::vec3 entity_rotation2 = (*entity->system->buffer->render_buffer)[entity]->rotation + rotation;

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

	//1
	test_a = abs(glm::dot(t, a_x));
	test_b = extents.x + abs(half_values.x * glm::dot(a_x, b_x)) + abs(half_values.y * glm::dot(a_x, b_y)) + abs(half_values.z * glm::dot(a_x, b_z));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;

	//2
	test_a = abs(glm::dot(t, a_y));
	test_b = extents.y + abs(half_values.x * glm::dot(a_y, b_x)) + abs(half_values.y * glm::dot(a_y, b_y)) + abs(half_values.z * glm::dot(a_y, b_z));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;

	//3
	test_a = abs(glm::dot(t, a_z));
	test_b = extents.z + abs(half_values.x * glm::dot(a_z, b_x)) + abs(half_values.y * glm::dot(a_z, b_y)) + abs(half_values.z * glm::dot(a_z, b_z));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;

	//4
	test_a = abs(glm::dot(t, b_x));
	test_b = abs(extents.x * glm::dot(a_x, b_x)) + abs(extents.y * glm::dot(a_y, b_x)) + abs(extents.z * glm::dot(a_z, b_x)) + half_values.x;
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;

	//5
	test_a = abs(glm::dot(t, b_y));
	test_b = abs(extents.x * glm::dot(a_x, b_y)) + abs(extents.y * glm::dot(a_y, b_y)) + abs(extents.z * glm::dot(a_z, b_y)) + half_values.y;
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;

	//6
	test_a = abs(glm::dot(t, b_z));
	test_b = abs(extents.x * glm::dot(a_x, b_z)) + abs(extents.y * glm::dot(a_y, b_z)) + abs(extents.z * glm::dot(a_z, b_z)) + half_values.z;
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;

	//7
	test_a = abs(glm::dot(t, a_z) * glm::dot(a_y, b_x) - glm::dot(t, a_y) * glm::dot(a_z, b_x));
	test_b = abs(extents.y * glm::dot(a_z, b_x)) + abs(extents.z * glm::dot(a_y, b_x)) + abs(half_values.y * glm::dot(a_x, b_z)) + abs(half_values.z * glm::dot(a_x, b_y));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;

	//8
	test_a = abs(glm::dot(t, a_z) * glm::dot(a_y, b_y) - glm::dot(t, a_y) * glm::dot(a_z, b_y));
	test_b = abs(extents.y * glm::dot(a_z, b_y)) + abs(extents.z * glm::dot(a_y, b_y)) + abs(half_values.x * glm::dot(a_x, b_z)) + abs(half_values.z * glm::dot(a_x, b_x));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;

	//9
	test_a = abs(glm::dot(t, a_z) * glm::dot(a_y, b_z) - glm::dot(t, a_y) * glm::dot(a_z, b_z));
	test_b = abs(extents.y * glm::dot(a_z, b_z)) + abs(extents.z * glm::dot(a_y, b_z)) + abs(half_values.x * glm::dot(a_x, b_y)) + abs(half_values.y * glm::dot(a_x, b_x));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;

	//10
	test_a = abs(glm::dot(t, a_x) * glm::dot(a_z, b_x) - glm::dot(t, a_z) * glm::dot(a_x, b_x));
	test_b = abs(extents.x * glm::dot(a_z, b_x)) + abs(extents.z * glm::dot(a_x, b_x)) + abs(half_values.y * glm::dot(a_y, b_z)) + abs(half_values.z * glm::dot(a_y, b_y));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;
	//11
	test_a = abs(glm::dot(t, a_x) * glm::dot(a_z, b_y) - glm::dot(t, a_z) * glm::dot(a_x, b_y));
	test_b = abs(extents.x * glm::dot(a_z, b_y)) + abs(extents.z * glm::dot(a_x, b_y)) + abs(half_values.x * glm::dot(a_y, b_z)) + abs(half_values.z * glm::dot(a_y, b_x));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;

	//12
	test_a = abs(glm::dot(t, a_x) * glm::dot(a_z, b_z) - glm::dot(t, a_z) * glm::dot(a_x, b_z));
	test_b = abs(extents.x * glm::dot(a_z, b_z)) + abs(extents.z * glm::dot(a_x, b_z)) + abs(half_values.x * glm::dot(a_y, b_y)) + abs(half_values.y * glm::dot(a_y, b_x));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;

	//13
	test_a = abs(glm::dot(t, a_y) * glm::dot(a_x, b_x) - glm::dot(t, a_x) * glm::dot(a_y, b_x));
	test_b = abs(extents.x * glm::dot(a_y, b_x)) + abs(extents.y * glm::dot(a_x, b_x)) + abs(half_values.y * glm::dot(a_z, b_z)) + abs(half_values.z * glm::dot(a_z, b_y));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;

	//14
	test_a = abs(glm::dot(t, a_y) * glm::dot(a_x, b_y) - glm::dot(t, a_x) * glm::dot(a_y, b_y));
	test_b = abs(extents.x * glm::dot(a_y, b_y)) + abs(extents.y * glm::dot(a_x, b_y)) + abs(half_values.x * glm::dot(a_z, b_z)) + abs(half_values.z * glm::dot(a_z, b_x));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;

	//15	
	test_a = abs(glm::dot(t, a_y) * glm::dot(a_x, b_z) - glm::dot(t, a_x) * glm::dot(a_y, b_z));
	test_b = abs(extents.x * glm::dot(a_y, b_z)) + abs(extents.y * glm::dot(a_x, b_z)) + abs(half_values.x * glm::dot(a_z, b_y)) + abs(half_values.y * glm::dot(a_z, b_x));
	//std::cout << test_a << " " << test_b << std::endl;
	if (test_a > test_b) return false;


	return true;
}
