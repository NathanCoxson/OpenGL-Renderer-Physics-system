#include "RenderMesh.h"



RenderMesh::RenderMesh()
{
}

RenderMesh::RenderMesh(std::vector<unsigned int> index, std::vector<Vertex> vertex)
{
	indices = index;
	vertices = vertex;
}


RenderMesh::~RenderMesh()
{
}
