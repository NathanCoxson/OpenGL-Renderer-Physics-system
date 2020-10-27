#pragma once
#include <glew.h>
#include <freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <fstream>
#include <thread>
#include <iostream>
#include <sstream>
#include <cmath>
#include "include/glm/gtx/rotate_vector.hpp"
#include "include/glm/gtx/string_cast.hpp"
#include "include/glm/gtx/euler_angles.hpp"


#pragma comment(lib, "glew32.lib") 

class RenderMesh;

struct BitMap
{
	int size_x;
	int size_y;
	unsigned char* data;
};

BitMap* BitMapReader(std::string filename);

char* ParseFile(const char* text_file);

void shaderCompileTest(GLuint shader);

RenderMesh* OBJReader(std::string path);