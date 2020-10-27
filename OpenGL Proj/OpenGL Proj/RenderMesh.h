#pragma once
#include "Utilities.h"
#include <vector>

enum EMeshTemplate
{
	eMeshCube,
	eMeshCustom
};

struct Vertex
{
	float coordinates[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	float normals[3] = {0.0f, 0.0f, 0.0f};
	float texture_coordinates[2] = {0.0f, 0.0f};

	Vertex() {};
	Vertex(std::vector<float> &coords, std::vector<float> &norms, std::vector<float> &text)
	{
		coordinates[0] = coords[0];
		coordinates[1] = coords[1];
		coordinates[2] = coords[2];
		coordinates[3] = coords[3];
		normals[0] = norms[0];
		normals[1] = norms[1];
		normals[2] = norms[2];
		texture_coordinates[0] = text[0];
		texture_coordinates[1] = text[1];
	}
};

struct Material
{
	glm::vec4 ambient_reflection;
	glm::vec4 diffuse_reflection;
	glm::vec4 specular_reflection;
	glm::vec4 emission;
	float shine;
	bool lighting = true;
};

struct Texture
{
	int width; int height; int channels;
	unsigned char* data;
};

class RenderMesh
{
public:

	std::vector<unsigned int> indices;
	std::vector<Vertex> vertices;
	Material material;
	BitMap* bitmap_texture;

	RenderMesh();
	RenderMesh(std::vector<unsigned int> index, std::vector<Vertex> vertex);
	~RenderMesh();
};

